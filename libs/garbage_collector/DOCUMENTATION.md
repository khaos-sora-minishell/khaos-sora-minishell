# Garbage Collector - Complete Technical Documentation

## Table of Contents

1. [Introduction](#introduction)
2. [Architecture](#architecture)
3. [Linker Wrapping System](#linker-wrapping-system)
4. [Algorithm Details](#algorithm-details)
5. [API Reference](#api-reference)
6. [Implementation Details](#implementation-details)
7. [Best Practices](#best-practices)
8. [Troubleshooting](#troubleshooting)
9. [Performance Analysis](#performance-analysis)

---

## Introduction

This garbage collector implements a **conservative mark-and-sweep** algorithm for C programs. The key innovation is using **linker wrapping** to intercept memory allocation calls without requiring any code changes.

### Key Concepts

**Conservative Collection**: Treats any value that looks like a valid pointer as a pointer. This ensures safety but may keep some memory alive longer than necessary.

**Mark-and-Sweep**: Two-phase algorithm:
- **Mark Phase**: Find and mark all reachable memory
- **Sweep Phase**: Free all unmarked memory

**Linker Wrapping**: Uses GCC's `--wrap` flag to intercept function calls at link time, allowing us to replace `malloc`/`free` without changing source code.

---

## Architecture

### System Overview

```
┌─────────────────────────────────────────────────────────────┐
│                        User Code                             │
│  malloc(), free(), calloc(), realloc()                      │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│                   Linker Wrapper Layer                       │
│  __wrap_malloc() → gc_malloc() or __real_malloc()          │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│                  Garbage Collector Core                      │
│  - Mark Phase: Scan stack for roots                        │
│  - Sweep Phase: Free unmarked blocks                       │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│                   Internal Allocator                         │
│  GC_INTERNAL_MALLOC → __real_malloc() (system)             │
└─────────────────────────────────────────────────────────────┘
```

### Memory Layout

Each allocation consists of a header and user data:

```
Memory Address Space:
┌──────────────────────────────────────┐
│  Header: t_collecter                 │  ← GC metadata
│  - is_marked: unsigned char (0/1)    │
│  - size: size_t (user data size)     │
│  - next: t_collecter* (linked list)  │
├──────────────────────────────────────┤
│  User Data                           │  ← User sees this pointer
│  (size bytes)                        │
└──────────────────────────────────────┘

Pointer returned to user: &(header + 1)
```

### Data Structures

#### Block Header

```c
typedef struct s_collecter
{
    unsigned char      is_marked;  // 0 or 1 (mark bit)
    size_t             size;       // Size of user data
    struct s_collecter *next;      // Next block in list
} t_collecter;
```

**Fields:**
- `is_marked`: Used during mark phase (0 = unmarked, 1 = marked)
- `size`: Number of bytes allocated for user
- `next`: Pointer to next allocation in global list

#### Global State

```c
// In gc_state.c (file-scope static)
static t_collecter *gc_head = NULL;        // Head of allocation list
static void *gc_stack_start = NULL;        // Stack start for scanning
```

**Note:** These are file-scope static, NOT global variables (42 Norm compliant).

---

## Linker Wrapping System

### How --wrap Works

The `--wrap` linker flag performs symbol renaming:

```bash
gcc -Wl,--wrap=malloc ...
```

**What happens:**

1. All calls to `malloc` → redirected to `__wrap_malloc`
2. Original `malloc` → renamed to `__real_malloc`
3. `__wrap_malloc` can call `__real_malloc` to get original behavior

### Example Flow

```c
// User code:
void *ptr = malloc(100);

// Linker transforms to:
void *ptr = __wrap_malloc(100);

// In gc_wrap.c:
void *__wrap_malloc(size_t size)
{
    #ifdef USE_GC_WRAP
        return gc_malloc(size);      // Use GC
    #else
        return __real_malloc(size);  // Use original malloc
    #endif
}
```

### Preventing Recursion

**Problem:** GC itself needs `malloc` for its internal structures. If we wrap those calls, we get infinite recursion.

**Solution:** `gc_internal_malloc.h` provides macros that bypass wrapping:

```c
// gc_internal_malloc.h
#ifdef USE_GC_WRAP
    // GC's internal use: call real malloc directly
    extern void *__real_malloc(size_t size);
    #define GC_INTERNAL_MALLOC(size)  __real_malloc((size))
#else
    // Normal mode: just use malloc
    #define GC_INTERNAL_MALLOC(size)  malloc((size))
#endif
```

**Usage in GC code:**

```c
// gc_malloc.c
void *gc_malloc(size_t size)
{
    // Use internal malloc - won't be wrapped!
    t_collecter *header = GC_INTERNAL_MALLOC(sizeof(t_collecter) + size);
    // ...
}
```

**Flow diagram:**

```
User Code:
malloc(100)
    ↓
__wrap_malloc(100)  ← Wrapped
    ↓
gc_malloc(100)
    ↓
GC_INTERNAL_MALLOC(...)  ← NOT wrapped
    ↓
__real_malloc(...)  ← System malloc directly
```

### Wrapper Implementation

```c
// gc_wrap.c

// Real functions (provided by linker)
extern void *__real_malloc(size_t size);
extern void *__real_calloc(size_t nmemb, size_t size);
extern void *__real_realloc(void *ptr, size_t size);
extern void __real_free(void *ptr);

// Wrapper functions
void *__wrap_malloc(size_t size)
{
#ifdef USE_GC_WRAP
    auto_init_gc();
    return gc_malloc(size);
#else
    return __real_malloc(size);
#endif
}

void __wrap_free(void *ptr)
{
#ifdef USE_GC_WRAP
    // No-op: GC handles cleanup
    (void)ptr;
#else
    __real_free(ptr);
#endif
}
```

---

## Algorithm Details

### Initialization Phase

```c
void collector_init(void *stack_start)
{
    void **stack_ptr = get_gc_stack_start();
    *stack_ptr = stack_start;
}
```

**Purpose:** Store the initial stack pointer for later root scanning.

**Why needed:** The GC must know where the stack starts to scan for root pointers.

**Usage:**
```c
int main(void)
{
    int stack_var;  // Stack variable
    collector_init(&stack_var);  // Save stack start
    // ...
}
```

---

### Allocation Phase

#### gc_malloc Implementation

```c
void *gc_malloc(size_t size)
{
    t_collecter **head_ptr;
    t_collecter *new_header;

    if (size == 0)
        return (NULL);
    
    // Step 1: Allocate header + data using internal malloc
    new_header = GC_INTERNAL_MALLOC(sizeof(t_collecter) + size);
    if (!new_header)
        return (NULL);
    
    // Step 2: Initialize header
    head_ptr = get_gc_head();
    new_header->is_marked = 0;
    new_header->size = size;
    
    // Step 3: Add to linked list (prepend)
    new_header->next = *head_ptr;
    *head_ptr = new_header;
    
    // Step 4: Return pointer to user data
    return ((void *)(new_header + 1));
}
```

**Memory layout after allocation:**

```
Before:
gc_head → [BlockA] → [BlockB] → NULL

After gc_malloc(100):
gc_head → [NewBlock] → [BlockA] → [BlockB] → NULL
           ↑
         size=100
         marked=0
```

#### gc_calloc Implementation

```c
void *gc_calloc(size_t nmemb, size_t size)
{
    size_t total_size;
    void *ptr;

    if (nmemb == 0 || size == 0)
        return (NULL);
    
    // Check for overflow
    if (size > (size_t)-1 / nmemb)
        return (NULL);
    
    total_size = nmemb * size;
    ptr = gc_malloc(total_size);
    if (!ptr)
        return (NULL);
    
    // Zero-initialize
    gc_bzero(ptr, total_size);
    return (ptr);
}
```

#### gc_realloc Implementation

```c
void *gc_realloc(void *ptr, size_t size)
{
    void *new_ptr;
    t_collecter *old_header;
    size_t copy_size;

    // Special cases
    if (!ptr)
        return (gc_malloc(size));
    if (size == 0)
        return (NULL);
    
    // Get old size
    old_header = get_header_from_ptr(ptr);
    if (!old_header)
        return (NULL);
    
    // Allocate new block
    new_ptr = gc_malloc(size);
    if (!new_ptr)
        return (NULL);
    
    // Copy data
    copy_size = (old_header->size < size) ? old_header->size : size;
    gc_memcpy(new_ptr, ptr, copy_size);
    
    // Old block will be collected later
    return (new_ptr);
}
```

---

### Mark Phase

#### Overview

```
1. Start from Stack
       ↓
2. Scan for Pointers
       ↓
3. Mark Valid Blocks
       ↓
4. Recursively Mark Referenced Blocks
```

#### Implementation

**Step 1: Entry Point**

```c
void gc_mark(void)
{
    mark_from_stack();
}
```

**Step 2: Scan Stack**

```c
void mark_from_stack(void)
{
    void **stack_start = get_gc_stack_start();
    void *stack_end;
    void **current;

    // Get current stack position
    stack_end = &stack_end;
    
    // Scan from start to current position
    current = (void **)stack_start;
    while (current <= (void **)stack_end)
    {
        if (is_valid_pointer(*current))
            mark_pointer(*current);
        current++;
    }
}
```

**Step 3: Mark Individual Pointer**

```c
void mark_pointer(void *ptr)
{
    t_collecter **head_ptr;
    t_collecter *node;
    void *data_ptr;

    head_ptr = get_gc_head();
    node = *head_ptr;
    
    // Find the block
    while (node)
    {
        data_ptr = (void *)(node + 1);
        if (ptr == data_ptr && !node->is_marked)
        {
            node->is_marked = 1;  // Mark it
            mark_memory_region(data_ptr, node->size);  // Recursive
            break;
        }
        node = node->next;
    }
}
```

**Step 4: Recursive Marking**

```c
void mark_memory_region(void *start, size_t size)
{
    size_t i;
    void **potential_ptr;

    i = 0;
    // Scan memory word by word
    while (i + sizeof(void *) <= size)
    {
        potential_ptr = (void **)((char *)start + i);
        
        // If this looks like a valid pointer, mark it
        if (is_valid_pointer(*potential_ptr))
            mark_pointer(*potential_ptr);  // Recursive!
        
        i += sizeof(void *);
    }
}
```

**Step 5: Pointer Validation**

```c
int is_valid_pointer(void *ptr)
{
    t_collecter **head_ptr;
    t_collecter *node;
    void *data_ptr;
    void *end_ptr;

    if (!ptr)
        return (0);
    
    head_ptr = get_gc_head();
    node = *head_ptr;
    
    // Check if pointer points into any GC block
    while (node)
    {
        data_ptr = (void *)(node + 1);
        end_ptr = (void *)((char *)data_ptr + node->size);
        
        if (ptr >= data_ptr && ptr < end_ptr)
            return (1);  // Valid!
        
        node = node->next;
    }
    return (0);  // Not a GC pointer
}
```

**Example Mark Flow:**

```
Stack:
  [ptr1] → Block A (marked)
  [ptr2] → Block B (marked)
  [data] = 42

Block A contains:
  [ptr3] → Block C (marked recursively)

Block B contains:
  [data] = 123

Block C contains:
  [data] = 456

Block D:
  Not referenced → unmarked
```

---

### Sweep Phase

```c
void gc_sweep(void)
{
    t_collecter **head_ptr;
    t_collecter *current;
    t_collecter *prev;
    t_collecter *next;

    head_ptr = get_gc_head();
    current = *head_ptr;
    prev = NULL;
    
    while (current)
    {
        next = current->next;
        
        if (!current->is_marked)
        {
            // Unmarked: remove from list and free
            if (prev)
                prev->next = next;
            else
                *head_ptr = next;
            
            GC_INTERNAL_FREE(current);  // Free using internal free
        }
        else
        {
            // Marked: keep it and reset mark bit
            current->is_marked = 0;
            prev = current;
        }
        
        current = next;
    }
}
```

**Before Sweep:**
```
gc_head → [A:marked=1] → [B:marked=0] → [C:marked=1] → NULL
```

**After Sweep:**
```
gc_head → [A:marked=0] → [C:marked=0] → NULL
                          [B freed]
```

---

### Collection Trigger

```c
void gc_collect(void)
{
    gc_mark();   // Mark reachable
    gc_sweep();  // Free unreachable
}
```

**When to call:**
- Periodically in long-running programs
- After processing batches of data
- When memory pressure is high
- Before program exit (optional)

---

## API Reference

### Core Functions

#### collector_init

```c
void collector_init(void *stack_start);
```

**Description:** Initialize the garbage collector with stack start address.

**Parameters:**
- `stack_start`: Pointer to a stack variable (typically in `main`)

**Returns:** Nothing

**Example:**
```c
int main(void)
{
    int x;
    collector_init(&x);
    // ...
}
```

**Thread Safety:** Not thread-safe. Call once from main thread.

---

#### collector_close

```c
void collector_close(void);
```

**Description:** Free all remaining memory and shutdown GC.

**Parameters:** None

**Returns:** Nothing

**Example:**
```c
int main(void)
{
    collector_init(&stack_var);
    // ... program ...
    collector_close();
    return 0;
}
```

**Note:** After calling, you must call `collector_init()` again before using GC.

---

#### gc_malloc

```c
void *gc_malloc(size_t size);
```

**Description:** Allocate memory tracked by garbage collector.

**Parameters:**
- `size`: Number of bytes to allocate

**Returns:** 
- Pointer to allocated memory on success
- NULL on failure

**Example:**
```c
int *array = gc_malloc(sizeof(int) * 100);
if (!array)
    return ERROR;
```

**Complexity:** O(1)

---

#### gc_calloc

```c
void *gc_calloc(size_t nmemb, size_t size);
```

**Description:** Allocate and zero-initialize memory.

**Parameters:**
- `nmemb`: Number of elements
- `size`: Size of each element

**Returns:** 
- Pointer to zeroed memory on success
- NULL on failure or overflow

**Example:**
```c
int *zeroed = gc_calloc(50, sizeof(int));
// All elements are 0
```

**Complexity:** O(n) where n = nmemb * size

---

#### gc_realloc

```c
void *gc_realloc(void *ptr, size_t size);
```

**Description:** Resize existing allocation.

**Parameters:**
- `ptr`: Previously allocated pointer (can be NULL)
- `size`: New size in bytes

**Returns:** 
- Pointer to reallocated memory on success
- NULL on failure

**Behavior:**
- If `ptr` is NULL, acts like `gc_malloc(size)`
- If `size` is 0, returns NULL
- Otherwise, allocates new block and copies data
- Old pointer becomes invalid

**Example:**
```c
int *data = gc_malloc(sizeof(int) * 10);
data = gc_realloc(data, sizeof(int) * 20);  // Resize
```

**Complexity:** O(min(old_size, new_size)) for copying

---

#### gc_collect

```c
void gc_collect(void);
```

**Description:** Run mark-and-sweep garbage collection.

**Parameters:** None

**Returns:** Nothing

**Example:**
```c
// After many allocations
for (int i = 0; i < 1000; i++)
    gc_malloc(1024);

gc_collect();  // Free unreachable memory
```

**Complexity:** O(n) where n = number of allocations

---

### Wrapper API (Optional)

These functions are optional and only needed if you want manual control.

#### gc_init_manual

```c
void gc_init_manual(void *stack_start);
```

**Description:** Manually initialize GC (optional, GC auto-initializes).

**Macro:** `GC_INIT(var)`

**Example:**
```c
#include "gc_wrap.h"

int main(void)
{
    int stack_var;
    GC_INIT(stack_var);  // Optional
    // ...
}
```

---

#### gc_collect_manual

```c
void gc_collect_manual(void);
```

**Description:** Manually trigger collection (optional).

**Macro:** `GC_COLLECT()`

**Example:**
```c
GC_COLLECT();  // Force collection now
```

---

#### gc_cleanup_manual

```c
void gc_cleanup_manual(void);
```

**Description:** Manually cleanup GC (optional).

**Macro:** `GC_CLEANUP()`

**Example:**
```c
GC_CLEANUP();  // Cleanup before exit
```

---

## Implementation Details

### Helper Functions

#### get_header_from_ptr

```c
t_collecter *get_header_from_ptr(void *ptr)
{
    if (!ptr)
        return (NULL);
    
    // User pointer points to data
    // Header is just before it
    return ((t_collecter *)ptr - 1);
}
```

**Usage:** Get header from user pointer (for `gc_realloc`).

---

#### Global State Accessors

```c
// In gc_state.c (file-scope static)
static t_collecter *gc_head = NULL;
static void *gc_stack_start = NULL;

t_collecter **get_gc_head(void)
{
    return (&gc_head);
}

void **get_gc_stack_start(void)
{
    return (&gc_stack_start);
}
```

**Note:** These maintain state without using global variables (42 Norm compliant).

---

### Memory Utilities

```c
void *gc_memcpy(void *dest, const void *src, size_t n);
void *gc_memmove(void *dest, const void *src, size_t n);
void *gc_memset(void *s, int c, size_t n);
int gc_memcmp(const void *s1, const void *s2, size_t n);
void *gc_memchr(const void *s1, int c, size_t n);
void gc_bzero(void *s, size_t n);
```

**Purpose:** Avoid circular dependencies with libc.

---

## Best Practices

### 1. Initialization Pattern

```c
int main(void)
{
    int stack_anchor;
    collector_init(&stack_anchor);  // Required if no auto-init
    
    // ... program logic ...
    
    collector_close();  // Good practice
    return 0;
}
```

---

### 2. Keep Root Pointers on Stack

**✅ Good:**
```c
void process_data(void)
{
    int *data = gc_malloc(sizeof(int) * 100);
    // data is on stack, GC can find it
    use_data(data);
}  // data goes out of scope, can be collected
```

**❌ Bad:**
```c
typedef struct { int *ptr; } Container;
Container *c = malloc(sizeof(Container));  // Regular malloc!
c->ptr = gc_malloc(100);  // GC pointer hidden in regular heap
// GC won't find c->ptr during stack scan!
```

---

### 3. Periodic Collection

```c
// Option 1: Count-based
static int alloc_count = 0;
if (++alloc_count > 1000)
{
    gc_collect();
    alloc_count = 0;
}

// Option 2: Phase-based
process_batch();
gc_collect();  // Clean up after batch
```

---

### 4. Avoid Circular References in Critical Paths

**Problem:**
```c
typedef struct s_node {
    struct s_node *next;
    struct s_node *prev;  // Circular!
} t_node;
```

**Solution:** Mark-and-sweep handles cycles correctly, but they take longer to traverse.

---

## Troubleshooting

### Memory Leaks

**Symptom:** Memory usage grows continuously

**Causes:**
1. Not calling `gc_collect()`
2. Pointers hidden in non-GC memory
3. Conservative collection keeping extra memory

**Solution:**
```c
// Regular collection
gc_collect();

// Use GC for all allocations
void *container = gc_malloc(sizeof(Container));
```

---

### Premature Collection

**Symptom:** Program crashes or data corruption

**Causes:**
1. Pointer stored in register (very rare)
2. Pointer stored in non-stack location
3. Stack start not initialized correctly

**Solution:**
```c
// Use volatile if needed
volatile int *data = gc_malloc(100);

// Ensure proper init
int stack_var;
collector_init(&stack_var);
```

---

### Performance Issues

**Symptom:** Slow collection cycles

**Causes:**
1. Too many allocations
2. Large stack
3. Frequent collections

**Solution:**
```c
// Batch allocations
int *big = gc_malloc(sizeof(int) * 10000);

// Less frequent collection
if (++count > 10000)
{
    gc_collect();
    count = 0;
}
```

---

### Recursion in Wrapping

**Symptom:** Stack overflow on first malloc

**Cause:** GC's internal malloc is being wrapped

**Solution:** Already handled by `gc_internal_malloc.h`:

```c
// GC code uses this:
GC_INTERNAL_MALLOC(size)  // → __real_malloc (no wrap)

// User code gets this:
malloc(size)  // → __wrap_malloc → gc_malloc (wrapped)
```

---

## Performance Analysis

### Memory Overhead

**Per Allocation:**
```
Header size: sizeof(t_collecter) ≈ 16-24 bytes
Minimum allocation: 16-24 bytes overhead + user data
```

**Example:**
```c
malloc(1)    → 17-25 bytes total (1 + header)
malloc(100)  → 116-124 bytes total (100 + header)
```

---

### Time Complexity

| Operation | Complexity | Notes |
|-----------|------------|-------|
| `gc_malloc` | O(1) | Prepend to list |
| `gc_calloc` | O(n) | Zero-initialize |
| `gc_realloc` | O(min(old, new)) | Copy data |
| `gc_collect` | O(n * m) | n=blocks, m=avg pointers |
| `gc_mark` | O(n * m) | Recursive marking |
| `gc_sweep` | O(n) | Linear scan |

---

### Space Complexity

| Component | Space | Notes |
|-----------|-------|-------|
| GC state | O(1) | Two pointers |
| Allocation list | O(n) | Linked list headers |
| Mark phase | O(d) | Recursion depth |
| Total | O(n) | Linear in allocations |

---

### Optimization Strategies

#### 1. Reduce Allocation Count

```c
// ❌ Bad: 1000 small allocations
for (int i = 0; i < 1000; i++)
    gc_malloc(sizeof(int));

// ✅ Good: 1 large allocation
int *array = gc_malloc(sizeof(int) * 1000);
```

#### 2. Control Collection Frequency

```c
// ❌ Bad: Collect too often
for (int i = 0; i < 1000; i++)
{
    gc_malloc(1024);
    gc_collect();  // Too frequent!
}

// ✅ Good: Periodic collection
for (int i = 0; i < 1000; i++)
{
    gc_malloc(1024);
    if (i % 100 == 0)
        gc_collect();
}
```

#### 3. Minimize Pointer Chasing

```c
// ❌ Bad: Deep pointer chains
struct A { struct B *b; };
struct B { struct C *c; };
struct C { struct D *d; };
// Mark phase must traverse entire chain

// ✅ Good: Flat structures when possible
struct Data {
    int a, b, c, d;  // Direct access
};
```

---

## Conclusion

This garbage collector provides automatic memory management for C programs using:
- ✅ Mark-and-sweep algorithm
- ✅ Linker wrapping for zero code changes
- ✅ Conservative pointer scanning
- ✅ 42 Norm compliance (no globals)

For examples and usage patterns, see the `examples/` directory.

---

**Last Updated:** 2025-11-23  
**Author:** suatkvam  
**Version:** 2.0 (with linker wrapping)
# Garbage Collector

A conservative mark-and-sweep garbage collector for C programs with **zero code changes required** using linker wrapping.

## Features

- 🔥 **Zero Code Changes** - Works with existing code as-is
- 🚀 **Mark-and-Sweep** - Conservative garbage collection algorithm
- 🔗 **Linker Magic** - Uses `--wrap` flag to intercept malloc/free
- 🎯 **Stack Scanning** - Automatically finds root pointers
- ⚡ **Easy Integration** - Just recompile with different flag
- 🛡️ **42 Norm Compliant** - No global variables, follows all rules

## Quick Start

### Installation

```bash
git clone https://github.com/suatkvam/garbage_collector.git
cd garbage_collector
```

### Option 1: Normal Mode (Standard malloc/free)

```bash
make
./program
```

Your code uses standard `malloc()` and `free()` - you must manage memory manually.

### Option 2: GC Mode (Automatic Memory Management)

```bash
make gc
./program
```

All `malloc()` calls automatically use garbage collector - no `free()` needed!

## How It Works

### The Magic of `--wrap`

When you compile with `make gc`, the linker intercepts all memory functions:

```
Your Code:           Linker Does:              Result:
malloc()       →     __wrap_malloc()     →     gc_malloc()
free()         →     __wrap_free()       →     (no-op)
calloc()       →     __wrap_calloc()     →     gc_calloc()
realloc()      →     __wrap_realloc()    →     gc_realloc()
```

**No code changes needed!** The linker handles everything automatically.

## Example Usage

### Your Existing Code (NO CHANGES!)

```c
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    // Your normal code - works in both modes!
    char *str = malloc(100);
    strcpy(str, "Hello, GC!");
    
    int *array = malloc(sizeof(int) * 10);
    for (int i = 0; i < 10; i++)
        array[i] = i;
    
    // In normal mode: you should call free()
    // In GC mode: free() is automatic!
    free(str);
    free(array);
    
    return 0;
}
```

**Compile and run:**

```bash
# Normal mode - must manage memory yourself
make normal
./program

# GC mode - automatic memory management
make gc
./program
```

Both produce the same output, but GC mode prevents memory leaks!

## API Reference

### Automatic (Default)

When using `make gc`, all memory functions are automatically intercepted:

```c
void *ptr = malloc(size);      // → gc_malloc(size)
void *ptr = calloc(n, size);   // → gc_calloc(n, size)
void *ptr = realloc(ptr, size);// → gc_realloc(ptr, size)
free(ptr);                     // → no-op (GC handles it)
```

### Manual Control (Optional)

If you want to control GC manually:

```c
#include "gc_wrap.h"

int main(void)
{
    int stack_var;
    
    // Optional: Manual initialization
    GC_INIT(stack_var);
    
    // Your code...
    char *data = malloc(100);  // Still intercepted!
    
    // Optional: Trigger collection manually
    GC_COLLECT();
    
    // Optional: Cleanup before exit
    GC_CLEANUP();
    
    return 0;
}
```

**Note:** Manual control is **optional**. GC auto-initializes on first malloc.

## Architecture

### Memory Block Structure

```
+------------------+
| t_collecter      |  ← Header (metadata)
| - is_marked: 0/1 |     Mark bit for GC
| - size: N bytes  |     Allocation size
| - next: ptr      |     Next in list
+------------------+
| User Data        |  ← Returned to user
| (N bytes)        |
+------------------+
```

### Mark-and-Sweep Algorithm

#### Mark Phase
1. Start from stack (root pointers)
2. Scan each pointer found
3. Mark reachable blocks
4. Recursively mark referenced blocks

#### Sweep Phase
1. Traverse all allocations
2. Free unmarked blocks
3. Reset mark bits on survivors

### Internal vs User Allocations

```c
// GC needs malloc for its own structures
// Uses __real_malloc to avoid recursion

GC_INTERNAL_MALLOC()  →  __real_malloc()  (no wrap)
                             ↓
                        System malloc

// User code malloc is wrapped
malloc()  →  __wrap_malloc()  →  gc_malloc()
                                      ↓
                              Uses GC_INTERNAL_MALLOC
```

## Building

### Makefile Targets

```bash
make           # Build with standard malloc/free
make gc        # Build with garbage collector (--wrap)
make normal    # Same as 'make' (explicit)
make clean     # Remove object files
make fclean    # Remove objects and executable
make re        # Rebuild from scratch
make help      # Show help message
```

### Compiler Flags

**Normal Mode:**
```bash
gcc your_code.c gc_*.c -o program
```

**GC Mode:**
```bash
gcc -DUSE_GC_WRAP \
    -Wl,--wrap=malloc,--wrap=calloc,--wrap=realloc,--wrap=free \
    your_code.c gc_*.c gc_wrap.c -o program
```

## Project Structure

```
garbage_collector/
├── README.md                    # This file
├── DOCUMENTATION.md             # Detailed technical docs
├── Makefile                     # Build system with GC support
│
├── garbage_collector.h          # Public API
├── internal_collector.h         # Internal structures
├── gc_internal_malloc.h         # Internal malloc macros (NEW!)
├── gc_wrap.h                    # Wrapper API (NEW!)
│
├── collector_init.c             # Initialization
├── collector_close.c            # Cleanup
├── gc_malloc.c                  # Allocation
├── gc_calloc.c                  # Zero-initialized allocation
├── gc_realloc.c                 # Reallocation
├── gc_collect.c                 # Collection trigger
├── gc_mark.c                    # Mark phase
├── gc_mark_utils.c              # Mark helpers
├── gc_sweep.c                   # Sweep phase
├── gc_state.c                   # State management
├── gc_memory_utils.c            # Memory utilities
├── get_header_from_ptr.c        # Pointer utilities
├── gc_wrap.c                    # Wrapper implementation (NEW!)
│
└── examples/
    ├── no_changes_example.c     # Works without any modifications
    ├── manual_control_example.c # Optional manual GC control
    ├── linked_list_example.c    # Data structures example
    └── stress_test_example.c    # Performance testing
```

## Examples

### Example 1: No Code Changes

```c
// Your existing code - works as-is!
#include <stdlib.h>
#include <stdio.h>

char *ft_strdup(const char *s)
{
    char *dup = malloc(strlen(s) + 1);
    if (dup)
        strcpy(dup, s);
    return dup;
}

int main(void)
{
    char *str = ft_strdup("Hello, World!");
    printf("%s\n", str);
    
    // In normal mode: must free
    // In GC mode: automatic!
    free(str);
    
    return 0;
}
```

**Build both ways:**
```bash
make normal && ./program  # Standard malloc/free
make gc && ./program      # Automatic GC
```

### Example 2: Linked List

```c
typedef struct s_node {
    int value;
    struct s_node *next;
} t_node;

t_node *create_node(int value)
{
    t_node *node = malloc(sizeof(t_node));
    node->value = value;
    node->next = NULL;
    return node;
}

int main(void)
{
    // Create list
    t_node *head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    
    // In normal mode: must free each node
    // In GC mode: just set head = NULL!
    
    return 0;
}
```

### Example 3: Optional Manual Control

```c
#include "gc_wrap.h"

int main(void)
{
    int stack_var;
    GC_INIT(stack_var);  // Optional init
    
    for (int i = 0; i < 1000; i++)
    {
        malloc(1024);  // Temporary allocations
        
        if (i % 100 == 0)
            GC_COLLECT();  // Optional periodic collection
    }
    
    GC_CLEANUP();  // Optional cleanup
    return 0;
}
```

## Advantages

### With GC Mode

✅ **No memory leaks** - Automatic cleanup  
✅ **No double-free bugs** - GC handles everything  
✅ **No dangling pointers** - Memory tracked properly  
✅ **Simpler code** - No manual free() calls  
✅ **Safer** - Prevents common memory errors  

### Normal Mode (for comparison)

✅ **Standard behavior** - Compatible with all tools  
✅ **Predictable** - You control when memory is freed  
✅ **No overhead** - Direct malloc/free calls  
✅ **Debugging** - Use valgrind, sanitizers, etc.  

## Performance Considerations

### Memory Overhead
- Header: ~16-24 bytes per allocation
- Linked list traversal during collection

### Collection Cost
- **Time Complexity:** O(n) where n = number of allocations
- **Space Complexity:** O(1) additional space

### Optimization Tips

1. **Reduce Allocations:**
```c
// Less efficient
for (int i = 0; i < 1000; i++)
    malloc(sizeof(int));

// More efficient
int *array = malloc(sizeof(int) * 1000);
```

2. **Control Collection Frequency:**
```c
static int count = 0;
if (++count > 1000) {
    GC_COLLECT();
    count = 0;
}
```

3. **Batch Operations:**
```c
// Allocate, use, then collect
process_batch();
GC_COLLECT();
```

## Limitations

⚠️ **Conservative Collection** - May keep memory alive if data looks like a pointer  
⚠️ **Stack-Only Roots** - Only scans stack for root pointers  
⚠️ **Single-Threaded** - Not thread-safe  
⚠️ **No Finalizers** - No custom cleanup code  

## 42 School Compliance

✅ **No global variables** - Uses static functions  
✅ **Norm compliant** - Passes norminette  
✅ **Allowed features only** - No forbidden functions  
✅ **Proper formatting** - 80 char limit, proper tabs  

## Troubleshooting

### Problem: Segmentation Fault

**Cause:** GC not initialized properly  
**Solution:** Ensure `collector_init()` is called (or use auto-init)

### Problem: Memory Still Leaks

**Cause:** Pointers hidden in non-stack memory  
**Solution:** Keep root pointers on stack

### Problem: Premature Collection

**Cause:** Pointer only in register (rare)  
**Solution:** Use `volatile` keyword

### Problem: Infinite Loop on Startup

**Cause:** Recursion in malloc wrapping  
**Solution:** Already handled by `gc_internal_malloc.h`

## Testing

```bash
# Test normal mode with valgrind
make normal
valgrind --leak-check=full ./program

# Test GC mode
make gc
valgrind ./program

# Compare outputs
diff <(make normal && ./program) <(make gc && ./program)
```

## Contributing

Contributions welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Follow 42 Norm
4. Test both modes
5. Submit a pull request

## License

MIT License - Feel free to use in your projects!

## Authors

- **suatkvam** -
- **hudayiarici** -

## Acknowledgments

- Mark-and-sweep algorithm inspiration from various GC implementations
- 42 School for the coding standards
- Community feedback and testing

## Further Reading

- [DOCUMENTATION.md](./DOCUMENTATION.md) - Detailed technical documentation
- [examples/](./examples/) - Complete working examples
- [GC Algorithm Wikipedia](https://en.wikipedia.org/wiki/Tracing_garbage_collection)

---

**Remember:** Switch between modes just by recompiling!

```bash
make normal  # Standard malloc/free
make gc      # Automatic garbage collection
```

**No code changes needed!** 🚀

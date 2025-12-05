# Garbage Collector Library - Documentation

## Table of Contents

1. [Introduction](#introduction)
2. [Architecture](#architecture)
3. [API Reference](#api-reference)
4. [Scope Management](#scope-management)
5. [String Utilities](#string-utilities)
6. [Best Practices](#best-practices)

## Introduction

The My Garbage Collector Library is a lightweight memory management solution for C programs. It uses a **Scope-Based** approach combined with a **Mark-and-Sweep** collector to provide automatic memory management.

### Key Features

- **Scope-Based Management**: Deterministic cleanup of temporary variables using scope stacks.
- **Auto-Collection**: Automatically triggers garbage collection based on memory usage thresholds.
- **Leak Prevention**: Keeps track of all allocations to ensure everything is freed at the end.
- **Standard-like API**: Provides `gc_malloc`, `gc_calloc`, and `gc_realloc` similar to standard libc.

## Architecture

### 1. The Context (`t_gc_context`)
The heart of the library. It holds:
- A global list of all allocations (for tracking and sweeping).
- A stack of "Scopes" (for managing variable lifespan).
- Statistics (memory usage, allocation counts).
- Configuration (modes, thresholds).

### 2. Scopes
Allocations are tied to the "current scope".
- When you enter a function or a logical block, you can **push** a new scope.
- When you leave, you **pop** the scope, which automatically frees all memory allocated within that duration.

### 3. Mark-and-Sweep
Used for general cleanup or when memory limits are reached:
1. **Mark**: Iterates through all active scopes and marks reachable memory.
2. **Sweep**: Iterates through the global list and frees anything that wasn't marked.

## API Reference

### Initialization and Cleanup

#### `t_gc_context *gc_create(void)`
Creates a new garbage collector context and initializes the root scope.

**Returns:**
- A pointer to the new context, or `NULL` on failure.

**Example:**
```c
t_gc_context *gc = gc_create();
if (!gc) return (1);
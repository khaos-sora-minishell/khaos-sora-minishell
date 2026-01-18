# Minishell

*This project has been created as part of the 42 curriculum by akivam, harici.*

## Description

Minishell is a simplified implementation of Bash, designed to work as a basic command-line shell. This project serves as an introduction to shell mechanics, process management, and system programming.

The primary goal is to create a functional shell through collaborative teamwork while learning essential concepts including process handling, forking, signal management, and system calls.

The shell operates through a multi-stage pipeline with an embedded Just-In-Time expansion system: user input is first tokenized by the lexer, which assigns tokens to each element. These tokens are then processed by the parser to construct an Abstract Syntax Tree (AST). Finally, the executor processes the AST with an embedded expander that performs JIT (Just-In-Time) expansion of variables, wildcards, and special parameters right before command execution.

## Instructions

### Prerequisites

- cc compiler (gcc or clang)
- GNU Make
- Readline library

### Compilation

**Mandatory features only:**
```bash
make
```

**With bonus features:**
```bash
make bonus
```

> **Note:** Switching between mandatory and bonus requires recompilation, as bonus features are compiled with the `-D` flag.

### Execution

**Interactive mode:**
```bash
./minishell
```

**Piped input:**
```bash
echo "pwd" | ./minishell
```

**Script execution (bonus only):**
```bash
./minishell script.sh
```

> **Note:** Bonus mode supports up to 9 positional arguments for shell scripts.

### Configuration (Bonus)

Create a `~/.shellrc` file to auto-load commands on shell startup:
```bash
# Example ~/.shellrc
alias ll='ls -la'
alias gs='git status'
export EDITOR=vim
```

## Features

### Mandatory Features

- **Quote Handling:**
  - Single quotes (`'`) prevent interpretation of all metacharacters
  - Double quotes (`"`) prevent interpretation of metacharacters except `$`

- **Redirections:**
  - `<` : Redirect input
  - `>` : Redirect output
  - `<<` : Here-document (reads until delimiter)
  - `>>` : Append output

- **Pipes:**
  - `|` : Connect output of one command to input of next

- **Environment Variables & Expansion:**
  - `$VAR` : Expand to variable value
  - `$?` : Expand to exit status of last command

- **Signal Handling:**
  - `Ctrl-C` : Display new prompt
  - `Ctrl-D` : Exit shell
  - `Ctrl-\` : Do nothing

- **Command History:**
  - Navigate through command history with ↑/↓ arrow keys
  - Powered by readline library

- **Built-in Commands:**
  - `echo` with `-n` option
  - `cd` with relative or absolute path
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`

### Bonus Features

- **Logical Operators:**
  - `&&` : AND operator
  - `||` : OR operator

- **Subshells:**
  - `()` : Execute commands in subshell context

- **Wildcards:**
  - `*` : Pattern matching

- **Script Execution:**
  - Execute `.sh` files with positional arguments (up to 9)

- **Additional Expansions:**
  - `~` : Home directory expansion
  - `$$` : Shell process ID
  - `$1`-`$9` : Positional arguments (script mode)

- **Extended Built-ins:**
  - `help` : Display help information
  - `type` : Display command type information
  - `tty` : Print terminal device name
  - `clear` : Clear terminal screen
  - `true` / `false` : Return success/failure codes
  - `alias` / `unalias` : Manage command aliases

- **Persistent History:**
  - Command history saved to `~/.minishell_history`
  - Automatically loads previous session commands
  - History persists across shell sessions

- **Configuration File:**
  - Auto-load commands from `~/.shellrc` on startup
  - Support for aliases, environment variables, and startup commands
  - Auto-creates config file if it doesn't exist

- **Fun Commands:**
  - `harici_matrix` : Animated matrix effect with random duration
  - `akivam_executer` : Executor workflow visualization
  - `set-terminal` : Customize terminal title and background color

### Architecture Highlights

- **JIT Expansion System:** 
  - Expander embedded within executor for just-in-time variable and wildcard expansion
  - Variables (`$VAR`, `$?`, `$$`), wildcards (`*`), and positional args expanded right before execution

- **Custom Memory Management:**
  - Garbage collector with scope-based mark & sweep algorithm
  - Arena-based allocation for efficient memory management
  - Zero memory leaks (readline leaks excluded as per subject)

- **AST-Based Parsing:**
  - Abstract Syntax Tree construction for complex command structures
  - Supports nested operations (pipes, logical operators, subshells)

- **Hash Table Environment:**
  - O(1) average time complexity for environment variable operations
  - Efficient collision handling with linked list buckets

### Interactive Visualizer

Explore the internal workings of our shell with an interactive web-based visualizer:

**🔗 [Khaos-Sora Shell Visualizer](https://khaos-sora-shell-visulazer.vercel.app/)**

Features:
- Visualize lexer tokenization process
- Explore AST (Abstract Syntax Tree) structure
- Step-by-step command execution simulation
- Interactive learning tool for understanding shell mechanics

## Resources

### References

- [GNU Bash Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [Bash Source Code Repository](https://github.com/bminor/bash/tree/master)
- [Build Your Own Shell - CodeCrafters](https://github.com/codecrafters-io/build-your-own-x?tab=readme-ov-file#build-your-own-shell)
- [xv6 Operating System](https://github.com/mit-pdos/xv6-public/tree/master) - AST implementation inspiration
- Man pages for system calls (fork, execve, pipe, dup, wait, signal, etc.)
- Man pages for readline library

### AI Usage

AI was used in this project for the following purposes:

- **Architecture Design:** AI assisted in designing the initial shell architecture and analyzing bash source code to understand shell mechanics and best practices. Help with understanding AST construction patterns and executor design.

- **AST & Executor Implementation:** AI helped in understanding and implementing the Abstract Syntax Tree structure, the embedded JIT expander logic, and proper command execution flow.

- **Debugging:** AI was instrumental in identifying and resolving logic errors, edge cases, and abnormal behavior throughout development. Particularly helpful with complex issues in signal handling, pipe management, and heredoc processing.

- **Norm Compliance:** AI assisted with 42 Norm requirements, particularly with preprocessor directives (e.g., proper spacing in `#ifdef` statements and macro definitions).

- **Code Generation:** Approximately 10% of the codebase was directly generated with AI assistance, primarily for boilerplate code and utility functions.

- **Documentation & Comments:** AI helped in writing comprehensive comments, function documentation, and this README file.

> **Note:** AI was used as a learning tool and debugging assistant. Core logic, architectural decisions, and implementation strategies were developed by the team through research, experimentation, and collaboration.

## Technical Details

### Memory Management
- Custom garbage collector with scope-based mark & sweep algorithm
- Two-arena system: `global_arena` (shell lifetime) and `cmd_arena` (per-command)
- Zero memory leaks in our code (readline's internal leaks are acceptable per subject)

### Signal Handling
- Single global variable: `g_signal` (volatile sig_atomic_t) as per subject requirements
- Signal-safe implementation with proper handler design
- Interactive mode signal behavior matches bash

### Compilation System
- Separate object directories for mandatory (`obj/`) and bonus (`obj_bonus/`)
- Conditional compilation with `-DBONUS` flag
- Automatic recompilation when switching between modes

## Authors

- **akivam** ([suatkvam](https://github.com/suatkvam)) - Architecture, Executor, Parser (AST), Built-ins, Environment Management, GC Integration
- **harici** ([hudayiarici](https://github.com/hudayiarici)) - Lexer, Parser (CMD), Expander, Signal Handling, Wildcard Implementation

---

*Made with 🐚 at 42 Istanbul*
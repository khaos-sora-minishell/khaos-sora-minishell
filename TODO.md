# TODO - Minishell

## 🔴 High Priority (Critical)
- [ ] **Semicolon operator (;)** - Command separator: `cmd1 ; cmd2 ; cmd3`
- [ ] **Custom readline implementation** - Replace GNU readline with custom implementation
- [ ] **Parser FD handling** - Update parser to read/write from specific file descriptors
- [ ]

## 🟡 Medium Priority (Bash Compatibility)

### Builtin Commands
- [ ] `history` - Show command history
- [ ] `fc` - Fix command (history editor)
- [ ]

### Core Features
- [ ] Command substitution - `$(command)` or `` `command` ``
- [ ] Arithmetic expansion - `$(( expression ))`
- [ ] Tilde expansion - `~` and `~user`
- [ ] Tab completion - Command and file completion
- [ ]

### Advanced Features
- [ ] Parameter expansion - `${var:-default}`, `${var#pattern}`, etc.
- [ ] Brace expansion - `{a,b,c}`, `{1..10}`
- [ ]

## 🟢 Low Priority (Advanced Bash)

### Job Control
- [ ] Background jobs - `&`
- [ ] `bg` - Resume job in background
- [ ] `fg` - Bring job to foreground
- [ ] `jobs` - List jobs
- [ ] `wait` - Wait for job completion
- [ ]

### Control Flow
- [ ] `if/then/else/fi` - Conditional execution
- [ ] `case/esac` - Pattern matching
- [ ] `for` loops
- [ ] `while` loops
- [ ] `until` loops
- [ ]

### Advanced Redirections
- [ ] `>&` - Redirect stdout to fd
- [ ] `&>` - Redirect both stdout and stderr
- [ ] `<>` - Open file for reading and writing
- [ ] Process substitution - `<()` and `>()`
- [ ]

### Functions & Advanced
- [ ] Function definitions - `function_name() { ... }`
- [ ] Arrays - `array=(a b c)`
- [ ] Local variables - `local var=value`
- [ ]

## 🐛 Bug Fixes
- [ ]

## ⚡ Optimization
- [ ]

---

## 🗺️ Development Roadmap

### Phase 1: Full Bash-like Shell (Current Focus)
**Goal:** Production-ready daily driver for developers (~90% bash compatibility)

**Milestones:**
1. **v1.0 - Core Completion** (2-3 months)
   - `;` operator
   - Command substitution `$()`
   - Tab completion
   - history builtin

2. **v1.5 - Advanced Features** (4-6 months)
   - Job control (bg, fg, jobs)
   - Arithmetic expansion
   - Custom readline

3. **v2.0 - Production Ready** (6-9 months)
   - Full testing suite
   - Performance optimization
   - Documentation
   - Bash compatibility: 90%+

### Phase 2: Embedded/Lite Version (Future)
**Goal:** Minimal footprint shell for embedded systems

**Fork:** `minishell-lite` branch
- Remove extras (tetris, colors, etc.)
- Replace GC with manual memory
- Size optimization (<200KB)
- POSIX-only mode
- Static linking support

---

## Current Bash-like Features ✅
- Pipes (|)
- Redirections (<, >, <<, >>)
- Logical operators (&&, ||)
- Subshells ( )
- Wildcards (*)
- Quotes (' ")
- Environment variables & $?
- Signals (Ctrl+C/D/\)
- Builtins: cd, echo, env, exit, export, pwd, unset, alias, unalias
- Extra builtins: help, type, tty, true, false
- Prompt customization (PS1, colors)
- Config file (.shellrc)
- Tetris game 🎮

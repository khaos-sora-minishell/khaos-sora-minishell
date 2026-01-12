# Minishell File Documentation

This document provides a description of each source file and its functions in the project.

## Main Files

### main.c
Entry point of the minishell program. Initializes the shell, sets up signal handlers, and runs the main REPL loop. Provides support for interactive, non-interactive, and script modes.

**Functions:**
- `run_shell_loop` - Main REPL loop for interactive mode, reads and processes user input
- `run_file_mode` - Reads and executes commands line-by-line from file for script mode (includes shebang #!/bin/bash support)
- `main` - Main entry point, runs script mode if argc > 1, otherwise runs interactive mode

### main_utils.c
Shell initialization, cleanup, and input processing utility functions.

**Functions:**
- `init_shell` - Initializes the shell structure with argc/argv, garbage collectors, environment, and history
- `clean_loop` - Cleans up command-specific resources and resets signal state between commands
- `cleanup_shell` - Final cleanup when exiting the shell (saves history, destroys garbage collectors)
- `cleanup_child_process` - Child process cleanup (history, GC, file descriptors)
- `process_input` - Processes user input through lexer, parser, and executor pipeline

### prompt.c
Handles prompt generation and display. Creates colorized prompts with terminal name and manages multiline input reading. Provides input reading support for both interactive and non-interactive modes.

**Functions:**
- `build_prompt_colors` - Builds prompt string with terminal name colors and background colors
- `get_prompt` - Constructs the full prompt string with colors and reset codes
- `join_lines` - Joins two lines of input with a newline character
- `remove_trailing_newline` - Removes trailing newline character from line read in non-interactive mode
- `read_continuation` - Reads continuation lines for unclosed quotes (supports terminal and non-interactive modes)
- `read_multiline` - Main function to read input supporting multi-line for unclosed quotes (supports terminal and non-interactive modes)

### history_manager.c
Manages command history using readline. Loads history from file on startup and saves it on exit.

**Functions:**
- `get_history_path` - Returns the path to the history file (~/.shell_history)
- `init_history` - Initializes history by loading from history file
- `save_history_file` - Saves command history to file

### config_loader.c
Loads and manages shell configuration from ~/.shellrc file. Handles terminal customization settings and aliases.

**Functions:**
- `process_config_line` - Processes a single line from .shellrc config file
- `load_shellrc` - Loads and executes commands from ~/.shellrc file
- `create_shellrc` - Creates a new .shellrc file if it doesn't exist
- `save_to_shellrc` - Appends a line to the .shellrc file

---

## Lexer Module (lexer/)

### lexer.c
Main lexer implementation. Converts raw input strings into tokens for parsing.

**Functions:**
- `check_unclosed_quotes` - Checks for unclosed quotes and prints error if found
- `process_operator` - Processes operator tokens (|, <, >, <<, >>, &&, ||, etc.)
- `process_word` - Processes word tokens
- `lexer` - Main lexer function that tokenizes input string
- `check_unclosed_quotes_char` - Returns the unclosed quote character or 0
- `parse_alias_value` - Parses alias value into tokens
- `append_tokens` - Appends user tokens to alias tokens
- `expand_alias_if_needed` - Expands alias if the first token matches an alias

### lexer_operator.c (Mandatory)
Handles tokenization of mandatory operators (pipes, redirections).

**Functions:**
- `handle_double_op` - Handles double-character operators (<<, >>)
- `handle_single_op` - Handles single-character operators (|, <, >)
- `handle_operator` - Main operator handler that tries double then single operators

### lexer_operator_bonus.c (Bonus)
Handles tokenization of bonus operators (&&, ||, parentheses) in addition to mandatory operators.

**Functions:**
- `handle_double_op` - Handles double-character operators (<<, >>, &&, ||)
- `handle_single_op` - Handles single-character operators (|, <, >, (, ))
- `handle_operator` - Main operator handler that tries double then single operators

### lexer_word.c
Processes word tokens, handling quoted strings and special characters. Manages token value extraction.

**Functions:**
- `skip_quoted` - Skips over quoted sections in input
- `find_word_end` - Finds the end position of a word token
- `extract_word` - Extracts a word from input string

### lexer_token.c (Mandatory)
Token creation and management utilities for mandatory operators.

**Functions:**
- `create_token` - Creates a new token with given type and value
- `add_token` - Adds a token to the end of token list
- `get_operator_value` - Returns string representation of mandatory operator token types

### lexer_token_bonus.c (Bonus)
Token creation and management utilities including bonus operator types.

**Functions:**
- `create_token` - Creates a new token with given type and value
- `add_token` - Adds a token to the end of token list
- `get_operator_value` - Returns string representation of all operator token types (including &&, ||, parentheses)

### quotes.c
Quote handling logic for lexer. Manages single and double quotes, ensuring proper string parsing.

**Functions:**
- `handle_quotes` - Handles quote validation while parsing
- `copy_quoted_content` - Copies content from within quotes to result string
- `remove_quotes` - Removes quote characters from a string

### lexer_utils.c
Utility functions for lexer. Supports alias expansion and token concatenation operations.

**Functions:**
- `check_unclosed_quotes_char` - Returns the unclosed quote character or 0
- `parse_alias_value` - Parses alias value into tokens
- `append_tokens` - Appends user tokens to alias tokens
- `expand_alias_if_needed` - Expands alias if the first token is an alias

---

## Parser Module (parser/)

### parser.c (Mandatory)
Main parser interface for mandatory features. Converts token stream into AST.

**Functions:**
- `validate_token_syntax` - Validates syntax of individual tokens (pipes, redirections)
- `check_syntax` - Checks overall syntax of token list
- `parser` - Main parser function that validates syntax and builds AST

### parser_bonus.c (Bonus)
Parser interface with bonus feature support (&&, ||, parentheses).

**Functions:**
- `validate_token_syntax` - Validates syntax of individual tokens (including &&, ||)
- `check_syntax` - Checks overall syntax including bonus operators
- `parser` - Main parser function that validates syntax and builds AST

### parse_cmd.c
Parses simple commands with arguments and redirections. Builds command structures from tokens.

**Functions:**
- `add_arg_to_cmd` - Adds an argument to command's argument array
- `parse_redirection` - Parses a redirection token and adds to command
- `process_current_token` - Processes current token (redirection or word)
- `parse_simple_command` - Parses a simple command with args and redirections

### parse_cmd_utils.c
Utility functions for command parsing. Includes token type checking and command/redirection creation helpers.

**Functions:**
- `is_redirection_token` - Checks if token type is a redirection
- `is_operator_token` - Checks if token type is an operator
- `create_cmd` - Creates and initializes a new command structure

### parse_cmd_utils2.c
Additional command parsing utilities. Redirection creation and list management.

**Functions:**
- `create_redir` - Creates a new redirection structure with type and file/delimiter
- `add_redir` - Adds a redirection to the end of redirection list

### build_ast.c (Mandatory)
Constructs the AST from tokens for mandatory features.

**Functions:**
- `parse_primary` - Parses primary expressions (commands only)
- `build_ast` - Builds the complete Abstract Syntax Tree from tokens

### build_ast_bonus.c (Bonus)
Constructs the AST with bonus features (subshells).

**Functions:**
- `parse_primary` - Parses primary expressions (commands or subshells with parentheses)
- `build_ast` - Builds the complete Abstract Syntax Tree from tokens

### build_ast_utils.c (Mandatory)
AST building helper functions for mandatory features.

**Functions:**
- `new_ast_node` - Creates a new AST node with given type
- `parse_logic` - Passes through to pipe parsing (no logic operators in mandatory)
- `parse_pipe` - Parses pipe operators

### build_ast_utils_bonus.c (Bonus)
AST building helper functions with bonus features (&&, ||).

**Functions:**
- `new_ast_node` - Creates a new AST node with given type
- `parse_logic` - Parses logical operators (&&, ||) at highest precedence level
- `parse_pipe` - Parses pipe operators at second precedence level

---

## Expander Module (expander/)

### expander.c
Main expansion coordinator. Handles variable expansion, tilde expansion, quote removal, and wildcard processing for command arguments.

**Functions:**
- `process_expansion` - Processes variable expansion, tilde expansion, quotes, and literal characters
- `update_len` - Calculates length needed for expanded string (includes tilde support)
- `get_expanded_length` - Returns total length needed for expansion
- `expand_string` - Main function to expand variables, tildes, and quotes in a string

### expander_utils.c
Expansion utility functions. Provides helper functions for string manipulation during expansion.

**Functions:**
- `extract_var_name` - Extracts variable name from string starting at position
- `expand_exit_status` - Expands $? to exit status value
- `expand_arg_count` - Expands $# to argument count
- `expand_positional_arg` - Expands $0-$9 to argument at specified index
- `expand_all_args` - Expands $@ and $* to all arguments joined with spaces
- `expand_variable` - Expands a variable name to its value from environment
- `process_dollar` - Processes $ expansion including $?, $$, $#, $0-$9, $@, $*, and $VAR
- `process_tilde` - Processes tilde expansion for ~ and ~/path (uses HOME variable)

### expand_args.c
Argument expansion implementation. Processes command arguments, handling environment variables and wildcards.

**Functions:**
- `process_arg` - Processes single argument with variable and wildcard expansion
- `count_single_expanded` - Counts expanded results for single argument
- `count_total_expanded` - Counts total expanded arguments
- `expand_args` - Expands all arguments with variables and wildcards

### expand_args2.c (Mandatory)
Word splitting utilities without wildcard expansion.

**Functions:**
- `split_words` - Splits string into words by IFS characters
- `count_word_with_wildcard` - Counts words (no wildcard in mandatory)
- `add_word_or_wc` - Adds words to result (no wildcard in mandatory)

### expand_args2_bonus.c (Bonus)
Word splitting utilities with wildcard expansion support.

**Functions:**
- `split_words` - Splits string into words by IFS characters
- `count_word_with_wildcard` - Counts words with wildcard expansion
- `add_word_or_wc` - Adds words or wildcard results to result array

### expand_helpers.c
Helper functions for expansion operations. Supports quote handling and special character processing during expansion.

**Functions:**
- `count_args` - Counts number of arguments in array
- `has_quotes` - Checks if string contains quotes
- `add_expanded_results` - Adds wildcard expansion results to result array

### wildcard_bonus.c (Bonus)
Wildcard (glob) pattern matching implementation. Expands * patterns to matching filenames in the current directory.

**Functions:**
- `create_single_result` - Creates a single-element result array with pattern
- `process_matches` - Processes directory matches and returns sorted results
- `expand_wildcard` - Expands wildcard patterns to matching filenames

### wildcard_utils_bonus.c (Bonus)
Wildcard matching utilities. Provides pattern matching algorithms and result sorting for glob expansion.

**Functions:**
- `match_pattern` - Recursively matches wildcard pattern against string
- `count_matches` - Counts number of matching directory entries
- `add_matches` - Adds matching entries to result array
- `sort_strings` - Sorts string array alphabetically (bubble sort)

---

## Executor Module (executor/)

### executor.c
Main executor implementation. Coordinates AST traversal and command execution.

**Functions:**
- `executor_run` - Main executor entry point, backs up stdin/stdout and executes AST

### exec_ast.c (Mandatory)
AST execution logic for mandatory features (commands and pipes).

**Functions:**
- `execute_ast` - Recursively executes AST nodes (commands, pipes)

### exec_ast_bonus.c (Bonus)
AST execution logic with bonus features (&&, ||, subshells).

**Functions:**
- `exec_subshell` - Executes subshell in forked process
- `execute_ast` - Recursively executes AST nodes (commands, pipes, &&, ||, subshells)

### exec_builtin.c
Builtin command dispatcher. Identifies and executes builtin commands like cd, echo, env, etc.

**Functions:**
- `get_builtins` - Returns static array of builtin commands and function pointers
- `is_builtin` - Checks if command is a builtin
- `execute_builtin` - Executes a builtin command by name

### exec_cmd.c
Command execution implementation. Handles fork, execve, and process management for external commands.

**Functions:**
- `handle_redirection_only` - Handles redirections without executing command
- `execute_command` - Main command execution (prepares, checks builtin, forks for external)

### exec_cmd_utils1.c
Command execution utilities (part 1). Path resolution, argument expansion, and exit status handling.

**Functions:**
- `expand_cmd_args` - Expands variables and wildcards in command arguments
- `handle_exit_status` - Processes wait status and sets shell exit status
- `find_command_path` - Finds full path to command using PATH environment variable

### exec_cmd_utils2.c
Command execution utilities (part 2). Command validation and child process execution.

**Functions:**
- `validate_and_execute` - Validates command path and executes with execve
- `exec_child_process` - Child process execution (setup signals, redirections, execute)
- `execute_builtin_with_redir` - Executes builtin with redirections (saves/restores stdio)
- `prepare_cmd_execution` - Prepares command for execution (heredocs, expansions)

### exec_pipe.c
Pipe execution implementation. Creates pipes and manages child processes for pipeline commands.

**Functions:**
- `exec_left_pipe` - Executes left side of pipe in child process
- `exec_right_pipe` - Executes right side of pipe in child process
- `execute_pipe` - Creates pipe, forks children for left/right, waits for completion

### redirections.c
Input/output redirection handler. Manages file descriptor redirection for <, >, and >> operators.

**Functions:**
- `handle_redir_in` - Handles input redirection (<)
- `handle_redir_out` - Handles output redirection (>, >>)
- `handle_heredoc` - Handles heredoc redirection (<<)
- `setup_redirections` - Sets up all redirections for a command

### here_doc_manager.c
Heredoc management system. Coordinates heredoc creation, processing, and cleanup.

**Functions:**
- `handle_heredoc_entry` - Handles a single heredoc redirection
- `process_cmd_heredoc` - Processes all heredocs for a command
- `clean_heredoc` - Cleans up heredoc temporary files

### here_doc_utils1.c
Heredoc utility functions (part 1). Heredoc signal checking and line processing.

**Functions:**
- `heredoc_signal_checker` - Checks for SIGINT during heredoc reading
- `process_heredoc_line` - Processes a single line of heredoc input

### here_doc_utils2.c
Heredoc utility functions (part 2). Heredoc file creation and loop reading functionality.

**Functions:**
- `read_heredoc_input` - Reads a line of heredoc input (interactive or non-interactive)
- `read_heredoc_loop` - Main loop for reading heredoc lines until delimiter
- `get_heredoc_filename` - Generates temporary filename for heredoc
- `create_heredoc_dir` - Creates /tmp/minishell_heredocs directory

---

## Executor Error Module (executor_error/)

### executor_error.c
Error handling for executor. Provides functions to handle command not found, permission denied, and directory execution errors.

**Functions:**
- `print_execution_error` - Prints execution error message to stderr
- `exit_with_error` - Prints error, cleans up shell, and exits with code
- `handle_cmd_not_found` - Handles command not found error (exit 127)
- `handle_permission_denied` - Handles permission denied error (exit 126)
- `handle_is_directory` - Handles "is a directory" error (exit 126)

### executer_error2.c
Additional executor error handlers. Handles file not found errors and provides error exit utilities.

**Functions:**
- `handle_no_such_file` - Handles no such file or directory error (exit 127)

---

## Builtins Module (builtins/)

### cd/builtin_cd.c
Change directory builtin implementation. Handles cd command with HOME, OLDPWD, and relative/absolute path support.

**Functions:**
- `update_pwd_env` - Updates PWD and OLDPWD environment variables
- `resolve_path` - Resolves target path (HOME, OLDPWD, or specified path)
- `builtin_cd` - Implements the cd builtin command

### echo/builtin_echo.c
Echo builtin implementation. Prints arguments with -n flag support for no newline.

**Functions:**
- `parse_flags` - Parses echo flags (-n for no newline)
- `echo_print_arg` - Prints echo arguments with spaces between
- `builtin_echo` - Implements the echo builtin command

### env/builtin_env.c
Environment variable display builtin. Prints all environment variables in the current shell.

**Functions:**
- `execute_env_cmd` - Executes command with current environment (env command args)
- `print_env_bucket` - Prints environment variables in a hash bucket
- `builtin_env` - Implements the env builtin command

### exit/builtin_exit.c
Exit builtin implementation. Terminates shell with optional exit status code. Conditionally prints exit message based on terminal input control.

**Functions:**
- `is_numeric` - Checks if string is a valid numeric argument
- `is_really_neg_one` - Checks if argument is specifically -1
- `quit_shell` - Cleans up and exits with given status
- `builtin_exit` - Implements the exit builtin command (prints "exit" message with terminal input control)

### export/builtin_export.c
Export builtin implementation. Sets environment variables and displays exported variables.

**Functions:**
- `is_valid_identifier` - Validates environment variable name
- `mark_as_exported` - Marks environment variable as exported
- `export_arg` - Processes single export argument
- `builtin_export` - Implements the export builtin command

### export/builtin_expot_utils.c
Export utility functions. Provides helper functions for variable validation and export processing.

**Functions:**
- `sort_buckets` - Sorts environment buckets alphabetically (bubble sort)
- `print_export_item` - Prints single export declaration
- `print_sorted_env` - Prints all exported variables in sorted order

### pwd/builtin_pwd.c
Print working directory builtin. Displays the current working directory path.

**Functions:**
- `builtin_pwd` - Implements the pwd builtin command (prints current directory)

### unset/builtin_unset.c
Unset builtin implementation. Removes environment variables from the shell environment.

**Functions:**
- `builtin_unset` - Implements the unset builtin command

### builtin_alias_bonus.c
Alias builtin implementation (bonus). Creates command aliases for shorter command names.

**Functions:**
- `get_alias_key_len` - Gets length of alias key before '=' sign
- `print_all_aliases` - Prints all defined aliases
- `process_alias_arg` - Processes a single alias argument (set or print)
- `builtin_alias` - Implements the alias builtin command (BONUS)

### builtin_unalias_bonus.c
Unalias builtin implementation (bonus). Removes previously defined command aliases.

**Functions:**
- `builtin_unalias` - Implements the unalias builtin command (BONUS)

---

## Environment Module (env/)

### env_manager.c
Environment variable management system. Implements hash table-based environment storage.

**Functions:**
- `update_env_node` - Updates existing environment node value
- `create_env_node` - Creates new environment bucket node
- `env_get` - Gets environment variable value by key
- `env_set` - Sets environment variable (creates or updates)
- `env_unset` - Removes environment variable from hash table

### env_manager_utils.c
Environment manager utilities. Provides environment variable getter, setter, and unset operations.

**Functions:**
- `set_default_prompt_vars` - Sets default PS1 and PS2 prompt variables
- `mark_env_as_exported` - Marks environment variable as exported
- `set_shlvl_var` - Sets or increments SHLVL environment variable
- `set_pwd_var` - Sets PWD variable if not already set
- `set_default_env_vars` - Sets default environment variables (SHLVL, PWD)
- `add_env_entry` - Adds environment entry from KEY=VALUE string

### env_manager_utils2.c
Additional environment manager utilities. Environment initialization and array conversion.

**Functions:**
- `set_default_shell_vars` - Sets all default shell variables
- `initialize_env_table` - Initializes environment hash table from envp array
- `join_env_pair` - Joins key and value into KEY=VALUE string
- `env_table_to_array` - Converts environment hash table to array of strings

### env_array.c
Environment array conversion. Converts hash table environment to string array format for execve.

**Functions:**
- `count_env_nodes` - Counts number of environment nodes in list
- `fill_env_array` - Fills array with KEY=VALUE strings from env list
- `env_list_to_array` - Converts environment linked list to array of strings

### env_crypto.c
Environment encryption utilities. Hash table indexing and simple encryption.

**Functions:**
- `fnv1a_hash` - Computes FNV-1a hash for hash table indexing
- `xor_cipher` - Encrypts/decrypts string with XOR cipher (reversible)

### parse_path.c
PATH environment variable parser. Extracts and parses directory paths from PATH for command resolution.

**Functions:**
- `parse_path` - Parses PATH environment variable into array of directories

---

## Signals Module (signals/)

### signals.c
Signal handling implementation. Sets up SIGINT and SIGQUIT handlers for interactive and child process modes.

**Functions:**
- `signal_handler` - SIGINT handler for interactive prompt
- `setup_signals` - Sets up signal handlers for parent process
- `setup_child_signals` - Resets signals to default for child processes
- `ignore_signals` - Ignores SIGINT and SIGQUIT during command execution

### signal_state.c
Signal state management. Maintains global signal state for communication between signal handlers and main loop.

**Functions:**
- `get_signal` - Returns current signal value (thread-safe)
- `set_signal` - Sets signal value (called from signal handler)
- `reset_signal` - Resets signal to 0

---

## Utils Module (utils/)

### error.c
Error reporting utilities. Provides functions for printing errors and syntax error messages.

**Functions:**
- `print_error` - Prints formatted error message
- `syntax_error` - Prints syntax error message with token
- `command_not_found` - Prints command not found error (returns 127)
- `error_exit` - Prints error and exits shell

### strings.c
String manipulation utilities. Helpers for file opening operations.

**Functions:**
- `open_file` - Opens file for reading
- `open_file_write` - Opens file for writing (truncate)
- `open_file_append` - Opens file for appending

### strings_utils.c
Additional string utilities. Provides extended string processing functions for the shell.

**Functions:**
- `is_whitespace` - Checks if character is whitespace
- `is_special_char` - Checks if character is special operator character
- `is_metachar` - Checks if character is metacharacter (special or quote)
- `ft_strcmp` - Compares two strings (custom implementation)
- `ft_atoll` - Converts string to long long with overflow detection

---

## Statistics

**Total Files Analyzed:** 68+ source files (includes separate _bonus.c files)

**By Directory:**
- Main files: 4 files
- Lexer: 8 files (4 mandatory + 4 bonus)
- Parser: 10 files (5 mandatory + 5 bonus)
- Expander: 8 files (4 mandatory + 4 bonus)
- Executor: 12 files (6 mandatory + 6 bonus)
- Executor_error: 2 files
- Builtins: 10 files
- Env: 6 files
- Signals: 2 files
- Utils: 3 files

**Note:** Bonus features are physically separated into `_bonus.c` files following RNCP requirements.

**Key Features:**
- Environment variable management with hash table and encryption
- Comprehensive lexer with quote handling and alias expansion
- Recursive descent parser building AST
- Full variable expansion: $VAR, $?, $$, ~, $0-$9, $#, $@, $*
- Positional arguments support (script arguments)
- Script mode support (execute commands from file, shebang support)
- Wildcard expansion (bonus)
- Heredoc support with signal handling
- Pipe and redirection support
- Logical operators && and || (bonus)
- Subshells with () (bonus)
- Signal handling for interactive use
- History management
- Configuration file support (.shellrc)
- 7 builtin commands: cd, echo, env, exit, export, pwd, unset
- Bonus builtins: alias, unalias

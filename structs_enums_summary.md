# Struct and Enum Definitions Summary

## From minishell.h

### Enums

#### e_token_type (BONUS version)
```c
typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
} t_token_type;
```

#### e_node_type (BONUS version)
```c
typedef enum e_node_type
{
    NODE_CMD,
    NODE_PIPE,
    NODE_AND,
    NODE_OR,
    NODE_SUBSHELL,
} t_node_type;
```

#### e_token_type (Standard version)
```c
typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC,
} t_token_type;
```

#### e_node_type (Standard version)
```c
typedef enum e_node_type
{
    NODE_CMD,
    NODE_PIPE,
} t_node_type;
```

### Structs

#### s_env_bucket
```c
typedef struct s_env_bucket
{
    char                *key;
    char                *value;
    int                 _has_value;
    int                 _is_exported;
    struct s_env_bucket *next;
} t_env_bucket;
```

#### s_env_table
```c
typedef struct s_env_table
{
    t_env_bucket        **buckets;
    int                 count;
} t_env_table;
```

#### s_token
```c
typedef struct s_token
{
    t_token_type        type;
    char                *value;
    struct s_token      *next;
} t_token;
```

#### s_redir
```c
typedef struct s_redir
{
    t_token_type        type;
    char                *file;
    char                *delimiter;
    char                *heredoc_tmpfile;
    int                 should_expand;
    struct s_redir      *next;
} t_redir;
```

#### s_cmd
```c
typedef struct s_cmd
{
    char                **args;
    t_redir             *redirs;
} t_cmd;
```

#### s_env
```c
typedef struct s_env
{
    char                *key;
    char                *value;
    struct s_env        *next;
} t_env;
```

#### s_ast_node
```c
typedef struct s_ast_node
{
    t_node_type         type;
    struct s_ast_node   *left;
    struct s_ast_node   *right;
    struct s_ast_node   *subshell_node;
    t_cmd               *cmd;
} t_ast_node;
```

#### s_expand_contex
```c
typedef struct s_expand_contex
{
    int                 i;
    int                 j;
    char                quote;
} t_expand_contex;
```

#### s_shell
```c
typedef struct s_shell
{
    void                *global_arena;
    void                *cmd_arena;

    char                *terminal_name;
    char                *terminal_name_color;
    char                *terminal_name_bg_color;
    char                *terminal_text_color;
    char                *terminal_bg_color;

    t_env_table         *env_table;
    char                **env_array;
    t_env_table         *alias_table;

    char                **path_dirs;

    int                 exit_status;
    t_ast_node          *ast_root;

    int                 stdin_backup;
    int                 stdout_backup;
    int                 redir_stdin_backup;
    int                 redir_stdout_backup;
    int                 history_fd;
    char                *history_file;
    char                *current_input;

    int                 argc;
    char                **argv;
} t_shell;
```

## From lexer/lexer.h
No struct or enum definitions found - contains only function declarations.

## From parser/parser.h
No struct or enum definitions found - contains only function declarations.

## From executor/executor.h
No struct or enum definitions found - contains only function declarations.

---

## Summary Statistics

**Total Enums:** 4 (2 for BONUS, 2 for standard version)
**Total Structs:** 10

### Data Structure Overview:

1. **Token System:** `t_token_type`, `t_token`
2. **AST System:** `t_node_type`, `t_ast_node`
3. **Command System:** `t_cmd`, `t_redir`
4. **Environment System:** `t_env_table`, `t_env_bucket`, `t_env`
5. **Shell State:** `t_shell`
6. **Helper Structures:** `t_expand_contex`

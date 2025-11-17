# 🐚 Minishell Mimari Dokümantasyonu

## 📋 İçindekiler
1. [Genel Mimari](#genel-mimari)
2. [Modül Yapısı](#modül-yapısı)
3. [Veri Yapıları](#veri-yapıları)
4. [İşlem Akışı](#işlem-akışı)
5. [Dosya Organizasyonu](#dosya-organizasyonu)

---

## 🏗️ Genel Mimari

```
┌─────────────────────────────────────────────────────────┐
│                    MINISHELL                             │
├─────────────────────────────────────────────────────────┤
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌─────────┐ │
│  │  LEXER   │→ │  PARSER  │→ │ EXPANDER │→ │ EXECUTOR│ │
│  └──────────┘  └──────────┘  └──────────┘  └─────────┘ │
│       ↓              ↓              ↓            ↓      │
│  ┌─────────────────────────────────────────────────┐   │
│  │         GARBAGE COLLECTOR (Mark & Sweep)        │   │
│  └─────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────┘
```

---

## 📦 Modül Yapısı

### 1. **MAIN** (`src/main/`)
Shell'in ana döngüsü ve başlatma mantığı

```
main/
├── main.c              # Ana entry point
├── shell_loop.c        # REPL döngüsü
├── signal_handler.c    # Signal yönetimi (SIGINT, SIGQUIT)
└── init.c              # Başlangıç setup'ı
```

**Sorumluluklar:**
- Readline ile input alma
- History yönetimi
- Signal setup'ı
- GC başlatma/kapatma
- Environment variable'ları başlatma

---

### 2. **LEXER** (`src/lexer/`)
Input string'ini token'lara ayırır

```
lexer/
├── lexer.c             # Ana lexer fonksiyonu
├── tokenizer.c         # Token oluşturma
├── token_utils.c       # Token yardımcı fonksiyonları
└── quote_handler.c     # Quote işleme mantığı
```

**Token Tipleri:**
```c
typedef enum e_token_type
{
    TOKEN_WORD,         // Normal kelime
    TOKEN_PIPE,         // |
    TOKEN_REDIR_IN,     // <
    TOKEN_REDIR_OUT,    // >
    TOKEN_REDIR_APPEND, // >>
    TOKEN_HEREDOC,      // <<
    TOKEN_AND,          // && (bonus)
    TOKEN_OR,           // || (bonus)
    TOKEN_LPAREN,       // ( (bonus)
    TOKEN_RPAREN,       // ) (bonus)
    TOKEN_EOF           // End of input
}   t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
}   t_token;
```

**Örnek:**
```
Input:  "echo hello | grep h > out.txt"
Output: [WORD:echo] → [WORD:hello] → [PIPE] → [WORD:grep] → 
        [WORD:h] → [REDIR_OUT] → [WORD:out.txt] → [EOF]
```

---

### 3. **PARSER** (`src/parser/`)
Token'ları Abstract Syntax Tree'ye (AST) dönüştürür

```
parser/
├── parser.c            # Ana parser
├── parse_command.c     # Command parsing
├── parse_pipeline.c    # Pipeline parsing
├── parse_redir.c       # Redirection parsing
├── syntax_checker.c    # Syntax hata kontrolü
└── ast_builder.c       # AST oluşturma
```

**AST Node Tipleri:**
```c
typedef enum e_node_type
{
    NODE_COMMAND,       // Basit komut
    NODE_PIPE,          // Pipeline
    NODE_REDIR,         // Redirection
    NODE_AND,           // && (bonus)
    NODE_OR,            // || (bonus)
    NODE_SUBSHELL       // ( ) (bonus)
}   t_node_type;

typedef struct s_ast_node
{
    t_node_type         type;
    char                **args;         // Command arguments
    struct s_redir      *redirs;        // Redirections
    struct s_ast_node   *left;          // Sol child
    struct s_ast_node   *right;         // Sağ child
}   t_ast_node;

typedef struct s_redir
{
    t_token_type        type;           // <, >, <<, >>
    char                *file;          // Dosya adı
    char                *heredoc_delim; // << için delimiter
    struct s_redir      *next;
}   t_redir;
```

**Örnek AST:**
```
Input: "cat file.txt | grep hello > out.txt"

         [PIPE]
        /      \
   [COMMAND]  [COMMAND]
   args:      args:
   - cat      - grep
   - file.txt - hello
              redirs:
              - type: >
              - file: out.txt
```

---

### 4. **EXPANDER** (`src/expander/`)
Environment variable'ları ve wildcard'ları genişletir

```
expander/
├── expander.c          # Ana expander
├── var_expansion.c     # $VAR expansion
├── quote_removal.c     # Quote'ları temizleme
└── wildcard.c          # * expansion (bonus)
```

**Expansion Sırası:**
1. **Variable Expansion**: `$VAR`, `$?`
2. **Quote Removal**: `'`, `"`
3. **Wildcard Expansion**: `*` (bonus)

**Kurallar:**
- Single quote içinde expansion yok: `'$HOME'` → `$HOME`
- Double quote içinde sadece `$`: `"$HOME"` → `/home/user`
- `$?` son exit code'u verir

**Örnek:**
```
Input:  echo "$HOME/file*.txt"
After:  echo "/home/user/file1.txt /home/user/file2.txt"
```

---

### 5. **EXECUTOR** (`src/executor/`)
AST'yi execute eder

```
executor/
├── executor.c          # Ana executor
├── exec_command.c      # Basit komut execute
├── exec_pipeline.c     # Pipeline execute
├── exec_builtin.c      # Built-in command'lar
├── exec_redir.c        # Redirection handling
└── path_resolver.c     # PATH'ten executable bulma
```

**Execution Stratejisi:**
```c
// 1. Built-in mi kontrol et
if (is_builtin(cmd))
    return (exec_builtin(cmd));

// 2. PATH'te ara veya absolute/relative path kontrol et
char *executable = resolve_path(cmd[0]);

// 3. Fork ve exec
pid_t pid = fork();
if (pid == 0)
    execve(executable, cmd, env);

// 4. Parent wait
waitpid(pid, &status, 0);
```

**Pipeline Handling:**
```
Input: cmd1 | cmd2 | cmd3

Process Tree:
Parent
├── Fork cmd1 (stdout → pipe1[1])
├── Fork cmd2 (stdin ← pipe1[0], stdout → pipe2[1])
└── Fork cmd3 (stdin ← pipe2[0])

Parent wait'ler ve exit code'u alır
```

---

### 6. **BUILT-INS** (`src/builtins/`)
Shell içinde çalışan komutlar

```
builtins/
├── builtin_cd.c        # cd komutu
├── builtin_echo.c      # echo -n
├── builtin_env.c       # env
├── builtin_exit.c      # exit
├── builtin_export.c    # export
├── builtin_pwd.c       # pwd
└── builtin_unset.c     # unset
```

**Neden Built-in?**
- `cd`, `export`, `unset`: Parent process'in environment'ını değiştirmeli
- `exit`: Shell'den çıkmalı
- `echo`, `pwd`, `env`: Performance için built-in

**Önemli Notlar:**
```c
// cd: chdir() kullan
int builtin_cd(char **args)
{
    if (chdir(args[1]) == -1)
        return (error_msg("cd", strerror(errno)));
    return (0);
}

// export: env linked list'e ekle
// unset: env linked list'ten sil
// exit: exit_status kontrol et ve exit()
```

---

### 7. **ENVIRONMENT** (`src/env/`)
Environment variable'ları yönetir

```
env/
├── env_init.c          # envp'den env list oluştur
├── env_get.c           # Değer al
├── env_set.c           # Değer set et
├── env_unset.c         # Değer sil
└── env_to_array.c      # List'i char** array'e çevir
```

**Veri Yapısı:**
```c
typedef struct s_env
{
    char            *key;       // Örn: "PATH"
    char            *value;     // Örn: "/usr/bin:/bin"
    struct s_env    *next;
}   t_env;
```

**Neden Linked List?**
- Dynamic ekleme/silme kolay
- `export`, `unset` için verimli
- execve için char** array'e dönüştürülebilir

---

### 8. **UTILS** (`src/utils/`)
Yardımcı fonksiyonlar

```
utils/
├── error.c             # Hata mesajları
├── string_utils.c      # String işlemleri
├── ft_split.c          # String split
├── ft_strjoin.c        # String birleştirme
└── free_utils.c        # GC ile koordinasyon
```

---

## 🗂️ Veri Yapıları Özeti

### Ana Shell Yapısı
```c
typedef struct s_shell
{
    char        **envp;         // Environment array
    t_env       *env_list;      // Environment linked list
    int         exit_status;    // Son komutun exit code'u
    int         stdin_backup;   // STDIN yedek fd
    int         stdout_backup;  // STDOUT yedek fd
}   t_shell;
```

---

## 🔄 İşlem Akışı

```
1. MAIN LOOP
   │
   ├─→ readline() ile input al
   │
   ├─→ add_history()
   │
   ├─→ LEXER: input → tokens
   │   └─ Quote handling
   │   └─ Token creation
   │
   ├─→ PARSER: tokens → AST
   │   └─ Syntax check
   │   └─ AST build
   │
   ├─→ EXPANDER: AST → expanded AST
   │   └─ Variable expansion ($VAR, $?)
   │   └─ Quote removal
   │   └─ Wildcard expansion (bonus)
   │
   ├─→ EXECUTOR: Execute AST
   │   ├─ Built-in mi?
   │   │  └─ YES: Execute built-in
   │   └─ NO: Fork & execve
   │       └─ Handle redirections
   │       └─ Handle pipes
   │
   ├─→ Update exit_status
   │
   └─→ gc_collect() (opsiyonel)
```

---

## 📁 Dosya Organizasyonu

```
minishell/
├── Makefile
├── includes/
│   ├── minishell.h         # Ana header
│   ├── lexer.h
│   ├── parser.h
│   ├── executor.h
│   └── builtins.h
├── src/
│   ├── main/
│   ├── lexer/
│   ├── parser/
│   ├── expander/
│   ├── executor/
│   ├── builtins/
│   ├── env/
│   └── utils/
├── garbage_collector/      # Garbage collector library
│   ├── garbage_collector.h
│   ├── internal_collector.h
│   └── *.c
└── libft/                  # Eğer libft kullanacaksanız
    └── ...
```

---

## 🎯 Önemli Dikkat Noktaları

### 1. **Memory Management**
- Tüm malloc'lar `gc_malloc()` ile yapılmalı
- Ana loop'un sonunda `gc_collect()` çağrılabilir
- Shell exit'te `collector_close()`

### 2. **File Descriptor Yönetimi**
```c
// Redirectiondan önce STDIN/STDOUT'u yedekle
int stdin_backup = dup(STDIN_FILENO);
int stdout_backup = dup(STDOUT_FILENO);

// Redirect işlemlerini yap
// ...

// Geri yükle
dup2(stdin_backup, STDIN_FILENO);
dup2(stdout_backup, STDOUT_FILENO);
close(stdin_backup);
close(stdout_backup);
```

### 3. **Signal Handling**
```c
// Interactive modda:
// CTRL-C: Yeni prompt
// CTRL-D: Exit
// CTRL-\: Hiçbir şey

// Global değişken sadece signal number için
volatile sig_atomic_t g_signal_received = 0;

void signal_handler(int signum)
{
    g_signal_received = signum;
}
```

### 4. **Heredoc**
```c
// << delimiter
// History güncellenmemeli
// Temp file kullanılabilir veya pipe

while (1)
{
    line = readline("> ");
    if (strcmp(line, delimiter) == 0)
        break;
    write(pipe_fd[1], line, strlen(line));
    write(pipe_fd[1], "\n", 1);
}
```

### 5. **Error Handling**
- **Syntax error**: Parse aşamasında yakala
- **Command not found**: PATH search başarısız
- **Permission denied**: execve error
- **No such file**: Redirection error

Bash'i referans al!

---

## 🚀 Geliştirme Sırası Önerisi

1. ✅ Garbage Collector (TAMAMLANMIŞ)
2. 📝 Lexer (Token oluşturma)
3. 🌳 Parser (AST oluşturma)
4. 💰 Environment management
5. 🔧 Built-in command'lar
6. 🚀 Executor (simple commands)
7. 🔗 Pipeline support
8. 📂 Redirections
9. 💲 Variable expansion
10. 🎯 Quote handling
11. 🎁 BONUS: &&, ||, (), *

---

## 🧪 Test Senaryoları

### Temel Komutlar
```bash
echo hello world
pwd
cd /tmp
ls -la
```

### Redirections
```bash
echo hello > out.txt
cat < in.txt
cat << EOF
grep pattern >> log.txt
```

### Pipes
```bash
ls | grep txt
cat file | wc -l | cat -e
```

### Environment
```bash
export VAR=value
echo $VAR
unset VAR
env
```

### Quotes
```bash
echo "Hello $USER"
echo 'Hello $USER'
echo "$HOME/file"
```

### Exit Status
```bash
ls
echo $?
invalid_command
echo $?
```

---

## 📚 Kaynaklar

- **Bash Manual**: https://www.gnu.org/software/bash/manual/
- **POSIX Shell**: https://pubs.opengroup.org/onlinepubs/9699919799/
- **Process Creation**: fork(), execve(), wait() man pages
- **42 Subject**: minishell.en.subject.pdf

---

**Not**: Bu mimari, 42'nin Norm kurallarına uygun şekilde tasarlanmıştır. Her modül bağımsız ve test edilebilir olmalıdır. GC entegrasyonu sayesinde memory management otomatikleştirilmiştir.

Başarılar! 🚀
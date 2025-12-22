# 🐚 Minishell: Mimarisi ve Teknik Dokümantasyonu v2.0

Bu doküman, Minishell projesinin genel mimarisini, güncellenmiş veri yapılarını, modüllerin sorumluluklarını ve ekip için önerilen geliştirme planını açıklamaktadır. Bu versiyon, GC entegrasyonu, signal handling kuralları ve subject gereksinimlerine tam uyumlu struct yapılarını içermektedir.

## 📋 İçindekiler

1.  [Genel Mimari](#1-genel-mimari)
2.  [Veri Yapıları (Structs)](#2-veri-yapıları-structs)
3.  [Global Değişken ve Signal Handling](#3-global-değişken-ve-signal-handling)
4.  [Dosya Organizasyonu](#4-dosya-organizasyonu)
5.  [Yeniden Dengelenmiş İş Bölümü](#5-yeniden-dengelenmiş-iş-bölümü)
6.  [Modül Detayları ve Sorumluluklar](#6-modül-detayları-ve-sorumluluklar)
7.  [Garbage Collector Entegrasyonu](#7-garbage-collector-entegrasyonu)
8.  [Geliştirme Yol Haritası](#8-geliştirme-yol-haritası)
9.  [Önemli Dikkat Noktaları](#9-önemli-dikkat-noktaları)
10. [Test Senaryoları](#10-test-senaryoları)
11. [Kaynaklar](#11-kaynaklar)

---

## 1. Genel Mimari

Proje, temel olarak bir "Oku-Ayrıştır-Genişlet-Yürüt" döngüsüne dayanır. Her aşama, projenin yönetimini kolaylaştırmak için ayrı modüller olarak tasarlanmıştır.

```
┌─────────────────────────────────────────────────────────┐
│                    MINISHELL                             │
├─────────────────────────────────────────────────────────┤
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌─────────┐ │
│  │  LEXER   │→ │  PARSER  │→ │ EXPANDER │→ │ EXECUTOR│ │
│  └──────────┘  └──────────┘  └──────────┘  └─────────┘ │
│       ↓              ↓              ↓            ↓      │
│  ┌─────────────────────────────────────────────────┐   │
│  │         GARBAGE COLLECTOR (Entegre Edilmiş)     │   │
│  └─────────────────────────────────────────────────┘   │
│                                                         │
│  Global: g_signal (sig_atomic_t) - SADECE signal için  │
└─────────────────────────────────────────────────────────┘
```

---

## Subject Kuralları ⚠️

### **Kritik Gereksinimler:**

- ✅ **Tek Global Değişken**: Sadece `g_signal` - signal numarası için
- ✅ **Signal Safety**: `sig_atomic_t` kullanımı zorunlu
- ✅ **Global Yasağı**: Struct, pointer, arena gibi hiçbir data structure global olamaz
- ✅ **Norm Uyumu**: Global struct kullanımı norm hatası verir
- ✅ **readline() Leak**: readline'ın leak'leri kabul edilebilir, kendi kodunuzun leak'i yasak

---

## 2. Veri Yapıları (Structs)

Bu yapılar, `includes/minishell.h` içerisinde tanımlanmalıdır.

### 2.1 Token Yapısı (Lexer Çıktısı)

```c
typedef enum e_token_type
{
    TOKEN_WORD,         // Normal kelime veya argüman
    TOKEN_PIPE,         // |
    TOKEN_REDIR_IN,     // <
    TOKEN_REDIR_OUT,    // >
    TOKEN_REDIR_APPEND, // >>
    TOKEN_HEREDOC,      // <<
    // --- BONUS ---
    TOKEN_AND,          // &&
    TOKEN_OR,           // ||
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN        // )
}   t_token_type;

typedef struct s_token
{
    t_token_type    type;       // Token tipi
    char            *value;     // Token'ın string değeri
    struct s_token  *next;      // Sonraki token
}   t_token;
```

### 2.2 AST (Abstract Syntax Tree) Yapıları

```c
// Yönlendirme bilgilerini tutan yapı
typedef struct s_redir
{
    t_token_type    type;           // Yönlendirme tipi (>, <, >>, <<)
    char            *file;          // Hedef dosya adı
    char            *delimiter;     // SADECE heredoc (<<) için
    char            *heredoc_tmpfile; // Heredoc için temp dosya yolu
    struct s_redir  *next;          // Sonraki yönlendirme
}   t_redir;

// Tek bir komutu temsil eden yapı
typedef struct s_cmd
{
    char    **args;     // execve'ye uygun argüman dizisi (NULL-terminated)
    t_redir *redirs;    // Bu komuta ait yönlendirmeler
}   t_cmd;

// AST düğüm tipleri
typedef enum e_node_type
{
    NODE_CMD,       // Yaprak: Tek bir komut
    NODE_PIPE,      // Dal: Pipe operatörü (|)
    // --- BONUS ---
    NODE_AND,       // Dal: AND operatörü (&&)
    NODE_OR,        // Dal: OR operatörü (||)
    NODE_SUBSHELL   // Dal: Subshell (parantez içi)
}   t_node_type;

// AST'nin ana düğüm yapısı (recursive)
typedef struct s_ast_node
{
    t_node_type         type;

    // PIPE, AND, OR için:
    struct s_ast_node   *left;
    struct s_ast_node   *right;

    // SUBSHELL için:
    struct s_ast_node   *subshell_node;

    // CMD için:
    t_cmd               *cmd;
}   t_ast_node;
```

### 2.3 Ortam Değişkenleri

```c
typedef struct s_env
{
    char            *key;       // Değişken ismi (örn: "PATH")
    char            *value;     // Değişken değeri
    struct s_env    *next;      // Sonraki env değişkeni
}   t_env;
```

### 2.4 Ana Shell Yapısı

```c
// Shell'in genel durumunu tutmak için
typedef struct s_shell
{
    // GC Arenaları (GLOBAL DEĞİL - Struct member!)
    void        *global_arena;  // Shell lifetime için arena
    void        *cmd_arena;     // Her komut için yeni arena

    // Ortam Değişkenleri
    t_env       *env_list;      // Linked list formatında
    char        **env_array;    // execve için char** formatında
    char        **path_dirs;    // PATH'ten split edilmiş dizinler

    // Durum Bilgileri
    int         exit_status;    // Son komutun çıkış kodu ($? için)
    t_ast_node  *ast_root;      // Parser'ın oluşturduğu AST

    // File Descriptor Yedekleri
    int         stdin_backup;   // stdin restore için
    int         stdout_backup;  // stdout restore için
}   t_shell;
```

---

## 3. Global Değişken ve Signal Handling

### 3.1 Tek İzin Verilen Global Değişken

```c
// minishell.h içinde:
extern volatile sig_atomic_t g_signal;

// main.c içinde:
volatile sig_atomic_t g_signal = 0;
```

### 3.2 Signal Handler

```c
void    signal_handler(int signum)
{
    // ✅ İZİN VERİLEN - Sadece g_signal'e yazma:
    g_signal = signum;

    // ✅ İZİN VERİLEN - Signal-safe fonksiyonlar:
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();

    // ❌ YASAK - Shell struct'ına erişim:
    // shell->exit_status = 130;  // Compile bile olmaz!

    // ❌ YASAK - GC fonksiyonları:
    // gc_malloc(arena, size);  // Signal-unsafe!

    // ❌ YASAK - printf, malloc, free vb.:
    // printf("Signal received\n");  // Undefined behavior!
}
```

### 3.3 Signal Kontrolü (Main Loop'ta)

```c
while (1)
{
    if (g_signal == SIGINT)
    {
        shell.exit_status = 130;  // ✅ Main'de shell'e erişim OK
        g_signal = 0;             // Reset
    }

    char *line = readline("minishell> ");
    // ...
}
```

---

## 4. Dosya Organizasyonu

```
minishell/
├── Makefile
├── minishell.h                 # Ana header, tüm struct'lar ve prototipler
├── main.c                      # Ana döngü, shell init
├── history_manager.c           # History yönetimi
├── config_loader.c             # Konfigürasyon yükleme
│
├── lexer/
│   ├── lexer.h                 # Lexer header
│   ├── lexer.c                 # Ana tokenizer
│   ├── lexer_operator.c        # Operatör işleme (|, <, >, vb.)
│   ├── lexer_word.c            # Word token işleme
│   ├── lexer_token.c           # Token oluşturma
│   └── quotes.c                # Quote handling
│
├── parser/
│   ├── parser.h                # Parser header
│   ├── parser.c                # Ana parser
│   ├── parse_cmd.c             # Basit komut parse
│   ├── parse_cmd_utils.c       # Parse yardımcı fonksiyonlar
│   └── build_ast.c             # AST oluşturma
│
├── expander/
│   ├── expander.c              # $VAR, $? genişletme
│   ├── expander_utils.c        # Expander yardımcılar
│   ├── expand_args.c           # Argüman genişletme
│   ├── expand_helpers.c        # Genişletme yardımcıları
│   ├── wildcard.c              # * genişletme (bonus)
│   └── wildcard_utils.c        # Wildcard yardımcıları
│
├── executor/
│   ├── executor.h              # Executor header
│   ├── executor.c              # Ana executor
│   ├── exec_ast.c              # AST traverse
│   ├── exec_cmd.c              # Komut çalıştırma
│   ├── exec_cmd_utils.c        # Komut yardımcıları
│   ├── exec_builtin.c          # Built-in çalıştırma
│   ├── exec_pipe.c             # Pipe yönetimi
│   ├── redirections.c          # Yönlendirme setup
│   ├── redirections.h          # Yönlendirme header
│   ├── here_doc_manager.c      # Heredoc yönetimi
│   ├── here_doc_utils.c        # Heredoc yardımcıları
│   └── easter_egg/             # Easter egg özellikleri (EASTEREGG flag)
│       ├── easteregg.h
│       ├── easter_egg.c
│       ├── harici_matrix.c     # Matrix rain efekti
│       ├── pars_vs_executer.c  # Şaka mesajları
│       └── set_terminal_name.c # Terminal adı değiştirme
│
├── builtins/
│   ├── builtins.h              # Built-in header
│   ├── cd/
│   │   └── builtin_cd.c
│   ├── echo/
│   │   ├── builtin_echo.c
│   │   └── builtin_echo_utils.c
│   ├── env/
│   │   └── builtin_env.c
│   ├── exit/
│   │   └── builtin_exit.c
│   ├── export/
│   │   ├── builtin_export.c
│   │   └── builtin_expot_utils.c
│   ├── pwd/
│   │   └── builtin_pwd.c
│   ├── unset/
│   │   └── builtin_unset.c
│   └── extras/                 # Ekstra built-in'ler
│       ├── builtin_help.c
│       ├── builtin_true_false.c
│       ├── builtin_tty.c
│       └── builtin_type.c
│
├── env/
│   ├── env_manager.c           # Env hash table yönetimi
│   ├── env_manager_utils.c     # Env yardımcıları
│   ├── env_array.c             # Env array dönüşümü
│   ├── env_crypto.c            # XOR cipher (bonus)
│   └── parse_path.c            # PATH parsing
│
├── signals/
│   ├── signals.c               # Signal handler setup
│   └── signal_state.c          # g_signal yönetimi
│
├── utils/
│   ├── utils.h                 # Utils header
│   ├── error.c                 # Hata mesajları
│   ├── strings.c               # String helpers
│   └── strings_utils.c         # String yardımcıları
│
├── executor_utils/
│   ├── file_utils.c            # Dosya yardımcıları
│   ├── ft_atoll.c              # atoll implementasyonu
│   ├── ft_strcmp.c             # strcmp implementasyonu
│   ├── is_special_char.c       # Özel karakter kontrolü
│   └── is_whitespace.c         # Whitespace kontrolü
│
├── executor_error/
│   ├── executor_error.h        # Hata header
│   └── executor_error.c        # Executor hata mesajları
│
└── libs/
    ├── libft/                  # Libft kütüphanesi
    ├── ft_printf/              # ft_printf kütüphanesi
    ├── garbage_collector/      # GC kütüphanesi
    └── get-next-line/          # GNL kütüphanesi
```

---

## 5. Yeniden Dengelenmiş İş Bölümü

### **`harici`'nin Görevleri (Hazırlık ve Kontrol)**

1.  **Lexer (Tamamı):**

    - Girdiyi token'lara ayırma
    - Quote handling (`'` ve `"`)
    - Özel karakterleri tanıma (`|`, `<`, `>`, vb.)

2.  **Parser (Basit Komut Seviyesi):**

    - Token listesinden `t_cmd` yapıları oluşturma
    - Argümanları ve yönlendirmeleri ayıklama
    - AST'nin "yaprak" düğümlerini hazırlama

3.  **Expander (Tamamı):**

    - `$VAR` genişletmesi
    - `$?` (exit status) genişletmesi
    - `*` wildcard genişletmesi (bonus)
    - Quote durumuna göre genişletme kuralları

4.  **Sinyal Yönetimi (Tamamı):**
    - `Ctrl+C`, `Ctrl+D`, `Ctrl+\` handling
    - Signal handler setup
    - Interactive/non-interactive mode ayrımı

### **`Akivam`'ın Görevleri (İcra ve Yapılandırma)**

1.  **Executor (Tamamı):**

    - AST'yi traverse etme
    - `fork`, `pipe`, `dup2` yönetimi
    - Yönlendirmeleri setup etme
    - Heredoc implementasyonu
    - Built-in vs external command ayrımı

2.  **Parser (Ağaç Kurulumu):**

    - `harici`'nin oluşturduğu `t_cmd`'leri kullanarak AST'yi kurma
    - Pipe, AND, OR operatörlerini ağaca ekleme
    - Operator precedence yönetimi
    - Subshell handling (bonus)

3.  **Built-ins (Tamamı):**

    - `cd`: Directory değiştirme
    - `echo`: `-n` flag desteği
    - `env`: Ortam değişkenlerini listeleme
    - `exit`: Shell'den çıkış
    - `export`: Değişken export etme
    - `pwd`: Current directory
    - `unset`: Değişken silme

4.  **Ortam Değişkeni Yönetimi:**
    - `t_env` linked list operasyonları
    - `env_list` ↔ `env_array` dönüşümü
    - PATH parsing

---

## 6. Modül Detayları ve Sorumluluklar

### 6.1 MAIN Module

- `readline()` ile input alma
- History yönetimi (`add_history()`)
- Shell struct initialization
- GC arena lifecycle yönetimi
- Ana döngü kontrol

### 6.2 LEXER Module

**Girdi:** Raw string (readline'dan gelen)
**Çıktı:** `t_token` linked list

**Sorumluluklar:**

- Whitespace'lere göre ayırma
- Quote içi boşlukları koruma
- Özel karakterleri tanıma (`|`, `<`, `>`, `>>`, `<<`)
- Bonus: `&&`, `||`, `(`, `)` tanıma

**Örnek:**

```
Input:  echo "hello   world" | grep hello > out.txt
Output: [WORD:echo] [WORD:hello   world] [PIPE] [WORD:grep] [WORD:hello] [REDIR_OUT] [WORD:out.txt]
```

### 6.3 PARSER Module

**Girdi:** `t_token` linked list
**Çıktı:** `t_ast_node` (AST root)

**Sorumluluklar:**

- Syntax error kontrolü
- Recursive descent parsing
- Operator precedence (`|` < `&&` < `||`)
- Basit komutları `t_cmd` yapısına dönüştürme
- AST ağacını kurma

**Örnek AST:**

```
cat file | grep hello

        PIPE
       /    \
     CMD    CMD
   (cat)  (grep)
```

### 6.4 EXPANDER Module

**Girdi:** AST (parse edilmiş)
**Çıktı:** AST (genişletilmiş)

**Sorumluluklar:**

- `$VAR` → value
- `$?` → exit_status
- `*` → dosya listesi (bonus)
- Quote içi/dışı kuralları:
  - `'...'`: Hiç genişletme yok
  - `"..."`: Sadece `$` genişletilir
  - Tırnak dışı: Her şey genişletilir

### 6.5 EXECUTOR Module

**Girdi:** AST (genişletilmiş)
**Çıktı:** Komutların çalıştırılması

**Sorumluluklar:**

- AST traverse (post-order)
- `NODE_CMD`: `execve()` veya built-in çalıştırma
- `NODE_PIPE`: `pipe()`, `fork()`, `dup2()` yönetimi
- `NODE_AND/OR`: Conditional execution (bonus)
- Yönlendirmeleri setup etme
- FD management (open, close, dup2)
- Wait ve exit status toplama

### 6.6 BUILT-INS Module

**Fork gerektirmez** - Ana shell prosesinde çalışır

**Liste:**

- `cd [path]`: `chdir()` ile directory değiştirme
- `echo [-n] [args...]`: Çıktı yazdırma
- `env`: Ortam değişkenlerini listele
- `exit [n]`: Shell'den çık
- `export [VAR=value]`: Değişken export et
- `pwd`: `getcwd()` ile current directory
- `unset [VAR]`: Değişkeni sil

---

## 7. Garbage Collector Entegrasyonu

### 7.1 Arena Stratejisi

```c
int main(int ac, char **av, char **envp)
{
    t_shell shell;

    // Global arena - shell lifetime boyunca kalır
    shell.global_arena = gc_create_arena();
    shell.env_list = init_env(envp, shell.global_arena);
    shell.path_dirs = parse_path(shell.env_list, shell.global_arena);

    setup_signals();

    while (1)
    {
        // Her komut için yeni arena
        shell.cmd_arena = gc_create_arena();

        char *line = readline("minishell> ");
        if (!line)
            break;

        if (*line)
            add_history(line);

        // Tüm işlemler cmd_arena kullanır
        process_command(line, &shell);

        // Komut bitti - arena'yı yok et (automatic cleanup!)
        gc_destroy_arena(shell.cmd_arena);

        free(line);  // readline'ın malloc'u - manuel free
    }

    gc_destroy_arena(shell.global_arena);
    return (shell.exit_status);
}
```

### 7.2 Allocation Örnekleri

```c
// Global arena kullanımı (shell lifetime):
char *path_copy = gc_strdup(shell->global_arena, path);
shell->env_list = gc_malloc(shell->global_arena, sizeof(t_env));

// Command arena kullanımı (tek komut):
t_token *token = gc_malloc(shell->cmd_arena, sizeof(t_token));
char **args = gc_malloc(shell->cmd_arena, sizeof(char *) * (count + 1));
```

### 7.3 Fork ile GC Kullanımı

```c
pid_t pid = fork();
if (pid == 0)
{
    // CHILD process
    // Parent'ın arena'sını kullanma!
    // GC'yi yeniden init et veya minimal kullan

    execute_external_command(cmd, shell);
    exit(shell->exit_status);
}
else
{
    // PARENT process
    waitpid(pid, &status, 0);
    shell->exit_status = WEXITSTATUS(status);
}
```

### 7.4 readline() İstisnası

```c
// ❌ YANLIŞ:
char *line = gc_readline(arena);  // readline GC ile takip edilemez!

// ✅ DOĞRU:
char *line = readline("minishell> ");  // readline kendi malloc yapar
// ... kullan ...
free(line);  // Manuel free - GC dışı
```

---

## 8. Geliştirme Yol Haritası

### **Faz 1: İskelet Kurulumu (1 hafta)**

**Ortak Görevler:**

- ✅ `Makefile` oluşturma (libft, GC entegrasyonu)
- ✅ `minishell.h` - tüm struct'ları tanımlama
- ✅ `main.c` - readline döngüsü
- ✅ GC initialization
- ✅ Signal handler setup (basit versiyon)

**Hedef:**

```bash
minishell> ls
# ls çalışmalı (PATH'ten bulup execve)
```

---

### **Faz 2: Basit Komutlar ve Argümanlar (1-1.5 hafta)**

**harici:**

- Lexer'ın temelini yazma (whitespace split)
- Quote handling (`'` ve `"`)
- TOKEN_WORD üretme

**Akivam:**

- Basit executor (tek komut, fork, execve)
- PATH parsing ve komut bulma
- Built-ins: `echo`, `pwd`
- Exit status yönetimi

**Hedef:**

```bash
minishell> ls -la /tmp
minishell> echo "hello   world"
minishell> pwd
```

---

### **Faz 3: Pipe ve Yönlendirmeler (2 hafta)**

**harici:**

- Lexer'a özel karakter tanıma (`|`, `<`, `>`, `>>`, `<<`)
- Parser - basit komut parse (args + redirs → t_cmd)
- Expander - `$VAR` ve `$?` genişletme

**Akivam:**

- Parser - AST kurma (pipe için)
- Executor - pipe yönetimi (`pipe()`, `fork()`, `dup2()`)
- Redirections setup (`<`, `>`, `>>`)
- Heredoc (`<<`) implementasyonu
- Built-ins: `cd`, `env`, `export`, `unset`

**Hedef:**

```bash
minishell> cat file | grep hello | wc -l
minishell> cat < input.txt > output.txt
minishell> cat << EOF
minishell> export VAR=test
minishell> echo $VAR
minishell> cd /tmp && pwd
```

---

### **Faz 4: Sağlamlaştırma (1 hafta)**

**Ortak:**

- Tüm edge case'leri test etme
- Memory leak kontrolü (valgrind)
- Syntax error mesajları
- Signal handling refinement
- Norm kontrolü

**Hedef:**

```bash
# Tüm zorunlu testler geçmeli
# Memory leak olmamalı (readline hariç)
# Bash ile davranış karşılaştırması
```

---

### **Faz 5: Bonuslar (İsteğe bağlı - 1-2 hafta)**

**harici:**

- Wildcard `*` genişletme
- Lexer'a `&&`, `||`, `()` desteği

**Akivam:**

- Parser - AND, OR, SUBSHELL node'ları
- Executor - conditional execution
- Operator precedence yönetimi

**Hedef:**

```bash
minishell> ls *.c
minishell> make && ./minishell
minishell> false || echo "fallback"
minishell> (cd /tmp && ls) && pwd
```

---

## 9. Önemli Dikkat Noktaları

### 9.1 Memory Management

- ✅ Tüm allocation'lar GC üzerinden
- ✅ `readline()` sonucu manuel `free()`
- ✅ Her komut sonrası `cmd_arena` destroy
- ✅ FD'leri kapatmayı unutma
- ❌ Memory leak yasak (readline hariç)

### 9.2 File Descriptor Management

```c
// Yönlendirme öncesi:
shell->stdin_backup = dup(STDIN_FILENO);
shell->stdout_backup = dup(STDOUT_FILENO);

// Yönlendirme setup
setup_redirections(cmd->redirs);

// Restore
dup2(shell->stdin_backup, STDIN_FILENO);
dup2(shell->stdout_backup, STDOUT_FILENO);
close(shell->stdin_backup);
close(shell->stdout_backup);
```

### 9.3 Signal Handling Rules

- ✅ Sadece `g_signal` kullanımı
- ✅ Handler'da sadece signal-safe fonksiyonlar
- ✅ Main loop'ta `g_signal` kontrolü
- ❌ Handler'da `malloc`, `printf`, GC yasak
- ❌ Handler'da shell struct'ına erişim yasak

### 9.4 Heredoc Implementation

```c
// Heredoc için pipe oluştur
int pipefd[2];
pipe(pipefd);

// Child process - input oku
while (true)
{
    char *line = readline("> ");
    if (strcmp(line, delimiter) == 0)
        break;
    write(pipefd[1], line, strlen(line));
    write(pipefd[1], "\n", 1);
    free(line);
}
close(pipefd[1]);

// Parent - pipe'ı stdin'e yönlendir
dup2(pipefd[0], STDIN_FILENO);
close(pipefd[0]);
```

### 9.5 Fork Strategy

```c
// Built-in mı?
if (is_builtin(cmd->args[0]))
{
    // Fork'sız çalıştır
    execute_builtin(cmd, shell);
}
else
{
    // External command - fork gerekli
    pid = fork();
    if (pid == 0)
    {
        // Child
        execve(path, cmd->args, shell->env_array);
        exit(127);
    }
    waitpid(pid, &status, 0);
}
```

### 9.6 Quote Handling

| Context     | `$VAR`      | `*`         | Whitespace |
| ----------- | ----------- | ----------- | ---------- |
| `'...'`     | ❌ Literal  | ❌ Literal  | ✅ Korunur |
| `"..."`     | ✅ Genişler | ❌ Literal  | ✅ Korunur |
| Tırnak dışı | ✅ Genişler | ✅ Genişler | ❌ Ayırıcı |

---

## 10. Test Senaryoları

### 10.1 Basit Komutlar

```bash
ls
ls -la /tmp
wc -l Makefile
echo hello
echo "hello   world"
echo 'hello $USER'
```

### 10.2 Pipe'lar

```bash
cat Makefile | grep src
ls | wc -l
cat file1 | grep test | sort | uniq
```

### 10.3 Yönlendirmeler

```bash
cat < input.txt
echo hello > output.txt
ls >> log.txt
cat << EOF
heredoc test
EOF
cat < in.txt | grep test > out.txt
```

### 10.4 Ortam Değişkenleri

```bash
echo $USER
echo $PATH
export MY_VAR=test
echo $MY_VAR
unset MY_VAR
echo $MY_VAR
echo $?
false
echo $?
```

### 10.5 Built-ins

```bash
pwd
cd /tmp
pwd
cd ..
env
export TEST=123
env | grep TEST
unset TEST
exit 42
```

### 10.6 Edge Cases

```bash
echo ""
echo ''
echo "$USER"
echo '$USER'
cat <
cat |
| cat
cat < nonexistent
./nonexistent_command
```

### 10.7 Bonuslar (İsteğe bağlı)

```bash
ls *.c
echo *.txt
ls && echo success
false || echo fallback
(cd /tmp && ls) && pwd
make && ./minishell || echo "build failed"
```

---

## 11. Kaynaklar

### 11.1 Resmi Dokümantasyon

- **Bash Manual**: https://www.gnu.org/software/bash/manual/
- **POSIX Shell**: https://pubs.opengroup.org/onlinepubs/9699919799/
- **GNU Readline**: https://tiswww.case.edu/php/chet/readline/rltop.html

### 11.2 Sistem Çağrıları (man pages)

```bash
man fork
man execve
man pipe
man dup2
man wait
man waitpid
man signal
man sigaction
man open
man close
man read
man write
```

### 11.3 42 Kaynakları

- **Minishell Tester**: https://github.com/LucasKuhn/minishell_tester
- **42 Docs**: https://harm-smits.github.io/42docs/projects/minishell

### 11.4 Faydalı Kavramlar

- Process creation and management
- File descriptors and redirection
- Signal handling (POSIX)
- Lexical analysis and parsing
- Abstract Syntax Trees (AST)
- Shell scripting fundamentals

---

## 📝 Son Notlar

### Başarı Kriterleri

- ✅ Tüm zorunlu özellikler çalışıyor
- ✅ Memory leak yok (readline hariç)
- ✅ Norm hatası yok
- ✅ Bash ile tutarlı davranış
- ✅ Segfault, bus error, double free yok
- ✅ Signal handling doğru çalışıyor
- ✅ Tüm edge case'ler handle ediliyor

### Takım İş Akışı Önerileri

1. **Git Workflow:**

   ```bash
   main (stable)
   ├── dev-harici (lexer, parser-cmd, expander)
   └── dev-akivam (executor, parser-ast, builtins)
   ```

2. **Daily Standup (Opsiyonel):**

   - Ne yaptım?
   - Ne yapacağım?
   - Blocker var mı?

3. **Code Review:**

   - Her modül tamamlandığında karşılıklı review
   - Merge öncesi test koşulması
   - Norm kontrolü

4. **Testing Strategy:**

   ```bash
   # Her commit sonrası:
   make re && valgrind ./minishell

   # Haftalık:
   bash minishell_tester.sh

   # Final:
   - Peer evaluation hazırlığı
   - Defense senaryoları
   ```

---

## 12. Debugging ve Troubleshooting

### 12.1 Valgrind Kullanımı

```bash
# Temel leak check
valgrind --leak-check=full ./minishell

# Detaylı analiz
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell

# Suppression file (readline için)
valgrind --leak-check=full --suppressions=readline.supp ./minishell
```

**readline.supp örneği:**

```
{
   readline_leak
   Memcheck:Leak
   ...
   fun:readline
}
```

### 12.2 GDB ile Debug

```bash
# GDB başlat
gdb ./minishell

# Breakpoint koy
(gdb) break executor.c:42
(gdb) break main

# Çalıştır
(gdb) run

# Değişken değerlerini gör
(gdb) print shell->exit_status
(gdb) print *cmd->args@3

# Stack trace
(gdb) backtrace

# Step by step
(gdb) next
(gdb) step
(gdb) continue
```

### 12.3 Sık Karşılaşılan Hatalar

#### ❌ Hata: "command not found" ama komut var

```c
// Sorun: PATH parsing yanlış
// Çözüm: PATH'i ':' ile split et ve her dizinde ara

char **path_dirs = ft_split(path, ':');
for (int i = 0; path_dirs[i]; i++)
{
    char *full_path = ft_strjoin3(path_dirs[i], "/", cmd);
    if (access(full_path, X_OK) == 0)
        return (full_path);
}
```

#### ❌ Hata: Pipe sonrası çıktı kayboldu

```c
// Sorun: FD'ler kapatılmadı
// Çözüm: Parent'ta kullanılmayan pipe end'leri kapat

pipe(pipefd);
if (fork() == 0)
{
    close(pipefd[0]);  // ✅ Read end'i kapat
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    // ...
}
close(pipefd[1]);  // ✅ Parent'ta write end'i kapat
```

#### ❌ Hata: Heredoc sonsuza kadar bekliyor

```c
// Sorun: Delimiter comparison yanlış
// Çözüm: strcmp + newline handling

while (1)
{
    char *line = readline("> ");
    if (!line || strcmp(line, delimiter) == 0)
    {
        free(line);
        break;
    }
    // Write to pipe...
    free(line);
}
```

#### ❌ Hata: Signal sonrası prompt çalışmıyor

```c
// Sorun: readline state bozuldu
// Çözüm: Signal handler'da readline'ı reset et

void    signal_handler(int signum)
{
    g_signal = signum;
    write(1, "\n", 1);
    rl_on_new_line();      // ✅ Yeni satıra geç
    rl_replace_line("", 0); // ✅ Buffer'ı temizle
    rl_redisplay();         // ✅ Prompt'u yeniden göster
}
```

#### ❌ Hata: Export edilen değişken görünmüyor

```c
// Sorun: env_array güncellenmedi
// Çözüm: Her export sonrası env_array'i rebuild et

void    builtin_export(char **args, t_shell *shell)
{
    // ... key=value parse ...
    set_env_value(&shell->env_list, key, value);

    // ✅ Array'i güncelle
    if (shell->env_array)
        free_split(shell->env_array);
    shell->env_array = env_list_to_array(shell->env_list);
}
```

### 12.4 Performance İpuçları

```c
// ❌ YAVAŞ - Her aramada PATH'i yeniden parse:
char *find_command(char *cmd, t_env *env)
{
    char *path = env_get(env, "PATH");
    char **dirs = ft_split(path, ':');  // Her seferinde!
    // ...
}

// ✅ HIZLI - PATH'i shell init'te parse et:
typedef struct s_shell
{
    char **path_dirs;  // Init'te bir kere parse edildi
    // ...
}

char *find_command(char *cmd, t_shell *shell)
{
    for (int i = 0; shell->path_dirs[i]; i++)
    {
        // Hazır dizileri kullan
    }
}
```

---

## 13. Peer Evaluation Hazırlığı

### 13.1 Temel Sorular (Mutlaka hazırlıklı olun!)

**Q: AST nedir ve neden kullanıyorsunuz?**

```
A: Abstract Syntax Tree - Komutların hiyerarşik yapısını temsil eder.
   Pipe'lar, operatörler ve subshell'leri doğru sırada yürütmek için
   gereklidir. Örneğin: "cat | grep" → PIPE node ile iki CMD node'u bağlar.
```

**Q: fork() ne zaman kullanılır?**

```
A: External komutlar için. Built-in'ler fork gerektirmez çünkü
   shell'in kendi state'ini değiştirirler (cd, export, exit).
   Pipe'larda her komut için ayrı child process gerekir.
```

**Q: Heredoc nasıl implement ettiniz?**

```
A: pipe() ile geçici bir kanal oluşturulur. Delimiter'a kadar okunan
   satırlar pipe'a yazılır, sonra pipe'ın read end'i STDIN'e dup2 ile
   bağlanır. Komut bu şekilde heredoc içeriğini stdin'den okur.
```

**Q: Signal handling nasıl çalışıyor?**

```
A: sig_atomic_t tipinde global bir değişken (g_signal) kullanıyoruz.
   Signal handler sadece bu değişkene signal numarasını yazar.
   Main loop'ta bu değişkeni kontrol edip gerekli işlemleri yapıyoruz.
   Bu sayede signal-unsafe fonksiyonlardan kaçınıyoruz.
```

**Q: Memory leak'leri nasıl yönettiniz?**

```
A: Garbage collector kullanıyoruz. Her komut için yeni bir arena
   oluşturuluyor, komut bittiğinde arena destroy ediliyor.
   readline()'dan dönen pointer'lar manuel free edilmeli.
```

### 13.2 Demo Senaryoları

Evaluation sırasında gösterebilecek senaryolar:

```bash
# 1. Temel işlevsellik
minishell> echo "Hello 42"
minishell> ls -la | grep mini | wc -l

# 2. Yönlendirmeler
minishell> cat << EOF > output.txt
> line 1
> line 2
> EOF
minishell> cat output.txt

# 3. Environment variables
minishell> export TEST=123
minishell> echo $TEST
minishell> echo "Value: $TEST"
minishell> unset TEST

# 4. Exit status
minishell> ls nonexistent
minishell> echo $?
minishell> true
minishell> echo $?

# 5. Built-ins
minishell> pwd
minishell> cd /tmp
minishell> pwd
minishell> cd -
minishell> env | grep USER

# 6. Signal handling
minishell> cat
^C
minishell> # Yeni prompt görülmeli

# 7. Syntax errors
minishell> cat |
minishell> | cat
minishell> cat <
# Uygun hata mesajları görülmeli

# 8. Quotes
minishell> echo "hello   world"    # 3 boşluk korunmalı
minishell> echo 'hello $USER'      # Literal $USER
minishell> echo "hello $USER"      # Genişletilmeli

# 9. Complex pipes
minishell> cat Makefile | grep src | sort | uniq | wc -l
```

### 13.3 Modifikasyon İsteği (Live Coding)

Evaluator şöyle bir şey isteyebilir:

**Örnek 1: "echo'ya yeni bir flag ekle (-e)"**

```c
// builtins/builtin_echo.c içinde:

int builtin_echo(char **args)
{
    bool    newline = true;
    bool    escape = false;  // ✅ Yeni flag
    int     i = 1;

    // Flag parsing
    while (args[i] && args[i][0] == '-')
    {
        if (strcmp(args[i], "-n") == 0)
            newline = false;
        else if (strcmp(args[i], "-e") == 0)  // ✅ Yeni
            escape = true;
        else
            break;
        i++;
    }

    // Output
    while (args[i])
    {
        if (escape)
            print_with_escapes(args[i]);  // ✅ \n, \t parse et
        else
            ft_putstr_fd(args[i], 1);
        // ...
    }
}
```

**Örnek 2: "History'i dosyaya kaydet"**

```c
// main.c içinde:

void    save_history(void)
{
    int fd = open(".minishell_history", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return;

    // readline'ın history API'sini kullan
    HIST_ENTRY **list = history_list();
    for (int i = 0; list && list[i]; i++)
    {
        ft_putendl_fd(list[i]->line, fd);
    }
    close(fd);
}

// main loop'ta:
while (1)
{
    // ...
    if (line && *line)
        add_history(line);
    // ...
}

// Shell çıkışında:
save_history();  // ✅ Çağrıldı
```

### 13.4 Checklist (Evaluation öncesi)

- [ ] Norm hatası yok (`norminette`)
- [ ] Makefile çalışıyor (re, clean, fclean)
- [ ] Derleme warning'i yok
- [ ] Memory leak yok (valgrind)
- [ ] Tüm mandatory özellikler çalışıyor
- [ ] Bonus yapıldıysa, mükemmel çalışıyor
- [ ] Git history temiz ve anlamlı
- [ ] README.md güncel
- [ ] Test senaryoları hazır
- [ ] Kod açıklanabilir durumda
- [ ] Live coding yapabilecek durumdayım

---

## 14. Ekstra Özellikler (Opsiyonel)

Projenin ötesine geçmek istiyorsanız:

### 14.1 Auto-completion

```c
// readline'ın completion API'si ile:
char **command_completion(const char *text, int start, int end)
{
    return rl_completion_matches(text, command_generator);
}

void    setup_readline(void)
{
    rl_attempted_completion_function = command_completion;
}
```

### 14.2 Colored Prompt

```c
#define COLOR_GREEN "\033[0;32m"
#define COLOR_BLUE  "\033[0;34m"
#define COLOR_RESET "\033[0m"

char *get_prompt(t_shell *shell)
{
    char *cwd = getcwd(NULL, 0);
    char *prompt;

    prompt = ft_strjoin4(
        COLOR_GREEN, getenv("USER"),
        COLOR_BLUE, "@minishell:",
        cwd, "$ " COLOR_RESET
    );

    free(cwd);
    return (prompt);
}
```

### 14.3 Command History Search

```c
// Ctrl+R için reverse-i-search:
void    setup_readline(void)
{
    rl_bind_key('\022', reverse_search);  // Ctrl+R
}
```

### 14.4 Job Control (Advanced)

```bash
# Background jobs (&), fg, bg, jobs commands
minishell> sleep 10 &
[1] 12345
minishell> jobs
[1]+  Running    sleep 10 &
```

---

## 15. Özet ve Son Tavsiyeler

### 🎯 Projenin Özü

Minishell, aslında **3 temel konseptin** birleşimidir:

1. **String Processing**: Lexing, parsing, expansion
2. **Process Management**: fork, exec, wait
3. **I/O Redirection**: pipe, dup2, file descriptors

Bu üçünü iyi anladıysanız, proje %80 hallolmuş demektir.

### 💪 Başarı İçin Altın Kurallar

1. **KISS Prensibi**: Keep It Simple, Stupid

   - Karmaşık çözümler yerine basit ve çalışan kod
   - Over-engineering yapmayın

2. **Test Driven Development**:

   - Önce test senaryosu yaz
   - Sonra kodu yaz
   - Her değişiklik sonrası test et

3. **Incremental Development**:

   - Büyük modülleri küçük parçalara böl
   - Her parça çalışır halde commit et
   - "Çalışan kod > Elegant kod"

4. **Pair Programming**:

   - Zor kısımlarda birlikte kod yazın
   - Birbirinizin kodunu review edin
   - Bilgi paylaşımı yapın

5. **Documentation**:
   - Her fonksiyona comment yazın
   - Karmaşık algoritmaları açıklayın
   - README'yi güncel tutun

### 🚫 Yapmamanız Gerekenler

- ❌ Subject'i atlamayın - her satırı okuyun
- ❌ Bash'i kopyalamaya çalışmayın - sadece gerekenleri yapın
- ❌ Test etmeden commit atmayın
- ❌ Deadline'a çok yaklaşmadan başlamayın
- ❌ Norm'u görmezden gelmeyin
- ❌ Memory leak'leri "sonra hallederim" demeyin

### 📚 Öğrenme Çıkarımları

Bu projeyi tamamladığınızda:

- ✅ Process yönetimini anlamış olacaksınız
- ✅ File descriptor'ların nasıl çalıştığını bileceksiniz
- ✅ Parsing ve AST kavramlarını öğrenmiş olacaksınız
- ✅ Signal handling yapabileceksiniz
- ✅ Memory management konusunda ustalaşacaksınız
- ✅ Shell scripting'i daha iyi anlayacaksınız
- ✅ Sistem programlama tecrübesi kazanacaksınız

### 🎓 Son Söz

Minishell, 42'nin en keyifli projelerinden biridir çünkü **somut ve kullanışlı** bir şey yapıyorsunuz. Kendi shell'inizi yazmak, bir sistem programcısı olarak büyük bir adımdır.

**Unutmayın:**

> "Bir shell yazabiliyorsanız, her şeyi yazabilirsiniz!"

İyi çalışmalar! 🚀

---

## 📞 İletişim ve Destek

**Git Repository:**

- Akivam: https://github.com/suatkvam/minishell
- Harici: https://github.com/hudayiarici/minishell

**Takım İletişimi:**

- Günlük sync: Slack/Discord
- Code review: GitHub PR
- Pair programming: VS Code Live Share

**Yardım Kaynakları:**

- 42 Slack: #minishell-help
- Peers: Evaluationlardan sonra feedback
- Staff: Eğer gerçekten takıldıysanız

---

**Son Güncelleme:** Kasım 2025  
**Versiyon:** 2.0  
**Hazırlayanlar:** harici (suatkvam) & Akivam (hudayiarici)

---

**Good luck and happy coding! 🐚✨**

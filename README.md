# 🐚 Minishell

42 School projesi - Kendi shell'imizi yazıyoruz!

## 📖 Proje Hakkında

Bu proje, bash'in basitleştirilmiş bir versiyonunu implement etmeyi amaçlar. Temel shell fonksiyonlarını, pipe'ları, yönlendirmeleri ve built-in komutları içerir.

## 🏗️ Proje Yapısı

```
minishell/
├── Makefile                # Derleme sistemi (mandatory/bonus)
├── minishell.h             # Ana header (tüm struct'lar)
├── main.c                  # Ana program ve shell loop
├── main_utils.c/2.c        # Main yardımcıları
├── prompt.c/utils.c        # Prompt oluşturma
├── history_manager.c       # History yönetimi
├── config_loader.c/utils.c # Konfigürasyon
│
├── lexer/                  # Tokenization
│   ├── lexer.c/h
│   ├── lexer_operator.c
│   ├── lexer_word.c
│   ├── lexer_token.c
│   ├── lexer_utils.c
│   └── quotes.c
│
├── parser/                 # AST oluşturma
│   ├── parser.c/h
│   ├── parse_cmd.c/utils.c/utils2.c
│   └── build_ast.c/utils.c
│
├── expander/               # Variable expansion
│   ├── expander.c/utils.c/utils2.c/utils3.c
│   ├── expand_args.c/helpers.c
│   └── wildcard.c/utils.c (bonus)
│
├── executor/               # Komut çalıştırma
│   ├── executor.c/h
│   ├── exec_ast.c/cmd.c/utils1.c/utils2.c
│   ├── exec_builtin.c/pipe.c
│   ├── redirections.c/h
│   ├── here_doc_manager.c/utils1.c/utils2.c/utils3.c
│   └── extra_commands/     # Extra özellikler
│       ├── extra_commands.c/h
│       ├── collor_command.c/color_manager.c
│       ├── harici_matrix.c/utils.c
│       ├── pars_vs_executer.c
│       └── set_terminal_name.c
│
├── builtins/               # Built-in komutlar
│   ├── builtins.h
│   ├── builtin_alias_bonus.c (BONUS)
│   ├── builtin_unalias_bonus.c (BONUS)
│   ├── cd/builtin_cd.c
│   ├── echo/builtin_echo.c
│   ├── env/builtin_env.c
│   ├── exit/builtin_exit.c
│   ├── export/builtin_export.c/utils.c
│   ├── pwd/builtin_pwd.c
│   ├── unset/builtin_unset.c
│   └── extras/ (BONUS)
│       ├── builtin_help.c
│       ├── builtin_true_false.c
│       ├── builtin_tty.c
│       └── builtin_type.c
│
├── env/                    # Environment variable yönetimi
│   ├── env_manager.c/utils.c/utils2.c
│   ├── env_array.c
│   ├── env_crypto.c
│   └── parse_path.c
│
├── signals/                # Signal handling
│   ├── signals.c
│   └── signal_state.c
│
├── utils/                  # Yardımcı fonksiyonlar
│   ├── utils.h
│   ├── error.c
│   ├── ft_atoll.c
│   └── strings.c/utils.c
│
├── executor_error/         # Hata yönetimi
│   ├── executor_error.c/h
│   └── executer_error2.c
│
├── libs/                   # Kütüphaneler
│   ├── libft/              # 42 libft
│   └── garbage_collector/  # GC sistemi
│
└── resources/              # Dokümantasyon
    └── mimari_ornegi.md    # Detaylı mimari
```

## ✨ Özellikler

### Zorunlu Özellikler (Mandatory)
- ✅ Prompt gösterme ve komut okuma
- ✅ Çalışan history (↑/↓ tuşları)
- ✅ PATH'ten executable bulma ve çalıştırma
- ✅ Pipe'lar (`|`)
- ✅ Yönlendirmeler (`<`, `>`, `>>`, `<<`)
- ✅ Environment variable expansion (`$VAR`, `$?`)
- ✅ Quote handling (`'` ve `"`)
- ✅ Signal handling (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)

### Zorunlu Built-in Komutlar
- `echo` (with `-n` flag)
- `cd` (relative or absolute path)
- `pwd` (no options)
- `export` (no options)
- `unset` (no options)
- `env` (no options or arguments)
- `exit` (no options)

### Bonus Özellikler (make bonus ile)
- ✅ `&&` ve `||` operatörleri
- ✅ Parantezler `()` ile önceliklendirme (subshell)
- ✅ Wildcard `*` expansion
- ✅ `alias` ve `unalias` komutları
- ✅ Extra built-ins: `help`, `type`, `tty`, `true`, `false`

### Extra Commands (Easter Eggs)
- 🎨 Color management (prompt, text, background)
- 🖥️ Terminal name değiştirme
- 🌧️ Matrix rain efekti (`harici_matrix`)
- 🎭 Easter egg mesajları

## 🚀 Kurulum ve Kullanım

### Derleme

```bash
# Mandatory versiyon (zorunlu özellikler)
make

# Bonus versiyon (tüm özellikler)
make bonus

# Debug mode ile derle
make debug

# Temizlik
make clean      # Object dosyalarını sil
make fclean     # Tümünü sil
make re         # Yeniden derle
```

### Çalıştırma

```bash
# Mandatory versiyon
./minishell

# Bonus versiyon (make bonus sonrası)
./minishell

# Shellrc dosyası ile (otomatik yüklenir)
# ~/.minishellrc dosyasına komutlar ekleyin
```

### Test

```bash
# Memory leak kontrolü (readline hariç)
valgrind --leak-check=full --suppressions=readline.supp ./minishell

# Leak check scripti kullanarak
./leak-check.sh

# Norminette kontrolü
norminette *.c *.h **/*.c **/*.h
```

## 📚 Mimari

Detaylı mimari dokümantasyonu için: [resources/mimari_ornegi.md](resources/mimari_ornegi.md)

### Temel Akış

```
Input → Lexer → Parser → Expander → Executor
          ↓       ↓         ↓          ↓
        Tokens   AST    Expanded    Output
```

1. **Lexer**: Girdiyi token'lara ayırır
2. **Parser**: Token'lardan AST (Abstract Syntax Tree) oluşturur
3. **Expander**: `$VAR` ve `$?` gibi değişkenleri genişletir
4. **Executor**: AST'yi traverse edip komutları çalıştırır

## 🧪 Test Örnekleri

### Mandatory Testler

```bash
# Basit komutlar
minishell> ls -la
minishell> echo "Hello World"
minishell> pwd

# Pipe'lar
minishell> cat Makefile | grep src | wc -l
minishell> ls | grep mini | sort

# Yönlendirmeler
minishell> echo "test" > file.txt
minishell> cat < file.txt > output.txt
minishell> cat << EOF
> line 1
> line 2
> EOF

# Environment variables
minishell> export TEST=123
minishell> echo $TEST
minishell> echo "Value: $TEST"
minishell> echo $?

# Built-ins
minishell> cd /tmp
minishell> pwd
minishell> env | grep USER
minishell> unset TEST
minishell> exit 42
```

### Bonus Testler

```bash
# AND/OR operators
minishell> make && ./minishell
minishell> false || echo "fallback"
minishell> ls && echo "success" || echo "fail"

# Subshell
minishell> (cd /tmp && ls) && pwd
minishell> (echo a; echo b) | cat

# Wildcard
minishell> ls *.c
minishell> echo *.txt

# Alias
minishell> alias ll='ls -la'
minishell> ll
minishell> unalias ll
```

### Extra Commands

```bash
# Terminal customization
minishell> set_terminal_name "My Shell"
minishell> set_prompt_color green
minishell> set_text_color blue
minishell> set_bg_color black

# Easter eggs
minishell> harici_matrix
minishell> pars_vs_executer
minishell> help
minishell> type ls
```

## 👥 Takım

- **Akivam** ([suatkvam](https://github.com/suatkvam)) - Executor, Parser (AST), Built-ins, Environment
- **Harici** ([hudayiarici](https://github.com/hudayiarici)) - Lexer, Parser (CMD), Expander, Signals

## 📝 Notlar

### Teknik Detaylar
- **Global değişken**: Sadece `g_signal` (sig_atomic_t) - subject şartı
- **Memory management**: Garbage collector (arena sistemi)
  - `global_arena`: Shell lifetime boyunca
  - `cmd_arena`: Her komut için yeni arena
- **Memory leak**: Yok (readline hariç kabul edilebilir)
- **Bonus sistemi**: `-DBONUS` flag ile ayrı derleme
- **Object files**: Mandatory ve bonus için ayrı dizinler (obj/, obj_bonus/)

### Özellikler
- **Shellrc**: `~/.minishellrc` otomatik yüklenir
- **History**: Komut geçmişi kalıcı olarak saklanır
- **Config**: Terminal renkleri ve isim özelleştirilebilir
- **Extra commands**: Easter egg komutlar ve matrix efekti

## 🔗 Kaynaklar

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/)
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html)
- [42 Docs - Minishell](https://harm-smits.github.io/42docs/projects/minishell)

## 📄 Lisans

Bu proje 42 School'un bir parçasıdır.

---

**"As beautiful as a shell" 🐚**

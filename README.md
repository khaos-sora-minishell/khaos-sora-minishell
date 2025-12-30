# 🐚 Minishell

42 School projesi - Kendi shell'imizi yazıyoruz!

## 📖 Proje Hakkında

Bu proje, bash'in basitleştirilmiş bir versiyonunu implement etmeyi amaçlar. Temel shell fonksiyonlarını, pipe'ları, yönlendirmeleri ve built-in komutları içerir.

## 🏗️ Proje Yapısı

```
minishell/
├── Makefile
├── includes/               # Header dosyaları
│   ├── minishell.h        # Ana header (tüm struct'lar ve prototipler)
│   ├── lexer.h
│   ├── parser.h
│   ├── expander.h
│   ├── executor.h
│   ├── builtins.h
│   ├── redirections.h
│   └── utils.h
├── src/                   # Kaynak dosyaları
│   ├── main/             # Ana program ve signal handling
│   ├── lexer/            # Tokenization
│   ├── parser/           # AST oluşturma
│   ├── expander/         # Variable expansion
│   ├── executor/         # Komut çalıştırma
│   ├── builtins/         # Built-in komutlar
│   ├── env/              # Environment variable yönetimi
│   └── utils/            # Yardımcı fonksiyonlar
├── libs/                  # Kütüphaneler
│   ├── libft/            # 42 libft
│   ├── ft_printf/        # Printf implementasyonu
│   ├── garbage_collector/# Garbage collector
│   └── get-next-line/    # GNL
└── resources/            # Dokümantasyon
    └── mimari_ornegi.md  # Detaylı mimari dokümantasyonu
```

## ✨ Özellikler

### Zorunlu Özellikler
- ✅ Prompt gösterme ve komut okuma
- ✅ Çalışan history (↑/↓ tuşları)
- ✅ PATH'ten executable bulma ve çalıştırma
- ✅ Pipe'lar (`|`)
- ✅ Yönlendirmeler (`<`, `>`, `>>`, `<<`)
- ✅ Environment variable expansion (`$VAR`, `$?`)
- ✅ Quote handling (`'` ve `"`)
- ✅ Signal handling (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)

### Built-in Komutlar
- `echo` (with `-n` flag)
- `cd` (relative or absolute path)
- `pwd` (no options)
- `export` (no options)
- `unset` (no options)
- `env` (no options or arguments)
- `exit` (no options)

### Bonus Özellikler (İsteğe Bağlı)
- `&&` ve `||` operatörleri
- Parantezler `()` ile önceliklendirme
- Wildcard `*` expansion

## 🚀 Kurulum ve Kullanım

### Derleme

```bash
# Projeyi derle
make

# Debug mode ile derle (address sanitizer ile)
make debug

# Temizlik
make clean      # Object dosyalarını sil
make fclean     # Tümünü sil
make re         # Yeniden derle
```

### Çalıştırma

```bash
./minishell
```

### Test

```bash
# Valgrind ile memory leak kontrolü
make valgrind

# Norminette kontrolü
make norm
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

```bash
# Basit komutlar
minishell> ls -la
minishell> echo "Hello World"

# Pipe'lar
minishell> cat Makefile | grep src | wc -l

# Yönlendirmeler
minishell> echo "test" > file.txt
minishell> cat < file.txt
minishell> cat << EOF

# Environment variables
minishell> export TEST=123
minishell> echo $TEST
minishell> echo $?

# Built-ins
minishell> cd /tmp
minishell> pwd
minishell> env | grep USER
```

## 👥 Takım

- **Akivam** ([suatkvam](https://github.com/suatkvam)) - Executor, Parser (AST), Built-ins, Environment
- **Harici** ([hudayiarici](https://github.com/hudayiarici)) - Lexer, Parser (CMD), Expander, Signals

## 📝 Notlar

- Global değişken sadece `g_signal` (sig_atomic_t) - subject şartı
- Memory leak yok (readline hariç)
- Garbage collector ile otomatik memory management
- Her komut için yeni arena oluşturulur

## 🔗 Kaynaklar

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/)
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html)
- [42 Docs - Minishell](https://harm-smits.github.io/42docs/projects/minishell)

## 📄 Lisans

Bu proje 42 School'un bir parçasıdır.

---

**"As beautiful as a shell" 🐚**

# Minishell - Değerlendirme Hazırlığı

## Yapılan Düzeltmeler

### ✅ 1. PATH Unset Sorunu Düzeltildi
**Dosya:** `env/parse_path.c:24`

**Önceki Kod:**
```c
if (!path)
    path = DEFAULT_PATH_VALUE;  // YANLIŞ!
```

**Yeni Kod:**
```c
if (!path)
    return (NULL);  // DOĞRU! PATH yoksa NULL dön
```

**Neden Önemli:**
- Değerlendirme sayfasına göre, PATH unset edildiğinde SADECE builtin komutlar çalışmalı
- `ls`, `cat` gibi komutlar "command not found" vermeli
- `echo`, `cd`, `pwd` gibi builtinler çalışmaya devam etmeli
- `/bin/ls` gibi absolute path'ler çalışmaya devam etmeli

**Test:**
```bash
./minishell
minishell$ unset PATH
minishell$ ls           # Hata vermeli: command not found
minishell$ echo hello   # Çalışmalı (builtin)
minishell$ /bin/ls      # Çalışmalı (absolute path)
```

### ✅ 2. Bonus Özellikleri Aktif
Projeniz `make bonus` ile derlendi. Şu bonus özellikler aktif:
- `&&` (AND) operatörü
- `||` (OR) operatörü
- `()` (parantez) ile öncelik
- `*` (wildcard) desteği

## Global Değişkenler

✅ **DOĞRU:** Sadece 1 global değişken var (`g_signal` - signals/signal_state.c:15)
- Bu signal handling için gerekli ve kabul edilebilir
- Değerlendirmede "kaç global değişken var ve neden" diye soracaklar
- Cevap: "Sadece 1 tane, signal handler için gerekli çünkü signal handler'lar global state'e ihtiyaç duyar"

## Memory Leak Kontrolü

### Test Etmek İçin:
```bash
# Basit test
echo "exit" | valgrind --leak-check=full --suppressions=readline.supp ./minishell

# Komutlarla test
echo -e "echo hello\nls\nexit" | valgrind --leak-check=full --suppressions=readline.supp ./minishell

# Veya Makefile target kullan
make valgrind
```

### Potansiyel Leak Kaynakları:
1. `prompt.c:62` - `join_lines()` malloc kullanıyor ama free ediliyor ✓
2. `here_doc_utils.c:76` - `ft_strtrim()` malloc kullanıyor ama free ediliyor ✓
3. readline() dönüş değerleri - main.c:95'te free ediliyor ✓

Tüm memory yönetimi garbage collector veya manuel free ile yapılıyor, ancak valgrind ile kesin kontrol öneriyorum.

## Mandatory Özellikler Durumu

| Özellik | Durum | Dosya |
|---------|-------|-------|
| Derleme (-Wall -Wextra -Werror) | ✅ | Makefile:63 |
| Global değişkenler | ✅ (1 tane) | signals/signal_state.c:15 |
| Basit komutlar | ✅ | executor/exec_cmd.c |
| Argümanlar | ✅ | - |
| echo (+ echo -n) | ✅ | builtins/echo/builtin_echo.c |
| exit | ✅ | builtins/exit/builtin_exit.c |
| Return value ($?) | ✅ | shell->exit_status |
| Sinyaller (Ctrl-C/D/\\) | ✅ | signals/signals.c |
| Çift tırnak (") | ✅ | lexer/quotes.c |
| Tek tırnak (') | ✅ | lexer/quotes.c |
| env | ✅ | builtins/env/builtin_env.c |
| export | ✅ | builtins/export/builtin_export.c |
| unset | ✅ | builtins/unset/builtin_unset.c |
| cd | ✅ | builtins/cd/builtin_cd.c |
| pwd | ✅ | builtins/pwd/builtin_pwd.c |
| Relative path | ✅ | executor/exec_cmd_utils.c |
| PATH | ✅ FIXED | env/parse_path.c |
| Redirection (<, >, >>) | ✅ | executor/redirections.c |
| Heredoc (<<) | ✅ | executor/here_doc_manager.c |
| Pipes (\|) | ✅ | executor/exec_pipe.c |
| History | ✅ | history_manager.c |
| Env variables ($VAR) | ✅ | expander/expander.c |

## Bonus Özellikler Durumu

| Özellik | Durum | Dosya |
|---------|-------|-------|
| && operatörü | ✅ | parser/build_ast.c (TOKEN_AND) |
| \|\| operatörü | ✅ | parser/build_ast.c (TOKEN_OR) |
| Parantez () | ✅ | parser (TOKEN_LPAREN/RPAREN) |
| Wildcards (*) | ✅ | expander/wildcard.c |

## Test Scriptleri

### Mandatory Testler:
```bash
./test_mandatory.sh
```

### Bonus Testler:
```bash
./test_bonus.sh
```

### Manuel Test Edilmesi Gerekenler:
1. **Sinyaller:**
   - Boş promptta Ctrl-C → Yeni satır ve yeni prompt
   - Boş promptta Ctrl-\\ → Hiçbir şey yapmamalı
   - Boş promptta Ctrl-D → Shell'den çıkmalı
   - Komut yazılıyken Ctrl-C → Yeni satır ve yeni prompt
   - Blocking komutta (cat) Ctrl-C → Komutu sonlandırmalı
   - Blocking komutta Ctrl-\\ → "Quit (core dumped)" mesajı

2. **History:**
   - Yukarı/aşağı ok tuşları ile komut geçmişinde gezinme

3. **Heredoc:**
   ```bash
   cat << EOF
   test
   EOF
   ```

4. **Çoklu Heredoc (bash ile karşılaştır):**
   ```bash
   cat << eof << a << b > d | wc -l
   ```

## Memory Leak Testi

### Readline Suppression
`readline.supp` dosyanız zaten var ve doğru yapılandırılmış. Readline ve ncurses leak'lerini bastırıyor.

### Test Komutu:
```bash
# Terminal 1 - Minishell çalıştır
make valgrind

# Minishell içinde:
minishell$ echo hello
minishell$ ls
minishell$ cat << EOF
> test line
> EOF
minishell$ exit

# Çıktıda "definitely lost: 0 bytes" görmek istiyorsunuz
```

## Değerlendirme İçin Hazırlık

### 1. Projeyi Derle
```bash
make bonus    # Bonus özelliklerle derle
```

### 2. Temel Testleri Çalıştır
```bash
./test_mandatory.sh
./test_bonus.sh
```

### 3. Memory Leak Kontrolü
```bash
make valgrind
# Veya:
echo -e "echo test\nls\nexit" | valgrind --leak-check=full --suppressions=readline.supp ./minishell
```

### 4. Bilinen Sorunlar/Sorular

**Global Değişken Sorusu:**
- "Kaç global değişken kullanıyorsunuz?"
  - Cevap: 1 tane (`g_signal`)
- "Neden kullanıyorsunuz?"
  - Cevap: Signal handler'lar asenkron çalıştığı için global state'e ihtiyaç var. SIGINT (Ctrl-C) yakalandığında diğer fonksiyonlara bilgi vermek için kullanılıyor.

**PATH Unset Sorusu:**
- "PATH unset edilince ne olur?"
  - Cevap: Sadece builtin komutlar çalışır. `ls` gibi external komutlar "command not found" hatası verir. Ancak `/bin/ls` gibi absolute path'ler çalışır.

**Heredoc Sorusu:**
- "Heredoc history'ye ekleniyor mu?"
  - Cevap: Hayır, heredoc input'u history'ye eklenmiyor (subject'te belirtildiği gibi).

## env -i Durumu

**NOT:** `env -i` komutu değerlendirme sayfasında belirtilen bir test değil. User notlarınızda bahsedilmiş ama mandatory requirements'ta yok. Eğer sorarlarsa:
- `env -i` boş environment ile çalıştırma demek
- Şu anki implementasyonda desteklenmiyor
- Bu bir bonus/extra özellik olabilir

## Son Kontrol Listesi

- [ ] `make bonus` ile derle
- [ ] `./test_mandatory.sh` çalıştır - tüm testler geçmeli
- [ ] `./test_bonus.sh` çalıştır - tüm testler geçmeli
- [ ] Manuel sinyal testleri yap (Ctrl-C, Ctrl-D, Ctrl-\\)
- [ ] History test et (yukarı/aşağı oklar)
- [ ] Valgrind ile memory leak kontrolü yap
- [ ] `unset PATH` sonrası sadece builtinlerin çalıştığını test et

## Yardımcı Komutlar

```bash
# Hızlı derleme ve test
make bonus && ./test_mandatory.sh

# Memory leak testi
make bonus && make valgrind

# Norminette kontrolü (eğer varsa)
make norm
```

## Özet

✅ **Hazır Özellikler:** Tüm mandatory + bonus features
✅ **Düzeltilen:** PATH unset davranışı
⚠️  **Kontrol Et:** Memory leaks (valgrind ile)
⚠️  **Test Et:** Multiple heredoc davranışı bash ile karşılaştır

Başarılar! 🚀

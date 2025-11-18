Anladım. Önceki detaylı ve görselliği zengin dokümanı, en son konuştuğumuz dengelenmiş iş bölümü ve güncellenmiş `struct` yapıları ile birleştirerek size nihai, tek bir `.md` dosyası olarak sunuyorum.

Bu doküman, projeniz için "tek doğru kaynak" (single source of truth) olarak hizmet edebilir.

---

# 🐚 Minishell: Mimarisi ve Teknik Dokümantasyonu (Revize Edilmiş)

Bu doküman, Minishell projesinin genel mimarisini, kullanılacak veri yapılarını, modüllerin sorumluluklarını ve ekip için önerilen geliştirme planını açıklamaktadır. Bu versiyon, takım üyelerinin tecrübeleri ve Parser modülünün karmaşıklığı göz önüne alınarak yeniden dengelenmiş iş bölümünü içermektedir.

## 📋 İçindekiler

1.  [Genel Mimari](#1-genel-mimari)
2.  [Veri Yapıları (Structs)](#2-veri-yapıları-structs)
3.  [Dosya Organizasyonu](#3-dosya-organizasyonu)
4.  [Yeniden Dengelenmiş İş Bölümü](#4-yeniden-dengelenmiş-i̇ş-bölümü)
5.  [Modül Detayları ve Sorumluluklar](#5-modül-detayları-ve-sorumluluklar)
6.  [Geliştirme Yol Haritası](#6-geliştirme-yol-haritası)
7.  [Önemli Dikkat Noktaları](#7-önemli-dikkat-noktaları)
8.  [Test Senaryoları](#8-test-senaryoları)
9.  [Kaynaklar](#9-kaynaklar)

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
└─────────────────────────────────────────────────────────┘
```

---

## 2. Veri Yapıları (Structs)

Bu yapılar, `includes/minishell.h` içerisinde tanımlanmalıdır.

#### Token Yapısı (Lexer Çıktısı)

```c
typedef enum e_token_type
{
    TOKEN_WORD,         // Normal kelime veya argüman
    TOKEN_PIPE,         // |
    TOKEN_REDIR_IN,     // <
    TOKEN_REDIR_OUT,    // >
    TOKEN_REDIR_APPEND, // >>
    TOKEN_HEREDOC,      // <<
    TOKEN_AND,          // && (bonus)
    TOKEN_OR,           // || (bonus)
    TOKEN_LPAREN,       // ( (bonus)
    TOKEN_RPAREN,       // ) (bonus)
    TOKEN_EOF           // Girdinin sonu
}   t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
}   t_token;
```

#### AST (Abstract Syntax Tree) Yapıları (Parser Çıktısı)

Bu yapılar, komutların hiyerarşisini ve ilişkilerini temsil eder. **Bu, projenin en kritik veri yapısıdır.**

```c
// Yönlendirme bilgilerini tutan yapı
typedef struct s_redir
{
    t_token_type    type;
    char            *file;
    struct s_redir  *next;
}   t_redir;

// Tek bir komutu (argümanları ve yönlendirmeleriyle) temsil eden yapı
typedef struct s_cmd
{
    char            **args;
    t_redir         *redirs;
}   t_cmd;

// AST düğüm tipleri
typedef enum e_node_type
{
    NODE_CMD,
    NODE_PIPE,
    NODE_AND,
    NODE_OR,
    NODE_SUBSHELL
}   t_node_type;

// AST'nin ana düğüm yapısı (recursive)
typedef struct s_ast_node
{
    t_node_type         type;
    struct s_ast_node   *left;
    struct s_ast_node   *right;

    // Sadece type == NODE_CMD ise kullanılır
    t_cmd               *cmd;

    // Sadece type == NODE_SUBSHELL ise kullanılır
    struct s_ast_node   *subshell_node;
}   t_ast_node;
```

#### Yardımcı Yapılar

```c
// Ortam değişkenlerini tutmak için
typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

// Shell'in genel durumunu tutmak için
typedef struct s_shell
{
    t_env       *env_list;
    int         exit_status;
    // GC referansı buraya eklenebilir
}   t_shell;
```

---

## 3. Dosya Organizasyonu

```
minishell/
├── Makefile
├── includes/
│   ├── minishell.h         # Ana header, tüm struct'lar ve prototipler
│   └── ...                 # Diğer modül header'ları
├── src/
│   ├── main/               # Ana döngü, başlangıç ayarları
│   ├── lexer/              # Tokenizer, quote yönetimi
│   ├── parser/             # AST oluşturma, syntax kontrolü
│   ├── expander/           # $VAR, *, $? genişletmeleri
│   ├── executor/           # Komut yürütme, pipe, redirection
│   ├── builtins/           # cd, echo, exit, vb.
│   ├── env/                # Ortam değişkeni yönetimi
│   └── utils/              # Yardımcı fonksiyonlar (hata, string vb.)
├── garbage_collector/      # GC kütüphaneniz
└── libft/                  # Libft kütüphaneniz
```

---

## 4. Yeniden Dengelenmiş İş Bölümü

Parser'ın karmaşıklığı ve ekip üyelerinin tecrübeleri göz önüne alınarak, görevler aşağıdaki gibi yeniden dağıtılmıştır:

#### **`harici`'nin Görevleri (Hazırlık ve Kontrol)**

1.  **Lexer (Tamamı):** Girdiyi token'lara ayırma.
2.  **Parser (Basit Komut Seviyesi):** Token listesi içindeki basit komut bloklarını ayrıştırıp `t_cmd` yapıları oluşturma (Ağacın "yaprakları").
3.  **Expander (Tamamı):** AST üzerinde gezinerek `$VAR`, `$?`, `*` ifadelerini genişletme.
4.  **Sinyal Yönetimi (Tamamı):** `Ctrl+C`, `Ctrl+D` gibi sinyalleri yönetme.

#### **`Akivam`'ın Görevleri (İcra ve Yapılandırma)**

1.  **Executor (Tamamı):** `Pipex` tecrübesiyle AST'yi çalıştırma, pipe'ları, yönlendirmeleri ve alt kabukları yönetme.
2.  **Parser (Ağaç Kurulumu):** `harici`'nin oluşturduğu `t_cmd` bloklarını ve operatör token'larını kullanarak AST'nin genel ağaç yapısını kurma (Ağacın "dalları ve gövdesi").
3.  **Built-ins (Tamamı):** `cd`, `export`, `exit` gibi yerleşik komutları yazma.
4.  **Ortam Değişkeni Yönetimi:** `t_env` listesini ve ilgili built-in'leri (`env`, `export`, `unset`) yönetme.

---

## 5. Modül Detayları ve Sorumluluklar

*   **MAIN**: Readline ile girdi alır, `history`'yi yönetir ve ana döngüyü çalıştırır.
*   **LEXER**: Tırnak işaretlerini (`'` ve `"`) doğru işleyerek metni tek bir `TOKEN_WORD` olarak alır.
*   **PARSER**: "Recursive Descent" metodolojisi ile sözdizimi hatalarını kontrol eder ve AST'yi kurar.
*   **EXPANDER**: Tırnak içi (`"`)/dışı durumuna göre genişletme kurallarını uygular.
*   **EXECUTOR**: AST üzerinde özyineli (recursive) olarak gezinir. Karşılaştığı düğüm tipine göre (`NODE_PIPE`, `NODE_CMD` vb.) `fork`, `pipe`, `dup2` gibi sistem çağrılarını yapar.
*   **BUILT-INS**: `fork` işlemi gerektirmeyen, ana `minishell` prosesini doğrudan etkilemesi gereken komutlardır (`cd`, `exit`, `export`, `unset`).

---

## 6. Geliştirme Yol Haritası

1.  **Faz 1: İskelet Kurulumu**
    *   **Ortak:** `Makefile`, GC entegrasyonu, `main` döngüsü, temel `struct`'lar.
    *   **Hedef:** Shell'e `ls` yazıldığında çalışması.

2.  **Faz 2: Basit Komutlar ve Argümanlar**
    *   **harici:** `Lexer`'ı (tırnak desteği) ve `Parser`'ın basit komut ayrıştırma kısmını yazar.
    *   **Akivam:** `Executor`'ı (argüman ve `PATH` desteği) ve basit `built-in`'leri (`echo`, `pwd`) geliştirir.
    *   **Hedef:** `ls -l` ve `echo "hello world"` komutlarının çalışması.

3.  **Faz 3: Pipe, Yönlendirme ve Genişletme**
    *   **harici:** `Expander` modülünü (`$VAR`, `$?`) geliştirir.
    *   **Akivam:** `Parser`'ın ağaç kurma mantığını (`|`) ve `Executor`'ın pipe/yönlendirme mantığını ekler.
    *   **Hedef:** `cat file | grep "str"` ve `echo $USER > file` gibi komutların çalışması.

4.  **Faz 4: Bonuslar ve Sağlamlaştırma**
    *   **Ortak:** `&&`, `||`, `()` ve `*` gibi bonus özellikler üzerinde birlikte çalışılır.
    *   **harici:** Sinyal yönetimini tamamlar.
    *   **Akivam:** Kalan `built-in`'leri ve `heredoc`'u tamamlar.
    *   **Hedef:** Projenin tüm gereksinimlerini karşılamak.

---

## 7. Önemli Dikkat Noktaları

*   **Bellek Yönetimi**: Tüm `malloc`'lar `garbage_collector` üzerinden yapılmalıdır. `readline`'dan dönen satır gibi harici kütüphane ayırmaları manuel olarak `free` edilmelidir.
*   **Dosya Tanımlayıcı (FD) Yönetimi**: Yönlendirme yapmadan önce `STDIN`/`STDOUT`'u yedekleyin ve işlem bittikten sonra geri yükleyin. Açılan tüm FD'lerin kapatıldığından emin olun.
*   **Hata Yönetimi**: `perror` ve `strerror` kullanarak Bash'e benzer, anlamlı hata mesajları üretin. `exit_status`'u her komuttan sonra doğru şekilde güncelleyin.
*   **Heredoc (`<<`)**: Girdi `delimiter` ile eşleşene kadar `readline` ile okunmalı ve bir `pipe` veya geçici dosya aracılığıyla komutun `STDIN`'ine yönlendirilmelidir.

---

## 8. Test Senaryoları

*   **Basit Komutlar**: `ls`, `ls -la /tmp`, `wc -l`
*   **Tırnak İşaretleri**: `echo "hello   world"`, `echo 'hello $USER'`
*   **Yönlendirmeler**: `cat < file1 > file2`, `ls >> log.txt`, `cat << EOF`
*   **Pipe'lar**: `cat Makefile | grep src | wc -l`
*   **Ortam Değişkenleri**: `export VAR=test`, `echo $VAR`, `unset VAR`, `echo $?`
*   **Bonuslar**: `ls && echo ok || echo fail`, `(cd /tmp && ls)`

---

## 9. Kaynaklar

*   **Bash Manual**: `https://www.gnu.org/software/bash/manual/`
*   **POSIX Shell**: `https://pubs.opengroup.org/onlinepubs/9699919799/`
*   **Sistem Çağrıları**: `man fork`, `man execve`, `man pipe`, `man dup2`, `man sigaction`
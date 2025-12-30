Minishell Projesini Nasıl Bölmeli?
1. Ana Modüller ve İş Bölümü:

Modül 1: Lexer & Parser (Takım Arkadaşınız)

    Tokenization (input'u parçalara ayırma)
    Quote handling (' ve ")
    Syntax validation
    Command structure oluşturma

Modül 2: Executor & Builtins (Siz)

    Built-in komutlar (cd, echo, pwd, export, unset, env, exit)
    External command execution (execve)
    PATH handling

Modül 3: Redirections & Pipes (Beraber veya bölüşümlü)

    Input/Output redirections (<, >, >>)
    Pipes (|) implementation
    Heredoc (<<)
    File descriptor management

Modül 4: Environment & Variables (Beraber)

    Environment variables yönetimi
    $VAR expansion
    $? (exit status)
    Export/unset functionality

2. Recursive ve Subshell İçin Mimari:

minishell/
├── src/
│   ├── lexer/          # Tokenization
│   ├── parser/         # AST oluşturma (recursive descent parser)
│   ├── executor/       # Command execution (recursive için önemli)
│   ├── builtins/       # Built-in commands
│   ├── redirections/   # I/O redirections
│   ├── signals/        # Signal handling
│   ├── env/           # Environment management
│   └── utils/         # Helper functions
├── includes/
└── Makefile

3. Recursive ve Subshell Stratejisi:

Subshell için:

    Parentezler içindeki komutları tespit edin: (command)
    Fork ile yeni process oluşturun
    Child process'te komutları execute edin
    Parent environment'ı etkilememeli

Recursive yapı için:

    AST (Abstract Syntax Tree) kullanın
    Parser'ı recursive descent olarak yazın
    Execute function'ını recursive yapın

4. Önerilen Çalışma Planı:

Hafta 1-2: Temel Yapı

    Takım arkadaşınız: Basic lexer + tokenizer
    Siz: Temel executor + 2-3 builtin

Hafta 3-4: Core Features

    Takım arkadaşınız: Parser + AST
    Siz: Tüm builtins + redirections

Hafta 5-6: Advanced Features

    İkisi birlikte: Pipes implementation
    Siz: Subshell logic (fork management)
    Takım arkadaşınız: Recursive parser improvements

Hafta 7-8: Bonus & Testing

    Wildcard (*) implementation
    Logical operators (&& ve ||)
    Extensive testing

5. Kritik Noktalar:

    Signal Handling:
        Ctrl+C, Ctrl+D, Ctrl+\
        Interactive vs non-interactive mode

    Memory Management:
        Her allocation için free
        Valgrind ile sürekli test

    Error Handling:
        Exit status ($?)
        Error messages bash ile aynı olmalı

# Minishell Dosya Dokümantasyonu

Bu belge, projedeki her kaynak dosya ve içindeki fonksiyonların açıklamasını içerir.

## Ana Dosyalar

### main.c
Minishell programının giriş noktası. Shell'i başlatır, sinyal yöneticilerini kurar ve ana REPL döngüsünü çalıştırır. İnteraktif, non-interactive ve script mode desteği sağlar.

**Fonksiyonlar:**
- `run_shell_loop` - İnteraktif mod için ana REPL döngüsü, kullanıcı girdisini okur ve işler
- `run_file_mode` - Script mode için dosyadan komutları satır satır okur ve çalıştırır (shebang #!/bin/bash desteği dahil)
- `main` - Ana giriş noktası, argc > 1 ise script mode, aksi halde interaktif mod çalıştırır

### main_utils.c
Shell başlatma, temizlik ve girdi işleme yardımcı fonksiyonları.

**Fonksiyonlar:**
- `init_shell` - Shell yapısını argc/argv, garbage collector'lar, ortam ve geçmişle başlatır
- `clean_loop` - Komutlar arası kaynakları temizler ve sinyal durumunu sıfırlar
- `cleanup_shell` - Shell çıkışında son temizlik (geçmişi kaydeder, GC'leri yok eder)
- `cleanup_child_process` - Alt süreç temizliği (history, GC, fd'ler)
- `process_input` - Kullanıcı girdisini lexer, parser ve executor üzerinden işler

### prompt.c
Prompt oluşturma ve görüntüleme işlemlerini yönetir. Terminal adı ile renklendirilmiş promptlar oluşturur ve çok satırlı girdi okumayı yönetir. İnteraktif ve non-interactive modlar için girdi okuma desteği sağlar.

**Fonksiyonlar:**
- `build_prompt_colors` - Terminal adı renkleri ve arkaplan renkleriyle prompt dizesi oluşturur
- `get_prompt` - Renkler ve sıfırlama kodlarıyla tam prompt dizesi oluşturur
- `join_lines` - İki girdi satırını yeni satır karakteriyle birleştirir
- `remove_trailing_newline` - Non-interactive modda okunan satırdaki sonundaki yeni satır karakterini kaldırır
- `read_continuation` - Kapatılmamış tırnaklar için devam satırları okur (terminal ve non-interactive mod desteği)
- `read_multiline` - Kapatılmamış tırnaklar için çok satırlı girdiyi destekleyen ana okuma fonksiyonu (terminal ve non-interactive mod desteği)

### history_manager.c
Readline kullanarak komut geçmişini yönetir. Başlangıçta geçmişi dosyadan yükler ve çıkışta kaydeder.

**Fonksiyonlar:**
- `get_history_path` - Geçmiş dosyasının yolunu döndürür (~/.shell_history)
- `init_history` - Geçmiş dosyasından yükleyerek geçmişi başlatır
- `save_history_file` - Komut geçmişini dosyaya kaydeder

### config_loader.c
~/.shellrc dosyasından shell yapılandırmasını yükler ve yönetir. Terminal özelleştirme ayarlarını ve aliasları işler.

**Fonksiyonlar:**
- `process_config_line` - .shellrc config dosyasından tek bir satırı işler
- `load_shellrc` - ~/.shellrc dosyasından komutları yükler ve çalıştırır
- `create_shellrc` - Yoksa yeni bir .shellrc dosyası oluşturur
- `save_to_shellrc` - .shellrc dosyasına bir satır ekler

---

## Lexer Modülü (lexer/)

### lexer.c
Ana lexer uygulaması. Ham girdi dizelerini ayrıştırma için tokenlara dönüştürür.

**Fonksiyonlar:**
- `check_unclosed_quotes` - Kapatılmamış tırnakları kontrol eder ve bulunursa hata yazdırır
- `process_operator` - Operatör tokenlerini işler (|, <, >, <<, >>, &&, ||, vb.)
- `process_word` - Kelime tokenlerini işler
- `lexer` - Girdi dizesini tokenize eden ana lexer fonksiyonu
- `check_unclosed_quotes_char` - Kapatılmamış tırnak karakterini veya 0 döndürür
- `parse_alias_value` - Alias değerini tokenlere ayrıştırır
- `append_tokens` - Kullanıcı tokenlerini alias tokenlerine ekler
- `expand_alias_if_needed` - İlk token bir alias'a eşleşirse alias'ı genişletir

### lexer_operator.c
Operatörlerin (pipe, yönlendirme, &&, ||, parantez) tokenizasyonunu işler. Hem zorunlu hem de bonus operatörleri destekler.

**Fonksiyonlar:**
- `handle_double_op` - İki karakterli operatörleri işler (<<, >>, &&, ||)
- `handle_single_op` - Tek karakterli operatörleri işler (|, <, >, (, ))
- `handle_operator` - Önce çift sonra tek operatörleri deneyen ana operatör işleyici

### lexer_word.c
Kelime tokenlerini işler, tırnaklı dizileri ve özel karakterleri yönetir. Token değeri çıkarma işlemlerini gerçekleştirir.

**Fonksiyonlar:**
- `skip_quoted` - Girdideki tırnaklı bölümleri atlar
- `find_word_end` - Bir kelime tokeninin bitiş konumunu bulur
- `extract_word` - Girdi dizesinden bir kelime çıkarır

### lexer_token.c
Token oluşturma ve yönetim yardımcı fonksiyonları. Token oluşturma ve operatör türlerini string değerlerine dönüştürme fonksiyonları sağlar.

**Fonksiyonlar:**
- `create_token` - Verilen tip ve değerle yeni bir token oluşturur
- `add_token` - Token listesinin sonuna bir token ekler
- `get_operator_value` - Operatör token tipinin string gösterimini döndürür

### quotes.c
Lexer için tırnak işleme mantığı. Tek ve çift tırnakları yönetir ve doğru string ayrıştırmasını sağlar.

**Fonksiyonlar:**
- `handle_quotes` - Ayrıştırma sırasında tırnak doğrulamasını işler
- `copy_quoted_content` - Tırnak içindeki içeriği sonuç dizesine kopyalar
- `remove_quotes` - Bir dizeden tırnak karakterlerini kaldırır

### lexer_utils.c
Lexer için yardımcı fonksiyonlar. Alias genişletme ve token birleştirme işlemlerini destekler.

**Fonksiyonlar:**
- `check_unclosed_quotes_char` - Kapatılmamış tırnak karakterini döndürür veya 0
- `parse_alias_value` - Alias değerini tokenlere ayrıştırır
- `append_tokens` - Kullanıcı tokenlerini alias tokenlerine ekler
- `expand_alias_if_needed` - İlk token bir alias ise genişletir

---

## Parser Modülü (parser/)

### parser.c
Ana parser arayüzü. Token akışını çalıştırma için soyut sözdizimi ağacına (AST) dönüştürür.

**Fonksiyonlar:**
- `validate_token_syntax` - Tekil tokenların sözdizimini doğrular
- `check_syntax` - Token listesinin genel sözdizimini kontrol eder
- `parser` - Sözdizimini doğrular ve AST oluşturan ana parser fonksiyonu

### parse_cmd.c
Argümanlar ve yönlendirmelerle basit komutları ayrıştırır. Tokenlerdan komut yapıları oluşturur.

**Fonksiyonlar:**
- `add_arg_to_cmd` - Komutun argüman dizisine bir argüman ekler
- `parse_redirection` - Bir yönlendirme tokenini ayrıştırır ve komuta ekler
- `process_current_token` - Mevcut tokeni işler (yönlendirme veya kelime)
- `parse_simple_command` - Argümanlar ve yönlendirmelerle basit bir komut ayrıştırır

### parse_cmd_utils.c
Komut ayrıştırma için yardımcı fonksiyonlar. Token tipi kontrolü ve komut/yönlendirme oluşturma yardımcılarını içerir.

**Fonksiyonlar:**
- `is_redirection_token` - Token tipinin yönlendirme olup olmadığını kontrol eder
- `is_operator_token` - Token tipinin operatör olup olmadığını kontrol eder
- `create_cmd` - Yeni bir komut yapısı oluşturur ve başlatır

### parse_cmd_utils2.c
Ek komut ayrıştırma yardımcı fonksiyonları. Yönlendirme oluşturma ve listesi yönetimi.

**Fonksiyonlar:**
- `create_redir` - Tip ve dosya/delimiter ile yeni yönlendirme yapısı oluşturur
- `add_redir` - Yönlendirme listesinin sonuna bir yönlendirme ekler

### build_ast.c
Tokenlerdan soyut sözdizimi ağacını oluşturur. Ana AST oluşturma mantığını yönetir.

**Fonksiyonlar:**
- `parse_primary` - Birincil ifadeleri ayrıştırır (komutlar veya alt kabuklar)
- `build_ast` - Tokenlerdan tam Soyut Sözdizim Ağacını oluşturur

### build_ast_utils.c
AST oluşturma yardımcı fonksiyonları. Bonus özellik desteği ile mantıksal operatör ayrıştırma, pipe ayrıştırma ve birincil ifade işleme sağlar.

**Fonksiyonlar:**
- `new_ast_node` - Verilen tipte yeni bir AST düğümü oluşturur
- `parse_logic` - En yüksek öncelik seviyesinde mantıksal operatörleri (&&, ||) ayrıştırır
- `parse_pipe` - İkinci öncelik seviyesinde pipe operatörlerini ayrıştırır

---

## Expander Modülü (expander/)

### expander.c
Ana genişletme koordinatörü. Komut argümanları için değişken genişletme, tilde genişletme, tırnak kaldırma ve wildcard işlemeyi yönetir.

**Fonksiyonlar:**
- `process_expansion` - Değişken genişletme, tilde genişletme, tırnaklar ve literal karakterleri işler
- `update_len` - Genişletilmiş dize için gerekli uzunluğu hesaplar (tilde desteği dahil)
- `get_expanded_length` - Genişletme için gereken toplam uzunluğu döndürür
- `expand_string` - Bir dizede değişkenleri, tilde'ları ve tırnakları genişletme ana fonksiyonu

### expander_utils.c
Genişletme yardımcı fonksiyonları. Genişletme sırasında string manipülasyonu için yardımcı fonksiyonlar sağlar.

**Fonksiyonlar:**
- `extract_var_name` - Konumdan başlayarak dizeden değişken adını çıkarır
- `expand_exit_status` - $? değerini çıkış durumu değerine genişletir
- `expand_arg_count` - $# değerini argüman sayısına genişletir
- `expand_positional_arg` - $0-$9 için belirtilen indeksteki argümanı genişletir
- `expand_all_args` - $@ ve $* için tüm argümanları boşlukla birleştirir
- `expand_variable` - Bir değişken adını ortamdan değerine genişletir
- `process_dollar` - $?, $$, $#, $0-$9, $@, $*, ve $VAR dahil $ genişletmesini işler
- `process_tilde` - ~ ve ~/path için tilde genişletmesini işler (HOME değişkenini kullanır)

### expand_args.c
Argüman genişletme uygulaması. Ortam değişkenlerini ve wildcardları işleyerek komut argümanlarını işler.

**Fonksiyonlar:**
- `process_arg` - Değişken ve wildcard genişletmesiyle tek argümanı işler
- `count_single_expanded` - Tek argüman için genişletilmiş sonuçları sayar
- `count_total_expanded` - Toplam genişletilmiş argümanları sayar
- `expand_args` - Tüm argümanları değişkenler ve wildcardlarla genişletir

### expand_helpers.c
Genişletme işlemleri için yardımcı fonksiyonlar. Genişletme sırasında tırnak işleme ve özel karakter işlemeyi destekler.

**Fonksiyonlar:**
- `count_args` - Dizideki argüman sayısını sayar
- `has_quotes` - Dizenin tırnak içerip içermediğini kontrol eder
- `add_expanded_results` - Wildcard genişletme sonuçlarını sonuç dizisine ekler

### wildcard.c
Wildcard (glob) desen eşleştirme uygulaması. Mevcut dizindeki eşleşen dosya adlarına * desenlerini genişletir (bonus özellik).

**Fonksiyonlar:**
- `create_single_result` - Desenle tek elemanlı sonuç dizisi oluşturur
- `process_matches` - Dizin eşleşmelerini işler ve sıralı sonuçlar döndürür
- `expand_wildcard` - Wildcard desenlerini eşleşen dosya adlarına genişletir (BONUS)

### wildcard_utils.c
Wildcard eşleştirme yardımcı fonksiyonları. Glob genişletme için desen eşleştirme algoritmaları ve sonuç sıralama sağlar.

**Fonksiyonlar:**
- `match_pattern` - Wildcard desenini dizeye karşı özyinelemeli olarak eşleştirir
- `count_matches` - Eşleşen dizin girdisi sayısını sayar
- `add_matches` - Eşleşen girdileri sonuç dizisine ekler
- `sort_strings` - Dize dizisini alfabetik olarak sıralar (bubble sort)

---

## Executor Modülü (executor/)

### executor.c
Ana executor uygulaması. AST gezinme ve komut yürütmeyi koordine eder.

**Fonksiyonlar:**
- `executor_run` - Ana executor giriş noktası, stdin/stdout'u yedekler ve AST'yi çalıştırır

### exec_ast.c
AST yürütme mantığı. Ayrı bonus/zorunlu uygulamalarla farklı düğüm türlerini (komutlar, pipe'lar, mantıksal operatörler) işler.

**Fonksiyonlar:**
- `exec_subshell` - Alt kabuğu fork edilmiş süreçte çalıştırır (BONUS)
- `execute_ast` - AST düğümlerini özyinelemeli olarak çalıştırır (komutlar, pipe'lar, mantık operatörleri)

### exec_builtin.c
Yerleşik komut dağıtıcısı. cd, echo, env gibi yerleşik komutları tanımlar ve çalıştırır.

**Fonksiyonlar:**
- `get_builtins` - Yerleşik komutların ve fonksiyon işaretçilerinin statik dizisini döndürür
- `is_builtin` - Komutun yerleşik olup olmadığını kontrol eder
- `execute_builtin` - İsme göre bir yerleşik komut çalıştırır

### exec_cmd.c
Komut yürütme uygulaması. Harici komutlar için fork, execve ve süreç yönetimini işler.

**Fonksiyonlar:**
- `handle_redirection_only` - Komut çalıştırmadan yönlendirmeleri işler
- `execute_command` - Ana komut yürütme (hazırlar, yerleşik kontrol eder, harici için forklar)

### exec_cmd_utils1.c
Komut yürütme yardımcı fonksiyonları (kısım 1). Yol çözümleme, argüman genişletme ve çıkış durumu işleme.

**Fonksiyonlar:**
- `expand_cmd_args` - Komut argümanlarında değişkenleri ve wildcardları genişletir
- `handle_exit_status` - Bekleme durumunu işler ve shell çıkış durumunu ayarlar
- `find_command_path` - PATH ortam değişkenini kullanarak komutun tam yolunu bulur

### exec_cmd_utils2.c
Komut yürütme yardımcı fonksiyonları (kısım 2). Komut doğrulama ve alt süreç yürütme.

**Fonksiyonlar:**
- `validate_and_execute` - Komut yolunu doğrular ve execve ile çalıştırır
- `exec_child_process` - Alt süreç yürütme (sinyalleri ayarlar, yönlendirmeler, çalıştır)
- `execute_builtin_with_redir` - Yönlendirmelerle yerleşik çalıştırır (stdio'yu kaydeder/geri yükler)
- `prepare_cmd_execution` - Komutu yürütme için hazırlar (heredoclar, genişletmeler)

### exec_pipe.c
Pipe yürütme uygulaması. Pipeline komutları için pipe oluşturur ve alt süreçleri yönetir.

**Fonksiyonlar:**
- `exec_left_pipe` - Pipe'ın sol tarafını alt süreçte çalıştırır
- `exec_right_pipe` - Pipe'ın sağ tarafını alt süreçte çalıştırır
- `execute_pipe` - Pipe oluşturur, sol/sağ için alt süreçleri forklar, tamamlanmayı bekler

### redirections.c
Girdi/çıktı yönlendirme işleyicisi. <, > ve >> operatörleri için dosya tanımlayıcı yönlendirmesini yönetir.

**Fonksiyonlar:**
- `handle_redir_in` - Girdi yönlendirmesini işler (<)
- `handle_redir_out` - Çıktı yönlendirmesini işler (>, >>)
- `handle_heredoc` - Heredoc yönlendirmesini işler (<<)
- `setup_redirections` - Bir komut için tüm yönlendirmeleri kurar

### here_doc_manager.c
Heredoc yönetim sistemi. Heredoc oluşturma, işleme ve temizleme işlemlerini koordine eder.

**Fonksiyonlar:**
- `handle_heredoc_entry` - Tek bir heredoc yönlendirmesini işler
- `process_cmd_heredoc` - Bir komut için tüm heredocları işler
- `clean_heredoc` - Heredoc geçici dosyalarını temizler

### here_doc_utils1.c
Heredoc yardımcı fonksiyonları (kısım 1). Heredoc sinyal kontrolü ve satır işleme.

**Fonksiyonlar:**
- `heredoc_signal_checker` - Heredoc okuma sırasında SIGINT'i kontrol eder
- `process_heredoc_line` - Tek bir heredoc girdi satırını işler

### here_doc_utils2.c
Heredoc yardımcı fonksiyonları (kısım 2). Heredoc dosya oluşturma ve döngü okuma işlevselliği.

**Fonksiyonlar:**
- `read_heredoc_input` - Bir satır heredoc girdisi okur (etkileşimli veya etkileşimsiz)
- `read_heredoc_loop` - Delimiter'a kadar heredoc satırlarını okuma ana döngüsü
- `get_heredoc_filename` - Heredoc için geçici dosya adı oluşturur
- `create_heredoc_dir` - /tmp/minishell_heredocs dizini oluşturur

---

## Executor Error Modülü (executor_error/)

### executor_error.c
Executor için hata işleme. Komut bulunamadı, izin reddedildi ve dizin yürütme hatalarını işler.

**Fonksiyonlar:**
- `print_execution_error` - Yürütme hata mesajını stderr'e yazdırır
- `exit_with_error` - Hatayı yazdırır, shell'i temizler ve kodla çıkar
- `handle_cmd_not_found` - Komut bulunamadı hatasını işler (çıkış 127)
- `handle_permission_denied` - İzin reddedildi hatasını işler (çıkış 126)
- `handle_is_directory` - "bir dizindir" hatasını işler (çıkış 126)

### executer_error2.c
Ek executor hata işleyicileri. Dosya bulunamadı hataları ve hata çıkış yardımcıları.

**Fonksiyonlar:**
- `handle_no_such_file` - Böyle bir dosya veya dizin yok hatasını işler (çıkış 127)

---

## Builtins Modülü (builtins/)

### cd/builtin_cd.c
Dizin değiştirme builtin uygulaması. HOME, OLDPWD ve göreceli/mutlak yol desteği ile cd komutunu işler.

**Fonksiyonlar:**
- `update_pwd_env` - PWD ve OLDPWD ortam değişkenlerini günceller
- `resolve_path` - Hedef yolu çözer (HOME, OLDPWD veya belirtilen yol)
- `builtin_cd` - cd yerleşik komutunu uygular

### echo/builtin_echo.c
Echo builtin uygulaması. Yeni satır yok için -n bayrağı desteği ile argümanları yazdırır.

**Fonksiyonlar:**
- `parse_flags` - Echo bayraklarını ayrıştırır (-n yeni satır yok için)
- `echo_print_arg` - Echo argümanlarını aralarında boşlukla yazdırır
- `builtin_echo` - echo yerleşik komutunu uygular

### env/builtin_env.c
Ortam değişkeni görüntüleme builtin'i. Mevcut shell'deki tüm ortam değişkenlerini yazdırır.

**Fonksiyonlar:**
- `execute_env_cmd` - Mevcut ortamla komut çalıştırır (env komut argümanları)
- `print_env_bucket` - Bir hash bucket'ındaki ortam değişkenlerini yazdırır
- `builtin_env` - env yerleşik komutunu uygular

### exit/builtin_exit.c
Exit builtin uygulaması. İsteğe bağlı çıkış durum koduyla shell'i sonlandırır. Terminal input kontrolü ile exit mesajını koşullu olarak yazdırır.

**Fonksiyonlar:**
- `is_numeric` - Dizenin geçerli sayısal argüman olup olmadığını kontrol eder
- `is_really_neg_one` - Argümanın özellikle -1 olup olmadığını kontrol eder
- `quit_shell` - Temizler ve verilen durumla çıkar
- `builtin_exit` - exit yerleşik komutunu uygular (terminal kontrolü ile "exit" mesajı yazdırır)

### export/builtin_export.c
Export builtin uygulaması. Ortam değişkenlerini ayarlar ve dışa aktarılan değişkenleri görüntüler.

**Fonksiyonlar:**
- `is_valid_identifier` - Ortam değişkeni adını doğrular
- `mark_as_exported` - Ortam değişkenini dışa aktarılmış olarak işaretler
- `export_arg` - Tek export argümanını işler
- `builtin_export` - export yerleşik komutunu uygular

### export/builtin_expot_utils.c
Export yardımcı fonksiyonları. Değişken doğrulama ve export işleme için yardımcı fonksiyonlar.

**Fonksiyonlar:**
- `sort_buckets` - Ortam bucket'larını alfabetik olarak sıralar (bubble sort)
- `print_export_item` - Tek export bildirimini yazdırır
- `print_sorted_env` - Tüm dışa aktarılan değişkenleri sıralı şekilde yazdırır

### pwd/builtin_pwd.c
Çalışma dizini yazdırma builtin'i. Mevcut çalışma dizini yolunu görüntüler.

**Fonksiyonlar:**
- `builtin_pwd` - pwd yerleşik komutunu uygular (mevcut dizini yazdırır)

### unset/builtin_unset.c
Unset builtin uygulaması. Shell ortamından ortam değişkenlerini kaldırır.

**Fonksiyonlar:**
- `builtin_unset` - unset yerleşik komutunu uygular

### builtin_alias_bonus.c
Alias builtin uygulaması (bonus). Daha kısa komut adları için komut aliasları oluşturur.

**Fonksiyonlar:**
- `get_alias_key_len` - '=' işaretinden önceki alias anahtarının uzunluğunu alır
- `print_all_aliases` - Tanımlanmış tüm aliasları yazdırır
- `process_alias_arg` - Tek bir alias argümanını işler (ayarlar veya yazdırır)
- `builtin_alias` - alias yerleşik komutunu uygular (BONUS)

### builtin_unalias_bonus.c
Unalias builtin uygulaması (bonus). Önceden tanımlanmış komut aliaslarını kaldırır.

**Fonksiyonlar:**
- `builtin_unalias` - unalias yerleşik komutunu uygular (BONUS)

---

## Environment Modülü (env/)

### env_manager.c
Ortam değişkeni yönetim sistemi. Hash tablosu tabanlı ortam depolama uygular.

**Fonksiyonlar:**
- `update_env_node` - Mevcut ortam düğümü değerini günceller
- `create_env_node` - Yeni ortam bucket düğümü oluşturur
- `env_get` - Anahtara göre ortam değişkeni değerini alır
- `env_set` - Ortam değişkenini ayarlar (oluşturur veya günceller)
- `env_unset` - Hash tablosundan ortam değişkenini kaldırır

### env_manager_utils.c
Ortam yöneticisi yardımcı fonksiyonları. Ortam değişkeni getter, setter ve unset işlemleri.

**Fonksiyonlar:**
- `set_default_prompt_vars` - Varsayılan PS1 ve PS2 prompt değişkenlerini ayarlar
- `mark_env_as_exported` - Ortam değişkenini dışa aktarılmış olarak işaretler
- `set_shlvl_var` - SHLVL ortam değişkenini ayarlar veya artırır
- `set_pwd_var` - Henüz ayarlanmadıysa PWD değişkenini ayarlar
- `set_default_env_vars` - Varsayılan ortam değişkenlerini ayarlar (SHLVL, PWD)
- `add_env_entry` - KEY=VALUE dizesinden ortam girdisi ekler

### env_manager_utils2.c
Ek ortam yöneticisi yardımcı fonksiyonları. Ortam başlatma ve array dönüşümü.

**Fonksiyonlar:**
- `set_default_shell_vars` - Tüm varsayılan shell değişkenlerini ayarlar
- `initialize_env_table` - Envp dizisinden ortam hash tablosunu başlatır
- `join_env_pair` - Anahtar ve değeri KEY=VALUE dizesine birleştirir
- `env_table_to_array` - Ortam hash tablosunu dize dizisine dönüştürür

### env_array.c
Ortam array dönüşümü. Hash tablosu ortamını execve için string array formatına dönüştürür.

**Fonksiyonlar:**
- `count_env_nodes` - Listedeki ortam düğümü sayısını sayar
- `fill_env_array` - Diziyi env listesinden KEY=VALUE dizeleriyle doldurur
- `env_list_to_array` - Ortam bağlı listesini dize dizisine dönüştürür

### env_crypto.c
Ortam şifreleme yardımcı fonksiyonları. Hash tablosu indeksleme ve basit şifreleme.

**Fonksiyonlar:**
- `fnv1a_hash` - Hash tablosu indeksleme için FNV-1a hash hesaplar
- `xor_cipher` - Basit XOR şifresiyle dizeyi şifreler/şifresini çözer (tersine çevrilebilir)

### parse_path.c
PATH ortam değişkeni ayrıştırıcısı. Komut çözümleme için PATH'den dizin yollarını çıkarır ve ayrıştırır.

**Fonksiyonlar:**
- `parse_path` - PATH ortam değişkenini dizin dizisine ayrıştırır

---

## Signals Modülü (signals/)

### signals.c
Sinyal işleme uygulaması. İnteraktif ve alt süreç modları için SIGINT ve SIGQUIT işleyicilerini kurar.

**Fonksiyonlar:**
- `signal_handler` - Etkileşimli prompt için SIGINT işleyici
- `setup_signals` - Ebeveyn süreç için sinyal işleyicilerini kurar
- `setup_child_signals` - Alt süreçler için sinyalleri varsayılana sıfırlar
- `ignore_signals` - Komut yürütme sırasında SIGINT ve SIGQUIT'i yoksayar

### signal_state.c
Sinyal durum yönetimi. Sinyal işleyicileri ve ana döngü arasında iletişim için global sinyal durumunu korur.

**Fonksiyonlar:**
- `get_signal` - Mevcut sinyal değerini döndürür (thread-safe)
- `set_signal` - Sinyal değerini ayarlar (sinyal işleyicisinden çağrılır)
- `reset_signal` - Sinyali 0'a sıfırlar

---

## Utils Modülü (utils/)

### error.c
Hata raporlama yardımcı fonksiyonları. Hataları ve sözdizimi hata mesajlarını yazdırmak için fonksiyonlar sağlar.

**Fonksiyonlar:**
- `print_error` - Biçimlendirilmiş hata mesajı yazdırır
- `syntax_error` - Tokenle sözdizimi hata mesajı yazdırır
- `command_not_found` - Komut bulunamadı hatası yazdırır (127 döndürür)
- `error_exit` - Hata yazdırır ve shell'den çıkar

### strings.c
String manipülasyon yardımcı fonksiyonları. Dosya açma işlemleri için yardımcılar.

**Fonksiyonlar:**
- `open_file` - Okumak için dosya açar
- `open_file_write` - Yazmak için dosya açar (kesme)
- `open_file_append` - Eklemek için dosya açar

### strings_utils.c
Ek string yardımcı fonksiyonları. Shell için genişletilmiş string işleme fonksiyonları sağlar.

**Fonksiyonlar:**
- `is_whitespace` - Karakterin boşluk olup olmadığını kontrol eder
- `is_special_char` - Karakterin özel operatör karakteri olup olmadığını kontrol eder
- `is_metachar` - Karakterin metakarakter (özel veya tırnak) olup olmadığını kontrol eder
- `ft_strcmp` - İki dizeyi karşılaştırır (özel uygulama)
- `ft_atoll` - Overflow tespiti ile dizeyi long long'a dönüştürür

---

## İstatistikler

**Toplam Analiz Edilen Dosya:** 60 kaynak dosya

**Dizin Bazında:**
- Ana dosyalar: 4 dosya
- Lexer: 6 dosya
- Parser: 6 dosya
- Expander: 6 dosya
- Executor: 11 dosya
- Executor_error: 2 dosya
- Builtins: 10 dosya
- Env: 6 dosya
- Signals: 2 dosya
- Utils: 3 dosya

**Temel Özellikler:**
- Hash tablosu ve şifreleme ile ortam değişkeni yönetimi
- Tırnak işleme ve alias genişletme ile kapsamlı lexer
- AST oluşturan özyinelemeli iniş parser
- Tam değişken genişletme: $VAR, $?, $$, ~, $0-$9, $#, $@, $*
- Positional arguments desteği (script argümanları)
- Script mode desteği (dosyadan komut çalıştırma, shebang desteği)
- Wildcard genişletme (bonus)
- Sinyal işleme ile heredoc desteği
- Pipe ve yönlendirme desteği
- Mantıksal operatörler && ve || (bonus)
- () ile alt kabuklar (bonus)
- Etkileşimli kullanım için sinyal işleme
- Geçmiş yönetimi
- Yapılandırma dosyası desteği (.shellrc)
- 7 yerleşik komut: cd, echo, env, exit, export, pwd, unset
- Bonus yerleşikler: alias, unalias

# Çöp Toplayıcı

Bağlayıcı sarmalama (linker wrapping) kullanarak C programları için **sıfır kod değişikliği gerektiren** konservatif bir `mark-and-sweep` (işaretle ve süpür) çöp toplayıcısı.

## Özellikler

- 🔥 **Sıfır Kod Değişikliği** - Mevcut kodunuzla olduğu gibi çalışır
- 🚀 **Mark-and-Sweep** - Konservatif bir çöp toplama algoritması
- 🔗 **Bağlayıcı Sihri** - `malloc`/`free` çağrılarını yakalamak için `--wrap` bayrağını kullanır
- 🎯 **Stack Tarama** - Kök işaretçileri otomatik olarak bulur
- ⚡ **Kolay Entegrasyon** - Sadece farklı bir bayrakla yeniden derleyin
- 🛡️ **42 Norm Uyumlu** - Global değişken yok, tüm kurallara uyar

## Hızlı Başlangıç

### Kurulum

```bash
git clone https://github.com/suatkvam/garbage_collector.git
cd garbage_collector
```

### Seçenek 1: Normal Mod (Standart malloc/free)

```bash
make
./program
```

Kodunuz standart `malloc()` ve `free()` kullanır - belleği manuel olarak yönetmelisiniz.

### Seçenek 2: GC Modu (Otomatik Bellek Yönetimi)

```bash
make gc
./program
```

Tüm `malloc()` çağrıları otomatik olarak çöp toplayıcıyı kullanır - `free()` çağırmanıza gerek yok!

## Nasıl Çalışır

### `--wrap` Sihri

`make gc` ile derlediğinizde, bağlayıcı tüm bellek fonksiyonlarını yakalar:

```
Sizin Kodunuz:       Bağlayıcı Ne Yapar:          Sonuç:
malloc()       →     __wrap_malloc()     →     gc_malloc()
free()         →     __wrap_free()       →     (işlem yapmaz)
calloc()       →     __wrap_calloc()     →     gc_calloc()
realloc()      →     __wrap_realloc()    →     gc_realloc()
```

**Kod değişikliği gerekmez!** Bağlayıcı her şeyi otomatik olarak halleder.

## Örnek Kullanım

### Mevcut Kodunuz (DEĞİŞİKLİK YOK!)

```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    // Normal kodunuz - her iki modda da çalışır!
    char *str = malloc(100);
    strcpy(str, "Merhaba, GC!");
    
    int *array = malloc(sizeof(int) * 10);
    for (int i = 0; i < 10;
        array[i] = i;
    
    // Normal modda: free() çağırmalısınız
    // GC modunda: free() otomatiktir!
    free(str);
    free(array);
    
    return 0;
}
```

**Derleyin ve çalıştırın:**

```bash
# Normal mod - belleği kendiniz yönetmelisiniz
make normal
./program

# GC modu - otomatik bellek yönetimi
make gc
./program
```

Her ikisi de aynı çıktıyı üretir, ancak GC modu bellek sızıntılarını önler!

## API Referansı

### Otomatik (Varsayılan)

`make gc` kullanıldığında, tüm bellek fonksiyonları otomatik olarak yakalanır:

```c
void *ptr = malloc(size);      // → gc_malloc(size)
void *ptr = calloc(n, size);   // → gc_calloc(n, size)
void *ptr = realloc(ptr, size);// → gc_realloc(ptr, size)
free(ptr);                     // → işlem yapmaz (GC halleder)
```

### Manuel Kontrol (İsteğe Bağlı)

GC'yi manuel olarak kontrol etmek isterseniz:

```c
#include "gc_wrap.h"

int main(void)
{
    int stack_var;
    
    // İsteğe bağlı: Manuel başlatma
    GC_INIT(stack_var);
    
    // Kodunuz...
    char *data = malloc(100);  // Hala yakalanıyor!
    
    // İsteğe bağlı: Toplamayı manuel tetikleme
    GC_COLLECT();
    
    // İsteğe bağlı: Çıkmadan önce temizlik
    GC_CLEANUP();
    
    return 0;
}
```

**Not:** Manuel kontrol **isteğe bağlıdır**. GC, ilk `malloc` çağrısında otomatik olarak başlar.

## Mimari

### Bellek Bloğu Yapısı

```
+------------------+
| t_collecter      |  ← Başlık (metadata)
| - is_marked: 0/1 |     GC için işaretleme biti
| - size: N bayt   |     Ayrılan boyut
| - next: ptr      |     Listedeki sonraki eleman
+------------------+
| Kullanıcı Verisi |  ← Kullanıcıya dönen işaretçi
| (N bayt)         |
+------------------+
```

### Mark-and-Sweep Algoritması

#### İşaretleme Aşaması (Mark Phase)
1. Stack'ten başla (kök işaretçiler)
2. Bulunan her işaretçiyi tara
3. Ulaşılabilir blokları işaretle
4. Referans verilen blokları özyinelemeli olarak işaretle

#### Süpürme Aşaması (Sweep Phase)
1. Tüm ayrılan bellekleri dolaş
2. İşaretlenmemiş blokları serbest bırak
3. Hayatta kalanların işaretleme bitlerini sıfırla

### Dahili ve Kullanıcı Bellek Ayırmaları

```c
// GC, kendi yapıları için malloc'a ihtiyaç duyar
// Özyinelemeyi önlemek için __real_malloc kullanır

GC_INTERNAL_MALLOC()  →  __real_malloc()  (sarmalanmaz)
                             ↓
                        Sistem malloc'u

// Kullanıcı kodu malloc'u sarmalanır
malloc()  →  __wrap_malloc()  →  gc_malloc()
                                      ↓
                              GC_INTERNAL_MALLOC kullanır
```

## Derleme

### Makefile Hedefleri

```bash
make           # Standart malloc/free ile derle
make gc        # Çöp toplayıcı ile derle (--wrap)
make normal    # 'make' ile aynı (açıkça belirtilmiş)
make clean     # Nesne dosyalarını sil
make fclean    # Nesne ve çalıştırılabilir dosyaları sil
make re        # Baştan derle
make help      # Yardım mesajını göster
```

### Derleyici Bayrakları

**Normal Mod:**
```bash
gcc your_code.c gc_*.c -o program
```

**GC Modu:**
```bash
gcc -DUSE_GC_WRAP \
    -Wl,--wrap=malloc,--wrap=calloc,--wrap=realloc,--wrap=free \
    your_code.c gc_*.c gc_wrap.c -o program
```

## Proje Yapısı

```
garbage_collector/
├── README.md                    # Bu dosya
├── DOKUMANTASYON.md             # Detaylı teknik dokümanlar
├── Makefile                     # GC destekli derleme sistemi
│
├── garbage_collector.h          # Genel API
├── internal_collector.h         # Dahili yapılar
├── gc_internal_malloc.h         # Dahili malloc makroları (YENİ!)
├── gc_wrap.h                    # Sarmalayıcı API (YENİ!)
│
├── collector_init.c             # Başlatma
├── collector_close.c            # Temizlik
├── gc_malloc.c                  # Bellek ayırma
├── gc_calloc.c                  # Sıfırlanmış bellek ayırma
├── gc_realloc.c                 # Yeniden boyutlandırma
├── gc_collect.c                 # Toplama tetikleyicisi
├── gc_mark.c                    # İşaretleme aşaması
├── gc_mark_utils.c              # İşaretleme yardımcıları
├── gc_sweep.c                   # Süpürme aşaması
├── gc_state.c                   # Durum yönetimi
├── gc_memory_utils.c            # Bellek yardımcıları
├── get_header_from_ptr.c        # İşaretçi yardımcıları
├── gc_wrap.c                    # Sarmalayıcı uygulaması (YENİ!)
│
└── examples/
    ├── no_changes_example.c     # Hiçbir değişiklik yapmadan çalışır
    ├── manual_control_example.c # İsteğe bağlı manuel GC kontrolü
    ├── linked_list_example.c    # Veri yapıları örneği
    └── stress_test_example.c    # Performans testi
```

## Örnekler

### Örnek 1: Kod Değişikliği Yok

```c
// Mevcut kodunuz - olduğu gibi çalışır!
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *ft_strdup(const char *s)
{
    char *dup = malloc(strlen(s) + 1);
    if (dup)
        strcpy(dup, s);
    return dup;
}

int main(void)
{
    char *str = ft_strdup("Merhaba, Dünya!");
    printf("%s\n", str);
    
    // Normal modda: free yapılmalı
    // GC modunda: otomatik!
    free(str);
    
    return 0;
}
```

**Her iki şekilde de derleyin:**
```bash
make normal && ./program  # Standart malloc/free
make gc && ./program      # Otomatik GC
```

### Örnek 2: Bağlı Liste

```c
typedef struct s_node {
    int value;
    struct s_node *next;
} t_node;

t_node *create_node(int value)
{
    t_node *node = malloc(sizeof(t_node));
    node->value = value;
    node->next = NULL;
    return node;
}

int main(void)
{
    // Liste oluştur
    t_node *head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    
    // Normal modda: her düğümü free yapmalısınız
    // GC modunda: sadece head = NULL yapın!
    
    return 0;
}
```

### Örnek 3: İsteğe Bağlı Manuel Kontrol

```c
#include "gc_wrap.h"

int main(void)
{
    int stack_var;
    GC_INIT(stack_var);  // İsteğe bağlı başlatma
    
    for (int i = 0; i < 1000;
    {
        malloc(1024);  // Geçici bellek ayırmaları
        
        if (i % 100 == 0)
            GC_COLLECT();  // İsteğe bağlı periyodik toplama
    }
    
    GC_CLEANUP();  // İsteğe bağlı temizlik
    return 0;
}
```

## Avantajlar

### GC Modu ile

✅ **Bellek sızıntısı yok** - Otomatik temizlik  
✅ **Double-free hatası yok** - GC her şeyi halleder  
✅ **Sallanan işaretçi yok** - Bellek düzgün takip edilir  
✅ **Daha basit kod** - Manuel `free()` çağrıları yok  
✅ **Daha güvenli** - Yaygın bellek hatalarını önler  

### Normal Mod (karşılaştırma için)

✅ **Standart davranış** - Tüm araçlarla uyumlu  
✅ **Tahmin edilebilir** - Belleğin ne zaman serbest bırakılacağını siz kontrol edersiniz  
✅ **Ek yük yok** - Doğrudan malloc/free çağrıları  
✅ **Hata ayıklama** - Valgrind, sanitizer vb. kullanabilirsiniz.  

## Performans Değerlendirmeleri

### Bellek Ek Yükü
- Başlık: Her ayırma için ~16-24 bayt
- Toplama sırasında bağlı liste dolaşımı

### Toplama Maliyeti
- **Zaman Karmaşıklığı:** O(n), n = ayrılan bellek sayısı
- **Alan Karmaşıklığı:** O(1) ek alan

### Optimizasyon İpuçları

1. **Bellek Ayırma Sayısını Azaltın:**
```c
// Daha az verimli
for (int i = 0; i < 1000;
    malloc(sizeof(int));

// Daha verimli
int *array = malloc(sizeof(int) * 1000);
```

2. **Toplama Sıklığını Kontrol Edin:**
```c
static int count = 0;
if (++count > 1000) {
    GC_COLLECT();
    count = 0;
}
```

3. **İşlemleri Gruplayın:**
```c
// Ayır, kullan, sonra topla
process_batch();
GC_COLLECT();
```

## Sınırlamalar

⚠️ **Konservatif Toplama** - Veri bir işaretçi gibi görünüyorsa belleği canlı tutabilir  
⚠️ **Sadece Stack Kökleri** - Kök işaretçiler için sadece stack'i tarar  
⚠️ **Tek İş Parçacıklı** - Thread-safe (iş parçacığı güvenli) değil  
⚠️ **Sonlandırıcı Yok** - Özel temizlik kodu yok  

## 42 Okulu Uyumluluğu

✅ **Global değişken yok** - Statik fonksiyonlar kullanır  
✅ **Norm uyumlu** - Norminette'ten geçer  
✅ **Sadece izin verilen özellikler** - Yasaklanmış fonksiyonlar yok  
✅ **Doğru formatlama** - 80 karakter limiti, doğru tablar  

## Sorun Giderme

### Problem: Segmentation Fault

**Neden:** GC düzgün başlatılmadı  
**Çözüm:** `collector_init()` fonksiyonunun çağrıldığından emin olun (veya otomatik başlatmayı kullanın)

### Problem: Bellek Hala Sızdırıyor

**Neden:** İşaretçiler stack dışı bellekte gizlenmiş  
**Çözüm:** Kök işaretçileri stack'te tutun

### Problem: Erken Toplama

**Neden:** İşaretçi sadece register'da (nadiren)  
**Çözüm:** `volatile` anahtar kelimesini kullanın

### Problem: Başlangıçta Sonsuz Döngü

**Neden:** `malloc` sarmalamasında özyineleme  
**Çözüm:** `gc_internal_malloc.h` tarafından zaten halledildi

## Test Etme

```bash
# Valgrind ile normal modu test et
make normal
valgrind --leak-check=full ./program

# GC modunu test et
make gc
valgrind ./program

# Çıktıları karşılaştır
diff <(make normal && ./program) <(make gc && ./program)
```

## Katkıda Bulunma

Katkılarınızı bekliyoruz! Lütfen:
1. Depoyu forklayın
2. Bir özellik dalı (feature branch) oluşturun
3. 42 Norm'una uyun
4. Her iki modu da test edin
5. Bir pull request gönderin

## Lisans

MIT Lisansı - Projelerinizde kullanmaktan çekinmeyin!

## Yazarlar

- **suatkvam** -
- **hudayiarici** - 


## Teşekkür

- Çeşitli GC uygulamalarından alınan mark-and-sweep algoritması ilhamı
- Kodlama standartları için 42 Okulu
- Topluluk geri bildirimleri ve testleri

## Daha Fazla Bilgi

- [DOKUMANTASYON.md](./DOKUMANTASYON.md) - Detaylı teknik dokümantasyon
- [examples/](./examples/) - Tam çalışan örnekler
- [GC Algoritması Wikipedia](https://en.wikipedia.org/wiki/Tracing_garbage_collection)

---

**Unutmayın:** Sadece yeniden derleyerek modlar arasında geçiş yapın!

```bash
make normal  # Standart malloc/free
make gc      # Otomatik çöp toplama
```

**Kod değişikliği gerekmez!** 🚀

```

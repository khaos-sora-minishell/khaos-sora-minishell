# Çöp Toplayıcı - Kapsamlı Teknik Dokümantasyon

## İçindekiler

1. [Giriş](#giriş)
2. [Mimari](#mimari)
3. [Bağlayıcı Sarmalama Sistemi](#bağlayıcı-sarmalama-sistemi)
4. [Algoritma Detayları](#algoritma-detayları)
5. [API Referansı](#api-referansı)
6. [Uygulama Detayları](#uygulama-detayları)
7. [En İyi Uygulamalar](#en-i̇yi-uygulamalar)
8. [Sorun Giderme](#sorun-giderme)
9. [Performans Analizi](#performans-analizi)

---

## Giriş

Bu çöp toplayıcı, C programları için **konservatif bir mark-and-sweep (işaretle ve süpür)** algoritmasını uygular. Temel yenilik, herhangi bir kod değişikliği gerektirmeden bellek ayırma çağrılarını yakalamak için **bağlayıcı sarmalama (linker wrapping)** kullanmasıdır.

### Anahtar Kavramlar

**Konservatif Toplama**: Geçerli bir işaretçi gibi görünen herhangi bir değeri işaretçi olarak kabul eder. Bu, güvenliği sağlar ancak bazı bellekleri gereğinden uzun süre canlı tutabilir.

**Mark-and-Sweep**: İki aşamalı bir algoritma:
- **İşaretleme Aşaması (Mark Phase)**: Tüm ulaşılabilir belleği bul ve işaretle
- **Süpürme Aşaması (Sweep Phase)**: İşaretlenmemiş tüm belleği serbest bırak

**Bağlayıcı Sarmalama**: GCC'nin `--wrap` bayrağını kullanarak, kaynak kodunu değiştirmeden `malloc`/`free` gibi fonksiyonları bağlama zamanında (link time) değiştirmemizi sağlar.

---

## Mimari

### Sisteme Genel Bakış

```
┌─────────────────────────────────────────────────────────────┐
│                        Kullanıcı Kodu                          │
│  malloc(), free(), calloc(), realloc()                      │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│                   Bağlayıcı Sarmalama Katmanı                │
│  __wrap_malloc() → gc_malloc() veya __real_malloc()          │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│                  Çöp Toplayıcı Çekirdeği                     │
│  - İşaretleme Aşaması: Kökler için stack'i tara             │
│  - Süpürme Aşaması: İşaretlenmemiş blokları serbest bırak    │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│                   Dahili Bellek Ayırıcı                      │
│  GC_INTERNAL_MALLOC → __real_malloc() (sistem)             │
└─────────────────────────────────────────────────────────────┘
```

### Bellek Düzeni

Her bellek ayırma, bir başlık (header) ve kullanıcı verisinden oluşur:

```
Bellek Adres Alanı:
┌──────────────────────────────────────┐
│  Başlık: t_collecter                 │  ← GC üstverisi (metadata)
│  - is_marked: unsigned char (0/1)    │
│  - size: size_t (kullanıcı veri boyutu)│
│  - next: t_collecter* (bağlı liste)  │
├──────────────────────────────────────┤
│  Kullanıcı Verisi                    │  ← Kullanıcı bu işaretçiyi görür
│  (size bayt)                         │
└──────────────────────────────────────┘

Kullanıcıya dönen işaretçi: &(header + 1)
```

### Veri Yapıları

#### Blok Başlığı

```c
typedef struct s_collecter
{
    unsigned char      is_marked;  // 0 veya 1 (işaretleme biti)
    size_t             size;       // Kullanıcı verisinin boyutu
    struct s_collecter *next;      // Listedeki sonraki blok
} t_collecter;
```

**Alanlar:**
- `is_marked`: İşaretleme aşamasında kullanılır (0 = işaretlenmemiş, 1 = işaretlenmiş)
- `size`: Kullanıcı için ayrılan bayt sayısı
- `next`: Genel listedeki bir sonraki bellek ayırma bloğuna işaretçi

#### Global Durum

```c
// gc_state.c içinde (dosya kapsamında statik)
static t_collecter *gc_head = NULL;        // Bellek ayırma listesinin başı
static void *gc_stack_start = NULL;        // Tarama için stack başlangıcı
```

**Not:** Bunlar dosya kapsamında statiktir, global değişkenler değildir (42 Norm uyumlu).

---

## Bağlayıcı Sarmalama Sistemi

### --wrap Nasıl Çalışır?

`--wrap` bağlayıcı bayrağı, sembol yeniden adlandırması yapar:

```bash
gcc -Wl,--wrap=malloc ...
```

**Ne olur:**

1.  `malloc`'a yapılan tüm çağrılar → `__wrap_malloc`'a yönlendirilir
2.  Orijinal `malloc` → `__real_malloc` olarak yeniden adlandırılır
3.  `__wrap_malloc`, orijinal davranışı elde etmek için `__real_malloc`'ı çağırabilir

### Örnek Akış

```c
// Kullanıcı kodu:
void *ptr = malloc(100);

// Bağlayıcı bunu şuna dönüştürür:
void *ptr = __wrap_malloc(100);

// gc_wrap.c içinde:
void *__wrap_malloc(size_t size)
{
    #ifdef USE_GC_WRAP
        return gc_malloc(size);      // GC kullan
    #else
        return __real_malloc(size);  // Orijinal malloc'u kullan
    #endif
}
```

### Özyinelemeyi Önleme

**Problem:** GC'nin kendisi, dahili yapıları için `malloc`'a ihtiyaç duyar. Bu çağrıları da sarmalarsak, sonsuz bir özyineleme (recursion) oluşur.

**Çözüm:** `gc_internal_malloc.h`, sarmalamayı atlatan makrolar sağlar:

```c
// gc_internal_malloc.h
#ifdef USE_GC_WRAP
    // GC'nin dahili kullanımı: doğrudan gerçek malloc'u çağır
    extern void *__real_malloc(size_t size);
    #define GC_INTERNAL_MALLOC(size)  __real_malloc((size))
#else
    // Normal mod: sadece malloc kullan
    #define GC_INTERNAL_MALLOC(size)  malloc((size))
#endif
```

**GC kodunda kullanımı:**

```c
// gc_malloc.c
void *gc_malloc(size_t size)
{
    // Dahili malloc kullan - bu sarmalanmayacak!
    t_collecter *header = GC_INTERNAL_MALLOC(sizeof(t_collecter) + size);
    // ...
}
```

**Akış diyagramı:**

```
Kullanıcı Kodu:
malloc(100)
    ↓
__wrap_malloc(100)  ← Sarmalanmış
    ↓
gc_malloc(100)
    ↓
GC_INTERNAL_MALLOC(...)  ← Sarmalanmamış
    ↓
__real_malloc(...)  ← Doğrudan sistem malloc'u
```

### Sarmalayıcı Uygulaması

```c
// gc_wrap.c

// Gerçek fonksiyonlar (bağlayıcı tarafından sağlanır)
extern void *__real_malloc(size_t size);
extern void *__real_calloc(size_t nmemb, size_t size);
extern void *__real_realloc(void *ptr, size_t size);
extern void __real_free(void *ptr);

// Sarmalayıcı fonksiyonlar
void *__wrap_malloc(size_t size)
{
#ifdef USE_GC_WRAP
    auto_init_gc();
    return gc_malloc(size);
#else
    return __real_malloc(size);
#endif
}

void __wrap_free(void *ptr)
{
#ifdef USE_GC_WRAP
    // İşlem yapmaz: GC temizliği halleder
    (void)ptr;
#else
    __real_free(ptr);
#endif
}
```

---

## Algoritma Detayları

### Başlatma Aşaması

```c
void collector_init(void *stack_start)
{
    void **stack_ptr = get_gc_stack_start();
    *stack_ptr = stack_start;
}
```

**Amaç:** Daha sonraki kök taraması için başlangıç stack işaretçisini saklamak.

**Neden gerekli:** GC, kök işaretçileri bulmak için stack'in nerede başladığını bilmelidir.

**Kullanım:**
```c
int main(void)
{
    int stack_var;  // Bir stack değişkeni
    collector_init(&stack_var);  // Stack başlangıcını kaydet
    // ...
}
```

---

### Bellek Ayırma Aşaması

#### gc_malloc Uygulaması

```c
void *gc_malloc(size_t size)
{
    t_collecter **head_ptr;
    t_collecter *new_header;

    if (size == 0)
        return (NULL);
    
    // Adım 1: Başlık + veri için dahili malloc ile bellek ayır
    new_header = GC_INTERNAL_MALLOC(sizeof(t_collecter) + size);
    if (!new_header)
        return (NULL);
    
    // Adım 2: Başlığı başlat
    head_ptr = get_gc_head();
    new_header->is_marked = 0;
    new_header->size = size;
    
    // Adım 3: Bağlı listeye ekle (başa ekleme)
    new_header->next = *head_ptr;
    *head_ptr = new_header;
    
    // Adım 4: Kullanıcı verisine işaretçiyi döndür
    return ((void *)(new_header + 1));
}
```

**Bellek ayırma sonrası bellek düzeni:**

```
Önce:
gc_head → [BlokA] → [BlokB] → NULL

gc_malloc(100) sonrası:
gc_head → [YeniBlok] → [BlokA] → [BlokB] → NULL
           ↑
         size=100
         marked=0
```

#### gc_calloc Uygulaması

```c
void *gc_calloc(size_t nmemb, size_t size)
{
    size_t total_size;
    void *ptr;

    if (nmemb == 0 || size == 0)
        return (NULL);
    
    // Taşma kontrolü
    if (size > (size_t)-1 / nmemb)
        return (NULL);
    
    total_size = nmemb * size;
    ptr = gc_malloc(total_size);
    if (!ptr)
        return (NULL);
    
    // Sıfırla
    gc_bzero(ptr, total_size);
    return (ptr);
}
```

#### gc_realloc Uygulaması

```c
void *gc_realloc(void *ptr, size_t size)
{
    void *new_ptr;
    t_collecter *old_header;
    size_t copy_size;

    // Özel durumlar
    if (!ptr)
        return (gc_malloc(size));
    if (size == 0)
        return (NULL);
    
    // Eski boyutu al
    old_header = get_header_from_ptr(ptr);
    if (!old_header)
        return (NULL);
    
    // Yeni blok ayır
    new_ptr = gc_malloc(size);
    if (!new_ptr)
        return (NULL);
    
    // Veriyi kopyala
    copy_size = (old_header->size < size) ? old_header->size : size;
    gc_memcpy(new_ptr, ptr, copy_size);
    
    // Eski blok daha sonra toplanacak
    return (new_ptr);
}
```

---

### İşaretleme Aşaması (Mark Phase)

#### Genel Bakış

```
1. Stack'ten Başla
       ↓
2. İşaretçileri Tara
       ↓
3. Geçerli Blokları İşaretle
       ↓
4. Referans Verilen Blokları Özyinelemeli Olarak İşaretle
```

#### Uygulama

**Adım 1: Giriş Noktası**

```c
void gc_mark(void)
{
    mark_from_stack();
}
```

**Adım 2: Stack'i Tara**

```c
void mark_from_stack(void)
{
    void **stack_start = get_gc_stack_start();
    void *stack_end;
    void **current;

    // Mevcut stack pozisyonunu al
    stack_end = &stack_end;
    
    // Başlangıçtan mevcut pozisyona kadar tara
    current = (void **)stack_start;
    while (current <= (void **)stack_end)
    {
        if (is_valid_pointer(*current))
            mark_pointer(*current);
        current++;
    }
}
```

**Adım 3: Tekil İşaretçiyi İşaretle**

```c
void mark_pointer(void *ptr)
{
    t_collecter **head_ptr;
    t_collecter *node;
    void *data_ptr;

    head_ptr = get_gc_head();
    node = *head_ptr;
    
    // Bloğu bul
    while (node)
    {
        data_ptr = (void *)(node + 1);
        if (ptr == data_ptr && !node->is_marked)
        {
            node->is_marked = 1;  // İşaretle
            mark_memory_region(data_ptr, node->size);  // Özyinelemeli
            break;
        }
        node = node->next;
    }
}
```

**Adım 4: Özyinelemeli İşaretleme**

```c
void mark_memory_region(void *start, size_t size)
{
    size_t i;
    void **potential_ptr;

    i = 0;
    // Belleği kelime kelime tara
    while (i + sizeof(void *) <= size)
    {
        potential_ptr = (void **)((char *)start + i);
        
        // Bu geçerli bir işaretçi gibi görünüyorsa, işaretle
        if (is_valid_pointer(*potential_ptr))
            mark_pointer(*potential_ptr);  // Özyinelemeli!
        
        i += sizeof(void *);
    }
}
```

**Adım 5: İşaretçi Doğrulama**

```c
int is_valid_pointer(void *ptr)
{
    t_collecter **head_ptr;
    t_collecter *node;
    void *data_ptr;
    void *end_ptr;

    if (!ptr)
        return (0);
    
    head_ptr = get_gc_head();
    node = *head_ptr;
    
    // İşaretçinin herhangi bir GC bloğunun içine işaret edip etmediğini kontrol et
    while (node)
    {
        data_ptr = (void *)(node + 1);
        end_ptr = (void *)((char *)data_ptr + node->size);
        
        if (ptr >= data_ptr && ptr < end_ptr)
            return (1);  // Geçerli!
        
        node = node->next;
    }
    return (0);  // GC işaretçisi değil
}
```

**Örnek İşaretleme Akışı:**

```
Stack:
  [ptr1] → Blok A (işaretlendi)
  [ptr2] → Blok B (işaretlendi)
  [veri] = 42

Blok A içeriği:
  [ptr3] → Blok C (özyinelemeli olarak işaretlendi)

Blok B içeriği:
  [veri] = 123

Blok C içeriği:
  [veri] = 456

Blok D:
  Referans yok → işaretlenmemiş
```

---

### Süpürme Aşaması (Sweep Phase)

```c
void gc_sweep(void)
{
    t_collecter **head_ptr;
    t_collecter *current;
    t_collecter *prev;
    t_collecter *next;

    head_ptr = get_gc_head();
    current = *head_ptr;
    prev = NULL;
    
    while (current)
    {
        next = current->next;
        
        if (!current->is_marked)
        {
            // İşaretlenmemiş: listeden çıkar ve serbest bırak
            if (prev)
                prev->next = next;
            else
                *head_ptr = next;
            
            GC_INTERNAL_FREE(current);  // Dahili free ile serbest bırak
        }
        else
        {
            // İşaretlenmiş: tut ve işaretleme bitini sıfırla
            current->is_marked = 0;
            prev = current;
        }
        
        current = next;
    }
}
```

**Süpürmeden Önce:**
```
gc_head → [A:marked=1] → [B:marked=0] → [C:marked=1] → NULL
```

**Süpürmeden Sonra:**
```
gc_head → [A:marked=0] → [C:marked=0] → NULL
                          [B serbest bırakıldı]
```

---

### Toplama Tetikleyicisi

```c
void gc_collect(void)
{
    gc_mark();   // Ulaşılabilirleri işaretle
    gc_sweep();  // Ulaşılamayanları serbest bırak
}
```

**Ne zaman çağrılmalı:**
- Uzun çalışan programlarda periyodik olarak
- Veri yığınlarını işledikten sonra
- Bellek baskısı yüksek olduğunda
- Programdan çıkmadan önce (isteğe bağlı)

---

## API Referansı

### Çekirdek Fonksiyonlar

#### collector_init

```c
void collector_init(void *stack_start);
```

**Açıklama:** Çöp toplayıcıyı stack başlangıç adresi ile başlatır.

**Parametreler:**
- `stack_start`: Bir stack değişkenine işaretçi (genellikle `main` içinde)

**Dönüş:** Yok

**Örnek:**
```c
int main(void)
{
    int x;
    collector_init(&x);
    // ...
}
```

**Thread Güvenliği:** Thread-safe değildir. Ana thread'den bir kez çağrılmalıdır.

---

#### collector_close

```c
void collector_close(void);
```

**Açıklama:** Kalan tüm belleği serbest bırakır ve GC'yi kapatır.

**Parametreler:** Yok

**Dönüş:** Yok

**Örnek:**
```c
int main(void)
{
    collector_init(&stack_var);
    // ... program ...
    collector_close();
    return 0;
}
```

**Not:** Çağrıldıktan sonra, GC'yi kullanmadan önce tekrar `collector_init()` çağırmalısınız.

---

#### gc_malloc

```c
void *gc_malloc(size_t size);
```

**Açıklama:** Çöp toplayıcı tarafından izlenen bellek ayırır.

**Parametreler:**
- `size`: Ayrılacak bayt sayısı

**Dönüş:**
- Başarılı olursa ayrılan belleğe işaretçi
- Başarısız olursa NULL

**Örnek:**
```c
int *array = gc_malloc(sizeof(int) * 100);
if (!array)
    return ERROR;
```

**Karmaşıklık:** O(1)

---

#### gc_calloc

```c
void *gc_calloc(size_t nmemb, size_t size);
```

**Açıklama:** Bellek ayırır ve sıfırlar.

**Parametreler:**
- `nmemb`: Eleman sayısı
- `size`: Her elemanın boyutu

**Dönüş:**
- Başarılı olursa sıfırlanmış belleğe işaretçi
- Başarısızlık veya taşma durumunda NULL

**Örnek:**
```c
int *zeroed = gc_calloc(50, sizeof(int));
// Tüm elemanlar 0'dır
```

**Karmaşıklık:** O(n), n = nmemb * size

---

#### gc_realloc

```c
void *gc_realloc(void *ptr, size_t size);
```

**Açıklama:** Mevcut bellek ayırmayı yeniden boyutlandırır.

**Parametreler:**
- `ptr`: Daha önce ayrılmış işaretçi (NULL olabilir)
- `size`: Yeni boyut (bayt cinsinden)

**Dönüş:**
- Başarılı olursa yeniden boyutlandırılmış belleğe işaretçi
- Başarısız olursa NULL

**Davranış:**
- `ptr` NULL ise, `gc_malloc(size)` gibi davranır
- `size` 0 ise, NULL döndürür
- Aksi takdirde, yeni blok ayırır ve veriyi kopyalar
- Eski işaretçi geçersiz olur

**Örnek:**
```c
int *data = gc_malloc(sizeof(int) * 10);
data = gc_realloc(data, sizeof(int) * 20);  // Yeniden boyutlandır
```

**Karmaşıklık:** O(min(eski_boyut, yeni_boyut)) kopyalama için

---

#### gc_collect

```c
void gc_collect(void);
```

**Açıklama:** Mark-and-sweep çöp toplama işlemini çalıştırır.

**Parametreler:** Yok

**Dönüş:** Yok

**Örnek:**
```c
// Birçok bellek ayırmadan sonra
for (int i = 0; i < 1000; i++)
    gc_malloc(1024);

gc_collect();  // Ulaşılamayan belleği serbest bırak
```

**Karmaşıklık:** O(n), n = bellek ayırma sayısı

---

### Sarmalayıcı API (İsteğe Bağlı)

Bu fonksiyonlar isteğe bağlıdır ve yalnızca manuel kontrol istediğinizde gereklidir.

#### gc_init_manual

```c
void gc_init_manual(void *stack_start);
```

**Açıklama:** GC'yi manuel olarak başlatır (isteğe bağlı, GC otomatik başlar).

**Makro:** `GC_INIT(var)`

**Örnek:**
```c
#include "gc_wrap.h"

int main(void)
{
    int stack_var;
    GC_INIT(stack_var);  // İsteğe bağlı
    // ...
}
```

---

#### gc_collect_manual

```c
void gc_collect_manual(void);
```

**Açıklama:** Toplamayı manuel olarak tetikler (isteğe bağlı).

**Makro:** `GC_COLLECT()`

**Örnek:**
```c
GC_COLLECT();  // Toplamayı şimdi zorla
```

---

#### gc_cleanup_manual

```c
void gc_cleanup_manual(void);
```

**Açıklama:** GC'yi manuel olarak temizler (isteğe bağlı).

**Makro:** `GC_CLEANUP()`

**Örnek:**
```c
GC_CLEANUP();  // Çıkmadan önce temizle
```

---

## Uygulama Detayları

### Yardımcı Fonksiyonlar

#### get_header_from_ptr

```c
t_collecter *get_header_from_ptr(void *ptr)
{
    if (!ptr)
        return (NULL);
    
    // Kullanıcı işaretçisi veriyi gösterir
    // Başlık hemen öncesindedir
    return ((t_collecter *)ptr - 1);
}
```

**Kullanım:** Kullanıcı işaretçisinden başlığı almak için (`gc_realloc` için).

---

#### Global Durum Erişimcileri

```c
// gc_state.c içinde (dosya kapsamında statik)
static t_collecter *gc_head = NULL;
static void *gc_stack_start = NULL;

t_collecter **get_gc_head(void)
{
    return (&gc_head);
}

void **get_gc_stack_start(void)
{
    return (&gc_stack_start);
}
```

**Not:** Bunlar global değişken kullanmadan durumu korur (42 Norm uyumlu).

---

### Bellek Yardımcıları

```c
void *gc_memcpy(void *dest, const void *src, size_t n);
void *gc_memmove(void *dest, const void *src, size_t n);
void *gc_memset(void *s, int c, size_t n);
int gc_memcmp(const void *s1, const void *s2, size_t n);
void *gc_memchr(const void *s1, int c, size_t n);
void gc_bzero(void *s, size_t n);
```

**Amaç:** libc ile döngüsel bağımlılıklardan kaçınmak.

---

## En İyi Uygulamalar

### 1. Başlatma Deseni

```c
int main(void)
{
    int stack_anchor;
    collector_init(&stack_anchor);  // Otomatik başlatma yoksa gerekli
    
    // ... program mantığı ...
    
    collector_close();  // İyi bir uygulama
    return 0;
}
```

---

### 2. Kök İşaretçileri Stack'te Tutun

**✅ İyi:**
```c
void process_data(void)
{
    int *data = gc_malloc(sizeof(int) * 100);
    // data stack'te, GC onu bulabilir
    use_data(data);
}  // data kapsam dışı kalır, toplanabilir
```

**❌ Kötü:**
```c
typedef struct { int *ptr; } Container;
Container *c = malloc(sizeof(Container));  // Normal malloc!
c->ptr = gc_malloc(100);  // GC işaretçisi normal heap'te gizli
// GC, stack taraması sırasında c->ptr'yi bulamaz!
```

---

### 3. Periyodik Toplama

```c
// Seçenek 1: Sayım tabanlı
static int alloc_count = 0;
if (++alloc_count > 1000)
{
    gc_collect();
    alloc_count = 0;
}

// Seçenek 2: Aşama tabanlı
process_batch();
gc_collect();  // Yığın işleminden sonra temizle
```

---

### 4. Kritik Yollarda Döngüsel Referanslardan Kaçının

**Problem:**
```c
typedef struct s_node {
    struct s_node *next;
    struct s_node *prev;  // Döngüsel!
} t_node;
```

**Çözüm:** Mark-and-sweep döngüleri doğru şekilde ele alır, ancak dolaşmaları daha uzun sürer.

---

## Sorun Giderme

### Bellek Sızıntıları

**Belirti:** Bellek kullanımı sürekli artar

**Nedenler:**
1. `gc_collect()` çağrılmıyor
2. İşaretçiler GC dışı bellekte gizlenmiş
3. Konservatif toplama ekstra bellek tutuyor

**Çözüm:**
```c
// Düzenli toplama
gc_collect();

// Tüm ayırmalar için GC kullan
void *container = gc_malloc(sizeof(Container));
```

---

### Erken Toplama

**Belirti:** Program çöküyor veya veri bozulması

**Nedenler:**
1. İşaretçi register'da saklanıyor (çok nadir)
2. İşaretçi stack dışı bir konumda saklanıyor
3. Stack başlangıcı doğru başlatılmadı

**Çözüm:**
```c
// Gerekirse volatile kullanın
volatile int *data = gc_malloc(100);

// Doğru başlatmayı sağlayın
int stack_var;
collector_init(&stack_var);
```

---

### Performans Sorunları

**Belirti:** Yavaş toplama döngüleri

**Nedenler:**
1. Çok fazla bellek ayırma
2. Büyük stack
3. Sık toplama

**Çözüm:**
```c
// Bellek ayırmalarını gruplayın
int *big = gc_malloc(sizeof(int) * 10000);

// Daha az sık toplama
if (++count > 10000)
{
    gc_collect();
    count = 0;
}
```

---

### Sarmalamada Özyineleme

**Belirti:** İlk malloc'ta stack taşması

**Neden:** GC'nin dahili malloc'u sarmalanıyor

**Çözüm:** `gc_internal_malloc.h` tarafından zaten halledildi:

```c
// GC kodu bunu kullanır:
GC_INTERNAL_MALLOC(size)  // → __real_malloc (sarmalanmaz)

// Kullanıcı kodu bunu alır:
malloc(size)  // → __wrap_malloc → gc_malloc (sarmalanmış)
```

---

## Performans Analizi

### Bellek Ek Yükü

**Her Ayırma İçin:**
```
Başlık boyutu: sizeof(t_collecter) ≈ 16-24 bayt
Minimum ayırma: 16-24 bayt ek yük + kullanıcı verisi
```

**Örnek:**
```c
malloc(1)    → 17-25 bayt toplam (1 + başlık)
malloc(100)  → 116-124 bayt toplam (100 + başlık)
```

### Zaman Karmaşıklığı

| İşlem      | Karmaşıklık        | Notlar                               |
|------------|--------------------|--------------------------------------|
| `gc_malloc`| O(1)               | Listeye başa ekleme                  |
| `gc_calloc`| O(n)               | Sıfırlama                            |
|`gc_realloc`| O(min(eski, yeni)) | Veri kopyalama                       |
|`gc_collect`| O(n * m)           | n=bloklar, m=ortalama işaretçi sayısı |
| `gc_mark`  | O(n * m)           | Özyinelemeli işaretleme              |
| `gc_sweep` | O(n)               | Doğrusal tarama                      |

### Alan Karmaşıklığı

| Bileşen            | Alan   | Notlar                   |
|--------------------|--------|--------------------------|
| GC durumu          | O(1)   | İki işaretçi             |
| Bellek ayırma listesi | O(n)   | Bağlı liste başlıkları   |
| İşaretleme aşaması | O(d)   | Özyineleme derinliği     |
| Toplam             | O(n)   | Bellek ayırmaları ile doğrusal |

---

### Optimizasyon Stratejileri

#### 1. Bellek Ayırma Sayısını Azaltma

```c
// ❌ Kötü: 1000 küçük ayırma
for (int i = 0; i < 1000; i++)
    gc_malloc(sizeof(int));

// ✅ İyi: 1 büyük ayırma
int *array = gc_malloc(sizeof(int) * 1000);
```

#### 2. Toplama Sıklığını Kontrol Etme

```c
// ❌ Kötü: Çok sık toplama
for (int i = 0; i < 1000; i++)
{
    gc_malloc(1024);
    gc_collect();  // Çok sık!
}

// ✅ İyi: Periyodik toplama
for (int i = 0; i < 1000; i++)
{
    gc_malloc(1024);
    if (i % 100 == 0)
        gc_collect();
}
```

#### 3. İşaretçi Takibini Azaltma

```c
// ❌ Kötü: Derin işaretçi zincirleri
struct A { struct B *b; };
struct B { struct C *c; };
struct C { struct D *d; };
// İşaretleme aşaması tüm zinciri dolaşmalı

// ✅ İyi: Mümkün olduğunda düz yapılar
struct Data {
    int a, b, c, d;  // Doğrudan erişim
};
```

---

## Sonuç

Bu çöp toplayıcı, C programları için otomatik bellek yönetimi sağlar:
- ✅ Mark-and-sweep algoritması
- ✅ Sıfır kod değişikliği için bağlayıcı sarmalama
- ✅ Konservatif işaretçi taraması
- ✅ 42 Norm uyumluluğu (global değişken yok)

Örnekler ve kullanım desenleri için `examples/` dizinine bakın.

---

**Son Güncelleme:** 2025-11-23  
**Yazar:** suatkvam  
**Sürüm:** 2.0 (bağlayıcı sarmalama ile)

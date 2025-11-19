# 🗑️ Garbage Collector for C

Basit bir **mark-and-sweep** garbage collector implementasyonu. C dilinde otomatik bellek yönetimi sağlar.

## ✨ Özellikler

- ✅ Otomatik bellek yönetimi
- ✅ Mark & Sweep algoritması
- ✅ Stack taraması (recursive fonksiyonlar için)
- ✅ Standart bellek fonksiyonları (malloc, calloc, realloc)
- ✅ Custom memory manipulation fonksiyonları
- ✅ Sıfır memory leak

## 📦 Kurulum

```bash
git clone https://github.com/suatkvam/garbage_collecter.git
cd garbage_collecter
make
```

## 🚀 Kullanım

```c
#include "garbage_collecter.h"

int main(void)
{
    int stack_marker;
    
    // GC'yi başlat
    collecter_init(&stack_marker);
    
    // Normal malloc gibi kullan
    char *str = gc_malloc(100);
    int *arr = gc_calloc(10, sizeof(int));
    
    // Manuel çöp toplama (opsiyonel)
    gc_collect();
    
    // Temizlik
    collecter_close();
    return (0);
}
```

## 📚 API

- `collecter_init(void *stack_start)` - GC'yi başlat
- `collecter_close(void)` - Tüm belleği temizle
- `gc_malloc(size_t size)` - Bellek ayır
- `gc_calloc(size_t nmemb, size_t size)` - Sıfırlanmış bellek ayır
- `gc_realloc(void *ptr, size_t size)` - Bellek boyutunu değiştir
- `gc_collect(void)` - Kullanılmayan belleği temizler
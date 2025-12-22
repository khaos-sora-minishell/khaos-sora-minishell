#!/bin/bash

# Renkler
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Dosya İsimleri
PASSED_FILE="tests_passed.log"
FAILED_FILE="tests_failed.log"
LEAK_FILE="tests_with_leaks.log"
SUMMARY_FILE="test_summary.log"

# Temizlik
rm -f "$PASSED_FILE" "$FAILED_FILE" "$LEAK_FILE" "$SUMMARY_FILE"
rm -f .minishell_output .bash_output .valgrind_output .minishell_clean .bash_clean

# Sayaçlar
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
LEAKED_TESTS=0

# Minishell Kontrolü
if [ ! -f "./minishell" ]; then
    echo -e "${RED}Hata: ./minishell bulunamadı! Önce 'make' çalıştırın.${NC}"
    exit 1
fi

# Valgrind Kontrolü
if ! command -v valgrind &> /dev/null; then
    echo -e "${YELLOW}Uyarı: Valgrind bulunamadı. Leak testleri atlanacak.${NC}"
    USE_VALGRIND=0
else
    USE_VALGRIND=1
fi

# --- Yardımcı Fonksiyonlar ---

# Çıktıyı temizleme fonksiyonu (Kritik Kısım)
clean_output() {
    # 1. Renk kodlarını sil
    # 2. Prompt satırını (minishell$ veya bash-3.2$) tamamen sil
    # 3. exit mesajını sil
    # 4. Hata mesajlarındaki kabuk ismini (minishell: / bash:) sil
    # 5. Başındaki/sonundaki boşlukları sil
    # 6. Boş satırları sil (echo -n testlerinde prompt temizlenince boş satır kalabilir)
    
    sed -r "s/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[mGK]//g" | \
    grep -v "minishell" | \
    grep -v "bash" | \
    grep -v "^exit$" | \
    sed 's/minishell: //g' | \
    sed 's/bash: //g' | \
    sed 's/line [0-9]*: //g' | \
    sed 's/^[ \t]*//;s/[ \t]*$//' | \
    awk 'NF' # Boş satırları atla
}

run_minishell() {
    local cmd="$1"
    if [ "$USE_VALGRIND" = "1" ]; then
        # Valgrind ile çalıştır, hataları .valgrind_output'a, çıktıyı .minishell_output'a yaz
        echo "$cmd" | timeout 3 valgrind --leak-check=full \
            --show-leak-kinds=all \
            --suppressions=readline.supp \
            --log-file=.valgrind_output \
            ./minishell > .minishell_output 2>&1
    else
        echo "$cmd" | timeout 3 ./minishell > .minishell_output 2>&1
    fi
}

run_bash() {
    local cmd="$1"
    # Bash çıktısını al
    echo "$cmd" | timeout 3 bash > .bash_output 2>&1
}

check_leaks() {
    if [ -f ".valgrind_output" ]; then
        if grep -q "definitely lost:" .valgrind_output || \
           grep -q "indirectly lost:" .valgrind_output; then
            
            # Valgrind özetinden sayıları çek
            local def_lost=$(grep "definitely lost:" .valgrind_output | awk '{print $4}' | sed 's/,//g')
            local ind_lost=$(grep "indirectly lost:" .valgrind_output | awk '{print $4}' | sed 's/,//g')
            
            # Sayısal kontrol
            if [[ "$def_lost" =~ ^[0-9]+$ ]] && [ "$def_lost" -gt 0 ]; then return 1; fi
            if [[ "$ind_lost" =~ ^[0-9]+$ ]] && [ "$ind_lost" -gt 0 ]; then return 1; fi
        fi
    fi
    return 0 # Leak yok
}

compare_outputs() {
    # İki çıktıyı da temizle ve geçici dosyalara kaydet
    cat .minishell_output | clean_output > .minishell_clean
    cat .bash_output | clean_output > .bash_clean
    
    # Karşılaştır
    diff -q .minishell_clean .bash_clean > /dev/null 2>&1
    return $?
}

process_test() {
    local test_cmd="$1"
    local test_num="$2"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo -ne "${BLUE}[Test $test_num]${NC} Komut: ${YELLOW}$test_cmd${NC}..."

    # Çalıştır
    run_bash "$test_cmd"
    run_minishell "$test_cmd"

    # Karşılaştır
    if compare_outputs; then
        echo -e " ${GREEN}✓ BAŞARILI${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        
        # Leak Kontrolü (Sadece başarılıysa bak, fail ise zaten incelenecek)
        if [ "$USE_VALGRIND" = "1" ]; then
            if ! check_leaks; then
                echo -e "  ${YELLOW}⚠ LEAK BULUNDU${NC}"
                LEAKED_TESTS=$((LEAKED_TESTS + 1))
                {
                    echo "Test #$test_num: $test_cmd"
                    cat .valgrind_output
                    echo "----------------------------------------"
                } >> "$LEAK_FILE"
            fi
        fi
    else
        echo -e " ${RED}✗ BAŞARISIZ${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        {
            echo "Test #$test_num: $test_cmd"
            echo "--- Beklenen (Bash - Temizlenmiş) ---"
            cat .bash_clean
            echo "--- Alınan (Minishell - Temizlenmiş) ---"
            cat .minishell_clean
            echo "--- Minishell Ham Çıktı ---"
            cat .minishell_output
            if [ "$USE_VALGRIND" = "1" ]; then
                echo "--- Valgrind ---"
                cat .valgrind_output
            fi
            echo "========================================"
        } >> "$FAILED_FILE"
    fi
}

# --- Ana Akış ---

if [ $# -eq 0 ]; then
    TEST_FILE="tests.txt"
else
    TEST_FILE="$1"
fi

if [ ! -f "$TEST_FILE" ]; then
    echo -e "${RED}Hata: '$TEST_FILE' dosyası bulunamadı!${NC}"
    echo "Kullanım: ./tester.sh [test_dosyası]"
    exit 1
fi

echo -e "${BLUE}Testler başlatılıyor: $TEST_FILE${NC}"
echo "=========================================="

count=0
while IFS= read -r line || [ -n "$line" ]; do
    # Boş satırları ve yorumları atla
    if [[ -z "$line" ]] || [[ "$line" =~ ^[[:space:]]*# ]]; then
        continue
    fi
    count=$((count + 1))
    process_test "$line" "$count"
done < "$TEST_FILE"

# --- Özet ---
echo ""
echo "=========================================="
echo -e "Toplam: $TOTAL_TESTS | ${GREEN}Geçen: $PASSED_TESTS${NC} | ${RED}Kalan: $FAILED_TESTS${NC} | ${YELLOW}Leak: $LEAKED_TESTS${NC}"
echo "Hatalar için $FAILED_FILE dosyasına bakabilirsiniz."
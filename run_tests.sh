#\!/bin/bash

MINISHELL="./minishell"
FAULTS_FILE="faults_found.txt"
rm -f ""
touch ""

echo "=== MINISHELL COMPREHENSIVE TESTS ==="
echo ""

TOTAL=0
FAILED=0

run_test() {
    local test_name=""
    local cmd=""
    local bash_expected=""
    
    TOTAL=1
    echo "Test #: "
    
    result=
    
    if [ "" \!= "" ]; then
        FAILED=1
        echo "  ✗ FAIL" 
        echo "FAIL: " >> ""
        echo "  Command: " >> ""
        echo "  Expected: " >> ""
        echo "  Got: " >> ""
        echo "" >> ""
    else
        echo "  ✓ PASS"
    fi
}

# Basic echo tests
echo ""
echo "=== BASIC ECHO TESTS ==="
run_test "Simple echo" "echo hello" "hello"
run_test "Echo with multiple words" "echo hello world" "hello world"

# PWD test
echo ""
echo "=== PWD TEST ==="
expected_pwd=/mnt/c/Users/sora/Desktop/minishell-merge
run_test "PWD command" "pwd" ""

# Environment variable tests
echo ""
echo "=== ENVIRONMENT VARIABLE TESTS ==="
run_test "Echo HOME variable" "echo \/c/Users/sora" "/home/sora"

# Pipe tests
echo ""
echo "=== PIPE TESTS ==="
run_test "Simple pipe" "echo test | cat" "test"

# Quotes tests
echo ""
echo "=== QUOTES TESTS ==="
run_test "Single quotes" "echo 'hello world'" "hello world"
run_test "Double quotes" "echo \"hello world\"" "hello world"

# Logical operators (bonus)
echo ""
echo "=== LOGICAL OPERATORS (BONUS) ==="
run_test "AND with true" "true && echo success" "success"
run_test "OR with false" "false || echo success" "success"

# Summary
echo ""
echo "=== TEST SUMMARY ==="
echo "Total tests: "
echo "Failed tests: "
echo "Passed tests: 0"

if [  -eq 0 ]; then
    echo "✓ All tests passed\!"
else
    echo "✗ Some tests failed. Check  for details."
fi

echo "" >> ""
echo "=== SUMMARY ===" >> ""
echo "Total: , Failed: , Passed: 0" >> ""

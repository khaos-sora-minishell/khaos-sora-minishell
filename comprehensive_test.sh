#!/bin/bash

MINI="./minishell"
RESULTS="test_results_comprehensive.txt"
rm -f 

echo "=== COMPREHENSIVE MINISHELL TESTS ===" | tee -a 
echo "" | tee -a 

PASSED=0
FAILED=0
TOTAL=0

test_cmd() {
    local category=""
    local name=""
    local cmd=""
    
    TOTAL=1
    
    # Bash result
    bash_out=bash: line 1: : command not found
    bash_exit=127
    
    # Minishell result  
    mini_out=Try 'timeout --help' for more information.
    mini_exit=0
    
    if [ "" = "" ]; then
        echo "[] : ✓ PASS"
        PASSED=1
    else
        echo "[] : ✗ FAIL"
        FAILED=1
        echo "" >> 
        echo "FAIL: " >> 
        echo "Command: " >> 
        echo "Bash: []" >> 
        echo "Mini: []" >> 
    fi
}

echo "=== 1. BASIC COMMANDS ==="
test_cmd "BASIC" "echo hello" "echo hello"
test_cmd "BASIC" "echo with args" "echo hello world test"
test_cmd "BASIC" "echo -n" "echo -n test"
test_cmd "BASIC" "pwd" "pwd"

echo ""
echo "=== 2. BUILT-IN COMMANDS ==="
test_cmd "BUILTIN" "export and echo" "export TESTVAR=hello && echo \"
test_cmd "BUILTIN" "cd and pwd" "cd /tmp && pwd"

echo ""
echo "=== 3. PIPES ==="
test_cmd "PIPE" "simple pipe" "echo test | cat"
test_cmd "PIPE" "double pipe" "echo test | cat | cat"
test_cmd "PIPE" "pipe with grep" "echo hello | grep hello"

echo ""
echo "=== 4. REDIRECTIONS ==="
test_cmd "REDIR" "output redirect" "echo test > /tmp/minitest.txt && cat /tmp/minitest.txt"
test_cmd "REDIR" "append redirect" "echo line1 > /tmp/minitest.txt && echo line2 >> /tmp/minitest.txt && cat /tmp/minitest.txt"

echo ""
echo "=== 5. ENVIRONMENT VARIABLES ==="
test_cmd "ENV" "PATH variable" "echo \/c/Users/sora/bin:/mingw64/bin:/usr/local/bin:/usr/bin:/bin:/mingw64/bin:/usr/bin:/c/Users/sora/bin:/c/WINDOWS/system32:/c/WINDOWS:/c/WINDOWS/System32/Wbem:/c/WINDOWS/System32/WindowsPowerShell/v1.0:/c/WINDOWS/System32/OpenSSH:/cmd:/c/Program Files/nodejs:/c/Program Files/PuTTY:/c/Program Files (x86)/Yarn/bin:/c/Program Files/dotnet:/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common:/c/Program Files/NVIDIA Corporation/NVIDIA App/NvDLISR:/c/MinGW/bin:/c/Program Files/Cloudflare/Cloudflare WARP:/c/ProgramData/chocolatey/bin:/c/Program Files (x86)/Windows Kits/10/Windows Performance Toolkit:/c/Program Files/Docker/Docker/resources/bin:/c/Users/sora/AppData/Local/Programs/Python/Python310/Scripts:/c/Users/sora/AppData/Local/Programs/Python/Python310:/c/Users/sora/.cargo/bin:/c/WINDOWS/system32:/c/WINDOWS:/c/WINDOWS/System32/Wbem:/c/WINDOWS/System32/WindowsPowerShell/v1.0:/c/WINDOWS/System32/OpenSSH:/cmd:/c/Program Files/nodejs:/c/Program Files/PuTTY:/c/Program Files (x86)/Yarn/bin:/c/Program Files/dotnet:/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common:/c/Program Files/NVIDIA Corporation/NVIDIA App/NvDLISR:/c/MinGW/bin:/c/Program Files/Cloudflare/Cloudflare WARP:/c/Program Files/Docker/Docker/resources/bin:/c/ProgramData/chocolatey/bin:/c/Users/sora/AppData/Local/Programs/Python/Python312/Scripts:/c/Users/sora/AppData/Local/Programs/Python/Python312:/c/Users/sora/AppData/Local/Programs/Python/Python313/Scripts:/c/Users/sora/AppData/Local/Programs/Python/Python313:/c/Users/sora/AppData/Local/Microsoft/WindowsApps:/c/Users/sora/AppData/Roaming/npm:/c/Users/sora/AppData/Local/Programs/Microsoft VS Code/bin:/c/Users/sora/AppData/Local/Yarn/bin:/c/ProgramData/chocolatey/lib/sui/tools:/c/ProgramData/chocolatey/lib/sui/tools:/c/Users/sora/AppData/Local/Programs/Antigravity/bin:/c/Users/sora/AppData/Local/Microsoft/WinGet/Links:/c/Program Files/JetBrains/JetBrains Rider 2025.3.1/bin:/usr/bin/vendor_perl:/usr/bin/core_perl | head -c 20"
test_cmd "ENV" "HOME variable" "echo \/c/Users/sora"
test_cmd "ENV" "exit status" "echo \0"

echo ""
echo "=== 6. QUOTES ==="
test_cmd "QUOTE" "double quotes" "echo \"hello world\""
test_cmd "QUOTE" "single quotes" "echo 'hello world'"
test_cmd "QUOTE" "mixed quotes" "echo \"hello\"'world'"
test_cmd "QUOTE" "empty quotes" "echo \"\""

echo ""
echo "=== 7. LOGICAL OPERATORS ==="
test_cmd "LOGIC" "AND success" "true && echo success"
test_cmd "LOGIC" "AND fail" "false && echo fail"
test_cmd "LOGIC" "OR success" "true || echo fail"
test_cmd "LOGIC" "OR fail" "false || echo success"

echo ""
echo "=== 8. WILDCARDS ==="
test_cmd "WILDCARD" "asterisk" "ls *.c | wc -l"

echo ""
echo "=== SUMMARY ==="
echo "Total: "
echo "Passed: "
echo "Failed: "
echo "" >> 
echo "SUMMARY: / passed,  failed" >> 

if [  -gt 0 ]; then
    echo ""
    echo "Check  for details"
fi

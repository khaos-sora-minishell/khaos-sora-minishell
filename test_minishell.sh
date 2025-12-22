#!/bin/bash

# Test script for minishell vs bash comparison
# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

MINISHELL="./minishell"
RESULTS_FILE="test_results.txt"
FAULTS=()

echo "Starting minishell tests..." > 
echo "" >> 

test_command() {
    local cmd=""
    local description=""
    
    echo -e "Testing: "
    echo "Test: " >> 
    echo "Command: " >> 
    
    # Run in bash
    bash_output=bash: line 1: : command not found
    bash_exit=127
    
    # Run in minishell
    minishell_output=
    minishell_exit=0
    
    # Compare outputs
    if [ "" = "" ] && [  -eq  ]; then
        echo -e "✓ PASS"
        echo "Result: PASS" >> 
    else
        echo -e "✗ FAIL"
        echo "Result: FAIL" >> 
        echo "Bash output:  (exit: )" >> 
        echo "Minishell output:  (exit: )" >> 
        FAULTS+=("[] ")
    fi
    echo "" >> 
}

# Basic commands
echo -e "=== Basic Commands ==="
test_command "echo hello world" "Basic echo"
test_command "echo -n hello" "Echo with -n flag"
test_command "pwd" "Print working directory"
test_command "env | grep PATH" "Environment variables"

# Built-ins
echo -e "\n=== Built-in Commands ==="
test_command "export TEST=123 && echo \" "Export and use variable"
test_command "unset PATH && echo \/c/Users/sora/bin:/mingw64/bin:/usr/local/bin:/usr/bin:/bin:/mingw64/bin:/usr/bin:/c/Users/sora/bin:/c/WINDOWS/system32:/c/WINDOWS:/c/WINDOWS/System32/Wbem:/c/WINDOWS/System32/WindowsPowerShell/v1.0:/c/WINDOWS/System32/OpenSSH:/cmd:/c/Program Files/nodejs:/c/Program Files/PuTTY:/c/Program Files (x86)/Yarn/bin:/c/Program Files/dotnet:/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common:/c/Program Files/NVIDIA Corporation/NVIDIA App/NvDLISR:/c/MinGW/bin:/c/Program Files/Cloudflare/Cloudflare WARP:/c/ProgramData/chocolatey/bin:/c/Program Files (x86)/Windows Kits/10/Windows Performance Toolkit:/c/Program Files/Docker/Docker/resources/bin:/c/Users/sora/AppData/Local/Programs/Python/Python310/Scripts:/c/Users/sora/AppData/Local/Programs/Python/Python310:/c/Users/sora/.cargo/bin:/c/WINDOWS/system32:/c/WINDOWS:/c/WINDOWS/System32/Wbem:/c/WINDOWS/System32/WindowsPowerShell/v1.0:/c/WINDOWS/System32/OpenSSH:/cmd:/c/Program Files/nodejs:/c/Program Files/PuTTY:/c/Program Files (x86)/Yarn/bin:/c/Program Files/dotnet:/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common:/c/Program Files/NVIDIA Corporation/NVIDIA App/NvDLISR:/c/MinGW/bin:/c/Program Files/Cloudflare/Cloudflare WARP:/c/Program Files/Docker/Docker/resources/bin:/c/ProgramData/chocolatey/bin:/c/Users/sora/AppData/Local/Programs/Python/Python312/Scripts:/c/Users/sora/AppData/Local/Programs/Python/Python312:/c/Users/sora/AppData/Local/Programs/Python/Python313/Scripts:/c/Users/sora/AppData/Local/Programs/Python/Python313:/c/Users/sora/AppData/Local/Microsoft/WindowsApps:/c/Users/sora/AppData/Roaming/npm:/c/Users/sora/AppData/Local/Programs/Microsoft VS Code/bin:/c/Users/sora/AppData/Local/Yarn/bin:/c/ProgramData/chocolatey/lib/sui/tools:/c/ProgramData/chocolatey/lib/sui/tools:/c/Users/sora/AppData/Local/Programs/Antigravity/bin:/c/Users/sora/AppData/Local/Microsoft/WinGet/Links:/c/Program Files/JetBrains/JetBrains Rider 2025.3.1/bin:/usr/bin/vendor_perl:/usr/bin/core_perl" "Unset variable"

# Pipes
echo -e "\n=== Pipes ==="
test_command "echo hello | cat" "Simple pipe"
test_command "ls | wc -l" "Pipe with wc"
test_command "echo test | cat | cat | cat" "Multiple pipes"

# Redirections
echo -e "\n=== Redirections ==="
test_command "echo test > /tmp/test_minishell.txt && cat /tmp/test_minishell.txt" "Output redirection"
test_command "echo test >> /tmp/test_minishell.txt && cat /tmp/test_minishell.txt" "Append redirection"
test_command "cat < /etc/passwd | head -n 1" "Input redirection"

# Quotes
echo -e "\n=== Quotes ==="
test_command "echo 'hello world'" "Single quotes"
test_command "echo \"hello world\"" "Double quotes"
test_command "echo '\/c/Users/sora/bin:/mingw64/bin:/usr/local/bin:/usr/bin:/bin:/mingw64/bin:/usr/bin:/c/Users/sora/bin:/c/WINDOWS/system32:/c/WINDOWS:/c/WINDOWS/System32/Wbem:/c/WINDOWS/System32/WindowsPowerShell/v1.0:/c/WINDOWS/System32/OpenSSH:/cmd:/c/Program Files/nodejs:/c/Program Files/PuTTY:/c/Program Files (x86)/Yarn/bin:/c/Program Files/dotnet:/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common:/c/Program Files/NVIDIA Corporation/NVIDIA App/NvDLISR:/c/MinGW/bin:/c/Program Files/Cloudflare/Cloudflare WARP:/c/ProgramData/chocolatey/bin:/c/Program Files (x86)/Windows Kits/10/Windows Performance Toolkit:/c/Program Files/Docker/Docker/resources/bin:/c/Users/sora/AppData/Local/Programs/Python/Python310/Scripts:/c/Users/sora/AppData/Local/Programs/Python/Python310:/c/Users/sora/.cargo/bin:/c/WINDOWS/system32:/c/WINDOWS:/c/WINDOWS/System32/Wbem:/c/WINDOWS/System32/WindowsPowerShell/v1.0:/c/WINDOWS/System32/OpenSSH:/cmd:/c/Program Files/nodejs:/c/Program Files/PuTTY:/c/Program Files (x86)/Yarn/bin:/c/Program Files/dotnet:/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common:/c/Program Files/NVIDIA Corporation/NVIDIA App/NvDLISR:/c/MinGW/bin:/c/Program Files/Cloudflare/Cloudflare WARP:/c/Program Files/Docker/Docker/resources/bin:/c/ProgramData/chocolatey/bin:/c/Users/sora/AppData/Local/Programs/Python/Python312/Scripts:/c/Users/sora/AppData/Local/Programs/Python/Python312:/c/Users/sora/AppData/Local/Programs/Python/Python313/Scripts:/c/Users/sora/AppData/Local/Programs/Python/Python313:/c/Users/sora/AppData/Local/Microsoft/WindowsApps:/c/Users/sora/AppData/Roaming/npm:/c/Users/sora/AppData/Local/Programs/Microsoft VS Code/bin:/c/Users/sora/AppData/Local/Yarn/bin:/c/ProgramData/chocolatey/lib/sui/tools:/c/ProgramData/chocolatey/lib/sui/tools:/c/Users/sora/AppData/Local/Programs/Antigravity/bin:/c/Users/sora/AppData/Local/Microsoft/WinGet/Links:/c/Program Files/JetBrains/JetBrains Rider 2025.3.1/bin:/usr/bin/vendor_perl:/usr/bin/core_perl'" "Variable in single quotes"
test_command "echo \"\/c/Users/sora/bin:/mingw64/bin:/usr/local/bin:/usr/bin:/bin:/mingw64/bin:/usr/bin:/c/Users/sora/bin:/c/WINDOWS/system32:/c/WINDOWS:/c/WINDOWS/System32/Wbem:/c/WINDOWS/System32/WindowsPowerShell/v1.0:/c/WINDOWS/System32/OpenSSH:/cmd:/c/Program Files/nodejs:/c/Program Files/PuTTY:/c/Program Files (x86)/Yarn/bin:/c/Program Files/dotnet:/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common:/c/Program Files/NVIDIA Corporation/NVIDIA App/NvDLISR:/c/MinGW/bin:/c/Program Files/Cloudflare/Cloudflare WARP:/c/ProgramData/chocolatey/bin:/c/Program Files (x86)/Windows Kits/10/Windows Performance Toolkit:/c/Program Files/Docker/Docker/resources/bin:/c/Users/sora/AppData/Local/Programs/Python/Python310/Scripts:/c/Users/sora/AppData/Local/Programs/Python/Python310:/c/Users/sora/.cargo/bin:/c/WINDOWS/system32:/c/WINDOWS:/c/WINDOWS/System32/Wbem:/c/WINDOWS/System32/WindowsPowerShell/v1.0:/c/WINDOWS/System32/OpenSSH:/cmd:/c/Program Files/nodejs:/c/Program Files/PuTTY:/c/Program Files (x86)/Yarn/bin:/c/Program Files/dotnet:/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common:/c/Program Files/NVIDIA Corporation/NVIDIA App/NvDLISR:/c/MinGW/bin:/c/Program Files/Cloudflare/Cloudflare WARP:/c/Program Files/Docker/Docker/resources/bin:/c/ProgramData/chocolatey/bin:/c/Users/sora/AppData/Local/Programs/Python/Python312/Scripts:/c/Users/sora/AppData/Local/Programs/Python/Python312:/c/Users/sora/AppData/Local/Programs/Python/Python313/Scripts:/c/Users/sora/AppData/Local/Programs/Python/Python313:/c/Users/sora/AppData/Local/Microsoft/WindowsApps:/c/Users/sora/AppData/Roaming/npm:/c/Users/sora/AppData/Local/Programs/Microsoft VS Code/bin:/c/Users/sora/AppData/Local/Yarn/bin:/c/ProgramData/chocolatey/lib/sui/tools:/c/ProgramData/chocolatey/lib/sui/tools:/c/Users/sora/AppData/Local/Programs/Antigravity/bin:/c/Users/sora/AppData/Local/Microsoft/WinGet/Links:/c/Program Files/JetBrains/JetBrains Rider 2025.3.1/bin:/usr/bin/vendor_perl:/usr/bin/core_perl\"" "Variable in double quotes"

# Bonus: Logical operators
echo -e "\n=== Logical Operators (Bonus) ==="
test_command "true && echo success" "AND operator - success case"
test_command "false && echo fail" "AND operator - fail case"
test_command "true || echo fail" "OR operator - success case"
test_command "false || echo success" "OR operator - fail case"

# Exit status
echo -e "\n=== Exit Status ==="
test_command "ls /nonexistent 2>/dev/null ; echo \0" "Exit status after failed command"
test_command "true ; echo \0" "Exit status after true"
test_command "false ; echo \0" "Exit status after false"

# Summary
echo -e "\n=== Test Summary ==="
if [ 0 -eq 0 ]; then
    echo -e "All tests passed!"
else
    echo -e "Failed tests: 0"
    for fault in ""; do
        echo -e "  - "
    done
fi

echo "Total faults: 0" >> 

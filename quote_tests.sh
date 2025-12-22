#!/bin/bash

MINISHELL="./minishell"
echo "=== COMPREHENSIVE QUOTE TESTS ==="
echo ""

test_quote() {
    local test_name=""
    local cmd=""
    
    echo "Test: "
    echo "Command: "
    
    # Bash result
    bash_result=bash: line 1: : command not found
    
    # Minishell result
    mini_result=Try 'timeout --help' for more information.
    
    echo "Bash: []"
    echo "Mini: []"
    
    if [ "" = "" ]; then
        echo "✓ PASS"
    else
        echo "✗ FAIL"
        echo "DIFFERENCE DETECTED!" >> quote_faults.txt
        echo "Test: " >> quote_faults.txt
        echo "Command: " >> quote_faults.txt
        echo "Bash: []" >> quote_faults.txt
        echo "Mini: []" >> quote_faults.txt
        echo "" >> quote_faults.txt
    fi
    echo ""
}

rm -f quote_faults.txt

# Standard double quotes
echo "=== DOUBLE QUOTES ==="
test_quote "Standard double quotes" 'echo "hello world"'
test_quote "Double quotes with variable" 'echo "/home/sora"'
test_quote "Empty double quotes" 'echo ""'
test_quote "Double quotes with spaces" 'echo "  hello  world  "'

# Standard single quotes
echo "=== SINGLE QUOTES ==="
test_quote "Standard single quotes" "echo 'hello world'"
test_quote "Single quotes with variable (no expansion)" "echo '\/c/Users/sora'"
test_quote "Empty single quotes" "echo ''"
test_quote "Single quotes with spaces" "echo '  hello  world  '"

# Mixed quotes
echo "=== MIXED QUOTES ==="
test_quote "Double then single" 'echo "hello" '"'"'world'"'"''
test_quote "Single then double" "echo 'hello' \"world\""
test_quote "Alternating quotes" 'echo "a"'"'"'b'"'"'"c"'

# Special characters in quotes
echo "=== SPECIAL CHARACTERS ==="
test_quote "Dollar in double quotes" 'echo "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/usr/lib/wsl/lib:/mnt/c/Users/sora/bin:/mnt/c/Program Files/Git/mingw64/bin:/mnt/c/Program Files/Git/usr/local/bin:/mnt/c/Program Files/Git/usr/bin:/mnt/c/Program Files/Git/usr/bin:/mnt/c/Program Files/Git/mingw64/bin:/mnt/c/Program Files/Git/usr/bin:/mnt/c/Users/sora/bin:/mnt/c/WINDOWS/system32:/mnt/c/WINDOWS:/mnt/c/WINDOWS/System32/Wbem:/mnt/c/WINDOWS/System32/WindowsPowerShell/v1.0:/mnt/c/WINDOWS/System32/OpenSSH:/mnt/c/Program Files/Git/cmd:/mnt/c/Program Files/nodejs:/mnt/c/Program Files/PuTTY:/mnt/c/Program Files (x86)/Yarn/bin:/mnt/c/Program Files/dotnet:/mnt/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common:/mnt/c/Program Files/NVIDIA Corporation/NVIDIA App/NvDLISR:/mnt/c/MinGW/bin:/mnt/c/Program Files/Cloudflare/Cloudflare WARP:/mnt/c/ProgramData/chocolatey/bin:/mnt/c/Program Files (x86)/Windows Kits/10/Windows Performance Toolkit:/mnt/c/Program Files/Docker/Docker/resources/bin:/mnt/c/Users/sora/AppData/Local/Programs/Python/Python310/Scripts:/mnt/c/Users/sora/AppData/Local/Programs/Python/Python310:/mnt/c/Users/sora/.cargo/bin:/mnt/c/WINDOWS/system32:/mnt/c/WINDOWS:/mnt/c/WINDOWS/System32/Wbem:/mnt/c/WINDOWS/System32/WindowsPowerShell/v1.0:/mnt/c/WINDOWS/System32/OpenSSH:/mnt/c/Program Files/Git/cmd:/mnt/c/Program Files/nodejs:/mnt/c/Program Files/PuTTY:/mnt/c/Program Files (x86)/Yarn/bin:/mnt/c/Program Files/dotnet:/mnt/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common:/mnt/c/Program Files/NVIDIA Corporation/NVIDIA App/NvDLISR:/mnt/c/MinGW/bin:/mnt/c/Program Files/Cloudflare/Cloudflare WARP:/mnt/c/Program Files/Docker/Docker/resources/bin:/mnt/c/ProgramData/chocolatey/bin:/mnt/c/Users/sora/AppData/Local/Programs/Python/Python312/Scripts:/mnt/c/Users/sora/AppData/Local/Programs/Python/Python312:/mnt/c/Users/sora/AppData/Local/Programs/Python/Python313/Scripts:/mnt/c/Users/sora/AppData/Local/Programs/Python/Python313:/mnt/c/Users/sora/AppData/Local/Microsoft/WindowsApps:/mnt/c/Users/sora/AppData/Roaming/npm:/mnt/c/Users/sora/AppData/Local/Programs/Microsoft VS Code/bin:/mnt/c/Users/sora/AppData/Local/Yarn/bin:/mnt/c/ProgramData/chocolatey/lib/sui/tools:/mnt/c/ProgramData/chocolatey/lib/sui/tools:/mnt/c/Users/sora/AppData/Local/Programs/Antigravity/bin:/mnt/c/Users/sora/AppData/Local/Microsoft/WinGet/Links:/mnt/c/Program Files/JetBrains/JetBrains Rider 2025.3.1/bin:/mnt/c/Program Files/Git/usr/bin/vendor_perl:/mnt/c/Program Files/Git/usr/bin/core_perl"'
test_quote "Dollar in single quotes" "echo '\/c/Users/sora/bin:/mingw64/bin:/usr/local/bin:/usr/bin:/bin:/mingw64/bin:/usr/bin:/c/Users/sora/bin:/c/WINDOWS/system32:/c/WINDOWS:/c/WINDOWS/System32/Wbem:/c/WINDOWS/System32/WindowsPowerShell/v1.0:/c/WINDOWS/System32/OpenSSH:/cmd:/c/Program Files/nodejs:/c/Program Files/PuTTY:/c/Program Files (x86)/Yarn/bin:/c/Program Files/dotnet:/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common:/c/Program Files/NVIDIA Corporation/NVIDIA App/NvDLISR:/c/MinGW/bin:/c/Program Files/Cloudflare/Cloudflare WARP:/c/ProgramData/chocolatey/bin:/c/Program Files (x86)/Windows Kits/10/Windows Performance Toolkit:/c/Program Files/Docker/Docker/resources/bin:/c/Users/sora/AppData/Local/Programs/Python/Python310/Scripts:/c/Users/sora/AppData/Local/Programs/Python/Python310:/c/Users/sora/.cargo/bin:/c/WINDOWS/system32:/c/WINDOWS:/c/WINDOWS/System32/Wbem:/c/WINDOWS/System32/WindowsPowerShell/v1.0:/c/WINDOWS/System32/OpenSSH:/cmd:/c/Program Files/nodejs:/c/Program Files/PuTTY:/c/Program Files (x86)/Yarn/bin:/c/Program Files/dotnet:/c/Program Files (x86)/NVIDIA Corporation/PhysX/Common:/c/Program Files/NVIDIA Corporation/NVIDIA App/NvDLISR:/c/MinGW/bin:/c/Program Files/Cloudflare/Cloudflare WARP:/c/Program Files/Docker/Docker/resources/bin:/c/ProgramData/chocolatey/bin:/c/Users/sora/AppData/Local/Programs/Python/Python312/Scripts:/c/Users/sora/AppData/Local/Programs/Python/Python312:/c/Users/sora/AppData/Local/Programs/Python/Python313/Scripts:/c/Users/sora/AppData/Local/Programs/Python/Python313:/c/Users/sora/AppData/Local/Microsoft/WindowsApps:/c/Users/sora/AppData/Roaming/npm:/c/Users/sora/AppData/Local/Programs/Microsoft VS Code/bin:/c/Users/sora/AppData/Local/Yarn/bin:/c/ProgramData/chocolatey/lib/sui/tools:/c/ProgramData/chocolatey/lib/sui/tools:/c/Users/sora/AppData/Local/Programs/Antigravity/bin:/c/Users/sora/AppData/Local/Microsoft/WinGet/Links:/c/Program Files/JetBrains/JetBrains Rider 2025.3.1/bin:/usr/bin/vendor_perl:/usr/bin/core_perl'"
test_quote "Asterisk in double quotes" 'echo "*"'
test_quote "Asterisk in single quotes" "echo '*'"
test_quote "Pipe in quotes" 'echo "|"'
test_quote "Semicolon in quotes" 'echo ";"'

# Edge cases
echo "=== EDGE CASES ==="
test_quote "Only quotes" 'echo """"'
test_quote "Space between quotes" 'echo "" ""'
test_quote "Quote at start" 'echo "hello'
test_quote "Quote at end" 'echo hello"'

echo ""
if [ -f quote_faults.txt ]; then
    echo "✗ Some tests failed. Check quote_faults.txt"
    cat quote_faults.txt
else
    echo "✓ All quote tests passed!"
fi

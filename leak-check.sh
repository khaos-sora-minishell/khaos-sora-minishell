#!/bin/bash

# Get program name from argument or use default
PROGRAM="${1:-minishell}"

# Remove ./ prefix if present
PROGRAM="${PROGRAM#./}"

# Check if program exists, if not run make
if [ ! -f "$PROGRAM" ]; then
    echo "Program '$PROGRAM' not found. Please run 'make' first."
    exit 1
fi

# Create readline.supp if it doesn't exist
if [ ! -f "readline.supp" ]; then
    echo "readline.supp not found, creating..."
    cat > readline.supp << 'EOF'
{
   readline_leaks
   Memcheck:Leak
   ...
   fun:readline
}

{
   readline_add_history
   Memcheck:Leak
   ...
   fun:add_history
}

{
   tinfo_leaks
   Memcheck:Leak
   ...
   obj:*/libtinfo.so*
}

{
   ncurses_leaks
   Memcheck:Leak
   ...
   fun:_nc_*
}
EOF
    echo "readline.supp created successfully."
fi

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=readline.supp ./"$PROGRAM"

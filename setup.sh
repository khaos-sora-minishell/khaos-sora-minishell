#!/bin/bash

# Minishell Setup Script
# This script sets up aliases and configurations for minishell

SHELL_RC=""
MINISHELL_PATH="$(pwd)/minishell"

# Detect shell configuration file
if [ -n "$ZSH_VERSION" ]; then
    SHELL_RC="$HOME/.zshrc"
elif [ -n "$BASH_VERSION" ]; then
    SHELL_RC="$HOME/.bashrc"
else
    echo "Error: Unsupported shell. Please use bash or zsh."
    exit 1
fi

echo "================================"
echo "  Minishell Setup"
echo "================================"
echo ""

# Check if minishell binary exists
if [ ! -f "$MINISHELL_PATH" ]; then
    echo "Error: minishell binary not found!"
    echo "Please run 'make' first to build minishell."
    exit 1
fi

# Backup shell configuration
if [ -f "$SHELL_RC" ]; then
    cp "$SHELL_RC" "${SHELL_RC}.backup.$(date +%Y%m%d_%H%M%S)"
    echo "✓ Backed up $SHELL_RC"
fi

# Add minishell alias and mode function
echo "" >> "$SHELL_RC"
echo "# Minishell Configuration" >> "$SHELL_RC"
echo "alias minishell='$MINISHELL_PATH'" >> "$SHELL_RC"
echo "" >> "$SHELL_RC"
echo "# Mode function to switch between minishell and zsh" >> "$SHELL_RC"
echo 'mode() {' >> "$SHELL_RC"
echo '    if [ "$1" = "0" ]; then' >> "$SHELL_RC"
echo '        echo "Switching to ZSH..."' >> "$SHELL_RC"
echo '        exec zsh' >> "$SHELL_RC"
echo '    elif [ "$1" = "1" ]; then' >> "$SHELL_RC"
echo '        echo "Switching to Minishell..."' >> "$SHELL_RC"
echo "        exec $MINISHELL_PATH" >> "$SHELL_RC"
echo '    else' >> "$SHELL_RC"
echo '        echo "Usage: mode [0|1]"' >> "$SHELL_RC"
echo '        echo "  0: Switch to ZSH"' >> "$SHELL_RC"
echo '        echo "  1: Switch to Minishell"' >> "$SHELL_RC"
echo '    fi' >> "$SHELL_RC"
echo '}' >> "$SHELL_RC"
echo "" >> "$SHELL_RC"

# Optionally set minishell as default shell in terminal
echo "# Auto-start minishell (uncomment to enable)" >> "$SHELL_RC"
echo "# if [ -z \"\$MINISHELL_LOADED\" ]; then" >> "$SHELL_RC"
echo "#     export MINISHELL_LOADED=1" >> "$SHELL_RC"
echo "#     exec $MINISHELL_PATH" >> "$SHELL_RC"
echo "# fi" >> "$SHELL_RC"

echo ""
echo "✓ Added minishell alias to $SHELL_RC"
echo "✓ Added mode function to switch between shells"
echo ""
echo "Usage:"
echo "  - Run 'source $SHELL_RC' to apply changes"
echo "  - Run 'minishell' to start minishell"
echo "  - Run 'mode 0' to switch to ZSH"
echo "  - Run 'mode 1' to switch to Minishell"
echo ""
echo "To auto-start minishell when opening terminal:"
echo "  Edit $SHELL_RC and uncomment the auto-start section"
echo ""
echo "Setup completed successfully!"

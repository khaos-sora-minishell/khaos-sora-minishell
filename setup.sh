#!/bin/bash

# khaos_sora_shell Setup Script
# This script sets up aliases and configurations for khaos_sora_shell

SHELL_RC=""
khaos_sora_shell_PATH="$(pwd)/khaos_sora_shell"

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
echo "  khaos_sora_shell Setup"
echo "================================"
echo ""

# Check if khaos_sora_shell binary exists
if [ ! -f "$khaos_sora_shell_PATH" ]; then
    echo "Error: khaos_sora_shell binary not found!"
    echo "Please run 'make' first to build khaos_sora_shell."
    exit 1
fi

# Backup shell configuration
if [ -f "$SHELL_RC" ]; then
    cp "$SHELL_RC" "${SHELL_RC}.backup.$(date +%Y%m%d_%H%M%S)"
    echo "✓ Backed up $SHELL_RC"
fi

# Add khaos_sora_shell alias and mode function
echo "" >> "$SHELL_RC"
echo "# khaos_sora_shell Configuration" >> "$SHELL_RC"
echo "alias khaos_sora_shell='$khaos_sora_shell_PATH'" >> "$SHELL_RC"
echo "" >> "$SHELL_RC"
echo "# Mode function to switch between khaos_sora_shell and zsh" >> "$SHELL_RC"
echo 'mode() {' >> "$SHELL_RC"
echo '    if [ "$1" = "0" ]; then' >> "$SHELL_RC"
echo '        echo "Switching to ZSH..."' >> "$SHELL_RC"
echo '        exec zsh' >> "$SHELL_RC"
echo '    elif [ "$1" = "1" ]; then' >> "$SHELL_RC"
echo '        echo "Switching to khaos_sora_shell..."' >> "$SHELL_RC"
echo "        exec $khaos_sora_shell_PATH" >> "$SHELL_RC"
echo '    else' >> "$SHELL_RC"
echo '        echo "Usage: mode [0|1]"' >> "$SHELL_RC"
echo '        echo "  0: Switch to ZSH"' >> "$SHELL_RC"
echo '        echo "  1: Switch to khaos_sora_shell"' >> "$SHELL_RC"
echo '    fi' >> "$SHELL_RC"
echo '}' >> "$SHELL_RC"
echo "" >> "$SHELL_RC"

# Optionally set khaos_sora_shell as default shell in terminal
echo "# Auto-start khaos_sora_shell (uncomment to enable)" >> "$SHELL_RC"
echo "# if [ -z \"\$khaos_sora_shell_LOADED\" ]; then" >> "$SHELL_RC"
echo "#     export khaos_sora_shell_LOADED=1" >> "$SHELL_RC"
echo "#     exec $khaos_sora_shell_PATH" >> "$SHELL_RC"
echo "# fi" >> "$SHELL_RC"

echo ""
echo "✓ Added khaos_sora_shell alias to $SHELL_RC"
echo "✓ Added mode function to switch between shells"
echo ""
echo "Usage:"
echo "  - Run 'source $SHELL_RC' to apply changes"
echo "  - Run 'khaos_sora_shell' to start khaos_sora_shell"
echo "  - Run 'mode 0' to switch to ZSH"
echo "  - Run 'mode 1' to switch to khaos_sora_shell"
echo ""
echo "To auto-start khaos_sora_shell when opening terminal:"
echo "  Edit $SHELL_RC and uncomment the auto-start section"
echo ""
echo "Setup completed successfully!"

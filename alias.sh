#!/bin/bash

GREEN="\033[32m"
YELLOW="\033[33m"
CYAN="\033[36m"
RESET="\033[0m"

PROJECT_PATH=$(pwd)
SHELL_RC=""

chmod +x run_webserv.sh

if [[ "$SHELL" == *"zsh"* ]]; then
    SHELL_RC="$HOME/.zshrc"
elif [[ "$SHELL" == *"bash"* ]]; then
    SHELL_RC="$HOME/.bashrc"
fi

if [ -n "$SHELL_RC" ]; then
    echo -e "${CYAN}Adding 'run' alias to $SHELL_RC...${RESET}"
    
    # Check if the alias already exists to avoid duplicates
    if grep -q "alias run=" "$SHELL_RC"; then
        echo -e "${YELLOW}Alias 'run' already exists in $SHELL_RC. Updating path...${RESET}"
        # Remove old alias before adding new one
        sed -i '' '/alias run=/d' "$SHELL_RC" 2>/dev/null || sed -i '/alias run=/d' "$SHELL_RC"
    fi

    # Append the new alias
    echo "alias run='cd $PROJECT_PATH && ./run_webserv.sh'" >> "$SHELL_RC"
    
    echo -e "${GREEN}Success! Alias added.${RESET}"
    echo -e "${YELLOW}Please run 'source $SHELL_RC' or restart your terminal to use it.${RESET}"
else
    echo -e "${RED}Could not detect .zshrc or .bashrc. Please add the alias manually.${RESET}"
fi
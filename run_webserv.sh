#!/bin/bash

CYAN="\033[36m"
GREEN="\033[32m"
RED="\033[31m"
RESET="\033[0m"

echo -e "${CYAN}Compiling Webserv...${RESET}"
make -j

if [ $? -eq 0 ]; then
    echo -e "${GREEN}Compilation Successful!${RESET}"
    CONFIG=${1:-"config/default.conf"}
    
    echo -e "${CYAN}Starting server with: ${CONFIG}${RESET}"
    echo -e "${GREEN}---------------------------------------${RESET}"
    ./webserv "$CONFIG"
else
    echo -e "${RED}Error: Compilation failed.${RESET}"
    exit 1
fi

make fclean
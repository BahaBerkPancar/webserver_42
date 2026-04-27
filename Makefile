NAME        = webserv
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -Iinclude

OBJDIR      = .obj
SRCS        =   srcs/main.cpp \
				srcs/Lexer.cpp \
				srcs/Parser.cpp \
				# srcs/etc/checks.cpp \
				# srcs/etc/Utils.cpp \
				# srcs/config/Utils.cpp \
				# srcs/config/Config.cpp \
				# srcs/server/Server.cpp \
				# srcs/server/Client.cpp \
				# srcs/request/HttpRequest.cpp \
				# srcs/response/HttpResponse.cpp \
				# srcs/cgi/CGI.cpp

OBJS        = $(SRCS:srcs/%.cpp=$(OBJDIR)/%.o)

BLACK  = \033[0;30m
RED    = \033[0;31m
GREEN  = \033[0;32m
YELLOW = \033[0;33m
BLUE   = \033[0;34m
PURPLE = \033[0;35m
CYAN   = \033[0;36m
WHITE  = \033[0;37m
B_RED    = \033[1;31m
B_GREEN  = \033[1;32m
B_YELLOW = \033[1;33m
B_BLUE   = \033[1;34m
B_PURPLE = \033[1;35m
B_CYAN   = \033[1;36m
B_WHITE  = \033[1;37m
HI_ORANGE = \033[0;91m
HI_MAGENTA = \033[0;95m
HI_CYAN    = \033[0;96m
RESET  = \033[0m

all: $(NAME)

$(OBJDIR)/%.o: srcs/%.cpp
	@mkdir -p $(dir $@)
	@printf "$(WHITE)Compiling: $(RESET)%-30s " "$<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@printf "$(GREEN)[OK]$(RESET)\n"

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "\n$(B_GREEN)@ DONE $(NAME) ✅$(RESET)\n"

clean:
	@rm -rf $(OBJDIR)
	@echo "$(BLACK)@ Cleaned All Objects$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(BLACK)@ Cleaned $(NAME)$(RESET)"

re: fclean all

run: all
	@echo "$(B_CYAN)---------------------------------------$(RESET)"
	@./$(NAME) config/default.conf

.PHONY: all clean fclean re run
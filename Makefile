# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -Ihdr
LDFLAGS = -lcheck -lsubunit

# Directories
SRCDIR = sources
OBJDIR = objects
HDRDIR = includes
TESTDIR = tests

# Project name and files
NAME = ft_ping
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
TEST_SRCS = $(wildcard $(TESTDIR)/*.c)
TEST_OBJS = $(patsubst $(TESTDIR)/%.c, $(OBJDIR)/%.o, $(TEST_SRCS))
TEST_NAMES = $(patsubst $(TESTDIR)/%.c, test_%, $(TEST_SRCS))

# Default rule to compile everything
all: $(NAME)

# Linking object files to create the final executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

# Compiling C files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(TESTDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure the object directory exists
$(OBJDIR):
	@mkdir -p $(OBJDIR)

# Clean rule to remove object files
clean:
	rm -f $(OBJS) $(TEST_OBJS)
	rm -rf $(OBJDIR)

# Full clean rule to remove object files and the executable
fclean: clean
	rm -f $(NAME) $(TEST_NAMES)
	rm -rf $(OBJDIR)

# Rebuild everything
re: fclean all 

run: $(NAME)
	@if [ -z "$(ARGS)" ]; then \
		if [ "$(SUDO)" = "1" ]; then \
			echo "Running $(NAME) with sudo without arguments."; \
			sudo $(NAME); \
		else \
			echo "Running $(NAME) without arguments."; \
			$(NAME); \
		fi \
	else \
		if [ "$(SUDO)" = "1" ]; then \
			echo "Running $(NAME) with sudo and arguments: $(ARGS)"; \
			sudo $(NAME) $(ARGS); \
		else \
			echo "Running $(NAME) with arguments: $(ARGS)"; \
			$(NAME) $(ARGS); \
		fi \
	fi

test: $(TEST_NAMES)

test_%: $(OBJDIR)/%.o $(filter-out $(OBJDIR)/main.o, $(OBJS))
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@if [ "$(SUDO)" = "1" ]; then \
		sudo ./$@; \
	else \
		./$@; \
	fi

ARGS ?= ""
SUDO ?= 0

# Phony targets to avoid conflicts with files named as the targets
.PHONY: all clean fclean re run test
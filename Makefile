# # kilo is what we want to build, kilo.c is what is required to build it
# kilo: kilo.c
# 	$(CC) kilo.c -o kilo -Wall -Wextra -pedantic -std=c99

# run: kilo
# 	./kilo

CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -pedantic
LDFLAGS = 
SRCDIR = src
OBJDIR = obj
BINDIR = bin
# generate list of all .c files in src directory
SOURCES = $(wildcard $(SRCDIR)/*.c)
# create list of object file paths from list of source file paths
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/kilo

# rule for building target from prerequisite (object files). After colon: prerequisite
# $^: list of prerequisites (object files), $@: name of the target
$(TARGET): $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) $^ -o $@

# rule to create object files from .c files. After colon: prerequisite
# -c: compile source file into object file but don't link it. Generates .o file
# $<: represents the first prerequisite of the rule
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: 
	$(BINDIR)/kilo

clean: 
	rm -rf $(OBJDIR) $(BINDIR)
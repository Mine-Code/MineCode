
# Commands
CXX := g++
CC  := gcc

## Directorys
SRCDIR := src

BUILD  := build
OBJDIR := $(BUILD)/obj
DEPDIR := $(BUILD)/dep

## Files
TARGET=MineCode

## Arguments
DEPEND   ?= -MMD -MP -MF $(subst :,/,$(subst /,_,$(@:obj/%.o=dep:%.d)))
INCLUDES ?= -Iinclude
COMPFLAGS?= -Wall -Wextra -c $(INCLUDES) $(DEPEND)

LDFLAGS  ?=
CXXFLAGS ?= $(COMPFLAGS) -std=gnu++17
CFLAGS   ?= $(COMPFLAGS) -std=gnu11

## File list
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEPS := $(SRCS:$(SRCDIR)/%.cpp=$(DEPDIR)/%.d)

### Targets
.PHONY: all clean run test
all: $(TARGET)

# PHONY Targets
clean:
	$(RM) dep/* obj/*
	$(RM) $(TARGET)

test:
	@echo "compile [test $(TEST)]"
	@$(CXX) $(INCLUDES) -o test/$(TEST) test/$(TEST).cpp
	@echo "run [test $(TEST)]"
	@test/$(TEST)

run: $(TARGET)
	@./$(TARGET)

info:
	@echo "--------------------"
	@echo "| source  : $(SRCDIR) : $(SRCS)"
	@echo "| includes: include : -----"
	@echo "| object  : $(OBJDIR) : $(OBJS)"
	@echo "| depend  : $(DEPDIR) : $(DEPS)"
	@echo "|-------------------"
	@echo "| exec"
	@echo "| cxx:$(CXX)"
	@echo "|   c:$(CC)"
	@echo "|-------------------"
	@echo "| flags"
	@echo "| cxx: $(CXXFLAGS)"
	@echo "|   c: $(CFLAGS)"
	@echo "|  ld: $(LDFLAGS)"
	@echo "--------------------"

# Program Rule
$(TARGET): $(OBJS)
	@echo "building $@"
	@$(CXX) -o $@ $(LDFLAGS) $(OBJS)

# .cpp -> .o
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "compile '$(SRCDIR)/$*.cpp' to '$@'"
	@$(CXX) -o $@ $(CXXFLAGS) $(SRCDIR)/$*.cpp

# .c -> .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "compile '$(SRCDIR)/$*.c' to '$@'"
	@$(CC)  -o $@ $(CFLAGS) $(SRCDIR)/$*.c

# Source Dependencies
-include $(DEPS)

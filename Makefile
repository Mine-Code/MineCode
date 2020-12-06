
# platform switcher
ifeq ($(OS),Windows_NT)
	FEXT := .exe
	executable = 
else
	FEXT :=
	executable = chmod +x $1
endif

# Commands
CXX := g++
CC  := gcc

## Directorys
SRCDIR := source/src

BUILD  := build
OBJDIR := $(BUILD)/obj
DEPDIR := $(BUILD)/dep
TESTEXE:=$(BUILD)/test

## Files
TARGET=MineCode

## Arguments
DEPEND   ?= -MMD -MP -MF $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d)
INCLUDES ?= -Isource/include
COMPFLAGS?= -Wall -Wextra $(INCLUDES)

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
	$(RM) $(DEPDIR)/* $(OBJDIR)/* $(TESTEXE)/*
	$(RM) $(TARGET)

test: $(TESTEXE)/$(TEST)
	@echo "run [test $(TEST)]"
	@./$<

run: $(TARGET)
	@./$<

info:
	@echo "--------------------"
	@echo "| source  : $(SRCDIR) : $(SRCS)"
	@echo "| includes: include : -----"
	@echo "| object  : $(OBJDIR) : $(OBJS)"
	@echo "| depend  : $(DEPDIR) : $(DEPS)"
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
	@$(call executable,$@)

# .cpp -> .o
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "compile[ src] $*"
	@$(CXX) $(DEPEND) $(CXXFLAGS) -c -o $@ $<

# .c -> .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "compile[ src] $*"
	@$(CC)  $(DEPEND) $(CFLAGS)   -c -o $@ $<

# test source
$(TESTEXE)/%: test/%.cpp
	@echo "compile[test] $*"
	@$(CXX) $(DEPEND) $(CXXFLAGS) -o $@ $<
	@$(call executable,$@)

# Source Dependencies
-include $(DEPS)

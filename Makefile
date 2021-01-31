
# Debug switcher
DEBUG := 0
ifeq ($(DEBUG),1)
	DBG_OPT_COMPILE := -g -Og -rdynamic
	DBG_OPT_COMMON  := -D _GLIBCXX_DEBUG -D _GLIBCXX_DEBUG_PEDANTIC
else
	DBG_OPT_COMPILE := 
	DBG_OPT_COMMON  := 
endif
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
SRCDIR := src

BUILD  := build
OBJDIR := $(BUILD)/obj
DEPDIR := $(BUILD)/dep
TESTEXE:=$(BUILD)/test

## Files
TARGET=MineCode

## Arguments
DEPEND   ?= -MMD -MP -MF $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d)
INCLUDES ?= -Isrc/include -Ilib/json/include
COMPFLAGS?= -Wall -Wextra $(INCLUDES) $(DBG_OPT_COMPILE) -pedantic

LDFLAGS  ?= $(DBG_OPT_COMMON)
CXXFLAGS ?= $(DBG_OPT_COMMON) $(COMPFLAGS) -std=c++14
CFLAGS   ?= $(DBG_OPT_COMMON) $(COMPFLAGS) -std=gnu11

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
	@echo "run    [test] $(TEST)"
	@./$<

run: $(TARGET)
	@./$< $(arg)

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
	@$(CXX) -o $@$(FEXT) $(LDFLAGS) $(OBJS)
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
$(TESTEXE)/%: test/%/main.cpp $(OBJS)
	@echo "compile[test] $*"
	@$(CXX) $(DEPEND) $(CXXFLAGS) -o $@$(FEXT) $(filter-out %/main.o,$^)
	@$(call executable,$@)

# Source Dependencies
-include $(DEPS)

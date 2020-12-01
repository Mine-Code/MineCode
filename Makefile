
# Commands
CXX := g++
CC  := gcc

## Directorys
SRCDIR := src
OBJDIR := obj
DEPDIR := dep
INCDIR := inc

## Files
TARGET=MineCode

## Arguments
DEPEND   ?= -MMD -MP -MF $(subst :,/,$(subst /,_,$(@:obj/%.o=dep:%.d)))
COMPFLAGS?= -Wall -Wextra -c
LDFLAGS  ?=
CXXFLAGS ?= $(addprefix -I,$(INCDIR)) $(DEPEND) $(COMPFLAGS) 
CFLAGS   ?= $(addprefix -I,$(INCDIR)) $(DEPEND) $(COMPFLAGS)

## File list
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEPS := $(SRCS:$(SRCDIR)/%.cpp=$(DEPDIR)/%.d)

### Targets
.PHONY: all clean
all: $(TARGET)

# PHONY Targets
clean:
	$(RM) dep/* obj/*
	$(RM) $(TARGET)

info:
	@echo "--------------------"
	@echo "| source  : $(SRCDIR) : $(SRCS)"
	@echo "| include : $(INCDIR) : -----"
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
	@$(CXX) -std=gnu++11 -o $@ $(CXXFLAGS) $(SRCDIR)/$*.cpp

# .c -> .o
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "compile '$(SRCDIR)/$*.c' to '$@'"
	@$(C) -std=gnu11 -o $@ $(CFLAGS) $(SRCDIR)/$*.c

# Source Dependencies
-include $(DEPS)

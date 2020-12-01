
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
CXXDEP   ?= -MMD -MP -MF $(subst :,/,$(subst /,_,$(@:obj/%.o=dep:%.d)))
LDFLAGS  ?=
CXXFLAGS ?= $(addprefix -I,$(INCDIR))

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
	@$(CXX) -o $@ $(WX_CPPFLAGS) $(LDFLAGS) $(OBJS) $(WX_LIB)

# .cpp -> .o
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "compile '$(SRCDIR)/$*.cpp' to '$@'"
	@$(CXX) -std=gnu++11 -c -o $@ $(CXXDEP) -Wall -Wextra  $(WX_CPPFLAGS) -I $(INCDIR) $(CXXFLAGS) $(SRCDIR)/$*.cpp

# Source Dependencies
-include $(DEPS)

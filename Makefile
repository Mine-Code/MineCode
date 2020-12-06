
# Commands
CXX := g++
CC  := gcc

## Directorys
SRCDIR := source/src

BUILD  := build
OBJDIR := $(BUILD)/obj
DEPDIR := $(BUILD)/dep

## Files
TARGET=MineCode

## Arguments
DEPEND   ?= -MMD -MP -MF $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d)
INCLUDES ?= -Isource/include
COMPFLAGS?= -Wall -Wextra -c $(INCLUDES)

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

test: test/$(TEST)
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
	@$(CXX) $(DEPEND) $(CXXFLAGS) -o $@ $(SRCDIR)/$*.cpp

# .c -> .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "compile '$(SRCDIR)/$*.c' to '$@'"
	@$(CC)  $(DEPEND) $(CFLAGS)   -o $@ $(SRCDIR)/$*.c

# test source
test/%: test/%.cpp
	@echo "compile 'test/$*.cpp' to '$@'"
	@$(CXX) $(DEPEND) $(CXXFLAGS) -o $@ $<

# Source Dependencies
-include $(DEPS)

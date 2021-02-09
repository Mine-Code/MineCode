# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/syoch/projects/wiiu/MineCode

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/syoch/projects/wiiu/MineCode

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/syoch/projects/wiiu/MineCode/CMakeFiles /home/syoch/projects/wiiu/MineCode/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/syoch/projects/wiiu/MineCode/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named MineCode

# Build rule for target.
MineCode: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 MineCode
.PHONY : MineCode

# fast build rule for target.
MineCode/fast:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/build
.PHONY : MineCode/fast

src/asm.o: src/asm.cpp.o

.PHONY : src/asm.o

# target to build an object file
src/asm.cpp.o:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/asm.cpp.o
.PHONY : src/asm.cpp.o

src/asm.i: src/asm.cpp.i

.PHONY : src/asm.i

# target to preprocess a source file
src/asm.cpp.i:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/asm.cpp.i
.PHONY : src/asm.cpp.i

src/asm.s: src/asm.cpp.s

.PHONY : src/asm.s

# target to generate assembly for a file
src/asm.cpp.s:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/asm.cpp.s
.PHONY : src/asm.cpp.s

src/condeval.o: src/condeval.cpp.o

.PHONY : src/condeval.o

# target to build an object file
src/condeval.cpp.o:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/condeval.cpp.o
.PHONY : src/condeval.cpp.o

src/condeval.i: src/condeval.cpp.i

.PHONY : src/condeval.i

# target to preprocess a source file
src/condeval.cpp.i:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/condeval.cpp.i
.PHONY : src/condeval.cpp.i

src/condeval.s: src/condeval.cpp.s

.PHONY : src/condeval.s

# target to generate assembly for a file
src/condeval.cpp.s:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/condeval.cpp.s
.PHONY : src/condeval.cpp.s

src/condutil.o: src/condutil.cpp.o

.PHONY : src/condutil.o

# target to build an object file
src/condutil.cpp.o:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/condutil.cpp.o
.PHONY : src/condutil.cpp.o

src/condutil.i: src/condutil.cpp.i

.PHONY : src/condutil.i

# target to preprocess a source file
src/condutil.cpp.i:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/condutil.cpp.i
.PHONY : src/condutil.cpp.i

src/condutil.s: src/condutil.cpp.s

.PHONY : src/condutil.s

# target to generate assembly for a file
src/condutil.cpp.s:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/condutil.cpp.s
.PHONY : src/condutil.cpp.s

src/eval.o: src/eval.cpp.o

.PHONY : src/eval.o

# target to build an object file
src/eval.cpp.o:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/eval.cpp.o
.PHONY : src/eval.cpp.o

src/eval.i: src/eval.cpp.i

.PHONY : src/eval.i

# target to preprocess a source file
src/eval.cpp.i:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/eval.cpp.i
.PHONY : src/eval.cpp.i

src/eval.s: src/eval.cpp.s

.PHONY : src/eval.s

# target to generate assembly for a file
src/eval.cpp.s:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/eval.cpp.s
.PHONY : src/eval.cpp.s

src/main.o: src/main.cpp.o

.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

src/mcl.o: src/mcl.cpp.o

.PHONY : src/mcl.o

# target to build an object file
src/mcl.cpp.o:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/mcl.cpp.o
.PHONY : src/mcl.cpp.o

src/mcl.i: src/mcl.cpp.i

.PHONY : src/mcl.i

# target to preprocess a source file
src/mcl.cpp.i:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/mcl.cpp.i
.PHONY : src/mcl.cpp.i

src/mcl.s: src/mcl.cpp.s

.PHONY : src/mcl.s

# target to generate assembly for a file
src/mcl.cpp.s:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/mcl.cpp.s
.PHONY : src/mcl.cpp.s

src/myassert.o: src/myassert.cpp.o

.PHONY : src/myassert.o

# target to build an object file
src/myassert.cpp.o:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/myassert.cpp.o
.PHONY : src/myassert.cpp.o

src/myassert.i: src/myassert.cpp.i

.PHONY : src/myassert.i

# target to preprocess a source file
src/myassert.cpp.i:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/myassert.cpp.i
.PHONY : src/myassert.cpp.i

src/myassert.s: src/myassert.cpp.s

.PHONY : src/myassert.s

# target to generate assembly for a file
src/myassert.cpp.s:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/myassert.cpp.s
.PHONY : src/myassert.cpp.s

src/optimizer.o: src/optimizer.cpp.o

.PHONY : src/optimizer.o

# target to build an object file
src/optimizer.cpp.o:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/optimizer.cpp.o
.PHONY : src/optimizer.cpp.o

src/optimizer.i: src/optimizer.cpp.i

.PHONY : src/optimizer.i

# target to preprocess a source file
src/optimizer.cpp.i:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/optimizer.cpp.i
.PHONY : src/optimizer.cpp.i

src/optimizer.s: src/optimizer.cpp.s

.PHONY : src/optimizer.s

# target to generate assembly for a file
src/optimizer.cpp.s:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/optimizer.cpp.s
.PHONY : src/optimizer.cpp.s

src/parserCore.o: src/parserCore.cpp.o

.PHONY : src/parserCore.o

# target to build an object file
src/parserCore.cpp.o:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/parserCore.cpp.o
.PHONY : src/parserCore.cpp.o

src/parserCore.i: src/parserCore.cpp.i

.PHONY : src/parserCore.i

# target to preprocess a source file
src/parserCore.cpp.i:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/parserCore.cpp.i
.PHONY : src/parserCore.cpp.i

src/parserCore.s: src/parserCore.cpp.s

.PHONY : src/parserCore.s

# target to generate assembly for a file
src/parserCore.cpp.s:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/parserCore.cpp.s
.PHONY : src/parserCore.cpp.s

src/parserTypes.o: src/parserTypes.cpp.o

.PHONY : src/parserTypes.o

# target to build an object file
src/parserTypes.cpp.o:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/parserTypes.cpp.o
.PHONY : src/parserTypes.cpp.o

src/parserTypes.i: src/parserTypes.cpp.i

.PHONY : src/parserTypes.i

# target to preprocess a source file
src/parserTypes.cpp.i:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/parserTypes.cpp.i
.PHONY : src/parserTypes.cpp.i

src/parserTypes.s: src/parserTypes.cpp.s

.PHONY : src/parserTypes.s

# target to generate assembly for a file
src/parserTypes.cpp.s:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/parserTypes.cpp.s
.PHONY : src/parserTypes.cpp.s

src/parserWrap.o: src/parserWrap.cpp.o

.PHONY : src/parserWrap.o

# target to build an object file
src/parserWrap.cpp.o:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/parserWrap.cpp.o
.PHONY : src/parserWrap.cpp.o

src/parserWrap.i: src/parserWrap.cpp.i

.PHONY : src/parserWrap.i

# target to preprocess a source file
src/parserWrap.cpp.i:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/parserWrap.cpp.i
.PHONY : src/parserWrap.cpp.i

src/parserWrap.s: src/parserWrap.cpp.s

.PHONY : src/parserWrap.s

# target to generate assembly for a file
src/parserWrap.cpp.s:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/parserWrap.cpp.s
.PHONY : src/parserWrap.cpp.s

src/stmtProcessor.o: src/stmtProcessor.cpp.o

.PHONY : src/stmtProcessor.o

# target to build an object file
src/stmtProcessor.cpp.o:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/stmtProcessor.cpp.o
.PHONY : src/stmtProcessor.cpp.o

src/stmtProcessor.i: src/stmtProcessor.cpp.i

.PHONY : src/stmtProcessor.i

# target to preprocess a source file
src/stmtProcessor.cpp.i:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/stmtProcessor.cpp.i
.PHONY : src/stmtProcessor.cpp.i

src/stmtProcessor.s: src/stmtProcessor.cpp.s

.PHONY : src/stmtProcessor.s

# target to generate assembly for a file
src/stmtProcessor.cpp.s:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/stmtProcessor.cpp.s
.PHONY : src/stmtProcessor.cpp.s

src/syntaxError.o: src/syntaxError.cpp.o

.PHONY : src/syntaxError.o

# target to build an object file
src/syntaxError.cpp.o:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/syntaxError.cpp.o
.PHONY : src/syntaxError.cpp.o

src/syntaxError.i: src/syntaxError.cpp.i

.PHONY : src/syntaxError.i

# target to preprocess a source file
src/syntaxError.cpp.i:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/syntaxError.cpp.i
.PHONY : src/syntaxError.cpp.i

src/syntaxError.s: src/syntaxError.cpp.s

.PHONY : src/syntaxError.s

# target to generate assembly for a file
src/syntaxError.cpp.s:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/syntaxError.cpp.s
.PHONY : src/syntaxError.cpp.s

src/util.o: src/util.cpp.o

.PHONY : src/util.o

# target to build an object file
src/util.cpp.o:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/util.cpp.o
.PHONY : src/util.cpp.o

src/util.i: src/util.cpp.i

.PHONY : src/util.i

# target to preprocess a source file
src/util.cpp.i:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/util.cpp.i
.PHONY : src/util.cpp.i

src/util.s: src/util.cpp.s

.PHONY : src/util.s

# target to generate assembly for a file
src/util.cpp.s:
	$(MAKE) -f CMakeFiles/MineCode.dir/build.make CMakeFiles/MineCode.dir/src/util.cpp.s
.PHONY : src/util.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... MineCode"
	@echo "... src/asm.o"
	@echo "... src/asm.i"
	@echo "... src/asm.s"
	@echo "... src/condeval.o"
	@echo "... src/condeval.i"
	@echo "... src/condeval.s"
	@echo "... src/condutil.o"
	@echo "... src/condutil.i"
	@echo "... src/condutil.s"
	@echo "... src/eval.o"
	@echo "... src/eval.i"
	@echo "... src/eval.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/mcl.o"
	@echo "... src/mcl.i"
	@echo "... src/mcl.s"
	@echo "... src/myassert.o"
	@echo "... src/myassert.i"
	@echo "... src/myassert.s"
	@echo "... src/optimizer.o"
	@echo "... src/optimizer.i"
	@echo "... src/optimizer.s"
	@echo "... src/parserCore.o"
	@echo "... src/parserCore.i"
	@echo "... src/parserCore.s"
	@echo "... src/parserTypes.o"
	@echo "... src/parserTypes.i"
	@echo "... src/parserTypes.s"
	@echo "... src/parserWrap.o"
	@echo "... src/parserWrap.i"
	@echo "... src/parserWrap.s"
	@echo "... src/stmtProcessor.o"
	@echo "... src/stmtProcessor.i"
	@echo "... src/stmtProcessor.s"
	@echo "... src/syntaxError.o"
	@echo "... src/syntaxError.i"
	@echo "... src/syntaxError.s"
	@echo "... src/util.o"
	@echo "... src/util.i"
	@echo "... src/util.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system


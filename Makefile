# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yt/Documents/lab_1_llp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yt/Documents/lab_1_llp

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/yt/Documents/lab_1_llp/CMakeFiles /home/yt/Documents/lab_1_llp//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/yt/Documents/lab_1_llp/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named llp

# Build rule for target.
llp: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 llp
.PHONY : llp

# fast build rule for target.
llp/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/build
.PHONY : llp/fast

#=============================================================================
# Target rules for targets named llp_lib

# Build rule for target.
llp_lib: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 llp_lib
.PHONY : llp_lib

# fast build rule for target.
llp_lib/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/build
.PHONY : llp_lib/fast

src/data/constants.o: src/data/constants.c.o
.PHONY : src/data/constants.o

# target to build an object file
src/data/constants.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/constants.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/constants.c.o
.PHONY : src/data/constants.c.o

src/data/constants.i: src/data/constants.c.i
.PHONY : src/data/constants.i

# target to preprocess a source file
src/data/constants.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/constants.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/constants.c.i
.PHONY : src/data/constants.c.i

src/data/constants.s: src/data/constants.c.s
.PHONY : src/data/constants.s

# target to generate assembly for a file
src/data/constants.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/constants.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/constants.c.s
.PHONY : src/data/constants.c.s

src/data/entity.o: src/data/entity.c.o
.PHONY : src/data/entity.o

# target to build an object file
src/data/entity.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/entity.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/entity.c.o
.PHONY : src/data/entity.c.o

src/data/entity.i: src/data/entity.c.i
.PHONY : src/data/entity.i

# target to preprocess a source file
src/data/entity.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/entity.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/entity.c.i
.PHONY : src/data/entity.c.i

src/data/entity.s: src/data/entity.c.s
.PHONY : src/data/entity.s

# target to generate assembly for a file
src/data/entity.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/entity.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/entity.c.s
.PHONY : src/data/entity.c.s

src/data/iterator.o: src/data/iterator.c.o
.PHONY : src/data/iterator.o

# target to build an object file
src/data/iterator.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/iterator.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/iterator.c.o
.PHONY : src/data/iterator.c.o

src/data/iterator.i: src/data/iterator.c.i
.PHONY : src/data/iterator.i

# target to preprocess a source file
src/data/iterator.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/iterator.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/iterator.c.i
.PHONY : src/data/iterator.c.i

src/data/iterator.s: src/data/iterator.c.s
.PHONY : src/data/iterator.s

# target to generate assembly for a file
src/data/iterator.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/iterator.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/iterator.c.s
.PHONY : src/data/iterator.c.s

src/data/node.o: src/data/node.c.o
.PHONY : src/data/node.o

# target to build an object file
src/data/node.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/node.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/node.c.o
.PHONY : src/data/node.c.o

src/data/node.i: src/data/node.c.i
.PHONY : src/data/node.i

# target to preprocess a source file
src/data/node.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/node.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/node.c.i
.PHONY : src/data/node.c.i

src/data/node.s: src/data/node.c.s
.PHONY : src/data/node.s

# target to generate assembly for a file
src/data/node.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/node.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/node.c.s
.PHONY : src/data/node.c.s

src/data/page.o: src/data/page.c.o
.PHONY : src/data/page.o

# target to build an object file
src/data/page.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/page.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/page.c.o
.PHONY : src/data/page.c.o

src/data/page.i: src/data/page.c.i
.PHONY : src/data/page.i

# target to preprocess a source file
src/data/page.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/page.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/page.c.i
.PHONY : src/data/page.c.i

src/data/page.s: src/data/page.c.s
.PHONY : src/data/page.s

# target to generate assembly for a file
src/data/page.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/page.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/page.c.s
.PHONY : src/data/page.c.s

src/data/property.o: src/data/property.c.o
.PHONY : src/data/property.o

# target to build an object file
src/data/property.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/property.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/property.c.o
.PHONY : src/data/property.c.o

src/data/property.i: src/data/property.c.i
.PHONY : src/data/property.i

# target to preprocess a source file
src/data/property.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/property.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/property.c.i
.PHONY : src/data/property.c.i

src/data/property.s: src/data/property.c.s
.PHONY : src/data/property.s

# target to generate assembly for a file
src/data/property.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/property.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/property.c.s
.PHONY : src/data/property.c.s

src/data/relationship.o: src/data/relationship.c.o
.PHONY : src/data/relationship.o

# target to build an object file
src/data/relationship.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/relationship.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/relationship.c.o
.PHONY : src/data/relationship.c.o

src/data/relationship.i: src/data/relationship.c.i
.PHONY : src/data/relationship.i

# target to preprocess a source file
src/data/relationship.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/relationship.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/relationship.c.i
.PHONY : src/data/relationship.c.i

src/data/relationship.s: src/data/relationship.c.s
.PHONY : src/data/relationship.s

# target to generate assembly for a file
src/data/relationship.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/relationship.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/relationship.c.s
.PHONY : src/data/relationship.c.s

src/data/stack.o: src/data/stack.c.o
.PHONY : src/data/stack.o

# target to build an object file
src/data/stack.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/stack.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/stack.c.o
.PHONY : src/data/stack.c.o

src/data/stack.i: src/data/stack.c.i
.PHONY : src/data/stack.i

# target to preprocess a source file
src/data/stack.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/stack.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/stack.c.i
.PHONY : src/data/stack.c.i

src/data/stack.s: src/data/stack.c.s
.PHONY : src/data/stack.s

# target to generate assembly for a file
src/data/stack.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/data/stack.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/data/stack.c.s
.PHONY : src/data/stack.c.s

src/main.o: src/main.c.o
.PHONY : src/main.o

# target to build an object file
src/main.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/main.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/main.c.o
.PHONY : src/main.c.o

src/main.i: src/main.c.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/main.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/main.c.i
.PHONY : src/main.c.i

src/main.s: src/main.c.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/main.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/main.c.s
.PHONY : src/main.c.s

src/managers/file_manager.o: src/managers/file_manager.c.o
.PHONY : src/managers/file_manager.o

# target to build an object file
src/managers/file_manager.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/managers/file_manager.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/managers/file_manager.c.o
.PHONY : src/managers/file_manager.c.o

src/managers/file_manager.i: src/managers/file_manager.c.i
.PHONY : src/managers/file_manager.i

# target to preprocess a source file
src/managers/file_manager.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/managers/file_manager.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/managers/file_manager.c.i
.PHONY : src/managers/file_manager.c.i

src/managers/file_manager.s: src/managers/file_manager.c.s
.PHONY : src/managers/file_manager.s

# target to generate assembly for a file
src/managers/file_manager.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/managers/file_manager.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/managers/file_manager.c.s
.PHONY : src/managers/file_manager.c.s

src/managers/page_manager.o: src/managers/page_manager.c.o
.PHONY : src/managers/page_manager.o

# target to build an object file
src/managers/page_manager.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/managers/page_manager.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/managers/page_manager.c.o
.PHONY : src/managers/page_manager.c.o

src/managers/page_manager.i: src/managers/page_manager.c.i
.PHONY : src/managers/page_manager.i

# target to preprocess a source file
src/managers/page_manager.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/managers/page_manager.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/managers/page_manager.c.i
.PHONY : src/managers/page_manager.c.i

src/managers/page_manager.s: src/managers/page_manager.c.s
.PHONY : src/managers/page_manager.s

# target to generate assembly for a file
src/managers/page_manager.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/managers/page_manager.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/managers/page_manager.c.s
.PHONY : src/managers/page_manager.c.s

src/operations/common.o: src/operations/common.c.o
.PHONY : src/operations/common.o

# target to build an object file
src/operations/common.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/operations/common.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/operations/common.c.o
.PHONY : src/operations/common.c.o

src/operations/common.i: src/operations/common.c.i
.PHONY : src/operations/common.i

# target to preprocess a source file
src/operations/common.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/operations/common.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/operations/common.c.i
.PHONY : src/operations/common.c.i

src/operations/common.s: src/operations/common.c.s
.PHONY : src/operations/common.s

# target to generate assembly for a file
src/operations/common.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/operations/common.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/operations/common.c.s
.PHONY : src/operations/common.c.s

src/operations/comparator.o: src/operations/comparator.c.o
.PHONY : src/operations/comparator.o

# target to build an object file
src/operations/comparator.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/operations/comparator.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/operations/comparator.c.o
.PHONY : src/operations/comparator.c.o

src/operations/comparator.i: src/operations/comparator.c.i
.PHONY : src/operations/comparator.i

# target to preprocess a source file
src/operations/comparator.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/operations/comparator.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/operations/comparator.c.i
.PHONY : src/operations/comparator.c.i

src/operations/comparator.s: src/operations/comparator.c.s
.PHONY : src/operations/comparator.s

# target to generate assembly for a file
src/operations/comparator.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/operations/comparator.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/operations/comparator.c.s
.PHONY : src/operations/comparator.c.s

src/operations/crud_methods.o: src/operations/crud_methods.c.o
.PHONY : src/operations/crud_methods.o

# target to build an object file
src/operations/crud_methods.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/operations/crud_methods.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/operations/crud_methods.c.o
.PHONY : src/operations/crud_methods.c.o

src/operations/crud_methods.i: src/operations/crud_methods.c.i
.PHONY : src/operations/crud_methods.i

# target to preprocess a source file
src/operations/crud_methods.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/operations/crud_methods.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/operations/crud_methods.c.i
.PHONY : src/operations/crud_methods.c.i

src/operations/crud_methods.s: src/operations/crud_methods.c.s
.PHONY : src/operations/crud_methods.s

# target to generate assembly for a file
src/operations/crud_methods.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/operations/crud_methods.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/operations/crud_methods.c.s
.PHONY : src/operations/crud_methods.c.s

src/operations/specific.o: src/operations/specific.c.o
.PHONY : src/operations/specific.o

# target to build an object file
src/operations/specific.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/operations/specific.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/operations/specific.c.o
.PHONY : src/operations/specific.c.o

src/operations/specific.i: src/operations/specific.c.i
.PHONY : src/operations/specific.i

# target to preprocess a source file
src/operations/specific.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/operations/specific.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/operations/specific.c.i
.PHONY : src/operations/specific.c.i

src/operations/specific.s: src/operations/specific.c.s
.PHONY : src/operations/specific.s

# target to generate assembly for a file
src/operations/specific.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/operations/specific.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/operations/specific.c.s
.PHONY : src/operations/specific.c.s

src/utils/checker.o: src/utils/checker.c.o
.PHONY : src/utils/checker.o

# target to build an object file
src/utils/checker.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/checker.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/checker.c.o
.PHONY : src/utils/checker.c.o

src/utils/checker.i: src/utils/checker.c.i
.PHONY : src/utils/checker.i

# target to preprocess a source file
src/utils/checker.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/checker.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/checker.c.i
.PHONY : src/utils/checker.c.i

src/utils/checker.s: src/utils/checker.c.s
.PHONY : src/utils/checker.s

# target to generate assembly for a file
src/utils/checker.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/checker.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/checker.c.s
.PHONY : src/utils/checker.c.s

src/utils/io_utils.o: src/utils/io_utils.c.o
.PHONY : src/utils/io_utils.o

# target to build an object file
src/utils/io_utils.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/io_utils.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/io_utils.c.o
.PHONY : src/utils/io_utils.c.o

src/utils/io_utils.i: src/utils/io_utils.c.i
.PHONY : src/utils/io_utils.i

# target to preprocess a source file
src/utils/io_utils.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/io_utils.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/io_utils.c.i
.PHONY : src/utils/io_utils.c.i

src/utils/io_utils.s: src/utils/io_utils.c.s
.PHONY : src/utils/io_utils.s

# target to generate assembly for a file
src/utils/io_utils.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/io_utils.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/io_utils.c.s
.PHONY : src/utils/io_utils.c.s

src/utils/logger.o: src/utils/logger.c.o
.PHONY : src/utils/logger.o

# target to build an object file
src/utils/logger.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/logger.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/logger.c.o
.PHONY : src/utils/logger.c.o

src/utils/logger.i: src/utils/logger.c.i
.PHONY : src/utils/logger.i

# target to preprocess a source file
src/utils/logger.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/logger.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/logger.c.i
.PHONY : src/utils/logger.c.i

src/utils/logger.s: src/utils/logger.c.s
.PHONY : src/utils/logger.s

# target to generate assembly for a file
src/utils/logger.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/logger.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/logger.c.s
.PHONY : src/utils/logger.c.s

src/utils/page_utils.o: src/utils/page_utils.c.o
.PHONY : src/utils/page_utils.o

# target to build an object file
src/utils/page_utils.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/page_utils.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/page_utils.c.o
.PHONY : src/utils/page_utils.c.o

src/utils/page_utils.i: src/utils/page_utils.c.i
.PHONY : src/utils/page_utils.i

# target to preprocess a source file
src/utils/page_utils.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/page_utils.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/page_utils.c.i
.PHONY : src/utils/page_utils.c.i

src/utils/page_utils.s: src/utils/page_utils.c.s
.PHONY : src/utils/page_utils.s

# target to generate assembly for a file
src/utils/page_utils.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/page_utils.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/page_utils.c.s
.PHONY : src/utils/page_utils.c.s

src/utils/stack_utils.o: src/utils/stack_utils.c.o
.PHONY : src/utils/stack_utils.o

# target to build an object file
src/utils/stack_utils.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/stack_utils.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/stack_utils.c.o
.PHONY : src/utils/stack_utils.c.o

src/utils/stack_utils.i: src/utils/stack_utils.c.i
.PHONY : src/utils/stack_utils.i

# target to preprocess a source file
src/utils/stack_utils.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/stack_utils.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/stack_utils.c.i
.PHONY : src/utils/stack_utils.c.i

src/utils/stack_utils.s: src/utils/stack_utils.c.s
.PHONY : src/utils/stack_utils.s

# target to generate assembly for a file
src/utils/stack_utils.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/stack_utils.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/stack_utils.c.s
.PHONY : src/utils/stack_utils.c.s

src/utils/test_data.o: src/utils/test_data.c.o
.PHONY : src/utils/test_data.o

# target to build an object file
src/utils/test_data.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/test_data.c.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/test_data.c.o
.PHONY : src/utils/test_data.c.o

src/utils/test_data.i: src/utils/test_data.c.i
.PHONY : src/utils/test_data.i

# target to preprocess a source file
src/utils/test_data.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/test_data.c.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/test_data.c.i
.PHONY : src/utils/test_data.c.i

src/utils/test_data.s: src/utils/test_data.c.s
.PHONY : src/utils/test_data.s

# target to generate assembly for a file
src/utils/test_data.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp.dir/build.make CMakeFiles/llp.dir/src/utils/test_data.c.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/llp_lib.dir/build.make CMakeFiles/llp_lib.dir/src/utils/test_data.c.s
.PHONY : src/utils/test_data.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... llp"
	@echo "... llp_lib"
	@echo "... src/data/constants.o"
	@echo "... src/data/constants.i"
	@echo "... src/data/constants.s"
	@echo "... src/data/entity.o"
	@echo "... src/data/entity.i"
	@echo "... src/data/entity.s"
	@echo "... src/data/iterator.o"
	@echo "... src/data/iterator.i"
	@echo "... src/data/iterator.s"
	@echo "... src/data/node.o"
	@echo "... src/data/node.i"
	@echo "... src/data/node.s"
	@echo "... src/data/page.o"
	@echo "... src/data/page.i"
	@echo "... src/data/page.s"
	@echo "... src/data/property.o"
	@echo "... src/data/property.i"
	@echo "... src/data/property.s"
	@echo "... src/data/relationship.o"
	@echo "... src/data/relationship.i"
	@echo "... src/data/relationship.s"
	@echo "... src/data/stack.o"
	@echo "... src/data/stack.i"
	@echo "... src/data/stack.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/managers/file_manager.o"
	@echo "... src/managers/file_manager.i"
	@echo "... src/managers/file_manager.s"
	@echo "... src/managers/page_manager.o"
	@echo "... src/managers/page_manager.i"
	@echo "... src/managers/page_manager.s"
	@echo "... src/operations/common.o"
	@echo "... src/operations/common.i"
	@echo "... src/operations/common.s"
	@echo "... src/operations/comparator.o"
	@echo "... src/operations/comparator.i"
	@echo "... src/operations/comparator.s"
	@echo "... src/operations/crud_methods.o"
	@echo "... src/operations/crud_methods.i"
	@echo "... src/operations/crud_methods.s"
	@echo "... src/operations/specific.o"
	@echo "... src/operations/specific.i"
	@echo "... src/operations/specific.s"
	@echo "... src/utils/checker.o"
	@echo "... src/utils/checker.i"
	@echo "... src/utils/checker.s"
	@echo "... src/utils/io_utils.o"
	@echo "... src/utils/io_utils.i"
	@echo "... src/utils/io_utils.s"
	@echo "... src/utils/logger.o"
	@echo "... src/utils/logger.i"
	@echo "... src/utils/logger.s"
	@echo "... src/utils/page_utils.o"
	@echo "... src/utils/page_utils.i"
	@echo "... src/utils/page_utils.s"
	@echo "... src/utils/stack_utils.o"
	@echo "... src/utils/stack_utils.i"
	@echo "... src/utils/stack_utils.s"
	@echo "... src/utils/test_data.o"
	@echo "... src/utils/test_data.i"
	@echo "... src/utils/test_data.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system


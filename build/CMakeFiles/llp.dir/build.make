# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.27.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.27.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/sasaovch/Development/ITMO/lab_1_llp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/sasaovch/Development/ITMO/lab_1_llp/build

# Include any dependencies generated for this target.
include CMakeFiles/llp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/llp.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/llp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/llp.dir/flags.make

CMakeFiles/llp.dir/src/data/common.c.o: CMakeFiles/llp.dir/flags.make
CMakeFiles/llp.dir/src/data/common.c.o: /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/common.c
CMakeFiles/llp.dir/src/data/common.c.o: CMakeFiles/llp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sasaovch/Development/ITMO/lab_1_llp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/llp.dir/src/data/common.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/llp.dir/src/data/common.c.o -MF CMakeFiles/llp.dir/src/data/common.c.o.d -o CMakeFiles/llp.dir/src/data/common.c.o -c /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/common.c

CMakeFiles/llp.dir/src/data/common.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/llp.dir/src/data/common.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/common.c > CMakeFiles/llp.dir/src/data/common.c.i

CMakeFiles/llp.dir/src/data/common.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/llp.dir/src/data/common.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/common.c -o CMakeFiles/llp.dir/src/data/common.c.s

CMakeFiles/llp.dir/src/data/crud_methods.c.o: CMakeFiles/llp.dir/flags.make
CMakeFiles/llp.dir/src/data/crud_methods.c.o: /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/crud_methods.c
CMakeFiles/llp.dir/src/data/crud_methods.c.o: CMakeFiles/llp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sasaovch/Development/ITMO/lab_1_llp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/llp.dir/src/data/crud_methods.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/llp.dir/src/data/crud_methods.c.o -MF CMakeFiles/llp.dir/src/data/crud_methods.c.o.d -o CMakeFiles/llp.dir/src/data/crud_methods.c.o -c /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/crud_methods.c

CMakeFiles/llp.dir/src/data/crud_methods.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/llp.dir/src/data/crud_methods.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/crud_methods.c > CMakeFiles/llp.dir/src/data/crud_methods.c.i

CMakeFiles/llp.dir/src/data/crud_methods.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/llp.dir/src/data/crud_methods.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/crud_methods.c -o CMakeFiles/llp.dir/src/data/crud_methods.c.s

CMakeFiles/llp.dir/src/data/node.c.o: CMakeFiles/llp.dir/flags.make
CMakeFiles/llp.dir/src/data/node.c.o: /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/node.c
CMakeFiles/llp.dir/src/data/node.c.o: CMakeFiles/llp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sasaovch/Development/ITMO/lab_1_llp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/llp.dir/src/data/node.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/llp.dir/src/data/node.c.o -MF CMakeFiles/llp.dir/src/data/node.c.o.d -o CMakeFiles/llp.dir/src/data/node.c.o -c /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/node.c

CMakeFiles/llp.dir/src/data/node.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/llp.dir/src/data/node.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/node.c > CMakeFiles/llp.dir/src/data/node.c.i

CMakeFiles/llp.dir/src/data/node.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/llp.dir/src/data/node.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/node.c -o CMakeFiles/llp.dir/src/data/node.c.s

CMakeFiles/llp.dir/src/data/page.c.o: CMakeFiles/llp.dir/flags.make
CMakeFiles/llp.dir/src/data/page.c.o: /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/page.c
CMakeFiles/llp.dir/src/data/page.c.o: CMakeFiles/llp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sasaovch/Development/ITMO/lab_1_llp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/llp.dir/src/data/page.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/llp.dir/src/data/page.c.o -MF CMakeFiles/llp.dir/src/data/page.c.o.d -o CMakeFiles/llp.dir/src/data/page.c.o -c /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/page.c

CMakeFiles/llp.dir/src/data/page.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/llp.dir/src/data/page.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/page.c > CMakeFiles/llp.dir/src/data/page.c.i

CMakeFiles/llp.dir/src/data/page.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/llp.dir/src/data/page.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/page.c -o CMakeFiles/llp.dir/src/data/page.c.s

CMakeFiles/llp.dir/src/data/property.c.o: CMakeFiles/llp.dir/flags.make
CMakeFiles/llp.dir/src/data/property.c.o: /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/property.c
CMakeFiles/llp.dir/src/data/property.c.o: CMakeFiles/llp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sasaovch/Development/ITMO/lab_1_llp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/llp.dir/src/data/property.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/llp.dir/src/data/property.c.o -MF CMakeFiles/llp.dir/src/data/property.c.o.d -o CMakeFiles/llp.dir/src/data/property.c.o -c /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/property.c

CMakeFiles/llp.dir/src/data/property.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/llp.dir/src/data/property.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/property.c > CMakeFiles/llp.dir/src/data/property.c.i

CMakeFiles/llp.dir/src/data/property.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/llp.dir/src/data/property.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/property.c -o CMakeFiles/llp.dir/src/data/property.c.s

CMakeFiles/llp.dir/src/data/relationship.c.o: CMakeFiles/llp.dir/flags.make
CMakeFiles/llp.dir/src/data/relationship.c.o: /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/relationship.c
CMakeFiles/llp.dir/src/data/relationship.c.o: CMakeFiles/llp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sasaovch/Development/ITMO/lab_1_llp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/llp.dir/src/data/relationship.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/llp.dir/src/data/relationship.c.o -MF CMakeFiles/llp.dir/src/data/relationship.c.o.d -o CMakeFiles/llp.dir/src/data/relationship.c.o -c /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/relationship.c

CMakeFiles/llp.dir/src/data/relationship.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/llp.dir/src/data/relationship.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/relationship.c > CMakeFiles/llp.dir/src/data/relationship.c.i

CMakeFiles/llp.dir/src/data/relationship.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/llp.dir/src/data/relationship.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/relationship.c -o CMakeFiles/llp.dir/src/data/relationship.c.s

CMakeFiles/llp.dir/src/data/specific.c.o: CMakeFiles/llp.dir/flags.make
CMakeFiles/llp.dir/src/data/specific.c.o: /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/specific.c
CMakeFiles/llp.dir/src/data/specific.c.o: CMakeFiles/llp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sasaovch/Development/ITMO/lab_1_llp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/llp.dir/src/data/specific.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/llp.dir/src/data/specific.c.o -MF CMakeFiles/llp.dir/src/data/specific.c.o.d -o CMakeFiles/llp.dir/src/data/specific.c.o -c /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/specific.c

CMakeFiles/llp.dir/src/data/specific.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/llp.dir/src/data/specific.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/specific.c > CMakeFiles/llp.dir/src/data/specific.c.i

CMakeFiles/llp.dir/src/data/specific.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/llp.dir/src/data/specific.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/specific.c -o CMakeFiles/llp.dir/src/data/specific.c.s

CMakeFiles/llp.dir/src/data/type.c.o: CMakeFiles/llp.dir/flags.make
CMakeFiles/llp.dir/src/data/type.c.o: /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/type.c
CMakeFiles/llp.dir/src/data/type.c.o: CMakeFiles/llp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sasaovch/Development/ITMO/lab_1_llp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/llp.dir/src/data/type.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/llp.dir/src/data/type.c.o -MF CMakeFiles/llp.dir/src/data/type.c.o.d -o CMakeFiles/llp.dir/src/data/type.c.o -c /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/type.c

CMakeFiles/llp.dir/src/data/type.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/llp.dir/src/data/type.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/type.c > CMakeFiles/llp.dir/src/data/type.c.i

CMakeFiles/llp.dir/src/data/type.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/llp.dir/src/data/type.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sasaovch/Development/ITMO/lab_1_llp/src/data/type.c -o CMakeFiles/llp.dir/src/data/type.c.s

CMakeFiles/llp.dir/src/io/io.c.o: CMakeFiles/llp.dir/flags.make
CMakeFiles/llp.dir/src/io/io.c.o: /Users/sasaovch/Development/ITMO/lab_1_llp/src/io/io.c
CMakeFiles/llp.dir/src/io/io.c.o: CMakeFiles/llp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sasaovch/Development/ITMO/lab_1_llp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/llp.dir/src/io/io.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/llp.dir/src/io/io.c.o -MF CMakeFiles/llp.dir/src/io/io.c.o.d -o CMakeFiles/llp.dir/src/io/io.c.o -c /Users/sasaovch/Development/ITMO/lab_1_llp/src/io/io.c

CMakeFiles/llp.dir/src/io/io.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/llp.dir/src/io/io.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sasaovch/Development/ITMO/lab_1_llp/src/io/io.c > CMakeFiles/llp.dir/src/io/io.c.i

CMakeFiles/llp.dir/src/io/io.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/llp.dir/src/io/io.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sasaovch/Development/ITMO/lab_1_llp/src/io/io.c -o CMakeFiles/llp.dir/src/io/io.c.s

CMakeFiles/llp.dir/src/main.c.o: CMakeFiles/llp.dir/flags.make
CMakeFiles/llp.dir/src/main.c.o: /Users/sasaovch/Development/ITMO/lab_1_llp/src/main.c
CMakeFiles/llp.dir/src/main.c.o: CMakeFiles/llp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sasaovch/Development/ITMO/lab_1_llp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/llp.dir/src/main.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/llp.dir/src/main.c.o -MF CMakeFiles/llp.dir/src/main.c.o.d -o CMakeFiles/llp.dir/src/main.c.o -c /Users/sasaovch/Development/ITMO/lab_1_llp/src/main.c

CMakeFiles/llp.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/llp.dir/src/main.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sasaovch/Development/ITMO/lab_1_llp/src/main.c > CMakeFiles/llp.dir/src/main.c.i

CMakeFiles/llp.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/llp.dir/src/main.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sasaovch/Development/ITMO/lab_1_llp/src/main.c -o CMakeFiles/llp.dir/src/main.c.s

CMakeFiles/llp.dir/src/utils/page_utils.c.o: CMakeFiles/llp.dir/flags.make
CMakeFiles/llp.dir/src/utils/page_utils.c.o: /Users/sasaovch/Development/ITMO/lab_1_llp/src/utils/page_utils.c
CMakeFiles/llp.dir/src/utils/page_utils.c.o: CMakeFiles/llp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sasaovch/Development/ITMO/lab_1_llp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/llp.dir/src/utils/page_utils.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/llp.dir/src/utils/page_utils.c.o -MF CMakeFiles/llp.dir/src/utils/page_utils.c.o.d -o CMakeFiles/llp.dir/src/utils/page_utils.c.o -c /Users/sasaovch/Development/ITMO/lab_1_llp/src/utils/page_utils.c

CMakeFiles/llp.dir/src/utils/page_utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/llp.dir/src/utils/page_utils.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sasaovch/Development/ITMO/lab_1_llp/src/utils/page_utils.c > CMakeFiles/llp.dir/src/utils/page_utils.c.i

CMakeFiles/llp.dir/src/utils/page_utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/llp.dir/src/utils/page_utils.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sasaovch/Development/ITMO/lab_1_llp/src/utils/page_utils.c -o CMakeFiles/llp.dir/src/utils/page_utils.c.s

CMakeFiles/llp.dir/src/utils/test_data.c.o: CMakeFiles/llp.dir/flags.make
CMakeFiles/llp.dir/src/utils/test_data.c.o: /Users/sasaovch/Development/ITMO/lab_1_llp/src/utils/test_data.c
CMakeFiles/llp.dir/src/utils/test_data.c.o: CMakeFiles/llp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sasaovch/Development/ITMO/lab_1_llp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object CMakeFiles/llp.dir/src/utils/test_data.c.o"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/llp.dir/src/utils/test_data.c.o -MF CMakeFiles/llp.dir/src/utils/test_data.c.o.d -o CMakeFiles/llp.dir/src/utils/test_data.c.o -c /Users/sasaovch/Development/ITMO/lab_1_llp/src/utils/test_data.c

CMakeFiles/llp.dir/src/utils/test_data.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/llp.dir/src/utils/test_data.c.i"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sasaovch/Development/ITMO/lab_1_llp/src/utils/test_data.c > CMakeFiles/llp.dir/src/utils/test_data.c.i

CMakeFiles/llp.dir/src/utils/test_data.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/llp.dir/src/utils/test_data.c.s"
	/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sasaovch/Development/ITMO/lab_1_llp/src/utils/test_data.c -o CMakeFiles/llp.dir/src/utils/test_data.c.s

# Object files for target llp
llp_OBJECTS = \
"CMakeFiles/llp.dir/src/data/common.c.o" \
"CMakeFiles/llp.dir/src/data/crud_methods.c.o" \
"CMakeFiles/llp.dir/src/data/node.c.o" \
"CMakeFiles/llp.dir/src/data/page.c.o" \
"CMakeFiles/llp.dir/src/data/property.c.o" \
"CMakeFiles/llp.dir/src/data/relationship.c.o" \
"CMakeFiles/llp.dir/src/data/specific.c.o" \
"CMakeFiles/llp.dir/src/data/type.c.o" \
"CMakeFiles/llp.dir/src/io/io.c.o" \
"CMakeFiles/llp.dir/src/main.c.o" \
"CMakeFiles/llp.dir/src/utils/page_utils.c.o" \
"CMakeFiles/llp.dir/src/utils/test_data.c.o"

# External object files for target llp
llp_EXTERNAL_OBJECTS =

llp: CMakeFiles/llp.dir/src/data/common.c.o
llp: CMakeFiles/llp.dir/src/data/crud_methods.c.o
llp: CMakeFiles/llp.dir/src/data/node.c.o
llp: CMakeFiles/llp.dir/src/data/page.c.o
llp: CMakeFiles/llp.dir/src/data/property.c.o
llp: CMakeFiles/llp.dir/src/data/relationship.c.o
llp: CMakeFiles/llp.dir/src/data/specific.c.o
llp: CMakeFiles/llp.dir/src/data/type.c.o
llp: CMakeFiles/llp.dir/src/io/io.c.o
llp: CMakeFiles/llp.dir/src/main.c.o
llp: CMakeFiles/llp.dir/src/utils/page_utils.c.o
llp: CMakeFiles/llp.dir/src/utils/test_data.c.o
llp: CMakeFiles/llp.dir/build.make
llp: CMakeFiles/llp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/sasaovch/Development/ITMO/lab_1_llp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking C executable llp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/llp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/llp.dir/build: llp
.PHONY : CMakeFiles/llp.dir/build

CMakeFiles/llp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/llp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/llp.dir/clean

CMakeFiles/llp.dir/depend:
	cd /Users/sasaovch/Development/ITMO/lab_1_llp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/sasaovch/Development/ITMO/lab_1_llp /Users/sasaovch/Development/ITMO/lab_1_llp /Users/sasaovch/Development/ITMO/lab_1_llp/build /Users/sasaovch/Development/ITMO/lab_1_llp/build /Users/sasaovch/Development/ITMO/lab_1_llp/build/CMakeFiles/llp.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/llp.dir/depend


# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_SOURCE_DIR = /home/eduard/programming/EvolutionaryCreator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/eduard/programming/EvolutionaryCreator

# Include any dependencies generated for this target.
include src/CMakeFiles/EvolutionaryCreator.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/EvolutionaryCreator.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/EvolutionaryCreator.dir/flags.make

src/CMakeFiles/EvolutionaryCreator.dir/Main.cpp.o: src/CMakeFiles/EvolutionaryCreator.dir/flags.make
src/CMakeFiles/EvolutionaryCreator.dir/Main.cpp.o: src/Main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eduard/programming/EvolutionaryCreator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/EvolutionaryCreator.dir/Main.cpp.o"
	cd /home/eduard/programming/EvolutionaryCreator/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EvolutionaryCreator.dir/Main.cpp.o -c /home/eduard/programming/EvolutionaryCreator/src/Main.cpp

src/CMakeFiles/EvolutionaryCreator.dir/Main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EvolutionaryCreator.dir/Main.cpp.i"
	cd /home/eduard/programming/EvolutionaryCreator/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eduard/programming/EvolutionaryCreator/src/Main.cpp > CMakeFiles/EvolutionaryCreator.dir/Main.cpp.i

src/CMakeFiles/EvolutionaryCreator.dir/Main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EvolutionaryCreator.dir/Main.cpp.s"
	cd /home/eduard/programming/EvolutionaryCreator/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eduard/programming/EvolutionaryCreator/src/Main.cpp -o CMakeFiles/EvolutionaryCreator.dir/Main.cpp.s

# Object files for target EvolutionaryCreator
EvolutionaryCreator_OBJECTS = \
"CMakeFiles/EvolutionaryCreator.dir/Main.cpp.o"

# External object files for target EvolutionaryCreator
EvolutionaryCreator_EXTERNAL_OBJECTS =

bin/EvolutionaryCreator: src/CMakeFiles/EvolutionaryCreator.dir/Main.cpp.o
bin/EvolutionaryCreator: src/CMakeFiles/EvolutionaryCreator.dir/build.make
bin/EvolutionaryCreator: lib/libOpenGLContext.a
bin/EvolutionaryCreator: lib/libimgui.a
bin/EvolutionaryCreator: lib/libEvolutionary.a
bin/EvolutionaryCreator: src/CMakeFiles/EvolutionaryCreator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/eduard/programming/EvolutionaryCreator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/EvolutionaryCreator"
	cd /home/eduard/programming/EvolutionaryCreator/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EvolutionaryCreator.dir/link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "symbolic link resources folder from /home/eduard/programming/EvolutionaryCreator/resources => /home/eduard/programming/EvolutionaryCreator/resources"
	cd /home/eduard/programming/EvolutionaryCreator/src && /usr/bin/cmake -E create_symlink /home/eduard/programming/EvolutionaryCreator/resources /home/eduard/programming/EvolutionaryCreator/resources

# Rule to build all files generated by this target.
src/CMakeFiles/EvolutionaryCreator.dir/build: bin/EvolutionaryCreator

.PHONY : src/CMakeFiles/EvolutionaryCreator.dir/build

src/CMakeFiles/EvolutionaryCreator.dir/clean:
	cd /home/eduard/programming/EvolutionaryCreator/src && $(CMAKE_COMMAND) -P CMakeFiles/EvolutionaryCreator.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/EvolutionaryCreator.dir/clean

src/CMakeFiles/EvolutionaryCreator.dir/depend:
	cd /home/eduard/programming/EvolutionaryCreator && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/eduard/programming/EvolutionaryCreator /home/eduard/programming/EvolutionaryCreator/src /home/eduard/programming/EvolutionaryCreator /home/eduard/programming/EvolutionaryCreator/src /home/eduard/programming/EvolutionaryCreator/src/CMakeFiles/EvolutionaryCreator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/EvolutionaryCreator.dir/depend


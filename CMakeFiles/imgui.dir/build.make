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
include CMakeFiles/imgui.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/imgui.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/imgui.dir/flags.make

CMakeFiles/imgui.dir/dependencies/imgui/imgui.cpp.o: CMakeFiles/imgui.dir/flags.make
CMakeFiles/imgui.dir/dependencies/imgui/imgui.cpp.o: dependencies/imgui/imgui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eduard/programming/EvolutionaryCreator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/imgui.dir/dependencies/imgui/imgui.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/imgui.dir/dependencies/imgui/imgui.cpp.o -c /home/eduard/programming/EvolutionaryCreator/dependencies/imgui/imgui.cpp

CMakeFiles/imgui.dir/dependencies/imgui/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/dependencies/imgui/imgui.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eduard/programming/EvolutionaryCreator/dependencies/imgui/imgui.cpp > CMakeFiles/imgui.dir/dependencies/imgui/imgui.cpp.i

CMakeFiles/imgui.dir/dependencies/imgui/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/dependencies/imgui/imgui.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eduard/programming/EvolutionaryCreator/dependencies/imgui/imgui.cpp -o CMakeFiles/imgui.dir/dependencies/imgui/imgui.cpp.s

CMakeFiles/imgui.dir/src/ui/ImGuiGlfw.cpp.o: CMakeFiles/imgui.dir/flags.make
CMakeFiles/imgui.dir/src/ui/ImGuiGlfw.cpp.o: src/ui/ImGuiGlfw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eduard/programming/EvolutionaryCreator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/imgui.dir/src/ui/ImGuiGlfw.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/imgui.dir/src/ui/ImGuiGlfw.cpp.o -c /home/eduard/programming/EvolutionaryCreator/src/ui/ImGuiGlfw.cpp

CMakeFiles/imgui.dir/src/ui/ImGuiGlfw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/src/ui/ImGuiGlfw.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eduard/programming/EvolutionaryCreator/src/ui/ImGuiGlfw.cpp > CMakeFiles/imgui.dir/src/ui/ImGuiGlfw.cpp.i

CMakeFiles/imgui.dir/src/ui/ImGuiGlfw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/src/ui/ImGuiGlfw.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eduard/programming/EvolutionaryCreator/src/ui/ImGuiGlfw.cpp -o CMakeFiles/imgui.dir/src/ui/ImGuiGlfw.cpp.s

CMakeFiles/imgui.dir/src/ui/ImGuiOpengl.cpp.o: CMakeFiles/imgui.dir/flags.make
CMakeFiles/imgui.dir/src/ui/ImGuiOpengl.cpp.o: src/ui/ImGuiOpengl.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eduard/programming/EvolutionaryCreator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/imgui.dir/src/ui/ImGuiOpengl.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/imgui.dir/src/ui/ImGuiOpengl.cpp.o -c /home/eduard/programming/EvolutionaryCreator/src/ui/ImGuiOpengl.cpp

CMakeFiles/imgui.dir/src/ui/ImGuiOpengl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/src/ui/ImGuiOpengl.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eduard/programming/EvolutionaryCreator/src/ui/ImGuiOpengl.cpp > CMakeFiles/imgui.dir/src/ui/ImGuiOpengl.cpp.i

CMakeFiles/imgui.dir/src/ui/ImGuiOpengl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/src/ui/ImGuiOpengl.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eduard/programming/EvolutionaryCreator/src/ui/ImGuiOpengl.cpp -o CMakeFiles/imgui.dir/src/ui/ImGuiOpengl.cpp.s

CMakeFiles/imgui.dir/dependencies/imgui/imgui_draw.cpp.o: CMakeFiles/imgui.dir/flags.make
CMakeFiles/imgui.dir/dependencies/imgui/imgui_draw.cpp.o: dependencies/imgui/imgui_draw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eduard/programming/EvolutionaryCreator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/imgui.dir/dependencies/imgui/imgui_draw.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/imgui.dir/dependencies/imgui/imgui_draw.cpp.o -c /home/eduard/programming/EvolutionaryCreator/dependencies/imgui/imgui_draw.cpp

CMakeFiles/imgui.dir/dependencies/imgui/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/dependencies/imgui/imgui_draw.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eduard/programming/EvolutionaryCreator/dependencies/imgui/imgui_draw.cpp > CMakeFiles/imgui.dir/dependencies/imgui/imgui_draw.cpp.i

CMakeFiles/imgui.dir/dependencies/imgui/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/dependencies/imgui/imgui_draw.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eduard/programming/EvolutionaryCreator/dependencies/imgui/imgui_draw.cpp -o CMakeFiles/imgui.dir/dependencies/imgui/imgui_draw.cpp.s

CMakeFiles/imgui.dir/dependencies/imgui/imgui_demo.cpp.o: CMakeFiles/imgui.dir/flags.make
CMakeFiles/imgui.dir/dependencies/imgui/imgui_demo.cpp.o: dependencies/imgui/imgui_demo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eduard/programming/EvolutionaryCreator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/imgui.dir/dependencies/imgui/imgui_demo.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/imgui.dir/dependencies/imgui/imgui_demo.cpp.o -c /home/eduard/programming/EvolutionaryCreator/dependencies/imgui/imgui_demo.cpp

CMakeFiles/imgui.dir/dependencies/imgui/imgui_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/dependencies/imgui/imgui_demo.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eduard/programming/EvolutionaryCreator/dependencies/imgui/imgui_demo.cpp > CMakeFiles/imgui.dir/dependencies/imgui/imgui_demo.cpp.i

CMakeFiles/imgui.dir/dependencies/imgui/imgui_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/dependencies/imgui/imgui_demo.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eduard/programming/EvolutionaryCreator/dependencies/imgui/imgui_demo.cpp -o CMakeFiles/imgui.dir/dependencies/imgui/imgui_demo.cpp.s

# Object files for target imgui
imgui_OBJECTS = \
"CMakeFiles/imgui.dir/dependencies/imgui/imgui.cpp.o" \
"CMakeFiles/imgui.dir/src/ui/ImGuiGlfw.cpp.o" \
"CMakeFiles/imgui.dir/src/ui/ImGuiOpengl.cpp.o" \
"CMakeFiles/imgui.dir/dependencies/imgui/imgui_draw.cpp.o" \
"CMakeFiles/imgui.dir/dependencies/imgui/imgui_demo.cpp.o"

# External object files for target imgui
imgui_EXTERNAL_OBJECTS =

lib/libimgui.a: CMakeFiles/imgui.dir/dependencies/imgui/imgui.cpp.o
lib/libimgui.a: CMakeFiles/imgui.dir/src/ui/ImGuiGlfw.cpp.o
lib/libimgui.a: CMakeFiles/imgui.dir/src/ui/ImGuiOpengl.cpp.o
lib/libimgui.a: CMakeFiles/imgui.dir/dependencies/imgui/imgui_draw.cpp.o
lib/libimgui.a: CMakeFiles/imgui.dir/dependencies/imgui/imgui_demo.cpp.o
lib/libimgui.a: CMakeFiles/imgui.dir/build.make
lib/libimgui.a: CMakeFiles/imgui.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/eduard/programming/EvolutionaryCreator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX static library lib/libimgui.a"
	$(CMAKE_COMMAND) -P CMakeFiles/imgui.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/imgui.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/imgui.dir/build: lib/libimgui.a

.PHONY : CMakeFiles/imgui.dir/build

CMakeFiles/imgui.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/imgui.dir/cmake_clean.cmake
.PHONY : CMakeFiles/imgui.dir/clean

CMakeFiles/imgui.dir/depend:
	cd /home/eduard/programming/EvolutionaryCreator && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/eduard/programming/EvolutionaryCreator /home/eduard/programming/EvolutionaryCreator /home/eduard/programming/EvolutionaryCreator /home/eduard/programming/EvolutionaryCreator /home/eduard/programming/EvolutionaryCreator/CMakeFiles/imgui.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/imgui.dir/depend

# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/alireza/Desktop/pole_handler_ws/src/window_pkg

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alireza/Desktop/upboard_backup/window_ws/src/build-window_pkg-Desktop-Default

# Include any dependencies generated for this target.
include CMakeFiles/include_window.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/include_window.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/include_window.dir/flags.make

CMakeFiles/include_window.dir/include_window_automoc.cpp.o: CMakeFiles/include_window.dir/flags.make
CMakeFiles/include_window.dir/include_window_automoc.cpp.o: include_window_automoc.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/alireza/Desktop/upboard_backup/window_ws/src/build-window_pkg-Desktop-Default/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/include_window.dir/include_window_automoc.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/include_window.dir/include_window_automoc.cpp.o -c /home/alireza/Desktop/upboard_backup/window_ws/src/build-window_pkg-Desktop-Default/include_window_automoc.cpp

CMakeFiles/include_window.dir/include_window_automoc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/include_window.dir/include_window_automoc.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/alireza/Desktop/upboard_backup/window_ws/src/build-window_pkg-Desktop-Default/include_window_automoc.cpp > CMakeFiles/include_window.dir/include_window_automoc.cpp.i

CMakeFiles/include_window.dir/include_window_automoc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/include_window.dir/include_window_automoc.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/alireza/Desktop/upboard_backup/window_ws/src/build-window_pkg-Desktop-Default/include_window_automoc.cpp -o CMakeFiles/include_window.dir/include_window_automoc.cpp.s

CMakeFiles/include_window.dir/include_window_automoc.cpp.o.requires:
.PHONY : CMakeFiles/include_window.dir/include_window_automoc.cpp.o.requires

CMakeFiles/include_window.dir/include_window_automoc.cpp.o.provides: CMakeFiles/include_window.dir/include_window_automoc.cpp.o.requires
	$(MAKE) -f CMakeFiles/include_window.dir/build.make CMakeFiles/include_window.dir/include_window_automoc.cpp.o.provides.build
.PHONY : CMakeFiles/include_window.dir/include_window_automoc.cpp.o.provides

CMakeFiles/include_window.dir/include_window_automoc.cpp.o.provides.build: CMakeFiles/include_window.dir/include_window_automoc.cpp.o

# Object files for target include_window
include_window_OBJECTS = \
"CMakeFiles/include_window.dir/include_window_automoc.cpp.o"

# External object files for target include_window
include_window_EXTERNAL_OBJECTS =

devel/lib/libinclude_window.so: CMakeFiles/include_window.dir/include_window_automoc.cpp.o
devel/lib/libinclude_window.so: CMakeFiles/include_window.dir/build.make
devel/lib/libinclude_window.so: CMakeFiles/include_window.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library devel/lib/libinclude_window.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/include_window.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/include_window.dir/build: devel/lib/libinclude_window.so
.PHONY : CMakeFiles/include_window.dir/build

CMakeFiles/include_window.dir/requires: CMakeFiles/include_window.dir/include_window_automoc.cpp.o.requires
.PHONY : CMakeFiles/include_window.dir/requires

CMakeFiles/include_window.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/include_window.dir/cmake_clean.cmake
.PHONY : CMakeFiles/include_window.dir/clean

CMakeFiles/include_window.dir/depend:
	cd /home/alireza/Desktop/upboard_backup/window_ws/src/build-window_pkg-Desktop-Default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alireza/Desktop/pole_handler_ws/src/window_pkg /home/alireza/Desktop/pole_handler_ws/src/window_pkg /home/alireza/Desktop/upboard_backup/window_ws/src/build-window_pkg-Desktop-Default /home/alireza/Desktop/upboard_backup/window_ws/src/build-window_pkg-Desktop-Default /home/alireza/Desktop/upboard_backup/window_ws/src/build-window_pkg-Desktop-Default/CMakeFiles/include_window.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/include_window.dir/depend


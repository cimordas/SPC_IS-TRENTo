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
CMAKE_SOURCE_DIR = /home/cindy/cernbox/MyProjects/SPC_IS-Trento/root_trento

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cindy/cernbox/MyProjects/SPC_IS-Trento/root_trento/build

# Include any dependencies generated for this target.
include CMakeFiles/merger.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/merger.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/merger.dir/flags.make

CMakeFiles/merger.dir/merger.cpp.o: CMakeFiles/merger.dir/flags.make
CMakeFiles/merger.dir/merger.cpp.o: ../merger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cindy/cernbox/MyProjects/SPC_IS-Trento/root_trento/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/merger.dir/merger.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/merger.dir/merger.cpp.o -c /home/cindy/cernbox/MyProjects/SPC_IS-Trento/root_trento/merger.cpp

CMakeFiles/merger.dir/merger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/merger.dir/merger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cindy/cernbox/MyProjects/SPC_IS-Trento/root_trento/merger.cpp > CMakeFiles/merger.dir/merger.cpp.i

CMakeFiles/merger.dir/merger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/merger.dir/merger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cindy/cernbox/MyProjects/SPC_IS-Trento/root_trento/merger.cpp -o CMakeFiles/merger.dir/merger.cpp.s

# Object files for target merger
merger_OBJECTS = \
"CMakeFiles/merger.dir/merger.cpp.o"

# External object files for target merger
merger_EXTERNAL_OBJECTS =

merger: CMakeFiles/merger.dir/merger.cpp.o
merger: CMakeFiles/merger.dir/build.make
merger: src/libsrc.so
merger: /home/cindy/root/lib/libCore.so
merger: /home/cindy/root/lib/libImt.so
merger: /home/cindy/root/lib/libRIO.so
merger: /home/cindy/root/lib/libNet.so
merger: /home/cindy/root/lib/libHist.so
merger: /home/cindy/root/lib/libGraf.so
merger: /home/cindy/root/lib/libGraf3d.so
merger: /home/cindy/root/lib/libGpad.so
merger: /home/cindy/root/lib/libROOTDataFrame.so
merger: /home/cindy/root/lib/libTree.so
merger: /home/cindy/root/lib/libTreePlayer.so
merger: /home/cindy/root/lib/libRint.so
merger: /home/cindy/root/lib/libPostscript.so
merger: /home/cindy/root/lib/libMatrix.so
merger: /home/cindy/root/lib/libPhysics.so
merger: /home/cindy/root/lib/libMathCore.so
merger: /home/cindy/root/lib/libThread.so
merger: /home/cindy/root/lib/libMultiProc.so
merger: /home/cindy/root/lib/libROOTVecOps.so
merger: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.71.0
merger: /usr/lib/x86_64-linux-gnu/libboost_program_options.so.1.71.0
merger: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
merger: CMakeFiles/merger.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cindy/cernbox/MyProjects/SPC_IS-Trento/root_trento/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable merger"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/merger.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/merger.dir/build: merger

.PHONY : CMakeFiles/merger.dir/build

CMakeFiles/merger.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/merger.dir/cmake_clean.cmake
.PHONY : CMakeFiles/merger.dir/clean

CMakeFiles/merger.dir/depend:
	cd /home/cindy/cernbox/MyProjects/SPC_IS-Trento/root_trento/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cindy/cernbox/MyProjects/SPC_IS-Trento/root_trento /home/cindy/cernbox/MyProjects/SPC_IS-Trento/root_trento /home/cindy/cernbox/MyProjects/SPC_IS-Trento/root_trento/build /home/cindy/cernbox/MyProjects/SPC_IS-Trento/root_trento/build /home/cindy/cernbox/MyProjects/SPC_IS-Trento/root_trento/build/CMakeFiles/merger.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/merger.dir/depend


# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /home/evgeny/BIG_PROJECT/clion-2018.2.5/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/evgeny/BIG_PROJECT/clion-2018.2.5/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/evgeny/Sem_7/prac-7/Task_2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/evgeny/Sem_7/prac-7/Task_2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Gen.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Gen.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Gen.dir/flags.make

CMakeFiles/Gen.dir/Generator.cpp.o: CMakeFiles/Gen.dir/flags.make
CMakeFiles/Gen.dir/Generator.cpp.o: ../Generator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/evgeny/Sem_7/prac-7/Task_2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Gen.dir/Generator.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Gen.dir/Generator.cpp.o -c /home/evgeny/Sem_7/prac-7/Task_2/Generator.cpp

CMakeFiles/Gen.dir/Generator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Gen.dir/Generator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/evgeny/Sem_7/prac-7/Task_2/Generator.cpp > CMakeFiles/Gen.dir/Generator.cpp.i

CMakeFiles/Gen.dir/Generator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Gen.dir/Generator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/evgeny/Sem_7/prac-7/Task_2/Generator.cpp -o CMakeFiles/Gen.dir/Generator.cpp.s

# Object files for target Gen
Gen_OBJECTS = \
"CMakeFiles/Gen.dir/Generator.cpp.o"

# External object files for target Gen
Gen_EXTERNAL_OBJECTS =

Gen: CMakeFiles/Gen.dir/Generator.cpp.o
Gen: CMakeFiles/Gen.dir/build.make
Gen: CMakeFiles/Gen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/evgeny/Sem_7/prac-7/Task_2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Gen"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Gen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Gen.dir/build: Gen

.PHONY : CMakeFiles/Gen.dir/build

CMakeFiles/Gen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Gen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Gen.dir/clean

CMakeFiles/Gen.dir/depend:
	cd /home/evgeny/Sem_7/prac-7/Task_2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/evgeny/Sem_7/prac-7/Task_2 /home/evgeny/Sem_7/prac-7/Task_2 /home/evgeny/Sem_7/prac-7/Task_2/cmake-build-debug /home/evgeny/Sem_7/prac-7/Task_2/cmake-build-debug /home/evgeny/Sem_7/prac-7/Task_2/cmake-build-debug/CMakeFiles/Gen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Gen.dir/depend

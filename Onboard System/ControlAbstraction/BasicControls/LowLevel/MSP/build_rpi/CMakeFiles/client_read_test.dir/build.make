# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_SOURCE_DIR = "/root/Gardien-System/Onboard System/ControlAbstraction/BasicControls/LowLevel/MSP"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/root/Gardien-System/Onboard System/ControlAbstraction/BasicControls/LowLevel/MSP/build_rpi"

# Include any dependencies generated for this target.
include CMakeFiles/client_read_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client_read_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client_read_test.dir/flags.make

CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.o: CMakeFiles/client_read_test.dir/flags.make
CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.o: ../examples/client_read_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/root/Gardien-System/Onboard System/ControlAbstraction/BasicControls/LowLevel/MSP/build_rpi/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.o -c "/root/Gardien-System/Onboard System/ControlAbstraction/BasicControls/LowLevel/MSP/examples/client_read_test.cpp"

CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/root/Gardien-System/Onboard System/ControlAbstraction/BasicControls/LowLevel/MSP/examples/client_read_test.cpp" > CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.i

CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/root/Gardien-System/Onboard System/ControlAbstraction/BasicControls/LowLevel/MSP/examples/client_read_test.cpp" -o CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.s

CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.o.requires:

.PHONY : CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.o.requires

CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.o.provides: CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.o.requires
	$(MAKE) -f CMakeFiles/client_read_test.dir/build.make CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.o.provides.build
.PHONY : CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.o.provides

CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.o.provides.build: CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.o


# Object files for target client_read_test
client_read_test_OBJECTS = \
"CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.o"

# External object files for target client_read_test
client_read_test_EXTERNAL_OBJECTS =

client_read_test: CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.o
client_read_test: CMakeFiles/client_read_test.dir/build.make
client_read_test: libmspclient.so
client_read_test: libmsp_msg_print.so
client_read_test: CMakeFiles/client_read_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/root/Gardien-System/Onboard System/ControlAbstraction/BasicControls/LowLevel/MSP/build_rpi/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable client_read_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client_read_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client_read_test.dir/build: client_read_test

.PHONY : CMakeFiles/client_read_test.dir/build

CMakeFiles/client_read_test.dir/requires: CMakeFiles/client_read_test.dir/examples/client_read_test.cpp.o.requires

.PHONY : CMakeFiles/client_read_test.dir/requires

CMakeFiles/client_read_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client_read_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client_read_test.dir/clean

CMakeFiles/client_read_test.dir/depend:
	cd "/root/Gardien-System/Onboard System/ControlAbstraction/BasicControls/LowLevel/MSP/build_rpi" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/root/Gardien-System/Onboard System/ControlAbstraction/BasicControls/LowLevel/MSP" "/root/Gardien-System/Onboard System/ControlAbstraction/BasicControls/LowLevel/MSP" "/root/Gardien-System/Onboard System/ControlAbstraction/BasicControls/LowLevel/MSP/build_rpi" "/root/Gardien-System/Onboard System/ControlAbstraction/BasicControls/LowLevel/MSP/build_rpi" "/root/Gardien-System/Onboard System/ControlAbstraction/BasicControls/LowLevel/MSP/build_rpi/CMakeFiles/client_read_test.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/client_read_test.dir/depend

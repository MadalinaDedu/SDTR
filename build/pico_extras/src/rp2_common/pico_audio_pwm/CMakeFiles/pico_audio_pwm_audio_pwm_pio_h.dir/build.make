# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /workspace

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /workspace/build

# Utility rule file for pico_audio_pwm_audio_pwm_pio_h.

# Include any custom commands dependencies for this target.
include pico_extras/src/rp2_common/pico_audio_pwm/CMakeFiles/pico_audio_pwm_audio_pwm_pio_h.dir/compiler_depend.make

# Include the progress variables for this target.
include pico_extras/src/rp2_common/pico_audio_pwm/CMakeFiles/pico_audio_pwm_audio_pwm_pio_h.dir/progress.make

pico_extras/src/rp2_common/pico_audio_pwm/CMakeFiles/pico_audio_pwm_audio_pwm_pio_h: pico_extras/src/rp2_common/pico_audio_pwm/audio_pwm.pio.h

pico_extras/src/rp2_common/pico_audio_pwm/audio_pwm.pio.h: /freertos-pico/pico-extras/src/rp2_common/pico_audio_pwm/audio_pwm.pio
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/workspace/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating audio_pwm.pio.h"
	cd /workspace/build/pico_extras/src/rp2_common/pico_audio_pwm && ../../../../pioasm/pioasm -o c-sdk /freertos-pico/pico-extras/src/rp2_common/pico_audio_pwm/audio_pwm.pio /workspace/build/pico_extras/src/rp2_common/pico_audio_pwm/audio_pwm.pio.h

pico_audio_pwm_audio_pwm_pio_h: pico_extras/src/rp2_common/pico_audio_pwm/CMakeFiles/pico_audio_pwm_audio_pwm_pio_h
pico_audio_pwm_audio_pwm_pio_h: pico_extras/src/rp2_common/pico_audio_pwm/audio_pwm.pio.h
pico_audio_pwm_audio_pwm_pio_h: pico_extras/src/rp2_common/pico_audio_pwm/CMakeFiles/pico_audio_pwm_audio_pwm_pio_h.dir/build.make
.PHONY : pico_audio_pwm_audio_pwm_pio_h

# Rule to build all files generated by this target.
pico_extras/src/rp2_common/pico_audio_pwm/CMakeFiles/pico_audio_pwm_audio_pwm_pio_h.dir/build: pico_audio_pwm_audio_pwm_pio_h
.PHONY : pico_extras/src/rp2_common/pico_audio_pwm/CMakeFiles/pico_audio_pwm_audio_pwm_pio_h.dir/build

pico_extras/src/rp2_common/pico_audio_pwm/CMakeFiles/pico_audio_pwm_audio_pwm_pio_h.dir/clean:
	cd /workspace/build/pico_extras/src/rp2_common/pico_audio_pwm && $(CMAKE_COMMAND) -P CMakeFiles/pico_audio_pwm_audio_pwm_pio_h.dir/cmake_clean.cmake
.PHONY : pico_extras/src/rp2_common/pico_audio_pwm/CMakeFiles/pico_audio_pwm_audio_pwm_pio_h.dir/clean

pico_extras/src/rp2_common/pico_audio_pwm/CMakeFiles/pico_audio_pwm_audio_pwm_pio_h.dir/depend:
	cd /workspace/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /workspace /freertos-pico/pico-extras/src/rp2_common/pico_audio_pwm /workspace/build /workspace/build/pico_extras/src/rp2_common/pico_audio_pwm /workspace/build/pico_extras/src/rp2_common/pico_audio_pwm/CMakeFiles/pico_audio_pwm_audio_pwm_pio_h.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : pico_extras/src/rp2_common/pico_audio_pwm/CMakeFiles/pico_audio_pwm_audio_pwm_pio_h.dir/depend


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
CMAKE_SOURCE_DIR = /home/kikuke/DV/EpollBase_CppGameServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kikuke/DV/EpollBase_CppGameServer/build

# Include any dependencies generated for this target.
include CMakeFiles/EpollServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/EpollServer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/EpollServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/EpollServer.dir/flags.make

CMakeFiles/EpollServer.dir/main.cpp.o: CMakeFiles/EpollServer.dir/flags.make
CMakeFiles/EpollServer.dir/main.cpp.o: ../main.cpp
CMakeFiles/EpollServer.dir/main.cpp.o: CMakeFiles/EpollServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kikuke/DV/EpollBase_CppGameServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/EpollServer.dir/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/EpollServer.dir/main.cpp.o -MF CMakeFiles/EpollServer.dir/main.cpp.o.d -o CMakeFiles/EpollServer.dir/main.cpp.o -c /home/kikuke/DV/EpollBase_CppGameServer/main.cpp

CMakeFiles/EpollServer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EpollServer.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kikuke/DV/EpollBase_CppGameServer/main.cpp > CMakeFiles/EpollServer.dir/main.cpp.i

CMakeFiles/EpollServer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EpollServer.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kikuke/DV/EpollBase_CppGameServer/main.cpp -o CMakeFiles/EpollServer.dir/main.cpp.s

CMakeFiles/EpollServer.dir/PacketHandler.cpp.o: CMakeFiles/EpollServer.dir/flags.make
CMakeFiles/EpollServer.dir/PacketHandler.cpp.o: ../PacketHandler.cpp
CMakeFiles/EpollServer.dir/PacketHandler.cpp.o: CMakeFiles/EpollServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kikuke/DV/EpollBase_CppGameServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/EpollServer.dir/PacketHandler.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/EpollServer.dir/PacketHandler.cpp.o -MF CMakeFiles/EpollServer.dir/PacketHandler.cpp.o.d -o CMakeFiles/EpollServer.dir/PacketHandler.cpp.o -c /home/kikuke/DV/EpollBase_CppGameServer/PacketHandler.cpp

CMakeFiles/EpollServer.dir/PacketHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EpollServer.dir/PacketHandler.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kikuke/DV/EpollBase_CppGameServer/PacketHandler.cpp > CMakeFiles/EpollServer.dir/PacketHandler.cpp.i

CMakeFiles/EpollServer.dir/PacketHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EpollServer.dir/PacketHandler.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kikuke/DV/EpollBase_CppGameServer/PacketHandler.cpp -o CMakeFiles/EpollServer.dir/PacketHandler.cpp.s

CMakeFiles/EpollServer.dir/PacketIO.cpp.o: CMakeFiles/EpollServer.dir/flags.make
CMakeFiles/EpollServer.dir/PacketIO.cpp.o: ../PacketIO.cpp
CMakeFiles/EpollServer.dir/PacketIO.cpp.o: CMakeFiles/EpollServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kikuke/DV/EpollBase_CppGameServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/EpollServer.dir/PacketIO.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/EpollServer.dir/PacketIO.cpp.o -MF CMakeFiles/EpollServer.dir/PacketIO.cpp.o.d -o CMakeFiles/EpollServer.dir/PacketIO.cpp.o -c /home/kikuke/DV/EpollBase_CppGameServer/PacketIO.cpp

CMakeFiles/EpollServer.dir/PacketIO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EpollServer.dir/PacketIO.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kikuke/DV/EpollBase_CppGameServer/PacketIO.cpp > CMakeFiles/EpollServer.dir/PacketIO.cpp.i

CMakeFiles/EpollServer.dir/PacketIO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EpollServer.dir/PacketIO.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kikuke/DV/EpollBase_CppGameServer/PacketIO.cpp -o CMakeFiles/EpollServer.dir/PacketIO.cpp.s

CMakeFiles/EpollServer.dir/RingBuffer.cpp.o: CMakeFiles/EpollServer.dir/flags.make
CMakeFiles/EpollServer.dir/RingBuffer.cpp.o: ../RingBuffer.cpp
CMakeFiles/EpollServer.dir/RingBuffer.cpp.o: CMakeFiles/EpollServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kikuke/DV/EpollBase_CppGameServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/EpollServer.dir/RingBuffer.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/EpollServer.dir/RingBuffer.cpp.o -MF CMakeFiles/EpollServer.dir/RingBuffer.cpp.o.d -o CMakeFiles/EpollServer.dir/RingBuffer.cpp.o -c /home/kikuke/DV/EpollBase_CppGameServer/RingBuffer.cpp

CMakeFiles/EpollServer.dir/RingBuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EpollServer.dir/RingBuffer.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kikuke/DV/EpollBase_CppGameServer/RingBuffer.cpp > CMakeFiles/EpollServer.dir/RingBuffer.cpp.i

CMakeFiles/EpollServer.dir/RingBuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EpollServer.dir/RingBuffer.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kikuke/DV/EpollBase_CppGameServer/RingBuffer.cpp -o CMakeFiles/EpollServer.dir/RingBuffer.cpp.s

CMakeFiles/EpollServer.dir/SocketManager.cpp.o: CMakeFiles/EpollServer.dir/flags.make
CMakeFiles/EpollServer.dir/SocketManager.cpp.o: ../SocketManager.cpp
CMakeFiles/EpollServer.dir/SocketManager.cpp.o: CMakeFiles/EpollServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kikuke/DV/EpollBase_CppGameServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/EpollServer.dir/SocketManager.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/EpollServer.dir/SocketManager.cpp.o -MF CMakeFiles/EpollServer.dir/SocketManager.cpp.o.d -o CMakeFiles/EpollServer.dir/SocketManager.cpp.o -c /home/kikuke/DV/EpollBase_CppGameServer/SocketManager.cpp

CMakeFiles/EpollServer.dir/SocketManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EpollServer.dir/SocketManager.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kikuke/DV/EpollBase_CppGameServer/SocketManager.cpp > CMakeFiles/EpollServer.dir/SocketManager.cpp.i

CMakeFiles/EpollServer.dir/SocketManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EpollServer.dir/SocketManager.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kikuke/DV/EpollBase_CppGameServer/SocketManager.cpp -o CMakeFiles/EpollServer.dir/SocketManager.cpp.s

CMakeFiles/EpollServer.dir/TcpService.cpp.o: CMakeFiles/EpollServer.dir/flags.make
CMakeFiles/EpollServer.dir/TcpService.cpp.o: ../TcpService.cpp
CMakeFiles/EpollServer.dir/TcpService.cpp.o: CMakeFiles/EpollServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kikuke/DV/EpollBase_CppGameServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/EpollServer.dir/TcpService.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/EpollServer.dir/TcpService.cpp.o -MF CMakeFiles/EpollServer.dir/TcpService.cpp.o.d -o CMakeFiles/EpollServer.dir/TcpService.cpp.o -c /home/kikuke/DV/EpollBase_CppGameServer/TcpService.cpp

CMakeFiles/EpollServer.dir/TcpService.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EpollServer.dir/TcpService.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kikuke/DV/EpollBase_CppGameServer/TcpService.cpp > CMakeFiles/EpollServer.dir/TcpService.cpp.i

CMakeFiles/EpollServer.dir/TcpService.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EpollServer.dir/TcpService.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kikuke/DV/EpollBase_CppGameServer/TcpService.cpp -o CMakeFiles/EpollServer.dir/TcpService.cpp.s

CMakeFiles/EpollServer.dir/spbitio.c.o: CMakeFiles/EpollServer.dir/flags.make
CMakeFiles/EpollServer.dir/spbitio.c.o: ../spbitio.c
CMakeFiles/EpollServer.dir/spbitio.c.o: CMakeFiles/EpollServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kikuke/DV/EpollBase_CppGameServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/EpollServer.dir/spbitio.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/EpollServer.dir/spbitio.c.o -MF CMakeFiles/EpollServer.dir/spbitio.c.o.d -o CMakeFiles/EpollServer.dir/spbitio.c.o -c /home/kikuke/DV/EpollBase_CppGameServer/spbitio.c

CMakeFiles/EpollServer.dir/spbitio.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/EpollServer.dir/spbitio.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kikuke/DV/EpollBase_CppGameServer/spbitio.c > CMakeFiles/EpollServer.dir/spbitio.c.i

CMakeFiles/EpollServer.dir/spbitio.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/EpollServer.dir/spbitio.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kikuke/DV/EpollBase_CppGameServer/spbitio.c -o CMakeFiles/EpollServer.dir/spbitio.c.s

CMakeFiles/EpollServer.dir/spepoll.c.o: CMakeFiles/EpollServer.dir/flags.make
CMakeFiles/EpollServer.dir/spepoll.c.o: ../spepoll.c
CMakeFiles/EpollServer.dir/spepoll.c.o: CMakeFiles/EpollServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kikuke/DV/EpollBase_CppGameServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/EpollServer.dir/spepoll.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/EpollServer.dir/spepoll.c.o -MF CMakeFiles/EpollServer.dir/spepoll.c.o.d -o CMakeFiles/EpollServer.dir/spepoll.c.o -c /home/kikuke/DV/EpollBase_CppGameServer/spepoll.c

CMakeFiles/EpollServer.dir/spepoll.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/EpollServer.dir/spepoll.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kikuke/DV/EpollBase_CppGameServer/spepoll.c > CMakeFiles/EpollServer.dir/spepoll.c.i

CMakeFiles/EpollServer.dir/spepoll.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/EpollServer.dir/spepoll.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kikuke/DV/EpollBase_CppGameServer/spepoll.c -o CMakeFiles/EpollServer.dir/spepoll.c.s

CMakeFiles/EpollServer.dir/sppacketparser.c.o: CMakeFiles/EpollServer.dir/flags.make
CMakeFiles/EpollServer.dir/sppacketparser.c.o: ../sppacketparser.c
CMakeFiles/EpollServer.dir/sppacketparser.c.o: CMakeFiles/EpollServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kikuke/DV/EpollBase_CppGameServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/EpollServer.dir/sppacketparser.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/EpollServer.dir/sppacketparser.c.o -MF CMakeFiles/EpollServer.dir/sppacketparser.c.o.d -o CMakeFiles/EpollServer.dir/sppacketparser.c.o -c /home/kikuke/DV/EpollBase_CppGameServer/sppacketparser.c

CMakeFiles/EpollServer.dir/sppacketparser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/EpollServer.dir/sppacketparser.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kikuke/DV/EpollBase_CppGameServer/sppacketparser.c > CMakeFiles/EpollServer.dir/sppacketparser.c.i

CMakeFiles/EpollServer.dir/sppacketparser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/EpollServer.dir/sppacketparser.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kikuke/DV/EpollBase_CppGameServer/sppacketparser.c -o CMakeFiles/EpollServer.dir/sppacketparser.c.s

CMakeFiles/EpollServer.dir/spsocket.c.o: CMakeFiles/EpollServer.dir/flags.make
CMakeFiles/EpollServer.dir/spsocket.c.o: ../spsocket.c
CMakeFiles/EpollServer.dir/spsocket.c.o: CMakeFiles/EpollServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kikuke/DV/EpollBase_CppGameServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/EpollServer.dir/spsocket.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/EpollServer.dir/spsocket.c.o -MF CMakeFiles/EpollServer.dir/spsocket.c.o.d -o CMakeFiles/EpollServer.dir/spsocket.c.o -c /home/kikuke/DV/EpollBase_CppGameServer/spsocket.c

CMakeFiles/EpollServer.dir/spsocket.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/EpollServer.dir/spsocket.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kikuke/DV/EpollBase_CppGameServer/spsocket.c > CMakeFiles/EpollServer.dir/spsocket.c.i

CMakeFiles/EpollServer.dir/spsocket.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/EpollServer.dir/spsocket.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kikuke/DV/EpollBase_CppGameServer/spsocket.c -o CMakeFiles/EpollServer.dir/spsocket.c.s

# Object files for target EpollServer
EpollServer_OBJECTS = \
"CMakeFiles/EpollServer.dir/main.cpp.o" \
"CMakeFiles/EpollServer.dir/PacketHandler.cpp.o" \
"CMakeFiles/EpollServer.dir/PacketIO.cpp.o" \
"CMakeFiles/EpollServer.dir/RingBuffer.cpp.o" \
"CMakeFiles/EpollServer.dir/SocketManager.cpp.o" \
"CMakeFiles/EpollServer.dir/TcpService.cpp.o" \
"CMakeFiles/EpollServer.dir/spbitio.c.o" \
"CMakeFiles/EpollServer.dir/spepoll.c.o" \
"CMakeFiles/EpollServer.dir/sppacketparser.c.o" \
"CMakeFiles/EpollServer.dir/spsocket.c.o"

# External object files for target EpollServer
EpollServer_EXTERNAL_OBJECTS =

EpollServer: CMakeFiles/EpollServer.dir/main.cpp.o
EpollServer: CMakeFiles/EpollServer.dir/PacketHandler.cpp.o
EpollServer: CMakeFiles/EpollServer.dir/PacketIO.cpp.o
EpollServer: CMakeFiles/EpollServer.dir/RingBuffer.cpp.o
EpollServer: CMakeFiles/EpollServer.dir/SocketManager.cpp.o
EpollServer: CMakeFiles/EpollServer.dir/TcpService.cpp.o
EpollServer: CMakeFiles/EpollServer.dir/spbitio.c.o
EpollServer: CMakeFiles/EpollServer.dir/spepoll.c.o
EpollServer: CMakeFiles/EpollServer.dir/sppacketparser.c.o
EpollServer: CMakeFiles/EpollServer.dir/spsocket.c.o
EpollServer: CMakeFiles/EpollServer.dir/build.make
EpollServer: CMakeFiles/EpollServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kikuke/DV/EpollBase_CppGameServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable EpollServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EpollServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/EpollServer.dir/build: EpollServer
.PHONY : CMakeFiles/EpollServer.dir/build

CMakeFiles/EpollServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/EpollServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/EpollServer.dir/clean

CMakeFiles/EpollServer.dir/depend:
	cd /home/kikuke/DV/EpollBase_CppGameServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kikuke/DV/EpollBase_CppGameServer /home/kikuke/DV/EpollBase_CppGameServer /home/kikuke/DV/EpollBase_CppGameServer/build /home/kikuke/DV/EpollBase_CppGameServer/build /home/kikuke/DV/EpollBase_CppGameServer/build/CMakeFiles/EpollServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/EpollServer.dir/depend


CC=g++ -g -std=c++17

QUEUE_SOURCES=queue.cpp

PIANO_SOURCES=piano.cpp
PIANO_OBJS=${PIANO_SOURCES:.cpp=.o}

LIBTHREAD=libthread.o

ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
    detected_OS := Windows
else
    detected_OS := $(shell uname)
endif
# Print out the detected OS
$(info Detected OS is: $(detected_OS))

# Use Mac library if on Mac
ifeq ($(detected_OS),Darwin)  # Important to not have a space after the comma
	LIBTHREAD = libthread_macos.o
	CC += -D_XOPEN_SOURCE
endif

# Compile with assertions enabled
debug: ${QUEUE_SOURCES} ${LIBTHREAD}
	${CC} $^ -ldl -pthread -o $@

# Compile with assertions disabled
ndebug: ${QUEUE_SOURCES} ${LIBTHREAD}
	${CC} -DNDEBUG $^ -ldl -pthread -o $@

# Compile the piano example
piano: ${PIANO_OBJS} $(LIBTHREAD)
	${CC} -o $@ $^ -ldl -pthread

# Generic rules for compiling a source file to an object file
%.o: %.cpp
	${CC} -c $<
%.o: %.cc
	${CC} -c $<

clean:
	rm -f ${PIANO_OBJS} debug ndebug

# ----------------------------------------------------------------
#
#	ringbuffer makefile
#
#	w/ help from:
#		- http://randu.org/tutorials/c/libraries.php
#		- http://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
#
#
# ----------------------------------------------------------------

# http://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html#Debugging-Options
# -g 					// Produces debugging info in native OS format
# -D  eg -DDEBUG		// Enables #ifdef DEBUG check - http://stackoverflow.com/questions/987637/define-debug-1
# -ggdb					// Produces debugging info for use by GDB
DEBUG		=	-g -ggdb -DDEBUG

# http://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#Optimize-Options
# -O2					// Enables mid level optimizations
# -03					// Enables aggresive optimizations
# -pthread				// Sets preprocessor and linker flags for adding multi-threading support
# -D_REENTRANT			// http://pauillac.inria.fr/~xleroy/linuxthreads/faq.html#H
OPT 		=	-O2 -pthread -D_REENTRANT

# -pedantic-errors		// Many strict ISO C standards
# -Wall					// Adds many compliance check
# -Wextra				// Additional checks beyond Wall
ERR			=	-pedantic-errors -Wall -Wextra -Wno-unused-parameter -Wbad-function-cast


# -I.					// Includes files from current path
INC_PATH	=	-I.

LIB_PATH	=	

MAC			= -arch i386 -macosx_version_min 10.7

# ----------------------------------------------------------------

CC 			=	gcc

LD			=	ld

# -fPIC 	// Position Independant Code - for shared libs
CC_FLAGS	=	$(DEBUG) $(INC_PATH) $(OPT) $(ERR) -fPIC

LD_FLAGS	=	$(LIB_PATH) $(LIBS) $(MAC)

# ----- TESTS ----------------------------------------------------

LINKING		=	tests/test_link.c

TESTS		=	batchringbuffer_test.c test_link.c

# ----- TARGETS --------------------------------------------------

all: main batchringbuffer

.PHONY: all clean check

main:
	# Main...
	gcc src/main.c -o bin/ringbuffer

# Make for libbatchringbuffer.a library
batchringbuffer:
	# Building: bin/batchringbuffer.a
	# Compile the source file
	gcc $(CC_FLAGS) -o obj/batchringbuffer.o -c src/batchringbuffer.c
	# Create the library
	ar rc bin/libbatchringbuffer.a obj/batchringbuffer.o
	# Create static library
	ranlib bin/libbatchringbuffer.a
	# Create the so file
	ld $(LD_FLAGS) -o bin/libbatchringbuffer.so -Lbin/ -lbatchringbuffer

clean:
	# Clean...
	[ -f bin/ringbuffer			 	] && rm -f bin/ringbuffer 				|| [ 1 ]
	# Remove files produced by building batchringbuffer library
	[ -f obj/batchringbuffer.o 		] && rm -f obj/batchringbuffer.o 		|| [ 1 ]
	[ -f bin/libbatchringbuffer.a 	] && rm -f bin/libbatchringbuffer.a 	|| [ 1 ]

#test: ${TESTS} ./bin/ringbuffer
test: all
	# Check...
	gcc test/test_link.c -o test/test_link -L/bin -lbatchringbuffer
	./bin/test_link
	./bin/ringbuffer





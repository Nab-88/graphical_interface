# Variable definitions.

PLATFORM	= $(shell uname)
CC		= gcc
OBJDIR		= ./objs
TESTS		= ./tests
INCLUDES	= ./include
SRC = ./src
INCFLAGS	:= -I${INCLUDES} -I${TESTS}
#OPTFLAGS	:= -Os -DNDEBUG
OPTFLAGS	:= -g
CCFLAGS		:= -c ${OPTFLAGS} -Wall -std=c99

# The list of objects to include in the library

LIBEIOBJS	:=


# Platform specific definitions (OS X, Linux)

ifeq (${PLATFORM},Darwin)

	# Building for Mac OS X

	PLATDIR		= _osx
	INCFLAGS	:= ${INCFLAGS} -I/opt/local/include/SDL
	LINK		= ${CC}
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase.a
	LIBS		= ${LIBEIBASE} -L/opt/local/lib -lSDL -lSDL_ttf -lSDL_image -framework AppKit
	CCFLAGS		:= ${CCFLAGS} -D__OSX__

else

	# Building for Linux

	PLATDIR		= _x11
	INCFLAGS	:= ${INCFLAGS} -I/usr/include/SDL
	LINK		= ${CC}
#	ARCH	        = 32
	ARCH	        = 64
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase${ARCH}.a
	LIBS		= ${LIBEIBASE} -lSDL -lSDL_ttf -lSDL_image -lm
	CCFLAGS		:= ${CCFLAGS} -D__LINUX__ -m${ARCH}
	LDFLAGS		= -m${ARCH} -g

endif



# Main target of the makefile. To build specific targets, call "make <target_name>"

TARGETS		=	${LIBEI} \
			minimal lines move_TCA test_polygon init_scanline test_text test_fill map_rgba frame button hello_world puzzle two048 arc_draw
all : ${TARGETS}



########## Test-programs
#arc_draw
arc_draw : ${OBJDIR}/arc_draw.o ${LIBEIBASE} ${OBJDIR}/ei_draw.o
	${LINK} -o arc_draw ${LDFLAGS} ${OBJDIR}/arc_draw.o ${LIBS} ${OBJDIR}/ei_draw.o

${OBJDIR}/arc_draw.o : ${TESTS}/arc_draw.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/arc_draw.c -o ${OBJDIR}/arc_draw.o

#acote
acote : ${OBJDIR}/acote.o ${LIBEIBASE} ${OBJDIR}/ei_draw.o
	${LINK} -o acote ${LDFLAGS} ${OBJDIR}/acote.o ${LIBS} ${OBJDIR}/ei_draw.o

${OBJDIR}/acote.o : ${TESTS}/acote.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/acote.c -o ${OBJDIR}/acote.o

#order_TCA
order_TCA : ${OBJDIR}/order_TCA.o ${LIBEIBASE} ${OBJDIR}/ei_draw.o
	${LINK} -o order_TCA ${LDFLAGS} ${OBJDIR}/order_TCA.o ${LIBS} ${OBJDIR}/ei_draw.o

${OBJDIR}/order_TCA.o : ${TESTS}/order_TCA.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/order_TCA.c -o ${OBJDIR}/order_TCA.o

#test delete_side
delete_side : ${OBJDIR}/delete_side.o ${LIBEIBASE} ${OBJDIR}/ei_draw.o
	${LINK} -o delete_side ${LDFLAGS} ${OBJDIR}/delete_side.o ${LIBS} ${OBJDIR}/ei_draw.o

${OBJDIR}/delete_side.o : ${TESTS}/delete_side.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/delete_side.c -o ${OBJDIR}/delete_side.o

#test move_TCA
move_TCA : ${OBJDIR}/move_TCA.o ${LIBEIBASE} ${OBJDIR}/ei_draw.o
	${LINK} -o move_TCA ${LDFLAGS} ${OBJDIR}/move_TCA.o ${LIBS} ${OBJDIR}/ei_draw.o

${OBJDIR}/move_TCA.o : ${TESTS}/move_TCA.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/move_TCA.c -o ${OBJDIR}/move_TCA.o


# test_polygon
test_polygon : ${OBJDIR}/test_polygon.o ${LIBEIBASE} ${OBJDIR}/ei_draw.o
	${LINK} -o test_polygon ${LDFLAGS} ${OBJDIR}/test_polygon.o ${LIBS} ${OBJDIR}/ei_draw.o

${OBJDIR}/test_polygon.o : ${TESTS}/test_polygon.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_polygon.c -o ${OBJDIR}/test_polygon.o


# init_scanline
init_scanline : ${OBJDIR}/init_scanline.o ${LIBEIBASE} ${OBJDIR}/ei_draw.o
	${LINK} -o init_scanline ${LDFLAGS} ${OBJDIR}/init_scanline.o ${LIBS} ${OBJDIR}/ei_draw.o

${OBJDIR}/init_scanline.o : ${TESTS}/init_scanline.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/init_scanline.c -o ${OBJDIR}/init_scanline.o


# minimal

minimal : ${OBJDIR}/minimal.o ${LIBEIBASE} ${OBJDIR}/ei_draw.o
	${LINK} -o minimal ${LDFLAGS} ${OBJDIR}/minimal.o ${LIBS} ${OBJDIR}/ei_draw.o

#test_text
test_text : ${OBJDIR}/test_text.o ${LIBEIBASE} ${OBJDIR}/ei_draw.o
	${LINK} -o test_text ${LDFLAGS} ${OBJDIR}/test_text.o ${LIBS} ${OBJDIR}/ei_draw.o

${OBJDIR}/test_text.o : ${TESTS}/test_text.c
		${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_text.c -o ${OBJDIR}/test_text.o

${OBJDIR}/minimal.o : ${TESTS}/minimal.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/minimal.c -o ${OBJDIR}/minimal.o

# ei_draw

${OBJDIR}/ei_draw.o : ${SRC}/ei_draw.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_draw.c -o ${OBJDIR}/ei_draw.o

#test_copy
test_copy : ${OBJDIR}/test_copy.o ${LIBEIBASE} ${OBJDIR}/ei_draw.o
	${LINK} -o test_copy ${LDFLAGS} ${OBJDIR}/test_copy.o ${LIBS} ${OBJDIR}/ei_draw.o


${OBJDIR}/test_copy.o : ${TESTS}/test_copy.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_copy.c -o ${OBJDIR}/test_copy.o

#test_fill
test_fill : ${OBJDIR}/test_fill.o ${LIBEIBASE} ${OBJDIR}/ei_draw.o
	${LINK} -o test_fill ${LDFLAGS} ${OBJDIR}/test_fill.o ${LIBS} ${OBJDIR}/ei_draw.o


${OBJDIR}/test_fill.o : ${TESTS}/test_fill.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_fill.c -o ${OBJDIR}/test_fill.o

# map_rgba
map_rgba : ${OBJDIR}/map_rgba.o ${LIBEIBASE} ${OBJDIR}/ei_draw.o
	${LINK} -o map_rgba ${LDFLAGS} ${OBJDIR}/map_rgba.o ${LIBS} ${OBJDIR}/ei_draw.o


${OBJDIR}/map_rgba.o : ${TESTS}/map_rgba.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/map_rgba.c -o ${OBJDIR}/map_rgba.o

# lines

lines : ${OBJDIR}/lines.o ${LIBEIBASE} ${OBJDIR}/ei_draw.o ${LIBEI}
	${LINK} -o lines ${OBJDIR}/lines.o ${OBJDIR}/ei_draw.o ${LIBEI} ${LIBS}

${OBJDIR}/lines.o : ${TESTS}/lines.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/lines.c -o ${OBJDIR}/lines.o

# frame

frame : ${OBJDIR}/frame.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o frame ${LDFLAGS} ${OBJDIR}/frame.o ${LIBEI} ${LIBS}

${OBJDIR}/frame.o : ${TESTS}/frame.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/frame.c -o ${OBJDIR}/frame.o

# button

button : ${OBJDIR}/button.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o button ${LDFLAGS} ${OBJDIR}/button.o ${LIBEI} ${LIBS}

${OBJDIR}/button.o : ${TESTS}/button.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/button.c -o ${OBJDIR}/button.o

# hello_world

hello_world : ${OBJDIR}/hello_world.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o hello_world ${LDFLAGS} ${OBJDIR}/hello_world.o ${LIBEI} ${LIBS}

${OBJDIR}/hello_world.o : ${TESTS}/hello_world.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/hello_world.c -o ${OBJDIR}/hello_world.o

# puzzle

puzzle : ${OBJDIR}/puzzle.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o puzzle ${LDFLAGS} ${OBJDIR}/puzzle.o ${LIBEI} ${LIBS}

${OBJDIR}/puzzle.o : ${TESTS}/puzzle.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/puzzle.c -o ${OBJDIR}/puzzle.o


# two048

two048 : ${OBJDIR}/two048.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o two048 ${LDFLAGS} ${OBJDIR}/two048.o ${LIBEI} ${LIBS}

${OBJDIR}/two048.o : ${TESTS}/two048.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/two048.c -o ${OBJDIR}/two048.o



# Building of the library libei

${LIBEI} : ${LIBEIOBJS}
	ar rcs ${LIBEI} ${LIBEIOBJS}



# Building of the doxygen documentation.

doc :
	doxygen docs/doxygen.cfg



# Removing all built files.

clean:
	rm -f ${TARGETS}
	rm -f *.exe
	rm -f ${OBJDIR}/*

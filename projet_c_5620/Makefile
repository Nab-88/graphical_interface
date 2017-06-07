# Variable definitions.

PLATFORM	= $(shell uname)
CC		= gcc
OBJDIR		= ./objs
TESTS		= ./tests
INCLUDES	= ./include
SRC = ./src
INCFLAGS	:= -I${INCLUDES} -I${TESTS}
# OPTFLAGS	:= -O3 -DNDEBUG
OPTFLAGS	:= -g
CCFLAGS		:= -c ${OPTFLAGS} -Wall -std=c99

# The list of objects to include in the library

LIBEIOBJS	:= ${OBJDIR}/ei_placer.o ${OBJDIR}/ei_widget.o ${OBJDIR}/ei_widget_button.o ${OBJDIR}/ei_widget_frame.o ${OBJDIR}/ei_widget_toplevel.o ${OBJDIR}/ei_event.o ${OBJDIR}/ei_application.o ${OBJDIR}/ei_draw.o ${OBJDIR}/ei_draw_poly.o ${OBJDIR}/ei_draw_widgets.o ${SRC}/ext_testclass.o


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
			minimal lines move_TCA test_polygon init_scanline test_text test_fill map_rgba frame_modified button_modified hello_world_modified puzzle_modified two048_modified arc_draw round_frame test_button delete_side
all : ${TARGETS}

# Make un test

% : ${OBJDIR}/%.o ${LIBEIBASE} ${LIBEI}
	@${LINK} -o $* ${LDFLAGS} ${OBJDIR}/$*.o ${LIBEI} ${LIBS}

########## Our Program
# test_ext_class
#
#test_ext_class 
# # ei_draw
#
${OBJDIR}/ei_draw.o : ${SRC}/ei_draw.c
	@${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_draw.c -o ${OBJDIR}/ei_draw.o
#

# ei_placer
#
${OBJDIR}/ei_placer.o : ${SRC}/ei_placer.c
	@${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_placer.c -o ${OBJDIR}/ei_placer.o
#
# # ei_widget
#
${OBJDIR}/ei_widget.o : ${SRC}/ei_widget.c
	@${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_widget.c -o ${OBJDIR}/ei_widget.o
#
# # ei_widget_button
#
${OBJDIR}/ei_widget_button.o : ${SRC}/ei_widget_button.c
	@${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_widget_button.c -o ${OBJDIR}/ei_widget_button.o

#
# # ei_widget_frame
#
${OBJDIR}/ei_widget_frame.o : ${SRC}/ei_widget_frame.c
	@${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_widget_frame.c -o ${OBJDIR}/ei_widget_frame.o
#
# # ei_widget_toplevel
#
${OBJDIR}/ei_widget_toplevel.o : ${SRC}/ei_widget_toplevel.c
	@${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_widget_toplevel.c -o ${OBJDIR}/ei_widget_toplevel.o
#
# # ei_event
#
${OBJDIR}/ei_event.o : ${SRC}/ei_event.c
	@${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_event.c -o ${OBJDIR}/ei_event.o
#
# # ei_application
#
${OBJDIR}/ei_application.o : ${SRC}/ei_application.c
	@${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_application.c -o ${OBJDIR}/ei_application.o
#
${OBJDIR}/ei_draw_poly.o : ${SRC}/ei_draw_poly.c
	@${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_draw_poly.c -o ${OBJDIR}/ei_draw_poly.o

${OBJDIR}/ei_draw_widgets.o : ${SRC}/ei_draw_widgets.c
	@${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_draw_widgets.c -o ${OBJDIR}/ei_draw_widgets.o
#
# Compilation Tests

${OBJDIR}/%.o : ${TESTS}/%.c
	@${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/$*.c -o ${OBJDIR}/$*.o

# Building of the library libei

${LIBEI} : ${LIBEIOBJS}
	@ar rcs ${LIBEI} ${LIBEIOBJS}

# Building of the doxygen documentation.

doc :
	doxygen docs/doxygen.cfg

# Removing all built files.

clean:
	@rm -f ${TARGETS}
	@rm -f *.exe
	@rm -f ${OBJDIR}/*

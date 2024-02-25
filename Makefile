MKDIR			= mkdir -p
RM				= rm -rf

SRCDIR		= src
BUILDDIR	= build
LIBSDIR		= libs
RAYLIBDIR	= $(LIBSDIR)/raylib/src

CC				= cc
CFLAGS		?= -std=c99 -Wall -I. -I../raylib/src
LDFLAGS		?= -L. -L./$(RAYLIBDIR)
LDLIBS		?= -lraylib -lGL -lm -lpthread -ldl -lX11

GAME_OBJS	= $(BUILDDIR)/game.o

GAME_NAME	?= game

run: all
	$(BUILDDIR)/$(GAME_NAME)

all: $(BUILDDIR) $(GAME_OBJS) $(RAYLIBDIR)/libraylib.a
	$(CC) $(GAME_OBJS) $(LDFLAGS) $(LDLIBS) -o $(BUILDDIR)/$(GAME_NAME)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(RAYLIBDIR)/libraylib.a:
	git submodule update --init --recursive && \
	cd $(LIBSDIR)/raylib/src && \
	make clean all

$(BUILDDIR):
	$(MKDIR) $(BUILDDIR)

.PHONY: clean
clean:
	$(RM) $(BUILDDIR)


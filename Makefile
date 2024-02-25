CC			= cc
CFLAGS	?= -std=c99 -Wall -I. -I../raylib/src
LDFLAGS	?= -L. -L./libs/raylib/src
LDLIBS	?= -lraylib -lGL -lm -lpthread -ldl -lX11

MKDIR		= mkdir -p
RM			= rm -rf

SRCDIR		= src
BUILDDIR	= build
GAME_OBJS	= $(BUILDDIR)/game.o

GAME_NAME	?= game

run: $(BUILDDIR) $(BUILDDIR)/$(GAME_NAME)
	$(BUILDDIR)/$(GAME_NAME)

all: $(BUILDDIR) $(GAME_OBJS)
	$(CC) $(GAME_OBJS) $(LDFLAGS) $(LDLIBS) -o $(BUILDDIR)/$(GAME_NAME)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR):
	$(MKDIR) $(BUILDDIR)

.PHONY: clean
clean:
	$(RM) $(BUILDDIR)


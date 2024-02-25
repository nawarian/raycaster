PLATFORM	?= PLATFORM_DESKTOP # PLATFORM_DESKTOP | PLATFORM_WEB

# Commands
MKDIR			= mkdir -p
RM				= rm -rf

# Paths
SRCDIR		= src
BUILDDIR	= build
LIBSDIR		= libs
RAYLIBDIR	= $(LIBSDIR)/raylib/src
RESDIR		= resources

# Output name
GAME_NAME	?= game
ifeq ($(PLATFORM), PLATFORM_WEB)
	GAME_NAME = index.html
endif

# Game objects to be compiled
GAME_OBJS	= $(BUILDDIR)/game.o \
						$(BUILDDIR)/world.o \
						$(BUILDDIR)/player.o

# Compiler flags
CC				?= cc
CFLAGS		?= -std=c99 -Wall -I. -I$(RAYLIBDIR) -s
LDFLAGS		?= -L.
LDLIBS		?= -lm -lpthread -lGL

ifeq ($(PLATFORM), PLATFORM_DESKTOP)
	LDFLAGS	+= -L./$(RAYLIBDIR)
	LDLIBS	+= -lraylib -ldl -lX11
endif

ifeq ($(PLATFORM), PLATFORM_WEB)
	CC						= emcc

	WEB_SHELL			?= shell.html
	WEB_HEAP_SIZE	?= 67108864 # 64 MB

	CFLAGS				+= -DPLATFORM_WEB
	LDFLAGS				+= -s USE_GLFW=3 -s TOTAL_MEMORY=$(WEB_HEAP_SIZE) -s FORCE_FILESYSTEM=1
	LDFLAGS				+= -s USE_PTHREADS=1 -s ASYNCIFY -O3
	LDFLAGS				+= --shell-file $(WEB_SHELL) --preload-file $(RESDIR)
endif

# Rules
run: all
	$(BUILDDIR)/$(GAME_NAME)

all: $(BUILDDIR) $(GAME_OBJS) $(RAYLIBDIR)/libraylib.a
	$(CC) $(GAME_OBJS) $(RAYLIBDIR)/libraylib.a $(LDFLAGS) $(LDLIBS) -o $(BUILDDIR)/$(GAME_NAME)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(RAYLIBDIR)/libraylib.a:
	git submodule update --init --recursive && \
	cd $(RAYLIBDIR) && \
	make clean all

$(BUILDDIR):
	$(MKDIR) $(BUILDDIR)

.PHONY: clean
clean:
	$(RM) $(BUILDDIR)
	cd $(RAYLIBDIR) && make clean


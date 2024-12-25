CC := cc
TARGET := bouncy
SRC := bouncy.c
SDL_FLAGS := $(shell sdl2-config --cflags --libs) # Flags for compiling with SDL2

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $@ $^ $(SDL_FLAGS)
clean:
	rm -f $(TARGET)

.PHONY: all clean


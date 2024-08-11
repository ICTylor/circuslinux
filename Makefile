CC = gcc
EMCC = emcc
CFLAGS = -Wall -O2 -I/usr/include/SDL2 -s ALLOW_MEMORY_GROWTH=1 -s STACK_SIZE=5MB
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer

SRCS = circuslinux.c
TARGET = circuslinux
EM_TARGET = circuslinux.html

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

emscripten: $(SRCS)
	$(EMCC) $(CFLAGS) $(SRCS) -o $(EM_TARGET) -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_MIXER=2 -s SDL2_IMAGE_FORMATS='["png"]' -s INITIAL_MEMORY=67108864 -s MAXIMUM_MEMORY=268435456 --preload-file data@/

clean:
	rm -f $(TARGET) $(EM_TARGET) $(EM_TARGET).js $(EM_TARGET).wasm

.PHONY: all clean emscripten

CC = gcc
EMCC = emcc
CFLAGS = -Wall -I/usr/include/SDL2
RELEASE_CFLAGS = -O3 $(COMMON_CFLAGS)
EMFLAGS = -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer
DEBUGFLAGS = -O0 -g -s ASSERTIONS=1 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=1 -s DEMANGLE_SUPPORT=1
RELEASEFLAGS = -s ENVIRONMENT=web -s NO_EXIT_RUNTIME=1 -s TEXTDECODER=2 -s ABORTING_MALLOC=0 --closure 1
MAPFLAGS = -gsource-map --source-map-base http://localhost:8000/

SRCS = circuslinux.c
TARGET = circuslinux
EM_TARGET = circuslinux.html

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

emscripten: $(SRCS)
	$(EMCC) $(CFLAGS) $(EMFLAGS) $(DEBUGFLAGS) $(SRCS) -o $(EM_TARGET) -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_MIXER=2 -s SDL2_IMAGE_FORMATS='["png"]' -s SDL2_MIXER_FORMATS=["mod"] --preload-file data/ $(MAPFLAGS)

emscripten_release: $(SRCS)
	$(EMCC) $(CFLAGS) $(RELEASE_CFLAGS) $(EMFLAGS) $(RELEASEFLAGS) $(SRCS) -o $(EM_TARGET) -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_MIXER=2 -s SDL2_IMAGE_FORMATS='["png"]' -s SDL2_MIXER_FORMATS=["mod"] --preload-file data/

clean:
	rm -f $(TARGET) $(EM_TARGET) $(EM_TARGET).js $(EM_TARGET).wasm

.PHONY: all clean emscripten

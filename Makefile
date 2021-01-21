LIBS = -lSDL2main -lSDL2
FLAGS = -Iinclude -Llib
# FLAGS = -I/usr/local/include -L/usr/local/lib # Pour linux

all:
	@clear
	@echo "Compiling game..."
	@gcc src/*.c -o app ${FLAGS} ${LIBS}
	@clear
	@./app
	@rm app
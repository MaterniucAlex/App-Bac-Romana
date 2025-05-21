appName := App
libs := -lraylib -lm -pthread -ldl -lrt -lX11

all:
	@clear
	gcc src/*.c src/**/*.c	-o ${appName} -Llib -Iinclude ${libs} -Wall
run:
	@clear
	./App
build:
	sh buildApp.sh

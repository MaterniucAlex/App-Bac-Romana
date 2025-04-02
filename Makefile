appName := App
libs := -lraylib -lm -pthread -ldl -lrt -lX11

all:
	@clear
	gcc src/*.c	-o ${appName} -Llib -Iinclude ${libs} 
run:
	@clear
	./App

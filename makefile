all: 
	echo "Command all"

compile: 
	gcc -g -Wall *.c -o lista3 -lglut -lGL -lGLU -lSOIL -lm 
 
execute: compile
	./lista3
 
clean:
	rm -rf lista3

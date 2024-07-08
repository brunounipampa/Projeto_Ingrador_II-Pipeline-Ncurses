index:
	gcc pipeline.c -c -g
	gcc -o exec main.c pipeline.c -lncurses -lrt
	./exec

clean:
	rm pipeline.o
	rm exec

program: main.c
	gcc -o program main.c

test: program Test.txt
	./program < Test.txt

clean:
	rm -f program

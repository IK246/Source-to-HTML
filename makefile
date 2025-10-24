a.out: main.o source2html.o tokens.o
	gcc -o a.out main.o source2html.o tokens.o

main.o: main.c main.h
	gcc -c main.c

source2html.o: source2html.c main.h
	gcc -c source2html.c

tokens.o: tokens.c main.h
	gcc -c tokens.c

clean:
	rm *.o a.out

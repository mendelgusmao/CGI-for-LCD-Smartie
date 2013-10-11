all: main

main: core.o cgi.c
	gcc core.o cgi.c -o main

core.o: core.go
	gccgo -c core.go -o core.o -fgo-prefix=cgicore

clean:
	rm -f main *.o

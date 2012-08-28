#Makefile
# Peter Nguyen

CC = gcc
BINPATH = /usr/bin

yahoo_cracking: function.o function.h main.o
	$(CC) function.o main.o -o yahoo_cracking -lcurl
main.o: main.c function.h
	$(CC) -c main.c
function.o:
	$(CC) -c function.c -lcurl

clean:
	@rm -rf yahoo_cracking
	@rm -rf *.o
install:
	@cp -Rv yahoo_cracking $(BINPATH)
uninstall:
	@rm -rf $(BINPATH)/yahoo_cracking
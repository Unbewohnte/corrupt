all:
	gcc -Wall -Werror -O2 -static corrupt.c -o corrupt

clean:
	rm -f corrupt

install: all
	mv corrupt /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/corrupt
# kilo is what we want to build, kilo.c is what is required to build it
kilo: kilo.c
	$(CC) kilo.c -o kilo -Wall -Wextra -pedantic -std=c99

run: kilo
	./kilo
CC := g++
CFLAGS := -Wall -std=c++17 -c

main: main.o
	$(CC) -o main main.o

main.o: main.cc rtwknd.h
	$(CC) $(CFLAGS) main.cc

clean:
	rm -rf main *.o

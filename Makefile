all: cache_sim
	rm *.o

cache_sim: BasicCache.o main.o
	g++ -o cache_sim BasicCache.o main.o

BasicCache.o: BasicCache.cc BasicCache.h
	g++ -c BasicCache.cc

main.o: main.cc
	g++ -c main.cc

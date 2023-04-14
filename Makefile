all: cache_sim
	rm *.o

cache_sim: WSDAMCache.o BasicCache.o JoeCache.o main.o
	g++ -o cache_sim WSDAMCache.o BasicCache.o JoeCache.o main.o

BasicCache.o: BasicCache.cc BasicCache.h
	g++ -c BasicCache.cc

WSDAMCache.o: WSDAMCache.cc WSDAMCache.h
	g++ -c WSDAMCache.cc

JoeCache.o: JoeCache.cc JoeCache.h
	g++ -c JoeCache.cc

main.o: main.cc
	g++ -c main.cc

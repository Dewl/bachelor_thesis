LIBS_OPENCV = `$(PREFIX)pkg-config --libs opencv`
INCLUDE_OPENCV = `$(PREFIX)pkg-config --cflags opencv`
OPT = -Wall -O0
OPT_CPP = -std=c++11 -g

all: vibe main cvutil config sensor drawer
	mkdir -p bin/
	$(make link)

vibe:
	mkdir -p obj/
	gcc $(OPT) -c src/vibe.c
	mv *.o obj/

main:
	mkdir -p obj/
	g++ $(OPT) $(OPT_CPP) -c src/main.cpp
	mv *.o obj/

config:
	mkdir -p obj/
	g++ $(OPT) $(OPT_CPP) -c src/config.cpp
	mv *.o obj/

sensor:
	mkdir -p obj/
	g++ $(OPT) $(OPT_CPP) -c src/sensor.cpp
	mv *.o obj/

drawer:
	mkdir -p obj/
	g++ $(OPT) $(OPT_CPP) -c src/drawer.cpp
	mv *.o obj/

cvutil:
	mkdir -p obj/
	g++ $(OPT) $(OPT_CPP) -c src/cvutil.cpp
	mv *.o obj/

clean:
	rm -f obj/*.o
	rm -f bin/main

link:
	g++ $(OPT) $(INCLUDE_OPENCV) obj/*.o -o bin/main $(LIBS_OPENCV)


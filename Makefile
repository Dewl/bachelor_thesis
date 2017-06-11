LIBS_OPENCV = `$(PREFIX)pkg-config --libs opencv`
INCLUDE_OPENCV = `$(PREFIX)pkg-config --cflags opencv`
OPT = -Wall -O3
OPT_CPP = -std=c++11

all: vibe main cvutil config sensor drawer
	mkdir -p bin/
	$(make link)

all2: vibe main2 cvutil config sensor drawer
	mkdir -p bin/
	$(make link)

vibe:
	mkdir -p obj/
	gcc $(OPT) -c src/vibe.c
	mv *.o obj/

sensor:
	mkdir -p obj/
	gcc $(OPT) -c src/sensor.c
	mv *.o obj/

main:
	mkdir -p obj/
	g++ $(OPT) $(OPT_CPP) -c src/main.cpp
	mv *.o obj/

main2:
	mkdir -p obj/
	g++ $(OPT) $(OPT_CPP) -c src/main2.cpp
	mv *.o obj/

config:
	mkdir -p obj/
	g++ $(OPT) $(OPT_CPP) -c src/config.cpp
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
	rm -f bin/main2

link:
	g++ $(OPT) $(INCLUDE_OPENCV) obj/*.o -o bin/main $(LIBS_OPENCV)

link2:
	g++ $(OPT) $(INCLUDE_OPENCV) obj/*.o -o bin/main2 $(LIBS_OPENCV)


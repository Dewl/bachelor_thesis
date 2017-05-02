LIBS_OPENCV = `$(PREFIX)pkg-config --libs opencv`
INCLUDE_OPENCV = `$(PREFIX)pkg-config --cflags opencv`
OPT = -Wall -O0
OPT_CPP = -std=c++11 -g

all: vibe main debug cvutil extractor blob tracker counter
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
debug:
	mkdir -p obj/
	g++ $(OPT) $(OPT_CPP) -c src/debug.cpp
	mv *.o obj/
cvutil:
	mkdir -p obj/
	g++ $(OPT) $(OPT_CPP) -c src/cvutil.cpp
	mv *.o obj/

extractor:
	mkdir -p obj/
	g++ $(OPT) $(OPT_CPP) -c src/extractor.cpp
	mv *.o obj/

blob:
	mkdir -p obj/
	g++ $(OPT) $(OPT_CPP) -c src/blob.cpp
	mv *.o obj/

tracker:
	mkdir -p obj/
	g++ $(OPT) $(OPT_CPP) -c src/tracker.cpp
	mv *.o obj/

counter:
	mkdir -p obj/
	g++ $(OPT) $(OPT_CPP) -c src/counter.cpp
	mv *.o obj/

clean:
	rm -f obj/*.o
	rm -f bin/main

link:
	g++ $(OPT) $(INCLUDE_OPENCV) obj/*.o -o bin/main $(LIBS_OPENCV)

/*default: */
	/*gcc -std=c99 -O3 -Wall -Werror -pedantic -Wno-unused-function -Wno-unused-parameter -Wno-deprecated -Wno-deprecated-declarations -Wno-sign-compare -Wno-unused-but-set-parameter -c vibe-background-sequential.c*/
	/*gcc -o main -std=c99 -O3 -Wall -Werror -pedantic your-main-file-sequential.c vibe-background-sequential.o*/
	/*g++ -o main-opencv -O3 -Wall -Werror -pedantic $(INCLUDE_OPENCV) main-opencv.cpp vibe-background-sequential.o $(LIBS_OPENCV)*/
/*clean:*/
	/*rm -f obj/*.o*/

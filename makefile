all:
	g++ -c point.cpp
	g++ -c vector.cpp
	g++ -c courbe.cpp
	g++ -c main.cpp
	g++ -o TP1 main.o vector.o point.o courbe.o -lglut -lGL -lGLU 
	rm *.o
	./TP1
clean:
	rm *.o
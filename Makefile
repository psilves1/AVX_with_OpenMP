main: main.o length
	g++ -o main main.o -fopenmp

main.o:
	g++ -c main.cc -fopenmp

length: length.o
	g++ -o length length.o -mavx

length.o: length.cc
	g++ -c length.cc -mavx

clean:
	rm -rf main main.o length.o length

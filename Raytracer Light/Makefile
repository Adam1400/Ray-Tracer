OBJ = main.o
INC = -I "./"

trace: $(OBJ)
	g++ $(OBJ) -o trace.exe
	rm -f $(OBJ)

main.o:
	g++ -c main.cpp $(INC)

clean:
	rm -f $(OBJ) trace
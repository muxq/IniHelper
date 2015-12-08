OBJ=Test.o

ALL: $(OBJ)
	g++ -o mytest $(OBJ) -L ../Make/ -lIniHelper
Test.o: Test.cpp
	g++ -c Test.cpp -I ../IniHelper/
run:
	LD_LIBRARY_PATH=../Make/ ./mytest
clean:
	rm -f *.o test

OBJ= IniHelper.o dllmain.o
FILE_PATH=../IniHelper/
ALL: $(OBJ)
	g++ -fPIC -shared -o libIniHelper.so $(OBJ)
IniHelper.o: $(FILE_PATH)IniHelper.cpp
	g++ -c -fPIC $(FILE_PATH)IniHelper.cpp
dllmain.o: $(FILE_PATH)dllmain.cpp
	g++ -c -fPIC $(FILE_PATH)dllmain.cpp
clean:
	rm -f $(OBJ) *.so


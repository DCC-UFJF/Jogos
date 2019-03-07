#all:
#ifeq ($(shell uname -s), Linux)
#	g++ -o sfml-app *.cpp -lsfml-graphics -lsfml-window -lsfml-system
#	./sfml-app
#else
#	g++ -o sfml-app.exe *.cpp -I"C:\SFML-2.5.0\include" -L"C:\SFML-2.5.0\lib" -lsfml-graphics -lsfml-window -lsfml-system
#	sfml-app.exe
#endif

linux:
	g++ -o sfml-app *.cpp -lsfml-graphics -lsfml-window -lsfml-system
	./sfml-app

windows:
	g++ -o sfml-app.exe *.cpp -I"C:\SFML-2.5.0\include" -L"C:\SFML-2.5.0\lib" -lsfml-graphics -lsfml-window -lsfml-system
	sfml-app.exe

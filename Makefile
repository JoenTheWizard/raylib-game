INDEX=1

game:
	g++ game.cpp includes/*.cpp -o rayapp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

debug:
	g++ -g game.cpp includes/*.cpp -o rayapp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	valgrind -s --leak-check=full ./rayapp

tests:
	g++ test/test$(INDEX).cpp -o rayapp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

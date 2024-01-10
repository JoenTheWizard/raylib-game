INDEX=1

game:
	g++ game.cpp -o rayapp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

tests:
	g++ test/test$(INDEX).cpp -o rayapp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

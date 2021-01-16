all: nasciBuild

nasciBuild: src/main.cpp
	mkdir -p bin; g++ -O2 src/main.cpp -lncursesw -o bin/nasci


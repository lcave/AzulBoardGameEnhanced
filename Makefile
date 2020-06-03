.default : all

all: azul

clean:
	@echo "cleaning up..."
	rm *.o Azul

azul: Azul.o Factory.o GameEngine.o PlayerTree.o PlayerNode.o Line.o Menu.o Mosaic.o Node.o Player.o Saver.o TileList.o Utils.o
	g++ -Wall -Werror -std=c++14 -O -g -o Azul $^
	
%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -O -g -c $^
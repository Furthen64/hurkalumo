CC=g++
CFLAGS=-fpermissive
LIBS=-lsfml-graphics -lsfml-window -lsfml-system
OBJECTS=Core.o TextureManager.o Block.o Bus.o BusRoute.o GameMatrix.o Graph.o BinarySearchTree.o FileManager.o Link.o Node.o HPos.o Utils.o HRect.o HurkaMatrix.o HurkaMap.o Grid.o RoadNetwork.o SlotPos.o SlotPath.o Locomotive.o TrafficManager.o Toolbar.o

all:	Main

Main:	Main.o $(OBJECTS)
	$(CC) Main.o $(OBJECTS) -o Main $(LIBS)

Main.o: Main.cpp 
	$(CC) $(CFLAGS) -c Main.cpp 

Core.o: Core.cpp
	$(CC) $(CFLAGS) -c Core.cpp

TextureManager.o: TextureManager.cpp
	$(CC) $(CFLAGS) -c TextureManager.cpp 

Block.o: Block.cpp
	$(CC) $(CFLAGS) -c Block.cpp

Bus.o: Bus.cpp
	$(CC) $(CFLAGS) -c Bus.cpp

BusRoute.o: BusRoute.cpp
	$(CC) $(CFLAGS) -c BusRoute.cpp

GameMatrix.o: GameMatrix.cpp
	$(CC) $(CFLAGS) -c GameMatrix.cpp

Graph.o: Graph.cpp
	$(CC) $(CFLAGS) -c Graph.cpp

Grid.o: Grid.cpp
	$(CC) $(CFLAGS) -c Grid.cpp

HRect.o: HRect.cpp
	$(CC) $(CFLAGS) -c HRect.cpp

HurkaMap.o: HurkaMap.cpp
	$(CC) $(CFLAGS) -c HurkaMap.cpp

HurkaMatrix.o: HurkaMatrix.cpp
	$(CC) $(CFLAGS) -c HurkaMatrix.cpp

Locomotive.o: Locomotive.cpp
	$(CC) $(CFLAGS) -c Locomotive.cpp

RoadNetwork.o: RoadNetwork.cpp
	$(CC) $(CFLAGS) -c RoadNetwork.cpp

SlotPath.o: SlotPath.cpp
	$(CC) $(CFLAGS) -c SlotPath.cpp

SlotPos.o: SlotPos.cpp
	$(CC) $(CFLAGS) -c SlotPos.cpp

Toolbar.o: Toolbar.cpp
	$(CC) $(CFLAGS) -c Toolbar.cpp

TrafficManager.o: TrafficManager.cpp
	$(CC) $(CFLAGS) -c TrafficManager.cpp

Link.o: Link.cpp
	$(CC) $(CFLAGS) -c Link.cpp

Node.o: Node.cpp
	$(CC) $(CFLAGS) -c Node.cpp

FileManager.o: FileManager.cpp
	$(CC) $(CFLAGS) -c FileManager.cpp

BinarySearchTree.o: BinarySearchTree.cpp
	$(CC) $(CFLAGS) -c  BinarySearchTree.cpp

HPos.o: HPos.cpp
	$(CC) $(CFLAGS) -c HPos.cpp

Utils.o: Utils.cpp
	$(CC) $(CFLAGS) -c Utils.cpp 

clean:
	rm -f *.o main

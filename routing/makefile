Routing:	main.o Router.o Packet.o
	g++ -o Routing main.o Router.o Packet.o

Router.o:	Router.h Router.cpp
	g++ -c Router.cpp -o Router.o

Packet.o:	Packet.h Packet.cpp
	g++ -c Packet.cpp -o Packet.o

clean:		
	rm Routing *.o
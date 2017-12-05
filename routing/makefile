Router.o:	Router.h Router.cpp
	g++ -c Router.cpp -o Router.o

Packet.o:	Packet.h Packet.cpp
	g++ -c Packet.cpp -o Packet.o

Routing:	Routing.o Router.o Packet.o
	g++ -o Routing Routing.o Router.o Packet.o

clean:		
	rm Routing *.o
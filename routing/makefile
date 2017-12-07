Routing:	main.o Router.o Packet.o
	g++ -std=gnu++11 main.o Router.o Packet.o -o Routing

main.o:		main.cpp Router.h Router.cpp Packet.h Packet.cpp
	g++ -std=gnu++11 -c main.cpp

Router.o:	Router.h Router.cpp
	g++ -std=gnu++11 -c Router.cpp

Packet.o:	Packet.h Packet.cpp
	g++ -std=gnu++11 -c Packet.cpp

clean:		
	rm Routing *.o
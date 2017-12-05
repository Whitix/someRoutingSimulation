//The Router class symbolizes one of the many potential routing nodes in a mesh network. 


#include <queue>
#include <list>
#include<vector>
#include "Packet.h"
class Router{

private:
	int routerID = -1;	//The identification number for this router; when the mesh is built, no two nodes should share this number
	std::queue<Packet> routBuffer;  //Represents how large this router's buffer is
	std::vector<Router> connections;  //List of other routers this one is connected to
	int bufferSize;		//Size of this buffer
	double bandwidth = 1500000;  //Size of the network's bandwidth (in bytes)  Default: 1.5 megabytes
	double physicalLink = 2000;  //Size of the physical (or wireless link) between another router.  This isn't the most accurate way to represent a physical link
						//but should be sufficient to show the routing algorithm  (in meters)
	double propagationSpeed = 200000000000;  //Propagation speed (in meters per millisecond)  Default: 2e8 m/s
	double processingDelay = 0;  //Time it takes to process where to send the packet (in milliseconds) Default: 0s
	double transmissionDelay = 0;  //Time it takes to transmit packet data(packet length / link bandwidth) (in milliseconds) Default:0s, although this is calculated later
	double lossChance = 0;  //This chance is rolled every time a packet is sent or arrives at another router.  If the roll suceeds, then the packet is lost and must be retransmitted

	

public:

	Router();	//Default constructor; constructs an empty router
	Router(int newID, double bandw, int buffS, double processingD, double lossC, double physLink);  //Specialized constructor
	int getID();	//Returns the unique ID number for the node
	std::vector<Router> getConnections(); //Returns a list of the router's connections
	void printInfo(); //Prints basic variable information so I can debug
	void addConnection(Router newConnection);  //Adds a new connection to the node
	void updateConnection(Router newConnection, int dest); //Updates a connection at the destination specified 
	void setProcessDelay(double newDelay);
	void setPhysicalLink(double newLength, int packet);
	void setPropagationSpeed(double newSpeed, int packet);
	void setLossChance(double newLoss);
	void calculateTransDelay(int packetSize);  //Calculates the transmission delay based on the packet being transmitted
	double timeToReach(Router nextRouter);  //Calculates how fast a packet can be sent between this router and the next one
	bool findNextRouter();  //Searches its own list of connections, placing the router with the shortest throughput time
};
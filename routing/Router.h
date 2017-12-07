//The Router class symbolizes one of the many potential routing nodes in a mesh network. 


#include <queue>
#include <list>
#include<vector>
#include <climits>
#include "Packet.h"
#include<random> //Used for to decide if a packet is randomly lost
class Router{

private:
	int routerID = -1;	//The identification number for this router; when the mesh is built, no two nodes should share this number
	std::queue<Packet> routBuffer;  //Represents how large this router's buffer is
	std::vector<Router> connections;  //List of other routers this one is connected to
	int bufferSize;		//Size of this buffer
	double bandwidth;;  //Size of the network's bandwidth (in bytes)  Default: 1.5 megabytes
	double physicalLink;  //Size of the physical (or wireless link) between another router.  This isn't the most accurate way to represent a physical link
						  //but should be sufficient to show the routing algorithm  (in meters)
	double propagationSpeed;  //Propagation speed (in meters per millisecond)  Default: 2e8 m/s
	double processingDelay;  //Time it takes to process where to send the packet (in milliseconds) Default: 10ms
	double transmissionDelay;  //Time it takes to transmit packet data(packet length / link bandwidth) (in milliseconds) Default:0s, although this is calculated later
	double lossChance = 0;  //This chance is rolled every time a packet is sent or arrives at another router.  If the roll suceeds, then the packet is lost and must be retransmitted
	bool isFull;
	bool canRouteAround; //Determines whether a router can be routed around, or if it acts as a bridge.  If it can't be routed around, then the program must go through it

	

public:

	Router();	//Default constructor; constructs an empty router
	Router(int newID, double bandw, int buffS, double processingD, double lossC, double physLink);  //Specialized constructor
	int getID();	//Returns the unique ID number for the node
	std::vector<Router> getConnections(); //Returns a list of the router's connections
	void printInfo(); //Prints basic variable information so I can debug
	void addConnection(Router newConnection);  //Adds a new connection to the node
	void updateConnection(Router newConnection, int dest); //Updates a connection at the destination specified 
	void removeConnection(int connectionToRemove); //Removes a connection from the list of connections, used for rerouting
	void setProcessDelay(double newDelay);
	void setPhysicalLink(double newLength, int packet);
	void setPropagationSpeed(double newSpeed, int packet);
	void setLossChance(double newLoss);
	void setBuffer(bool full); //Sets the buffer to empty or full for testing
	void calculateTransDelay(int packetSize);  //Calculates the transmission delay based on the packet being transmitted
	double timeToReach(Router nextRouter);  //Calculates how fast a packet can be sent between this router and the next one
	int getBufferSize(); //returns buffer size
	double getProcessingDelay(); //returns processing delay
	bool findNextRouter();  //Used for debugging
	bool sendAckOrReq(double& time); //Used to simulate the process of routers communicating with one another.  Packet loss can occur here (If an ack or request isn't received); also updates the time spent
	bool isBufferFull(); //Returns whether or not the buffer is full
	bool canGoAround(); //Returns whether the router can be routed around
	void setRouteAround(bool bridge); //Set whether the router can be routed around

};
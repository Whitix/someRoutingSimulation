#include "Router.h"
#include<iostream>  //Used for output


//Default constructor used for null routers
//Null routers are assigned to every router when a connection does not exist
//This allows an adjacency matrix to be utilized and it took me too long to realize that
Router::Router()
{
	routerID = -1;
	physicalLink = INT_MAX; //Use this so that other routers will not try and connect to null routers
	processingDelay = INT_MAX; //Both of these values should ensure that these routers are never utilized for connections
	bandwidth = 1500000;
	propagationSpeed = 200000000000;
	isFull = true;
	transmissionDelay = 0;
	canRouteAround = false;
}

//Specialized constructor with all of the relevant information supplied (other than connections)
Router::Router(int newID, double bandw, int buffS, double processingD, double lossC, double physLink)
{
	routerID = newID;
	bandwidth = bandw;
	bufferSize = buffS;
	processingDelay = processingD;
	lossChance = lossC;
	physicalLink = physLink;
	isFull = false;
	propagationSpeed = 200000000000;
	transmissionDelay = 0;
	canRouteAround = false;
}
//Returns the router ID
//Input: none
//Output: The router's ID tag
int Router::getID()
{
	return routerID;
}

std::vector<Router> Router::getConnections()
{
	return connections;
}

//Used for debugging
void Router::printInfo()
{
	std::cout << "ID: " << routerID << "Bandwidth: " << bandwidth << "Chance of packet loss: "  << lossChance << std::endl;
}

//Establishes a link between two routers; best to add later once all of the routers have been created
//Input: A router node to connect to
//Output: none
void Router::addConnection(Router newConnection)
{
	connections.push_back(newConnection);
}


//Updates the connections list with the given router
//Inserts the new router and then deletes the old one (presumably a null router)
void Router::updateConnection(Router newConnection, int dest)
{
	connections.insert((connections.begin() + dest), newConnection); //Insert new connection before old connection
	connections.erase((connections.begin() + dest + 1)); //Delete old connection

}

//Removes a router connection, so that we can temporarily reroute a packet
//Places a null router at that spot instead
void Router::removeConnection(int connectionToRemove)
{
	Router nullrouter;
	connections.insert((connections.begin() + connectionToRemove), nullrouter);
	connections.erase((connections.begin() + connectionToRemove + 1));
}

//These are all basic set functions, used to change values during the simulation
void Router::setProcessDelay(double newDelay)
{
	processingDelay = newDelay;
}
void Router::setPhysicalLink(double newLength, int packet)
{
	physicalLink = newLength;
	this->calculateTransDelay(packet);
}
void Router::setPropagationSpeed(double newSpeed, int packet)
{
	propagationSpeed = newSpeed;
	this->calculateTransDelay(packet);
}
void Router::setLossChance(double newLoss)
{
	lossChance = newLoss;
}

void Router::setBuffer(bool full)
{
	isFull = full;
}

//Calculates the transmission delay based on the packets being sent and bandwidth
//Input:  The packet size in bytes
//Output: none
void Router::calculateTransDelay(int packetSize)
{
	transmissionDelay = packetSize / bandwidth;
}

//Calculates how quickly a packet can be sent between two routers, ignoring queueing and packet loss.
//Input: The router to connect to
//Output: The estimated delay for connecting to that router
double Router::timeToReach(Router nextRouter)
{
	double propagationDelay = (physicalLink + nextRouter.physicalLink) / propagationSpeed; //Total propagation distance between two routers
	return (propagationDelay + transmissionDelay);
}

int Router::getBufferSize()
{
	return bufferSize;
}

double Router::getProcessingDelay()
{
	return processingDelay;
}
//Used for debugging.  Prints out all of the router IDs in the connections list
bool Router::findNextRouter()
{
	if (connections.empty())
		return false;
	
	for (int i = 0; i < connections.size(); i++)
	{
		std::cout << connections[i].getID() << std::endl;
	}
	return true;
	
}

bool Router::sendAckOrReq(double& time)
{
	time += transmissionDelay;  //Simulates the time it would take for a router to message an adjacent router, adding the transmission delay to the packet's total time
	double target = ((double)rand()) / (RAND_MAX);  //Generates a random number between 0 and 1 to use as a target
	if (lossChance <= target)
	{
		return true; //Represents the chance that a transmission failed
	}
	return false; //Represents the chance that a transmission succeeded
}

bool Router::isBufferFull ()
{
	return isFull;
}

bool Router::canGoAround()
{
	return canRouteAround;
}

void Router::setRouteAround(bool bridge)
{
	canRouteAround = bridge;
}
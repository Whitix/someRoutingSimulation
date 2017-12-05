#include "Router.h"
#include<iostream>

//Default constructor used for null routers
//Null routers are assigned to every router when a connection does not exist
//This allows an adjacency matrix to be utilized and it took me too long to realize that
Router::Router()
{
	routerID = -1;
	physicalLink = INT_MAX; //Use this so that other routers will not try and connect to null routers
	processingDelay = INT_MAX; //Both of these values should ensure that these routers are never utilized for connections
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

void Router::updateConnection(Router newConnection, int dest)
{
	connections.insert((connections.begin() + dest), newConnection); //Insert new connection before old connection
	connections.erase((connections.begin() + dest + 1)); //Delete old connection

}

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
	return (propagationDelay + transmissionDelay + processingDelay);
}

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
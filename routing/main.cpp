#include <iostream>
#include "Router.h" //For the router class
#include <string>  //For user input
#include<list> //Used instead of a stack to get the ideal route path
#include <limits.h> //For defining infinity to be used with the routing algorithm

std::list<int> findShortestPath(int, int, std::vector< std::vector<double> >, double&);

int main()
{
	std::string userInput;
	std::vector<Router> mesh;  //Holds all of the nodes of the mesh network
	std::vector< std::vector<double> > meshDistances; //Holds all of the distances between routers (like graph edges)
	int packetSize = 512;  //Size of packets to be used during the simulation
	int bufferSize = 10; //How many packets a node can hold at once.  This is a small number for observation purporses
	int tempNum = -1;  //To be used with input
	int tempNum2 = -1; //To be used with input.  I know I could resuse, and I'll clean up later
	double bandwidth = 1500000;  //Bandwidth over the network
	double randomPacketLoss = .05;  //Chance that a packet may be randomly lost over the network
	double processingDelay = .05;  //Time it takes to process a request at each router
	double distanceDelay = 0;
	double totalDelay = 0; //Will be used to track a single packet's total time spent from its starting router to the destination router
	double totalDelayCopy = 0; //Will be used for the second iteration, so we don't need to run the algorithm again
	int numberOfLosses = 0; //Indicates when a packet was lost

	std::cout << "This is where we'll simulate the routing algorithm.  First, we need to create a mesh network to test it on." << std::endl;
	std::cout << "The default network is configured like the example given in the topics slides.  All of the routers have the same characteristics." << std::endl;
	std::cout << "This network contains 16 nodes.  Currently, only the physical link sizes are different." << std::endl;
	std::cout << "Use default settings for routers and packets? y/n: ";
	std::cin >> userInput;
	if (!userInput.compare("n"))
	{
		std::cout << "Enter the packet size (in bytes) for all of the packets being sent over the network (Default is 512 bytes): ";
		std::cin >> packetSize;
		std::cout << "Enter the bandwidth (in bytes) to be used with the network (Default is 1.5 megabytes): ";
		std::cin >> bandwidth;
		std::cout << "Enter the percent chance that a packet is randomly lost over the network (default is  5% or .05): ";
		std::cin >> randomPacketLoss;
		std::cout << "Enter how long it takes to process a request at the local router (Default is .05 milliseconds): ";
		std::cin >> processingDelay;
		std::cout << "Enter the size of the buffer for each router (Default is 10): ";
		std::cin >> bufferSize;
		userInput = "";

	}

	//Builds the mesh network, using the given values to build the routers.  Will probably change this later, as it is a bit messy.
	//Also, adds in physical link distances for each router.  The physical distance between two routers is the sum of both of their link distances.
	//The physical link is equal to: (500 + (router's ID * 1000))
	for (int i = 0; i <= 15; i++)
	{
		Router newNode(i, bandwidth, bufferSize, processingDelay, randomPacketLoss, ((i * 1000) + 500));
		newNode.calculateTransDelay(packetSize);
		mesh.push_back(newNode);
	}
	Router nullRouter; //Represents an empty connection.
	for (int i = 0; i < mesh.size(); i++)
	{
		for (int j = 0; j < mesh.size(); j++)
		{
			mesh[i].addConnection(nullRouter);  //Adds 16 empty connections to the router.  These will be updated and used in the adjacency matrix
		}
	}
	//Assigns connections for the nodes.  Yes this is messy and no I dont care right now
	//These connections are updated, such that every vector will still have 16 connections, although these are not all utilized
	mesh[0].updateConnection(mesh[1], 1);
	mesh[1].updateConnection(mesh[0], 0);
	mesh[1].updateConnection(mesh[2], 2);
	mesh[1].updateConnection(mesh[4], 4);
	mesh[2].updateConnection(mesh[1], 1);
	mesh[2].updateConnection(mesh[3], 3);
	mesh[3].updateConnection(mesh[2], 2);
	mesh[3].updateConnection(mesh[4], 4);
	mesh[3].updateConnection(mesh[13], 13);
	mesh[4].updateConnection(mesh[1], 1);
	mesh[4].updateConnection(mesh[3], 3);
	mesh[4].updateConnection(mesh[5], 5);
	mesh[4].updateConnection(mesh[12], 12);
	mesh[5].updateConnection(mesh[4], 4);
	mesh[5].updateConnection(mesh[10], 10);
	mesh[5].updateConnection(mesh[11], 11);
	mesh[6].updateConnection(mesh[9], 9);
	mesh[6].updateConnection(mesh[10], 10);
	mesh[6].updateConnection(mesh[14], 14);
	mesh[7].updateConnection(mesh[8], 8);
	mesh[7].updateConnection(mesh[9], 9);
	mesh[8].updateConnection(mesh[7], 7);
	mesh[8].updateConnection(mesh[9], 9);
	mesh[8].updateConnection(mesh[15], 15);
	mesh[9].updateConnection(mesh[6], 6);
	mesh[9].updateConnection(mesh[7], 7);
	mesh[9].updateConnection(mesh[8], 8);
	mesh[9].updateConnection(mesh[14], 14);
	mesh[9].updateConnection(mesh[15], 15);
	mesh[10].updateConnection(mesh[5], 5);
	mesh[10].updateConnection(mesh[6], 6);
	mesh[11].updateConnection(mesh[5], 5);
	mesh[11].updateConnection(mesh[12], 12);
	mesh[12].updateConnection(mesh[4], 4);
	mesh[12].updateConnection(mesh[11], 11);
	mesh[13].updateConnection(mesh[3], 3);
	mesh[14].updateConnection(mesh[6], 6);
	mesh[14].updateConnection(mesh[9], 9);
	mesh[14].updateConnection(mesh[15], 15);
	mesh[15].updateConnection(mesh[8], 8);
	mesh[15].updateConnection(mesh[9], 9);
	mesh[15].updateConnection(mesh[14], 14);


	//Allows users to (slowly) edit single nodes to test further
	userInput = "";
	std::cout << "Edit individual router characteristics? y/n: ";
	std::cin >> userInput;
	while (!userInput.compare("y"))
	{
		std::cout << "Enter a router number from 0 to 15: ";
		std::cin >> tempNum;
		std::cout << "Enter the router's new processing delay(in milliseconds): ";
		std::cin >> tempNum2;
		mesh[tempNum].setProcessDelay(tempNum2);
		std::cout << "Enter the router's new physical link size (in meters): ";
		std::cin >> tempNum2;
		mesh[tempNum].setPhysicalLink(tempNum2, packetSize);
		std::cout << "Enter the router's new propagation speed (in bytes): ";
		std::cin >> tempNum2;
		mesh[tempNum].setPropagationSpeed(tempNum2, packetSize);
		std::cout << "Enter the router's new packet loss chance (from 0 to 1): ";
		std::cin >> tempNum2;
		mesh[tempNum].setLossChance(tempNum2);
		std::cout << "Enter another connection to the router (from 0 to 15): ";
		std::cin >> tempNum2;
		mesh[tempNum].updateConnection(mesh[tempNum2], tempNum2);
		std::cout << "Edit another router? y/n: ";
		std::cin >> userInput;
	}

	//Calculate the distance between connections and place them all in an adjaceny matrix
	//Might take a second to get through 256 connections

	for (int i = 0; i < mesh.size(); i++)
	{
		int connSize = (int)mesh[i].getConnections().size();
		meshDistances.push_back(std::vector<double>());
		for (int j = 0; j < connSize; j++)
		{
			distanceDelay = mesh[i].timeToReach(mesh[i].getConnections()[j]);  //Calculates the distance between a router i, and its connections
			meshDistances[i].push_back(distanceDelay);
		}
	}
	//Was used for debugging, but we're good now
	/*for (int i = 0; i < meshDistances.size(); i++)
	{
		std::cout << "Router Adjacencies for router " << i << ":" << std::endl;
		for (int j = 0; j < meshDistances[i].size(); j++)
		std::cout << meshDistances[i][j] << std::endl;
		std::cout << "Size: " << meshDistances[i].size() << std::endl;
	}*/
	while (tempNum < 0 || tempNum > 15)
	{
		std::cout << "Let's send a single packet.  Enter a starting router ID (from 0 to 15): ";
		std::cin >> tempNum;
	}
	while (tempNum2 < 0 || tempNum2 > 15)
	{
		std::cout << "Now enter a destination router: ";
		std::cin >> tempNum2;
	}

	//The source and destination routers determine whether a router can be routed around.  We'll update those routers here based on tempNum and tempNum2


	std::cout << "Sending packet..." << std::endl;
	//std::queue<Router> packetPath = calcShortestPath(mesh, tempNum, tempNum2);
	std::list<int> packetPath = findShortestPath(tempNum, tempNum2, meshDistances, totalDelay); //We'll print this out to show that the basic routing worked
	std::list<int> packetPathCopy = packetPath;  //We'll use this in the next stage, where we test the routing algorithm with full routers
	std::cout << "Here's the router path the packet will take starting from router " << tempNum <<std::endl;
	totalDelayCopy = totalDelay;
	//Print the packet path of routers
	//Also, updates the total time that the packet spends inbetween each router
	//Packet has a chance to be lost based on the packet loss chance
	while (!packetPath.empty())
	{
		std::cout << "Moving to router " << packetPath.front() << std::endl; //Print router
		while (true)//This is where the losses will be calculated; if the packet loss chance is 1, then this will loop forever.
		{
			totalDelay += mesh[packetPathCopy.front()].getProcessingDelay();
			if (mesh[packetPath.front()].sendAckOrReq(totalDelay)) //Represents a request
			{
				if (mesh[packetPath.front()].sendAckOrReq(totalDelay)) //Represents an acknowledge
				{
					if (packetPath.front() == tempNum2) //If the current router equals the destination router, don't print)
					{
						break; //break early so we don't get an out of bounds
					}
					std::cout << "Request and acknowledge received between routers adjacent routers" << std::endl;
					break; //If both transmissions are successful, then we're done and can move on.  Otherwise, we'll need to try again.  Delay time is added each time
				}
			}
			numberOfLosses++; //Indicates that some loss occurred along the way
		}
		packetPath.pop_front(); //Pop the front, allowing the next router to be examined
	}
	std::cout << "And here is the total time that packet took through the network (in milliseconds): " << totalDelay << std::endl; 
	std::cout << "There were a total of " << numberOfLosses << " packet losses."  << std::endl;
	numberOfLosses = 0;
	std::cout << "Now, let's do the same thing, except this time, some routing buffers can be full." << std::endl;
	std::cout << "If a packet reaches a full buffer, then it will be lost, requiring it to be re-sent.  Also, some packets may be lost randomly as well." << std::endl;
	userInput = "";
	std::cout << "Set a router to have a full buffer? y/n: ";
	std::cin >> userInput;
	int fullNum; //We'll use this to determine which routers are full, as designated by the user
	while (!userInput.compare("y"))
	{
		std::cout << "Enter a router number to set as full: ";
		std::cin >> fullNum;
		mesh[fullNum].setBuffer(true); //Sets a router to be full and routed around
		std::cout << "Choose another router to set as full (Too many full routers may cause errors) y/n: ";
		std::cin >> userInput;
	}
	//Alright, let's give this a shot
	//In this case, if the starting router is full, we'll ignore that.  If the ending router is full, we'll ignore that too.
	std::cout << "Packet begins at router " << tempNum << std::endl; //Do this in case the routers are directly adjacent to one another
	int prevNum = -1;  //This represents the previous router, we'll go back to this one in case we need to reroute
	while (!packetPathCopy.empty())
	{
		std::cout << "Moving to router " << packetPathCopy.front() << std::endl;
		if (mesh[packetPathCopy.front()].isBufferFull()) //Checks to see if the router along the path is full
		{
			std::cout << "Router " << packetPathCopy.front() << " is full.  Rerouting around it by going to router " << prevNum << std::endl;
			//If the router is full, then we'll route around it. To do this, we'll remove it as a connection from its adjacent routers.  Then, we'll try to go around it.
			//If a router is a bridge, then it may not be possible to go around.  In this case, we'll just need to wait for the buffer to not be full
			try
			{
				int holder = packetPathCopy.front(); //This variable will hold the next router temporarily, so that it may be removed and re-added
				mesh[tempNum].removeConnection(holder); //The current router, signified by tempNum, removes the next router in the path from its connections

				for (int i = 0; i < mesh.size(); i++)//Loop through the adjacency matrix, removing all connections to this router, so it isn't used
				{
					meshDistances[i].insert((meshDistances[i].begin() + holder), INT_MAX); //Update the adjacency matrix as well; this should ensure that the path is not taken when we move to calculate a new path
					meshDistances[i].erase((meshDistances[i].begin() + holder + 1));
				}

				if (prevNum == -1) //If there is no previous router, then we must wait for the buffer to be full
				{
					std::cout << "Could not reroute packet.  Waiting for buffer instead." << std::endl;
					totalDelayCopy += (mesh[packetPathCopy.front()].getBufferSize() * mesh[packetPathCopy.front()].getProcessingDelay());
				}
				else
				{
					packetPathCopy = findShortestPath(prevNum, tempNum2, meshDistances, totalDelayCopy); //If this works, then we'll use this for our new path
				}
				
				mesh[tempNum].updateConnection(mesh[holder], holder); //Re-add the router back to the connections list

				for (int i = 0; i < mesh.size(); i++)//Loop through the adjacency matrix, restoring all connections to this router
				{
					meshDistances[i].insert((meshDistances[i].begin() + holder), holder); 
					meshDistances[i].erase((meshDistances[i].begin() + holder + 1));
				}
			}
			catch (const std::out_of_range)  //An out of range exception signifies that a new path could not be found.  In this case, we'll just wait for the router instead
			{
				std::cout << "Could not reroute packet.  Waiting for buffer instead." << std::endl;
				totalDelayCopy += (mesh[packetPathCopy.front()].getBufferSize() * mesh[packetPathCopy.front()].getProcessingDelay()); 
				//The delay here is the buffer size of the next router multiplied by the time it takes to process at that router
			}
		}
		while (true)//This is where the losses will be calculated; if the packet loss chance is 1, then this will loop forever.
		{
			totalDelayCopy += mesh[packetPathCopy.front()].getProcessingDelay(); //Add the time taken to process the packet to the total delay
			if (mesh[packetPathCopy.front()].sendAckOrReq(totalDelayCopy)) //Represents a request
			{
				if (mesh[packetPathCopy.front()].sendAckOrReq(totalDelay)) //Represents an acknowledge
				{
					if (packetPathCopy.front() == tempNum2) //If the current router equals the destination router, don't print)
					{
						break; //break early so we don't get an out of bounds
					}
					std::cout << "Request and acknowledge received between routers adjacent routers" << std::endl;
					break; //If both transmissions are successful, then we're done and can move on.  Otherwise, we'll need to try again.  Delay time is added each time
				}
			}
			numberOfLosses++;
		}
		prevNum = tempNum; //Set the previous router 
		tempNum = packetPathCopy.front(); //Update tempNum to hold the current router
		packetPathCopy.pop_front(); //Pop the front and move on to the next one in the list
	}
	std::cout << "Packet reached destination, with a delay of " << totalDelayCopy << " milliseconds." << std::endl;
	std::cout << "There were a total of " << numberOfLosses << " packet losses." << std::endl;
	std::cout << "Enter any key to close the program" << std::endl;
	std::cin >> userInput;
	return 0;
}



//Calculates the path a packet should take to maxmize throughput based on Dijkstra's algorithm
//Does NOT account for queue processing, loss, or overloaded buffers.  Those are dealt with when the path is being executed
//Input: The mesh network, a starting node, and a destination node
//Output: A list outlining the shortest path from A to B
std::list<int> findShortestPath(int startID, int dest, std::vector< std::vector<double> > adj, double& time) 
{
	int adjSize = (int) adj.size();
	int map[16]; //Will store the path used to get the shortest distance  I kept getting some weird memory error with visual studio when using a dynamically allocated array, so we'll use this instead
	std::vector<double> dist(adjSize);
	std::vector<bool> vis(adjSize);
	std::list<int> finalRoute;

	for (int i = 0; i < adjSize; i++)   //Initialize the unvisited distances as infinite and the visited vertices as zero
	{
		dist[i] = INT_MAX;
		vis[i] = false;
	}
	dist[startID] = 0;
	for (int i = 0; i < adjSize; i++)
	{
		int cur = -1;
		for (int j = 0; j < adjSize; j++) //Will loop through the visited vector
		{
			if (vis[j])  //Finds the next vertice that hasn't been visited
			{
				continue;  //Skip the next part if the vertice has been visited and loop through again
			}
			if (cur == -1 || dist[j] < dist[cur])
			{
				cur = j; 
			}
		}
		vis[cur] = true;
		for (int j = 0; j < adjSize; j++)
		{
			double path = dist[cur] + adj[cur][j];
			if (path < dist[j])
			{
				dist[j] = path;
				map[j] = cur; //Add this router to the map
			}
		}
	}
	int iterator = dest;
	while (iterator != startID)
	{
		//std::cout << "Router " << iterator << std::endl;
		finalRoute.push_front(iterator);  //Pushes the router onto the front (like a stack) to be popped later during the list
		iterator = map[iterator];
	}
	time += dist[dest];  //Used to track the total time between the packet spends while on its destination path.  Does NOT account for queueing delays
	return finalRoute;
}

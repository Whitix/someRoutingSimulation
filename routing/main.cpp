#include <iostream>
#include "Router.h" //For the router class
#include <string>  //For user input
#include <limits.h> //For defining infinity to be used with the routing algorithm

std::queue<Router> calcShortestPath(std::vector<Router>, int, int); //Will return an ideal queue for a packet to traverse the network
int findShortestConnection(Router, bool*);  //Finds the shortest connection from a router to all of its connections
double dijk(int, int, std::vector<std::vector<double>>);

int main()
{
	std::string userInput;
	std::vector<Router> mesh;  //Holds all of the nodes of the mesh network
	std::vector<std::vector<double>> meshDistances; //Holds all of the distances between routers (like graph edges)
	int packetSize = 512;  //Size of packets to be used during the simulation
	int bufferSize = 10; //How many packets a node can hold at once.  This is a small number for observation purporses
	int tempNum;  //To be used with input
	int tempNum2; //To be used with input.  I know I could resuse, and I'll clean up later
	double bandwidth = 1500000;  //Bandwidth over the network
	double randomPacketLoss = .01;  //Chance that a packet may be randomly lost over the network
	double processingDelay = 0;  //Time it takes to process a request at each router
	double distanceDelay = 0;

	std::cout << "This is where we'll simulate the routing algorithm.  First, we need to create a mesh network to test it on."  << std::endl;
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
		std::cout << "Enter the percent chance that a packet is randomly lost over the network (default is  1% or .01): ";
		std::cin >> randomPacketLoss;
		std::cout << "Enter how long it takes to process a request at the local router (Default is 0 milliseconds): ";
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
		Router newNode(i, bandwidth, bufferSize, processingDelay, randomPacketLoss, ((i *1000) + 500));
		newNode.calculateTransDelay(packetSize);
		mesh.push_back(newNode);
	}
	//Assigns connections for the nodes.  Yes this is messy and no I dont care right now
	mesh[0].addConnection(mesh[1]);
	mesh[1].addConnection(mesh[0]);
	mesh[1].addConnection(mesh[2]);
	mesh[1].addConnection(mesh[4]);
	mesh[2].addConnection(mesh[1]);
	mesh[2].addConnection(mesh[3]);
	mesh[3].addConnection(mesh[2]);
	mesh[3].addConnection(mesh[4]);
	mesh[3].addConnection(mesh[13]);
	mesh[4].addConnection(mesh[1]);
	mesh[4].addConnection(mesh[3]);
	mesh[4].addConnection(mesh[5]);
	mesh[4].addConnection(mesh[12]);
	mesh[5].addConnection(mesh[4]);
	mesh[5].addConnection(mesh[10]);
	mesh[5].addConnection(mesh[11]);
	mesh[6].addConnection(mesh[9]);
	mesh[6].addConnection(mesh[10]);
	mesh[6].addConnection(mesh[14]);
	mesh[7].addConnection(mesh[8]);
	mesh[7].addConnection(mesh[9]);
	mesh[8].addConnection(mesh[7]);
	mesh[8].addConnection(mesh[9]);
	mesh[8].addConnection(mesh[15]);
	mesh[9].addConnection(mesh[6]);
	mesh[9].addConnection(mesh[7]);
	mesh[9].addConnection(mesh[8]);
	mesh[9].addConnection(mesh[14]);
	mesh[9].addConnection(mesh[15]);
	mesh[10].addConnection(mesh[5]);
	mesh[10].addConnection(mesh[6]);
	mesh[11].addConnection(mesh[5]);
	mesh[11].addConnection(mesh[12]);
	mesh[12].addConnection(mesh[4]);
	mesh[12].addConnection(mesh[11]);
	mesh[13].addConnection(mesh[3]);
	mesh[14].addConnection(mesh[6]);
	mesh[14].addConnection(mesh[9]);
	mesh[14].addConnection(mesh[15]);
	mesh[15].addConnection(mesh[8]);
	mesh[15].addConnection(mesh[9]);
	mesh[15].addConnection(mesh[14]);

	//Allows users to (slowly) edit single nodes to test further
	userInput = "";
	std::cout << "Edit individual router characteristics? y/n: ";
	std::cin >> userInput;
	while (!userInput.compare("y"))
	{
		std::cout << "Enter a router number from 0 to 15: ";
		std::cin >> tempNum;
		std::cout << "Enter the router's new processing delay: ";
		std::cin >> tempNum2;
		mesh[tempNum].setProcessDelay(tempNum2);
		std::cout << "Enter the router's new physical link size: ";
		std::cin >> tempNum2;
		mesh[tempNum].setPhysicalLink(tempNum2, packetSize);
		std::cout << "Enter the router's new propagation speed: ";
		std::cin >> tempNum2;
		mesh[tempNum].setPropagationSpeed(tempNum2, packetSize);
		std::cout << "Enter the router's new packet loss chance: ";
		std::cin >> tempNum2;
		mesh[tempNum2].setLossChance(tempNum2);
		std::cout << "Edit another router? y/n: ";
		std::cin >> userInput;
	}

	//Calculate the distance between connections and place them all in an adjaceny matrix
	for (int i = 0; i < mesh.size(); i++)
	{
		int connSize = mesh[i].getConnections().size();
		meshDistances.push_back(std::vector<double>());
		for (int j = 0; j < connSize; j++)
		{
			distanceDelay = mesh[i].timeToReach(mesh[i].getConnections()[j]);  //Calculates the distance between a router i, and its connections
			meshDistances[i].push_back(distanceDelay);
		}
	}
	for (int i = 0; i < meshDistances.size(); i++)
	{
		std::cout << "Router Adjacencies for router " << i << ":" << std::endl;
		for (int j = 0; j < meshDistances[i].size(); j++)
		std::cout << meshDistances[i][j] << std::endl;
		std::cout << "Size: " << meshDistances[i].size() << std::endl;
	}

	std::cout << "Let's send a single packet.  Enter a starting router ID: ";
	std::cin >> tempNum;
	std::cout << "Now enter a destination router: ";
	std::cin >> tempNum2;
	std::cout << "Sending packet..." << std::endl;
	//std::queue<Router> packetPath = calcShortestPath(mesh, tempNum, tempNum2);
	double pathLength = dijk(tempNum, tempNum2, meshDistances);
	std::cout <<"Here is packet solution " << pathLength << std::endl;


	std::cin >> userInput;
	return 0;
}

//Calculates the path a packet should take to maxmize throughput based on Dijkstra's algorithm
//Does NOT account for queue processing, loss, or overloaded buffers.  Those are dealt with when the path is being executed
//Input: The mesh network, a starting node, and a destination node
//Output: A queue outlining the shortest path from A to B
std::queue<Router> calcShortestPath(std::vector<Router> network, int startID, int destID)
{
	std::queue<Router> path; //Output path.  Will hold the routers for the packet to pass through
	int size = network.size();
	bool *marks = new bool[size]; //Will mark will vertices have been visited.
	double *dist = new double[size]; //Will hold the total distance of the path
	for (int i = 0; i < size; i++)
	{
		dist[i] = INT_MAX; //Set unvisited distances to infinity
		marks[i] = false; //Set all vertices as unvisited
	}
	dist[startID] = 0;
	for (int iteration = 0; iteration < size - 1; iteration++) //Searches for the minimum distance
	{
		int pickedVertice = findShortestConnection(network[iteration], marks);  //Choose the minimum distance based on the given router
		marks[pickedVertice] = true;  //Mark that the vertice has been visited
	}
	
	return path;
}

int findShortestConnection(Router rout, bool *marks)
{
	std::vector<Router> connecs = rout.getConnections();
	double min = INT_MAX;
	int nearest;  //Represents the router ID tag to go through
	for (int i = 0; i < connecs.size(); i++) //Loops through all of the nearby connections to find the one where routing will be quickest
	{
		if (!marks[i] && rout.timeToReach(connecs[i]) <= min) //Check if the vertice has been visited and whether it is shorter than the rest
		{
			nearest = i;
			min = rout.timeToReach(connecs[i]);
		}
	}
	return nearest;  //Return the closest router's ID
}


double dijk(int A, int B, std::vector<std::vector<double>> adj) {
	int n = adj.size();
	std::vector<double> dist(n);
	std::vector<bool> vis(n);
	for (int i = 0; i < n; i++) 
	{
		dist[i] = INT_MAX;
		vis[i] = false;
	}
	dist[A] = 0;
	for (int i = 0; i < n; i++) 
	{
		int cur = -1;
		for (int j = 0; j < n; j++)
		{
			if (vis[j]) continue;
			if (cur == -1 || dist[j] < dist[cur])
			{
				cur = j;
			}
		}
		vis[cur] = true;
		for (int j = 0; j < adj[cur].size(); j++)
		{
			double path = dist[cur] + adj[cur][j];
			if (path < dist[j])
			{
				dist[j] = path;
			}
			std::cout << cur << std::endl;
			std::cout << j << std::endl;
			std::cout << path << std::endl;
			std::cout << dist[j] << std::endl;

			
		}
	}

	return dist[B];
}
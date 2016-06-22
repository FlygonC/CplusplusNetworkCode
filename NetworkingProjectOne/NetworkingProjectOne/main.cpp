#pragma once

#include <iostream>
#include <time.h>

#include "NetCode.h"


time_t timer;
double currentTime = 0;
double lastTime = 0;
void startTime()
{
	currentTime = time(&timer);
	lastTime = time(&timer);
}
double deltaTime()
{
	currentTime = time(&timer);
	double delta = currentTime - lastTime;
	lastTime = currentTime;
	return delta;
}


int main(int cargs, char *vargs[])
{
	// Start Net
	NetCode::StartNetwork();
	// Open a socket on a port
	NetCode::Socket mySocket;
	mySocket.open(50000);
	// Define addresses
	sockaddr_in outAddress = NetCode::StringToAddress("10.15.22.54:50000");
	sockaddr_in inAddress;
	// Buffer data to send and recieve
	char outPack[256] = "HOTDOGS!HOTDOGS!HOTDOGS!HOTDOGS!HOTDOGS";
	char inPack[256] = "";

	//startTime();
	//double waitTime = 0;
	while (true)
	{
		/*if (waitTime >= 10)
		{
			mySocket.send(outPack, 256, outAddress);
			waitTime -= 10;
		}

		waitTime += deltaTime();*/
		std::cin.getline( outPack, 256);
		// send data
		mySocket.send(outPack, 256, outAddress);
	}
	// get data
	mySocket.recieve(inPack, 256, inAddress);
	
	std::cout << inPack << std::endl;
	//close socket
	mySocket.close();
	//stop net
	NetCode::CloseNetwork();
	system("pause");

	return 0;
}

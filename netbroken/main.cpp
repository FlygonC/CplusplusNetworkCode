#include "game\Game.h"

#include "netbroke.h"
#include <iostream>
#include <vector>
#include <unordered_map>

void main()
{
    sfwl::initContext();
    nsfw::initNetworking();

	bool server = false;

	Player localPlayer;
	std::vector<Ship> ships;
	std::vector<ShipInput> inputs;
	std::unordered_map<unsigned long, size_t> connections;

	ships.emplace_back();
	inputs.emplace_back();

    ships[0].color = BLUE;
	//ship3.color = GREEN;

	nsfw::Socket mySocket;
	mySocket.open(50000);
	nsfw::Address inAddress, outAddress(10, 15, 22, 54, 50000);

	struct PACKET { unsigned long keyIP; float time; ShipInput input; Ship state; };
	PACKET outData;
	PACKET inData;

    while (sfwl::stepContext())
    {
        float dt = sfwl::getDeltaTime();

		inputs[0] = localPlayer.getInput(ships[0]);

		for (int i = 0; i < ships.size(); i++)
		{
			ships[i].update(dt, inputs[i]);
			ships[i].draw();
		}

        Particle::updateAll(dt);
        Particle::drawAll();

		//outData = { sfwl::getTime(), input1, ship1 };
		if (!server)
		{
			outData = { 0, sfwl::getTime(), inputs[0], ships[0] };
			mySocket.send((char*)&outData, sizeof(PACKET), outAddress);
		}

		mySocket.send((char*)&outData, sizeof(PACKET), outAddress);
		
		float latest = 0;
		while (mySocket.recv((char*)&inData, sizeof(PACKET), inAddress))
		{
			//mySocket.send((char*)&outData, sizeof(PACKET), inAddress);
			unsigned long tIP = server ? inAddress.getIPH() : inData.keyIP;

			if (connections.count(inAddress.getIPH()) == 0)
			{
				connections[tIP] = ships.size();
				ships.emplace_back(inData.state);
				inputs.emplace_back();
			}

			size_t current = connections[tIP];
			inputs[current] = inData.input;
			ships[current].lerpPhysics(inData.state, 0.1f);

			if (server)
			{
				for each (auto c in connections)
				{
					if (c.first != tIP)
					{
						outData = { c.first, sfwl::getTime(), inputs[c.second], ships[c.second] };
						mySocket.send((char*)&outData, sizeof(PACKET), inAddress);
					}
				}
			}
		}
    }

    sfwl::termContext();
    nsfw::termNetworking();
}
#pragma once

#pragma comment( lib, "wsock32.lib" )
#pragma comment( lib, "ws2_32.lib" )

#include <WinSock2.h>
#include <WS2tcpip.h>

namespace NetCode
{

	WSAData data;

	inline bool StartNetwork()
	{
		//Startup
		return WSAStartup(MAKEWORD(2, 2), &data) == 0;
	}
	inline bool CloseNetwork()
	{
		//Shutdown
		return WSACleanup() == 0;
	}
	/*bool NetIsRunning()
	{
		return data != NULL;
	}*/

	// 127.0.0.1:50000 Localhost
	inline sockaddr_in StringToAddress(const char* src)
	{
		union { char values[4]; unsigned ui; } IP;
		char values[4];
		unsigned short port;
		const char* ptr = src;
		// atoi(string to number) gets number, strcspn + 1 moves to next number
		IP.values[0] = atoi(ptr); ptr += strcspn(ptr, ".") + 1;
		IP.values[1] = atoi(ptr); ptr += strcspn(ptr, ".") + 1;
		IP.values[2] = atoi(ptr); ptr += strcspn(ptr, ".") + 1;
		IP.values[3] = atoi(ptr); ptr += strcspn(ptr, ":") + 1;
		port = atoi(ptr);

		sockaddr_in dest;
		dest.sin_family = AF_INET;
		dest.sin_addr.S_un.S_addr = IP.ui;
		dest.sin_port = htons(port);

		return dest;
	}

	class Socket
	{
		int handle;
	public:
		Socket() {}
		~Socket() { close(); }

		bool isOpen() const { return handle != 0; }

		bool open(unsigned port = 0)
		{
			handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = INADDR_ANY;
			addr.sin_port = htons(port);
			bind(handle, (sockaddr*)&addr, sizeof(sockaddr_in));
			DWORD param = 1;
			ioctlsocket(handle, FIONBIO, &param);
			return true;
		}
		bool close()
		{
			if (!isOpen()) return false;
			closesocket(handle);
			handle = 0;
			return true;
		}

		// Packet(data, data length), Address
		bool send(const char* data, const int length, const sockaddr_in &address)
		{
			return sendto(handle, data, length, 0, (sockaddr*)&(address), sizeof(sockaddr_in)) > 0;
		}
		bool recieve(char* data, int length, sockaddr_in &address)
		{
			int inLength = sizeof(sockaddr_in);
			int bytes = recvfrom(handle, data, length, 0, (sockaddr*)&address, &inLength);
			return bytes == length;
		}
	};

	/*class Address
	{

	};*/
}
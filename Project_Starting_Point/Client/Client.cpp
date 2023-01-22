#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

unsigned int GetSize();

int main()
{
	WSADATA wsaData;
	SOCKET ClientSocket;   
	sockaddr_in SvrAddr;  
	unsigned int uiSize = 0;
	vector<string> ParamNames;
	char Rx[128];

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_port = htons(27001);
	SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)); // connecting client with server

	uiSize = GetSize(); // calling in-home function which returns size O(n)
	for (unsigned int l = 0; l < uiSize; l++)  //O(n)
	{
		string strInput;
		ifstream ifs("DataFile.txt"); // opens ifstream object on File 
		for (unsigned int iStart = 0; iStart < l; iStart++) // O(n) // this is costly and time consuming as it really will make it O(n^2) efficient
			getline(ifs, strInput);

		getline(ifs, strInput); 
		if (l > 0) // if it's not the first line there are numbers of data following
		{
			size_t offset, preOffset;
			offset = preOffset = 0;
			unsigned int iParamIndex = 0;
			//while (offset != std::string::npos)
			while(iParamIndex != 8) // go through until all parameters are calculated
			{
				offset = strInput.find_first_of(',', preOffset+1);
				string strTx = strInput.substr(preOffset+1, offset - (preOffset+1)); // this gets the string reading of the value: number 
				send(ClientSocket, ParamNames[iParamIndex].c_str(), (int)ParamNames[iParamIndex].length(), 0); // this sends to server the name of parameter, as well as its length
				recv(ClientSocket, Rx, sizeof(Rx), 0);
				send(ClientSocket, strTx.c_str(), (int)strTx.length(), 0); // this now sends the string object that contains the number as a c string, passing its length as well
				recv(ClientSocket, Rx, sizeof(Rx), 0);
				cout << ParamNames[iParamIndex] << " Avg: " << Rx << endl;
				preOffset = offset;
				iParamIndex++; // increment
			}
		}
		else 
		// this is the first line which means the parameters are showing
		{
			// goes through as many delimitters(,) to pushback as many parameters until it hits end of the line
			ParamNames.push_back("TIME STAMP");
			size_t offset, preOffset;
			offset = 0;
			preOffset = -1;
			while (offset != std::string::npos)
			{
				offset = strInput.find_first_of(',', preOffset + 1);
				string newParam = strInput.substr(preOffset + 1, offset - (preOffset + 1));
				ParamNames.push_back(newParam);
				preOffset = offset;
			}
		}
		ifs.close();
	}

	closesocket(ClientSocket);
	WSACleanup();

	return 1;
}


// returns how many lines there are in the DataFile.txt (REFACTOR IDEAS: maybe take file input String as input)
unsigned int GetSize()
{
	string strInput;
	unsigned int uiSize = 0;
	ifstream ifs("DataFile.txt");
	if (ifs.is_open())
	{
		while (!ifs.eof())
		{
			getline(ifs, strInput);
			uiSize++;
		}
	}

	return uiSize;
}
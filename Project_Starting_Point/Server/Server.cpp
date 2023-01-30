#include <windows.networking.sockets.h>
#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

// This macro causes the latency to be measured. Results are stored to a directory
// called performance_data
#define PROFILE_LATENCY

// Only include profiler code when necessary
#ifdef PROFILE_LATENCY
#include "../LatencyProfiler/PerformanceProfiler.h"
#endif // PROFILE_LATENCY

using namespace std;

struct StorageTypes 
{ 
	unsigned int size = 0; //referring to the size of the array at address pData 
	float* pData; // pointer to future float array
};

/*An array of size 7 of type StorageTypes
Stored in memory as opposed to saved to disk, if server fails and restarts all data in memory will be lost
*/
StorageTypes RxData[7];

void UpdateData(unsigned int, float);
float CalcAvg(unsigned int);

#ifdef PROFILE_LATENCY
performance_profiler::LatencyRecorder recorder;
#endif // PROFILE_LATENCY

int main()
{

	WSADATA wsaData;
	SOCKET ServerSocket, ConnectionSocket; // creating two sockets, one for server and one for connection
	char RxBuffer[128] = {}; //Buffer containing 128 bytes
	sockaddr_in SvrAddr;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // create socket in Internet Domain, sock_stream referring to a reliable connection
															 // and  the transfer protocol is set to TCP
	if (ServerSocket == SOCKET_ERROR) //if error creating socket then server shuts down
		return -1;

	// bind ServerSocket using wildcards 
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = INADDR_ANY;
	SvrAddr.sin_port = htons(27001); // listens on PORT 27001
	bind(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)); 

	if (ServerSocket == SOCKET_ERROR)
		return -1;

	// Listen: indicates a readiness to accept client connection requests to ServerSocket but only a maximum of 1 queue of pending connections
	listen(ServerSocket, 1); 
	cout << "Waiting for client connection\n" << endl;
	ConnectionSocket = SOCKET_ERROR;

	//Accept: accepting a connection request from first in queue, creates a new socket descriptor with same properties as socket  
	ConnectionSocket = accept(ServerSocket, NULL, NULL); // first param: Socket Descriptor, second: client socket address (NULL if it doesn't matter), third: address length

	if (ConnectionSocket == SOCKET_ERROR)
		return -1;

	cout << "Connection Established" << endl;

	/* EXPLANATION of while loop as well as REFACTORING IDEAS 
	1) The server will only connect to the first connection ignore the rest until its buffer's first character matches '*'
	2) Once its finished with that first connection, the server socket is closed as well and the server application ends
	REFACTOR IDEAS: 
	For 1) 			There should be a case in the if elseif else statements that if the RxBuffer string is equal to ex. "DONE"
					Server will send an ack and then from there memset the RxBuffer to all '*' so that it can leave the while loop
					which indicates that the client is done sending requests
	For 2)
					If multiple clients are in queue instead of just one client to server relationship,
					then we should reset RxData[i].size to 0 and delete old array
					then we should the client socket to the next available queue (call accept) and keep server socket running waiting for next client
	*/
	while (RxBuffer[0] != '*')
	{

		float fValue = 0;
		memset(RxBuffer, 0, sizeof(RxBuffer));
		recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0); // this receives from client the name of the parameter
		send(ConnectionSocket, "ACK", sizeof("ACK"), 0);
		// switch case of some sort making sure the information is being sent correctly

#ifdef PROFILE_LATENCY
		performance_profiler::LatencyMeasurement measurement(workload_ids::SERVER_DETERMINE_CATEGORY);
#endif // PROFILE_LATENCY

		if (strcmp(RxBuffer, "ACCELERATION BODY X") == 0) 
		{
#ifdef PROFILE_LATENCY
			measurement.end();
			recorder.add(measurement);
#endif // PROFILE_LATENCY

			memset(RxBuffer, 0, sizeof(RxBuffer));
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			fValue = (float)atof(RxBuffer);
			UpdateData(0, fValue); // 0 is representing ACCELERATION BODY X, fValue is the float Value
			fValue = CalcAvg(0);
		}
		else if (strcmp(RxBuffer, "ACCELERATION BODY Y") == 0)
		{
#ifdef PROFILE_LATENCY
			measurement.end();
			recorder.add(measurement);
#endif // PROFILE_LATENCY

			memset(RxBuffer, 0, sizeof(RxBuffer));
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			fValue = (float)atof(RxBuffer);
			UpdateData(1, fValue);
			fValue = CalcAvg(1);
		}
		else if (strcmp(RxBuffer, "ACCELERATION BODY Z") == 0)
		{
#ifdef PROFILE_LATENCY
			measurement.end();
			recorder.add(measurement);
#endif // PROFILE_LATENCY

			memset(RxBuffer, 0, sizeof(RxBuffer));
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			fValue = (float)atof(RxBuffer);
			UpdateData(2, fValue);
			fValue = CalcAvg(2);
		}
		else if (strcmp(RxBuffer, "TOTAL WEIGHT") == 0)
		{
#ifdef PROFILE_LATENCY
			measurement.end();
			recorder.add(measurement);
#endif // PROFILE_LATENCY

			memset(RxBuffer, 0, sizeof(RxBuffer));
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			fValue = (float)atof(RxBuffer);
			UpdateData(3, fValue);
			fValue = CalcAvg(3);
		}
		else if (strcmp(RxBuffer, "PLANE ALTITUDE") == 0)
		{
#ifdef PROFILE_LATENCY
			measurement.end();
			recorder.add(measurement);
#endif // PROFILE_LATENCY

			memset(RxBuffer, 0, sizeof(RxBuffer));
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			fValue = (float)atof(RxBuffer);
			UpdateData(4, fValue);
			fValue = CalcAvg(4);
		}
		else if (strcmp(RxBuffer, "ATTITUDE INDICATOR PICTH DEGREES") == 0) //Does not align with DataFile.txt 'ATTITUDE INDICATOR PICTH DEGREES'
		{
#ifdef PROFILE_LATENCY
			measurement.end();
			recorder.add(measurement);
#endif // PROFILE_LATENCY

			memset(RxBuffer, 0, sizeof(RxBuffer));
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			fValue = (float)atof(RxBuffer);
			UpdateData(5, fValue);
			fValue = CalcAvg(5);
		}
		else if (strcmp(RxBuffer, "ATTITUDE INDICATOR BANK DEGREES") == 0) // Does not align with DataFile.txt column 'ATTITUDE INDICATOR BANK'
		{
#ifdef PROFILE_LATENCY
			measurement.end();
			recorder.add(measurement);
#endif // PROFILE_LATENCY

			memset(RxBuffer, 0, sizeof(RxBuffer));
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			fValue = (float)atof(RxBuffer);
			UpdateData(6, fValue);
			fValue = CalcAvg(6);
		}
		else
		{
#ifdef PROFILE_LATENCY
			measurement.end();
			recorder.add(measurement);
#endif // PROFILE_LATENCY

			memset(RxBuffer, 0, sizeof(RxBuffer));
			recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			fValue = 0.0;
		}


		char Tx[128];
		sprintf_s(Tx, "%f", fValue); // sets the Tx buffer to be a formatted string containing the value of the float
		send(ConnectionSocket, Tx, sizeof(Tx), 0); // sends the buffer to the client
	}

	closesocket(ConnectionSocket);	//closes incoming socket
	closesocket(ServerSocket);	    //closes server socket	
	WSACleanup();					//frees Winsock resources

#ifdef PROFILE_LATENCY
	recorder.saveToDisk();
#endif // PROFILE_LATENCY

	return 1;
}

/*given the column index and the current float value
to add, it will make a new array dynamically, delete the old array,
set pArray to the new array, and increase the count of size of Array by 1
*/
void UpdateData(unsigned int uiIndex, float value)
{
#ifdef PROFILE_LATENCY
	performance_profiler::LatencyMeasurement measurement(workload_ids::SERVER_UPDATE_RECEIVED);
#endif // PROFILE_LATENCY

	//if array hasn't been updated with information
	if (RxData[uiIndex].size == 0)
	{
		RxData[uiIndex].pData = new float[1]; // changing size of float array to size 1 since only 1 is needed
		RxData[uiIndex].pData[0] = value; // setting the first value of array to value
		RxData[uiIndex].size = 1;  // updating the size of the pData
	}
	else // otherwise we should make sure to 
	{
		float* pNewData = new float[RxData[uiIndex].size + 1]; // creates a new array of the size + 1, since array should be increasing in size
		for (unsigned int x = 0; x < RxData[uiIndex].size; x++) // copying all of old array to new array (only the last element of array hasn't been filled)
			pNewData[x] = RxData[uiIndex].pData[x]; 

		pNewData[RxData[uiIndex].size] = value; // filling the last element of new array with  the value
		delete[] RxData[uiIndex].pData; // delete old array since it was created dynamically (this frees up memory)
		RxData[uiIndex].pData = pNewData; // set the value of pData(pointer) to the address of the new array pNewData
		RxData[uiIndex].size++; // update the size of the new Array
	}

#ifdef PROFILE_LATENCY
	measurement.end();
	recorder.add(measurement);
#endif // PROFILE_LATENCY
}

// calculates the average of a column by its given the index 
float CalcAvg(unsigned int uiIndex)
{
#ifdef PROFILE_LATENCY
	performance_profiler::LatencyMeasurement measurement(workload_ids::SERVER_CALCULATE_AVERAGE);
#endif // PROFILE_LATENCY

	float Avg = 0;
	for (unsigned int x = 0; x < RxData[uiIndex].size; x++)
		Avg += RxData[uiIndex].pData[x];

	Avg = Avg / RxData[uiIndex].size;

#ifdef PROFILE_LATENCY
	measurement.end();
	recorder.add(measurement);
#endif // PROFILE_LATENCY

	return Avg;
}
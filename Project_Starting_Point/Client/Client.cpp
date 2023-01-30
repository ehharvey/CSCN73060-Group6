#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// This macro causes the latency to be measured. Results are stored to a directory
// called performance_data
#define PROFILE_LATENCY

#ifdef PROFILE_LATENCY
#include "../LatencyProfiler/PerformanceProfiler.h"
#endif // PROFILE_LATENCY


using namespace std;

unsigned int GetSize();

int main()
{
	// Profiler initialization
#ifdef PROFILE_LATENCY
	performance_profiler::LatencyRecorder recorder = performance_profiler::LatencyRecorder();
#endif // PROFILE_LATENCY

	
	WSADATA wsaData;
	SOCKET ClientSocket;   
	sockaddr_in SvrAddr;  
	unsigned int uiSize = 0;
	vector<string> ParamNames;
	char Rx[128];

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//Establishing Client Socket to communicate in TC/IP
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_port = htons(27001);
	SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

	// connecting client Socket with Server Address
	connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)); 


	uiSize = GetSize(); // calling in-home function which returns size O(n)
	
	/*
		This section the client application is reading from disk Line By Line
		and sending Parameter Name & Values One by One & printing out Average of each Column
		Outerloop has O(n), inner loop has O(n) so where n is number of lines
		And for while loop in each if statement it has O(c) where c is number of columns
		So this has loop has O(n*max(n,m)) --> so at least O(n^2) efficiency
	*/
	for (unsigned int l = 0; l < uiSize; l++)  //O(n) 
	{
		string strInput;
		
		/* opens ifstream object on File "DataFile.txt"
		Refactor Idea: Move this ifstream call outside of loop and
		and close the file stream after the while loop
		this would remove the need of the inner for loop completely
		as we would just need to call getline per outerloop iteration
		*/
		ifstream ifs("DataFile.txt"); 
		// O(n) 

#ifdef PROFILE_LATENCY
		performance_profiler::LatencyMeasurement measurement(workload_ids::CLIENT_FILE_READ);
#endif // PROFILE_LATENCY

		
		for (unsigned int iStart = 0; iStart < l; iStart++) 
			getline(ifs, strInput);

#ifdef PROFILE_LATENCY
		measurement.end();
		recorder.add(measurement);
#endif // PROFILE_LATENCY


		getline(ifs, strInput); 
		if (l > 0){ // If we are past the header/column name row
			size_t offset, preOffset;
			offset = preOffset = 0;
			unsigned int iParamIndex = 0;
			//while (offset != std::string::npos)
			while(iParamIndex != 8) // go through until all parameters are calculated
			{
				offset = strInput.find_first_of(',', preOffset+1);
				string strTx = strInput.substr(preOffset+1, offset - (preOffset+1)); // this gets the string reading of the value: number 
				/*
				In the send function, the first parameter is Client Socket, 
				second parameter is the address of the array containing the bytes encoded in ASCII (buffer),
				third paramter is an int representing the length of the second parameter (buffer)
				
				In the recv function the parameters:
				1) ClientSocket
				2) Address of Buffer that will be written to
				3) Size of the Buffer
				4) Flag (we set to 0 flags)
				*/
				
				// NOTE: This measurement must be subtracted from the sum of (workload 3, 4, 5)
#ifdef PROFILE_LATENCY
				performance_profiler::LatencyMeasurement measurement(workload_ids::CLIENT_ROUND_TRIP);
#endif // PROFILE_LATENCY

				send(ClientSocket, ParamNames[iParamIndex].c_str(), (int)ParamNames[iParamIndex].length(), 0); // this sends to server the name of parameter, as well as its length
				recv(ClientSocket, Rx, sizeof(Rx), 0);
				send(ClientSocket, strTx.c_str(), (int)strTx.length(), 0); 
				recv(ClientSocket, Rx, sizeof(Rx), 0);

#ifdef PROFILE_LATENCY
				measurement.end();
				recorder.add(measurement);
#endif // PROFILE_LATENCY

				cout << ParamNames[iParamIndex] << " Avg: " << Rx << endl;
				preOffset = offset;
				iParamIndex++; 
			}
		}
		else { // We are on the Header/Column Row
			// goes through as many delimitters(,) to pushback as many parameter names until it hits end of the line
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

	recorder.saveToDisk();

	return 1;
}


/* returns how many lines there are in the DataFile.txt 
(REFACTOR IDEAS: Take file input String as input) for flexiblity
Also this has O(n) efficiency where n is number of lines in file
*/
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
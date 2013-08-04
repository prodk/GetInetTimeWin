// (c) Mykola Prodanov, January 2013, Juelich, Germany.

#define TIMEDLL __declspec(dllexport)
#include "Timedll.h"

// Helper functions
int ConnectAndGetData(TimeData *td);
int ParseTimeString(TimeData *td, char* buf);
int CheckFirstEl(char *buf);

// Globals
char *g_szServer[] = {"128.138.140.44","207.200.81.113", "64.113.32.5", "64.147.116.229", "64.90.182.55",
					  "96.47.67.105", "165.193.126.229", "206.246.122.250", "64.250.177.145"};
unsigned int g_port = 13;	// Port number
int g_numOfServers = 9;
int g_iError = 0;
TimeData td;

// Exported functions
int getyear();
int getmonth();
int getday();
int gethour();
int getminute();
int getsecond();

// Connects to one of the servers, gets and parses the string with the date and time
// returns 0 if success
// returns -1 if socket problem
// returns -2 if cannot connect to the server
// returns -3 if connection is closed
// returns -4 if recv failed
// returns -5 if parsing error
int ConnectAndGetData(TimeData *td)
{
	char recvbuf[BUF_SIZE];
	int recvbuflen = BUF_SIZE;
	//----------------------
	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR){		
		//printf("Error at WSAStartup()\n");
		g_iError = -1;
		return g_iError;
	}

	//----------------------
	// Create a SOCKET for connecting to server
	SOCKET ConnectSocket;
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		//printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		g_iError = -1;
		return g_iError;
	}
	//printf("Socket %i created. \n", ConnectSocket);	

	//----------------------
	// Try connecting to one of the servers
	int i = 0;
	for(i=0; i < g_numOfServers; i++){
		// The sockaddr_in structure specifies the address family,
		// IP address, and port of the server to be connected to.
		sockaddr_in clientService; 
		clientService.sin_family = AF_INET;
		clientService.sin_addr.s_addr = inet_addr( g_szServer[i] );
		clientService.sin_port = htons( g_port );

		//----------------------
		// Connect to server.	
		if ( connect( ConnectSocket, (SOCKADDR*) &clientService, sizeof(clientService) ) 
			== SOCKET_ERROR) {
				if(i < g_numOfServers - 1)
					continue;
				else{
					//printf( "Failed to connect.\n" );
					closesocket(ConnectSocket);
					WSACleanup();
					g_iError = -2;
					return g_iError;				}
		}// end if connect
		
		//printf("Connected to server %s.\n", g_szServer[i]);

		// Clean the buffer
		memset(recvbuf, 0, BUF_SIZE*sizeof(char));
		// Receive the data.
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if ( iResult > 0 ){
			//printf("\rBytes received: %d\n", iResult);
			//printf("\r%s\n", recvbuf);

			// Parse the string
			iResult = ParseTimeString(td, recvbuf);
			if(iResult < 0)	// if parsing error
				g_iError = -5;
			else break;
		}
		else if ( iResult == 0 )
			//printf("Connection closed.\n");
			g_iError = -3;
		else
			//printf("recv failed: %d\n", WSAGetLastError());
			g_iError = -4;

		// If connected, then break the loop.
		if(g_iError >= 0)
			break;
	}// end for i

	closesocket(ConnectSocket);

	// Cleanup
	WSACleanup();

	return g_iError;
}

// Parse time string
// Its format is 
// 56319 13-01-27 21:16:34 00 0 0 268.5 UTC(NIST) * 
// returns -1 in case of error
int ParseTimeString(TimeData *td, char* buf)
{
	char szTmp[5];
	int iError = 0;
	memset(szTmp, 0, 5);

	int i = 0, j = 0, k = 0;

	// Skip the first number
	while(buf[i] != ' '){
		++i;
		// Prevent infinite loops
		if(BUF_SIZE - 1 == i){
			iError = -1;
			return iError;
		}
	}
	// Skip the space itself
	++i;

	// Get year, month, day
	j = 0;
	k = 0; // number of dividors, e. g. -, or :	
	while(buf[i] != ' '){
		szTmp[j] = buf[i];
		++j; ++i;
		if(buf[i] == '-'){
			++i;//skip '-'
			szTmp[j] = '\0';
			j = 0;
			if(k == 0)			
				td->year = atoi(szTmp);				
			else if(k == 1){				
				CheckFirstEl(szTmp);// check whether we have 0 at the beginning
				td->month = atoi(szTmp);				
			}
			++k;
		}// end if
		// Prevent from infinite loop
		if(BUF_SIZE - 1 == k){
			iError = -1;
			return iError;
		}
	}// end while !=' '
	++i;// skip ' '
	++j;// get space for the terminating 0
	szTmp[j] = '\0';
	CheckFirstEl(szTmp);// check whether we have 0 at the beginning
	td->day = atoi(szTmp);

	// Get hour, minutes, seconds
	j = 0;
	j = 0;
	k = 0; // number of dividors, e. g. -, or :	
	while(buf[i] != ' '){
		szTmp[j] = buf[i];
		++j; ++i;
		if(buf[i] == ':'){
			++i;//skip ':'
			szTmp[j] = '\0';
			j = 0;
			if(k == 0){
				CheckFirstEl(szTmp);// check whether we have 0 at the beginning
				td->hour = atoi(szTmp);
			}
			else if(k == 1){				
				CheckFirstEl(szTmp);// check whether we have 0 at the beginning
				td->minute = atoi(szTmp);				
			}
			++k;
		}// end if
		// Prevent from infinite loop
		if(BUF_SIZE - 1 == k){
			iError = -1;
			return iError;
		}
	}// end while !=' '
	++i;// skip ' '
	++j;// get space for the terminating 0
	szTmp[j] = '\0';
	CheckFirstEl(szTmp);// check whether we have 0 at the beginning
	td->second = atoi(szTmp);

	return iError;
}

int CheckFirstEl(char *szTmp){
	if(szTmp[0] == '0'){// skip 0 at the beginning of numbers
		szTmp[0] = szTmp[1];
		szTmp[1] = '\0';
	}
	return 0;
}

// This functions to export into the dll.
int getyear()
{
	int err = ConnectAndGetData(&td);
	if(err < 0)
		return err;
	if(g_iError != 0)
		return g_iError;

	return td.year;
}
int getmonth()
{
	if(g_iError != 0)
		return g_iError;

	return td.month;
}

int getday()
{
	if(g_iError != 0)
		return g_iError;

	return td.day;
}

int gethour()
{
	if(g_iError != 0)
		return g_iError;

	return td.hour;
}

int getminute()
{
	if(g_iError != 0)
		return g_iError;

	return td.minute;
}

int getsecond()
{
	if(g_iError != 0)
		return g_iError;

	return td.second;
}
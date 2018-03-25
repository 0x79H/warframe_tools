/* J. David's webserver */
/* This is a simple webserver.
* Created November 1999 by J. David Blackstone.
* CSE 4344 (Network concepts), Prof. Zeigler
* University of Texas at Arlington
*/

/*
This File is modified windows httpd server based on J. David's tinyhttpd
Created 2017-08-17 by HanPeicong 
I reserved most of the original comments for i have almost changed nothing :P
*/

// link with Ws2_32.lib
#pragma comment(lib,"Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>  
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <stdlib.h>
/**********************************************************************************************/
#include "hardwareKey.h"
#include <time.h>
#define random(x) (rand()%x)
#pragma warning(disable:4996)


//https://stackoverflow.com/questions/18647053/sendinput-not-equal-to-pressing-key-manually-on-keyboard-in-c

bool flag = false;
unsigned int speed = 150;
char ingame_chat[512] ;
int isWareframe() {
	WCHAR buf[MAX_PATH] = { 0 };
	GetWindowText(GetForegroundWindow(), buf, MAX_PATH);
	return	!wcscmp(buf, L"WARFRAME");
}
void send_key(unsigned short  key) {
	INPUT Input;
	Input.type = INPUT_KEYBOARD;
	Input.ki.wVk = key;
	Input.ki.wScan = 0;
	Input.ki.dwFlags = 0;
	Input.ki.time = 1000;
	Input.ki.dwExtraInfo = GetMessageExtraInfo();
	SendInput(1, &Input, sizeof(INPUT));
	Input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &Input, sizeof(INPUT));
}
void send_string(char *str)
{
	INPUT Input;
	WCHAR unicodeWord[512];
	memset(unicodeWord, 0, sizeof(unicodeWord));
	MultiByteToWideChar(CP_THREAD_ACP, MB_COMPOSITE, str, strlen(str), unicodeWord, 512);;

	for (int i = 0; unicodeWord[i] != 0; i++)
	{
		Input.type = INPUT_KEYBOARD;
		Input.ki.wVk = 0;
		Input.ki.wScan = unicodeWord[i];
		Input.ki.dwFlags = KEYEVENTF_UNICODE;
		Input.ki.time = 1000;
		Input.ki.dwExtraInfo = GetMessageExtraInfo();
		SendInput(1, &Input, sizeof(INPUT));
		Input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
		SendInput(1, &Input, sizeof(INPUT));

	}
}
void key_down_h(unsigned short  key) {
	//Structure for the keyboard event
	INPUT ip;
	//Set up the INPUT structure
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.wVk = 0; //We're doing scan codes instead
	ip.ki.dwExtraInfo = 0;
	//This let's you do a hardware scan instead of a virtual keypress
	ip.ki.dwFlags = KEYEVENTF_SCANCODE;
	ip.ki.wScan = key;
	SendInput(1, &ip, sizeof(INPUT));
	Sleep(50 + random(10));
}
void key_up_h(unsigned short  key) {
	//Structure for the keyboard event
	INPUT ip;
	//Set up the INPUT structure
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.wVk = 0; //We're doing scan codes instead
	ip.ki.dwExtraInfo = 0;
	//This let's you do a hardware scan instead of a virtual keypress
	ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	ip.ki.wScan = key;
	SendInput(1, &ip, sizeof(INPUT));
	Sleep(50 + random(10));
}
void spin_attack() {
	srand((int)time(0));
	unsigned short key[2] = { DIK_LCONTROL,DIK_E };
	for (int i = 0; i < 2; i++) {
		key_down_h(key[i]);
		Sleep(2);
	}
	Sleep(80 + random(10));
	for (int i = 1; i >= 0; i--) {
		key_up_h(key[i]);
		Sleep(2);
	}
	Sleep(random(50) + speed);
}
void ingame_say(char * text) {
	key_down_h(DIK_T);
	key_up_h(DIK_T);
	send_string(text);
	Sleep(1000);
	key_down_h(DIK_RETURN);
	key_up_h(DIK_RETURN);
	//send_key(VK_RETURN);
}
int no_afk(int attacktime) {
	key_down_h(DIK_W);
	Sleep(100);
	for (int i = 0; i < (35 + random(10)); i++) {
		if ((!flag) | (!isWareframe())) {
			key_up_h(DIK_W);
			return 0;
		}
		spin_attack();
		attacktime++;
		if (strlen(ingame_chat) != 0) {
			key_up_h(DIK_W);
			ingame_say(ingame_chat);
			key_down_h(DIK_W);
			*ingame_chat='\0';
		}
		//const char *info[] = { "啥时候走..." ,"装备信息:[阿特拉克斯][灼热黄蜂][剑风][压迫点][致残突击][急进猛突][狂战士][异况超量][爆裂刀刃][电击触电]","缺少的mod:[剑风 Prime][压迫点 Prime][败兴虐杀][锯刃][漂移接触]","白金剩余:9 (攒钱买了致残突击了)" ,"期望的[阿特拉克斯]配置:[急进猛突] [狂战士] [漂移接触] [压迫点] [压迫点 Prime] [剑风] [剑风 Prime] [狂暴] [败兴虐杀] [炙热黄蜂] ","","","","" ,"","","","" ,"","","","" };

		const char *emoji[] = { ":crylaugh:",":blush:",":laugh:",":facepalm:" };
		const char **info = emoji;
		//std::cout << (char*)info[random(16)] << "       " << (char*)emoji[random(4)] << std::endl;
		if (attacktime % 360 == 0) {
			switch (attacktime / 360)
			{
			case 0:
				key_up_h(DIK_W);
				//ingame_say((char*)info[0]);
				ingame_say((char*)emoji[random(4)]);
				//ingame_say(e);
				key_down_h(DIK_W);
				break;
			case 1:
				key_up_h(DIK_W);
				//ingame_say((char*)info[1]);
				ingame_say((char*)emoji[random(4)]);
				//ingame_say(e);
				key_down_h(DIK_W);
				break;
			case 2:
				key_up_h(DIK_W);
				//ingame_say((char*)info[2]);
				ingame_say((char*)emoji[random(4)]);
				//ingame_say(e);
				key_down_h(DIK_W);
				break;
			case 3:
				key_up_h(DIK_W);
				//ingame_say((char*)info[3]);
				ingame_say((char*)emoji[random(4)]);
				//ingame_say(e);
				key_down_h(DIK_W);
				break;
			case 10:
				key_up_h(DIK_W);
				ingame_say((char*)"号主可能睡着了:facepalm:");
				ingame_say((char*)emoji[random(4)]);
				//ingame_say(e);
				key_down_h(DIK_W);
				flag = false;
				break;
			default:
				break;
			}
		}
	}
	key_up_h(DIK_W);
	Sleep(50);
	//key_down_h(DIK_A);
	key_down_h(DIK_S);

	Sleep(random(50) + 150);
	key_down_h(DIK_LCONTROL);
	Sleep(200);
	//key_up_h(DIK_A);
	key_down_h(DIK_E);
	Sleep(random(100) + 130);
	key_up_h(DIK_S);

	key_up_h(DIK_E);

	//key_down_h(DIK_2);
	//key_up_h(DIK_2);
	return attacktime;
}
DWORD WINAPI  fun1(LPVOID lpParam) {
	int attacktime = 0;
	while (true) {
		if (flag == true) {
			if (isWareframe()) {
				attacktime = no_afk(attacktime);
			}
			else {
				Sleep(1000);
			}
		}
		else {
			attacktime = 0;
			Sleep(100);
		}
	}
}
int fun_main() {
	HWND	hwndMyWnd = GetConsoleWindow();
	RECT r;
	GetWindowRect(hwndMyWnd, &r);
	HANDLE buff = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD sizeOfBuff;
	sizeOfBuff.X = 200;
	sizeOfBuff.Y = 50;
	SetConsoleScreenBufferSize(buff, sizeOfBuff);
	::SetWindowPos(hwndMyWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(hwndMyWnd, SW_NORMAL);
	::MoveWindow(hwndMyWnd, 0, 0, 222, 100, TRUE);
	return 0;
}
/**********************************************************************************************/



#define ISspace(x) isspace((int)(x))

#define SERVER_STRING "Server: hanpchttpd/0.1.0\r\n"

using namespace std;

unsigned int __stdcall accept_request(void *);
void bad_request(SOCKET);
void cat(SOCKET, FILE *);
void cannot_execute(int);
void error_die(const char *);
// void execute_cgi(SOCKET, const char *, const char *, const char *);
int get_line(int, char *, int);
void headers(int, const char *);
void not_found(int);
void serve_file(int, const char *);
int startup(u_short *);
void unimplemented(SOCKET);

/**********************************************************************/
/* A request has caused a call to accept() on the server port to
* return.  Process the request appropriately.
* Parameters: the socket connected to the client */
/**********************************************************************/
unsigned int __stdcall accept_request(void * pClient)
{
	SOCKET client = (SOCKET)pClient;

	char buf[1024];
	ZeroMemory(buf, 1024);
	int numchars;
	char method[255];
	ZeroMemory(method, 255);
	char url[255];
	ZeroMemory(url, 255);
	char path[512];
	ZeroMemory(path, 512);
	size_t i, j;
	struct stat st;
	int cgi = 0;      /* becomes true if server decides this is a CGI
					  * program */
	char *query_string = NULL;

	numchars = get_line(client, buf, sizeof(buf));
	i = 0; j = 0;
	//////////////////////////////////////////////////////////////////////////
	// this will cause a bug if buf HAS NOT BEEN INITAILIZED
	// because ISSpace only accept -1 ~ 255
	while (!ISspace(buf[j]) && (i < sizeof(method) - 1))
	//////////////////////////////////////////////////////////////////////////
	{
		method[i] = buf[j];
		i++; j++;
	}
	method[i] = '\0';

	if (stricmp(method, "GET") && stricmp(method, "POST"))
	{
		unimplemented(client);
		return 0;
	}

	if (stricmp(method, "POST") == 0)
		cgi = 1;

	i = 0;
	while (ISspace(buf[j]) && (j < sizeof(buf)))
		j++;
	while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
	{
		url[i] = buf[j];
		i++; j++;
	}
	url[i] = '\0';

	if (stricmp(method, "GET") == 0)
	{
		query_string = url;
		while ((*query_string != '?') && (*query_string != '\0'))
			query_string++;
		if (*query_string == '?')
		{
			*query_string='\0';
			while (*query_string != '=') {
				query_string++;
			}
		//	cgi = 1;
			*query_string = '\0';
			query_string++;
			
		if (*ingame_chat == '\0') {
				strcpy(ingame_chat, query_string);
			}
		}
	}

			strcpy(buf, "HTTP/1.0 200 OK\r\n");
			send(client, buf, strlen(buf), 0);
			strcpy(buf, SERVER_STRING);
			send(client, buf, strlen(buf), 0);
			sprintf(buf, "Content-Type: text/html\r\n");
			send(client, buf, strlen(buf), 0);
			strcpy(buf, "\r\n");
			send(client, buf, strlen(buf), 0);
			sprintf(buf, "<form action=\"/index.html\" method=\"get\"><input type=\"text\" name=\"fake\" style=\"height: 50;\"><br/>  <input type=\"submit\" value=\"Submit\" />\r\n");
			send(client, buf, strlen(buf), 0);
			while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
				numchars = get_line(client, buf, sizeof(buf));
//			//serve_file(client, path);
	//	}else
	//		// execute_cgi(client, path, method, query_string);
	//		unimplemented(client);
	//}


	closesocket(client);

	return 0;
}

/**********************************************************************/
/* Inform the client that a request it has made has a problem.
* Parameters: client socket */
/**********************************************************************/
void bad_request(SOCKET client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "<P>Your browser sent a bad request, ");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "such as a POST without a Content-Length.\r\n");
	send(client, buf, sizeof(buf), 0);
}

/**********************************************************************/
/* Put the entire contents of a file out on a socket.  This function
* is named after the UNIX "cat" command, because it might have been
* easier just to do something like pipe, fork, and exec("cat").
* Parameters: the client socket descriptor
*             FILE pointer for the file to cat */
/**********************************************************************/
void cat(SOCKET client, FILE *resource)
{
	char buf[1024];

	fgets(buf, sizeof(buf), resource);
	while (!feof(resource))
	{
		send(client, buf, strlen(buf), 0);
		fgets(buf, sizeof(buf), resource);
	}
}

/**********************************************************************/
/* Inform the client that a CGI script could not be executed.
* Parameter: the client socket descriptor. */
/**********************************************************************/
void cannot_execute(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 500 Internal Server Error\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<P>Error prohibited CGI execution.\r\n");
	send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Print out an error message with perror() (for system errors; based
* on value of errno, which indicates system call errors) and exit the
* program indicating an error. */
/**********************************************************************/
void error_die(const char *sc)
{
	perror(sc);
	exit(1);
}

/**********************************************************************/
/* Execute a CGI script.  Will need to set environment variables as
* appropriate.
* Parameters: client socket descriptor
*             path to the CGI script */
/**********************************************************************/
//void execute_cgi(SOCKET client, const char *path,
//	const char *method, const char *query_string)
//{
//	char buf[1024];
//	int cgi_output[2];
//	int cgi_input[2];
//	pid_t pid;
//	int status;
//	int i;
//	char c;
//	int numchars = 1;
//	int content_length = -1;
//
//	buf[0] = 'A'; buf[1] = '\0';
//	if (stricmp(method, "GET") == 0)
//		while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
//			numchars = get_line(client, buf, sizeof(buf));
//	else    /* POST */
//	{
//		numchars = get_line(client, buf, sizeof(buf));
//		while ((numchars > 0) && strcmp("\n", buf))
//		{
//			buf[15] = '\0';
//			if (stricmp(buf, "Content-Length:") == 0)
//				content_length = atoi(&(buf[16]));
//			numchars = get_line(client, buf, sizeof(buf));
//		}
//		if (content_length == -1) {
//			bad_request(client);
//			return;
//		}
//	}
//
//	sprintf(buf, "HTTP/1.0 200 OK\r\n");
//	send(client, buf, strlen(buf), 0);
//
//	if (pipe(cgi_output) < 0) {
//		cannot_execute(client);
//		return;
//	}
//	if (pipe(cgi_input) < 0) {
//		cannot_execute(client);
//		return;
//	}
//
//	if ((pid = fork()) < 0) {
//		cannot_execute(client);
//		return;
//	}
//	if (pid == 0)  /* child: CGI script */
//	{
//		char meth_env[255];
//		char query_env[255];
//		char length_env[255];
//
//		dup2(cgi_output[1], 1);
//		dup2(cgi_input[0], 0);
//		close(cgi_output[0]);
//		close(cgi_input[1]);
//		sprintf(meth_env, "REQUEST_METHOD=%s", method);
//		putenv(meth_env);
//		if (strcasecmp(method, "GET") == 0) {
//			sprintf(query_env, "QUERY_STRING=%s", query_string);
//			putenv(query_env);
//		}
//		else {   /* POST */
//			sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
//			putenv(length_env);
//		}
//		execl(path, path, NULL);
//		exit(0);
//	}
//	else {    /* parent */
//		close(cgi_output[1]);
//		close(cgi_input[0]);
//		if (strcasecmp(method, "POST") == 0)
//			for (i = 0; i < content_length; i++) {
//				recv(client, &c, 1, 0);
//				write(cgi_input[1], &c, 1);
//			}
//		while (read(cgi_output[0], &c, 1) > 0)
//			send(client, &c, 1, 0);
//
//		close(cgi_output[0]);
//		close(cgi_input[1]);
//		waitpid(pid, &status, 0);
//	}
//}

/**********************************************************************/
/* Get a line from a socket, whether the line ends in a newline,
* carriage return, or a CRLF combination.  Terminates the string read
* with a null character.  If no newline indicator is found before the
* end of the buffer, the string is terminated with a null.  If any of
* the above three line terminators is read, the last character of the
* string will be a linefeed and the string will be terminated with a
* null character.
* Parameters: the socket descriptor
*             the buffer to save the data in
*             the size of the buffer
* Returns: the number of bytes stored (excluding null) */
/**********************************************************************/
int get_line(int sock, char *buf, int size)
{
	int i = 0;
	char c = '\0';
	int n;

	while ((i < size - 1) && (c != '\n'))
	{
		n = recv(sock, &c, 1, 0);
		/* DEBUG printf("%02X\n", c); */
		if (n > 0)
		{
			if (c == '\r')
			{
				n = recv(sock, &c, 1, MSG_PEEK);
				/* DEBUG printf("%02X\n", c); */
				if ((n > 0) && (c == '\n'))
					recv(sock, &c, 1, 0);
				else
					c = '\n';
			}
			buf[i] = c;
			i++;
		}
		else
			c = '\n';
	}
	buf[i] = '\0';

	return(i);
}

/**********************************************************************/
/* Return the informational HTTP headers about a file. */
/* Parameters: the socket to print the headers on
*             the name of the file */
/**********************************************************************/
void headers(int client, const char *filename)
{
	char buf[1024];
	(void)filename;  /* could use filename to determine file type */

	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(client, buf, strlen(buf), 0);
	strcpy(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Give a client a 404 not found status message. */
/**********************************************************************/
void not_found(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "your request because the resource specified\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "is unavailable or nonexistent.\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>\r\n");
	send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Send a regular file to the client.  Use headers, and report
* errors to client if they occur.
* Parameters: a pointer to a file structure produced from the socket
*              file descriptor
*             the name of the file to serve */
/**********************************************************************/
void serve_file(int client, const char *filename)
{
	FILE *resource = NULL;
	int numchars = 1;
	char buf[1024];

	buf[0] = 'A'; buf[1] = '\0';
	while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
		numchars = get_line(client, buf, sizeof(buf));

	resource = fopen(filename, "r");
	if (resource == NULL)
		not_found(client);
	else
	{
		headers(client, filename);
		cat(client, resource);
	}
	fclose(resource);
}

/**********************************************************************/
/* This function starts the process of listening for web connections
* on a specified port.  If the port is 0, then dynamically allocate a
* port and modify the original port variable to reflect the actual
* port.
* Parameters: pointer to variable containing the port to connect on
* Returns: the socket */
/**********************************************************************/
int startup(u_short *port)
{
	int httpd = 0;
	struct sockaddr_in name;
	//-----------------------------------------
	// Declare and initialize variables
	WSADATA wsaData = { 0 };
	int iResult = 0;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wprintf(L"WSAStartup failed: %d\n", iResult);
		return 1;
	}

	httpd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//////////////////////////////////////////////////////////////////////////
	/*
	* This is used instead of -1, since the
	* SOCKET type is unsigned.
	*/
	if (httpd == INVALID_SOCKET)
	//////////////////////////////////////////////////////////////////////////
		error_die("socket");
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	//////////////////////////////////////////////////////////////////////////
	// simple byte order
	name.sin_port = htons(*port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	//////////////////////////////////////////////////////////////////////////
	if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
		error_die("bind");
	if (*port == 0)  /* if dynamically allocating a port */
	{
		int namelen = sizeof(name);
		if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
			error_die("getsockname");
		*port = ntohs(name.sin_port);
	}
	if (listen(httpd, 5) < 0)
		error_die("listen");
	return(httpd);
}

/**********************************************************************/
/* Inform the client that the requested web method has not been
* implemented.
* Parameter: the client socket */
/**********************************************************************/
void unimplemented(SOCKET client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</TITLE></HEAD>\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>\r\n");
	send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
DWORD WINAPI  server(LPVOID lpParam) {
	int server_sock = -1;
	u_short port = 80;				//端口
	int client_sock = -1;
	struct sockaddr_in client_name;
	int client_name_len = sizeof(client_name);
	HANDLE newthread;

	server_sock = startup(&port);
	printf("http://127.0.0.1:%d/\n", port);

	while (1)
	{
		client_sock = accept(server_sock,
			(struct sockaddr *)&client_name,
			&client_name_len);
		if (client_sock == -1)
			error_die("accept");
		/* accept_request(client_sock); */
		newthread = (HANDLE)_beginthreadex(NULL, 0, accept_request , (void *)client_sock, 0, NULL);
		if (newthread == NULL)
		{
			perror("pthread_create");
		}
		Sleep(100);
	}

	int iResult = closesocket(server_sock);
	if (iResult == SOCKET_ERROR) {
		printf("closesocket failed with error = %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	return(0);
}

int main()
{
	//ingame_say((char*)"test233测试");
	fun_main();



	HANDLE hThread;
	hThread = CreateThread(NULL,
		0,
		server,
		NULL,
		0, 0
	);





	HANDLE hThread2;
	hThread = CreateThread(NULL,
		0,
		fun1,
		NULL,
		0, 0
	);
	//==
	if (RegisterHotKey(
		NULL,
		1,
		//MOD_ALT | MOD_NOREPEAT,
		MOD_ALT | MOD_CONTROL,
		0x42))  //0x42 is 'b'
	{
	//	std::cout << ("hotkey: Ctrl + Alt + b\r\nspeed=350\t\t") << std::endl;
		printf("hotkey: Ctrl+Alt+B\n");
	}
	else {
		exit(0);
	}

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (msg.message == WM_HOTKEY)
		{
			flag = !flag;
		//	std::cout << (flag == TRUE ? "打开" : "关闭") << std::endl;
			printf(flag == TRUE ? "打开" : "关闭");
		}
	}


	return 0;
	//==
}

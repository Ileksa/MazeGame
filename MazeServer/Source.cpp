#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <io.h>
#include <fcntl.h>
//#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "game_map.h"
#include "common_constants.h"
#include "server_communications.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main() {
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wcout << L"WSAStartup failed:" << iResult << endl;
		return 1;
	}

	lsm_server* server = new lsm_server(PORT_COMMUNICATION);
	server->accepting_clients();

	//HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	//wcout << L"first";
	//for (int i = 0; i < 16; i++) {
	//	SetConsoleTextAttribute(hConsoleHandle, i); //Задаем цвет фона и цвет текста
	//	wcout << L"++++" << i << L"++++";
	//}
	//wcout << endl;

	//game_map* gmap = new game_map(1, 3);
	//gmap->initialize(1);

	//node* nd = gmap->get_node(4);
	//nd->add_player(new player(456));
	//gmap->output_map();

end_of_the_program:
	system("pause");

	return 0;
}
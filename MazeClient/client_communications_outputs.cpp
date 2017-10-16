#include "client_communications.h"

void output_header(char* nickname, int uid)
{
	wcout << L"UID: " << uid << endl;
	wcout << L"Nickname: " << nickname << endl;
}

void output_menu()
{
	wcout << "-----------------------------" << endl;
	wcout << L"Choose action and type it." << endl;
	wcout << "start [num] (use 0 to autochoosing)" << endl;
	wcout << "quit" << endl;
	wcout << "-----------------------------" << endl;
}
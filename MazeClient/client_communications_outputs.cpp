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
	wcout << "list" << endl;
	wcout << "quit" << endl;
	wcout << "-----------------------------" << endl;
}
void output_header_games_info() {
	wcout << L"┌";
	for (int i = 0; i < 4; i++)
		wcout << L"┄";
	wcout << L"┬";
	for (int i = 0; i < GAMENAME_LEN + 1; i++)
		wcout << L"┄";
	wcout << L"┬";
	for (int i = 0; i < 8; i++)
		wcout << L"┄";
	wcout << L"┬";
	for (int i = 0; i < 8; i++)
		wcout << L"┄";
	wcout << L"┐" << endl;

	wcout << L"┊";
	wcout.width(4);
	wcout << "GID";

	wcout << L"┊";
	wcout.width(GAMENAME_LEN + 1);
	wcout << "Game name";

	wcout << L"┊";
	wcout.width(8);
	wcout << "Current";

	wcout << L"┊";
	wcout.width(8);
	wcout << "Max";
	wcout << L"┊" << endl;
	output_delimiter_game_info();
}
void output_game_info(int gid, int count, int max_count, char* name) {
	wcout << L"┊";
	wcout.width(4);
	wcout << gid;

	wcout << L"┊";
	wcout.width(GAMENAME_LEN + 1);
	wcout << name;

	wcout << L"┊";
	wcout.width(8);
	wcout << count;

	wcout << L"┊";
	wcout.width(8);
	wcout << max_count;
	wcout << L"┊" << endl;
	output_delimiter_game_info();
}

void output_delimiter_game_info() {
	wcout << L"└";
	for (int i = 0; i < 4; i++)
		wcout << L"┄";
	wcout << L"┴";
	for (int i = 0; i < NICKNAME_LEN + 1; i++)
		wcout << L"┄";
	wcout << L"┴";
	for (int i = 0; i < 8; i++)
		wcout << L"┄";
	wcout << L"┴";
	for (int i = 0; i < 8; i++)
		wcout << L"┄";
	wcout << L"┘" << endl;
}

void output_unexpected_error() {
	system("cls");
	wcout << L"An unexpected error occurred while working with the server. Application will be closed." << endl;
	_getch();
}
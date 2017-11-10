#include "server_communications.h"

lsm_server::lsm_server(int port)
{
	socket_listener = socket(AF_INET, SOCK_STREAM, 0);
	if (WSAGetLastError() != 0) {
		wcout << L"Creating socket error:" << WSAGetLastError() << endl;
		throw "Creating socket error";
	}

	sockaddr_in tcpaddr;
	tcpaddr.sin_family = AF_INET;
	tcpaddr.sin_port = htons(port);
	tcpaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int size = sizeof(tcpaddr);


	DWORD timeout = TIMEOUT_SECS * 1000;
	if (setsockopt(socket_listener, SOL_SOCKET, SO_RCVTIMEO, (char*)(&timeout), sizeof(timeout)) < 0) {
		wcout << L"Can not apply timeout to socket listener" << endl;
	}

	bind(socket_listener, (SOCKADDR*)&tcpaddr, sizeof(tcpaddr));
	if (WSAGetLastError() != 0) {
		wcout << L"Binding error:" << WSAGetLastError() << endl;
		throw "Binding error";
	}

	

	listen(socket_listener, 5);

	players_count = PLAYERS_COUNT;
	players = new player*[players_count];
	for (int i = 0; i < PLAYERS_COUNT; i++)
		players[i] = nullptr;

	games_count = 0;
	games = new game*[GAMES_COUNT];
	for (int i = 0; i < GAMES_COUNT; i++)
		games[i] = nullptr;

	initialize_default_games();
}


lsm_server::~lsm_server()
{
	delete games[0];
	delete[] games;

	delete[] players;
	closesocket(socket_listener);
}

void lsm_server::accepting_clients()
{
	wcout << L"Server started." << endl;

	SOCKET sockfd;
	sockaddr_in tcpaddr;
	int size = sizeof(tcpaddr);
	while (true)
	{
		sockfd = accept(socket_listener, (SOCKADDR*)&tcpaddr, &size);
		if (sockfd == INVALID_SOCKET){
			if (errno == EAGAIN) {
				wcout << L"Timeout exit" << endl;
				continue;
			}
			else {
				wcout << L"Accepting error:" << WSAGetLastError() << endl;
				return;
			}
		}

		char ip[IP_BUF_SIZE];
		inet_ntop(AF_INET, &(tcpaddr.sin_addr), ip, 100);
		wcout << L"Connection accept with client " << ip << endl;

		client_communication_data* data = static_cast<client_communication_data*>(malloc(sizeof(client_communication_data)));
		data->initiator = this;
		data->socket = sockfd;
		data->tcpaddr = tcpaddr;

		if (CreateThread(NULL, 0, client_communication_wraper, data, 0, NULL) == NULL) {
			send_error(sockfd, "ER 401 Internal server error. Connection will be closed.\r\n");
			closesocket(sockfd);
		}
	}

}

DWORD WINAPI lsm_server::client_communication_wraper(LPVOID data)
{
	static_cast<client_communication_data*>(data)->initiator->client_communication(data);
	free(data);
	return 0;
}

DWORD WINAPI lsm_server::client_communication(LPVOID _data)
{
	client_communication_data* data = static_cast<client_communication_data*>(_data);
	SOCKET s = data->socket;
	send_greetings_message(s);

	int state = STATE_WAIT_HELO;
	char message[MSG_SIZE];
	int result;
	player* pl = nullptr;
	game* g = nullptr;

	while (true) {
		memset(message, '\0', MSG_SIZE);
		result = get_command(s, message, MSG_SIZE);

		wcout << L"[DEBUG] Get a command: " << message << endl;

		if (result == -2)
		{
			if (g != nullptr)
				g->remove_player(pl);
			if (pl != nullptr)
				remove_player(pl);
			goto end;
		}

		if (result <= 0) {
			send_error(s, "ER 500 Syntax error, command isn't recognized.\r\n");
			continue;
		}

		switch (state)
		{
		case STATE_WAIT_HELO:
			if (strlen(message) == 4 && strncmp(message, "QUIT", COMMAND_LEN) == 0)
			{
				int res = process_quit_message(s, message, result, &state, nullptr);
				if (res >= 0)
					goto end;
			}
			else if (strncmp(message, "HELO ", COMMAND_LEN + 1) == 0)
				pl = process_helo_message(s, message, result, &state);
			else
				send_error(s, "ER 580 Invalid command. Expected HELO username.\r\n");
			break;
		case STATE_WORKING:
			if (strlen(message) == 4 && strncmp(message, "QUIT", COMMAND_LEN) == 0) {
				int res = process_quit_message(s, message, result, &state, pl);
				if (res >= 0)
					goto end;
			}
			else if (strncmp(message, "STAR 0", COMMAND_LEN + 1) == 0) {
				g = process_star_message(s, static_cast<client_communication_data*>(data)->tcpaddr, message, result, &state, pl);
			}
			else if (strncmp(message, "LIST", COMMAND_LEN) == 0) {
				process_list_message(s, message, result, &state, pl);
			}
			else
				send_error(s, "ER 581 Invalid command.\r\n");
			break;
		case STATE_IN_GAME:
			if (strlen(message) == 4 && strncmp(message, "ENDG", COMMAND_LEN) == 0) {
				int res = process_endg_message(s, message, result, &state, pl, g);
				if (res >= 0)
					g = nullptr;
			}
			else if (strncmp(message, "MOVE", COMMAND_LEN) == 0)
			{
				int res = process_move_message(s, message, result, &state, pl, g);
			}
			else if (strncmp(message, "SHOT", COMMAND_LEN) == 0)
			{
				int res = process_shot_message(s, message, result, &state, pl, g);
			}
			else
				send_error(s, "ER 581 Invalid command.\r\n");
			break;
		default:
			send_error(s, "ER 401 Internal server error. Connection will be closed.\r\n");
			goto end;
		}

	}
end:


	//if (pl != nullptr)	{
	//	players[pl->get_uid()] = nullptr;
	//	//delete pl;
	//}
	wcout << L"[DEBUG] before close socket" << endl;
	closesocket(s);
	wcout << L"Connection is closed." << endl;
	return 0;
}

int lsm_server::get_command(SOCKET s, char* buf, int buf_size)
{
	memset(buf, '\0', buf_size);
	int state = 0; //0 - ждем \r, 1 - ждем \n
	int res;
	char c[1];
	for (int i = 0; i < buf_size - 1 && state < 2; i++)
	{
		res = recv(s, c, 1, 0);
		if (res <= 0)
			return -2;

		if (state == 0)
		{
			if (*c == '\r')
				state = 1;
			buf[i] = *c;
		}
		else if (state == 1)
		{
			if (*c == '\n')
				state = 2;
			else if (*c != '\r')
				state = 0;
			buf[i] = *c;
		}
	}
	if (state < 2)
		return -1;
	int len = strlen(buf);
	buf[len - 2] = '\0'; //обрезаются \r\n
	return len - 2;
}

void lsm_server::send_greetings_message(SOCKET s)
{
	char message[MSG_SIZE];
	memset(message, '\0', MSG_SIZE);
	strncpy(message, "Server of the game 'Last standing man' greets you!\r\n", MSG_SIZE);
	send(s, message, strlen(message), 0);
}

void lsm_server::send_message(SOCKET s, char* msg, int msg_size)
{
	char message[MSG_SIZE];
	memset(message, '\0', MSG_SIZE);
	strncpy(message, msg, min(MSG_SIZE, msg_size));
	send(s, message, strlen(message), 0);
}


void lsm_server::send_error(SOCKET s, char* msg)
{
	char message[MSG_SIZE];
	memset(message, '\0', 256);
	strncpy(message, msg, MSG_SIZE);
	send(s, message, strlen(message), 0);
}

int lsm_server::add_player(player* pl)
{
	for (int i = 1; i < players_count; i++)
		if (players[i] == nullptr)
		{
			players[i] = pl;
			pl->set_uid(i);
			return i;
		}
	return -1;
}

void lsm_server::remove_player(player* pl)
{
	int uid = pl->get_uid();
	if (uid != -1)
		players[uid] = nullptr;
}

int lsm_server::get_index(game* g)
{
	for (int i = 0; i < games_count; i++)
		if (games[i] == g)
			return i;
	return -1;
}


void lsm_server::initialize_default_games()
{
	ifstream maps("maps.txt");

	if (!maps.is_open()) {
		wcout << L"File \"map.txt\" isn't found." << endl;
		return;
	}

	//внешний цикл - проход по картам
	while (!maps.eof()) {
		if (games_count >= GAMES_COUNT)
			return;

		int lvl_size, count;
		char name[GAMENAME_LEN + 1];
		memset(name, '\0', GAMENAME_LEN + 1);
		
		maps.getline(name, GAMENAME_LEN);
		maps >> lvl_size;
		maps >> count;

		game* g = new game(lvl_size);
		g->set_game_name(name);

		//внутренний цикл - получение узло
		int num, up, down, left, right, upleft, upright, downleft, downright;
		node* n = nullptr;
		int counter = 0;
		while (counter < count) {
			maps >> num;
			maps >> up;
			maps >> down;
			maps >> left;
			maps >> right;
			maps >> upleft;
			maps >> upright;
			maps >> downleft;
			maps >> downright;
			n = new node(num, up, down, left, right, upleft, upright, downleft, downright);
			g->add_node(n);
			counter++;
		}

		games_count++;
		games[games_count] = g;
		wcout << L"Add game " << name << "." << endl;
		maps.getline(name, GAMENAME_LEN);
	}
}
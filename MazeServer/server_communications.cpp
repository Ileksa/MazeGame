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
}


lsm_server::~lsm_server()
{
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
		if (WSAGetLastError() != 0) {
			wcout << L"Accepting error:" << WSAGetLastError() << endl;
			continue;
		}

		char ip[IP_BUF_SIZE];
		inet_ntop(AF_INET, &(tcpaddr.sin_addr), ip, 100);
		wcout << L"Connection accept with client " << ip << endl;

		SOCKET* client_socket = static_cast<SOCKET*>(malloc(sizeof(SOCKET)));
		memcpy(client_socket, &sockfd, sizeof(sockfd));

		client_communication_data* data = static_cast<client_communication_data*>(malloc(sizeof(client_communication_data)));
		data->initiator = this;
		data->socket = client_socket;
		data->tcpaddr = tcpaddr;

		if (CreateThread(NULL, 0, client_communication_wraper, data, 0, NULL) == NULL) {
			send_error(sockfd, "ER 401 Internal server error. Connection will be closed.\r\n");
			closesocket(sockfd);
			free(client_socket);
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
	SOCKET* s = static_cast<SOCKET*>(data->socket);
	send_greetings_message(*s);

	int state = STATE_WAIT_HELO;
	char message[MSG_SIZE];
	int result;
	player* pl;

	while (true) {
		memset(message, '\0', MSG_SIZE);
		result = get_command(*s, message, MSG_SIZE);

		wcout << L"[DEBUG] Get a command: " << message << endl;

		if (result == -2)
			goto end;
		

		if (result <= 0) {
			send_error(*s, "ER 500 Syntax error, command isn't recognized.\r\n");
			continue;
		}

		switch (state)
		{
		case STATE_WAIT_HELO:
			if (strncmp(message, "QUIT", COMMAND_LEN) == 0)
			{
				if (process_quit_message(*s, message, result, &state, nullptr) == 0)
					goto end;
			} else if (strncmp(message, "HELO ", COMMAND_LEN + 1) == 0)
				pl = process_helo_message(*s, message, result, &state);
			else 
				send_error(*s, "ER 580 Invalid command. Expected HELO username.\r\n");
			break;
		case STATE_WORKING:
			if (strncmp(message, "QUIT", COMMAND_LEN) == 0)
			{
				if (process_quit_message(*s, message, result, &state, nullptr) == 0)
					goto end;
			}
			break;
		default:
			send_error(*s, "ER 401 Internal server error. Connection will be closed.\r\n");
			goto end;
		}

	}
end:
	closesocket(*s);
	free(s);
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

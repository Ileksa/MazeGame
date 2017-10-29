#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "server_communications.h"
player* lsm_server::process_helo_message(SOCKET s, char* msg, int size, int* server_state)
{
	if (size <= COMMAND_LEN + 1) {
		send_error(s, "ER 501 Argument syntax error.\r\n");
		return nullptr;
	}

	player* pl = new player(&(msg[COMMAND_LEN + 1]));
	int uid = add_player(pl);

	if (uid == -1) {
		send_error(s, "ER 402 Server is full. Please try later.\r\n");
		return nullptr;
	}
	else {
		char buf[NICKNAME_LEN + 20];

		memset(buf, '\0', sizeof(buf));
		sprintf(buf, "OK 220 Welcome, %s! Your UID is %d.\r\n", pl->get_name(), uid);

		send_message(s, buf, strlen(buf));
		*server_state = STATE_WORKING;
		return pl;
	}
}


int lsm_server::process_endg_message(SOCKET s, char* msg, int size, int* server_state, player* pl, game* g)
{
	if (g != nullptr) {
		g->remove_player(pl);
		int index = get_index(g);
		if (index > 0 && g->get_players_count() == 0)
		{
			delete games[index];
			games[index] = nullptr;
		}
	}
	*server_state = STATE_WORKING;
	char buf[30] = "OK 251 You leaved the game.\r\n";
	send_message(s, buf, sizeof(buf));
	return 0;
	
}

int lsm_server::process_quit_message(SOCKET s, char* msg, int size, int* server_state, player* pl)
{
	if (pl != nullptr)
	{
		remove_player(pl);
		delete pl;
	}
	*server_state = STATE_QUIT;
	char buf[25] = "OK 221 Bye!\r\n";
	send_message(s, buf, sizeof(buf));
	return 0;
}

game* lsm_server::process_star_message(SOCKET s, sockaddr_in tcpaddr, char* msg, int size, int* server_state, player* pl)
{
	wcout << L"[DEBUG] Process star message." << endl;

	//если нет аргумента или аргумент слишком большой
	if (size <= COMMAND_LEN + 1 || size - COMMAND_LEN - 1 > 3) {
		send_error(s, "ER 501 Argument syntax error.\r\n");
		return nullptr;
	}

	int number = atoi(&(msg[COMMAND_LEN + 1]));
	if (number < 0 || number > GAMES_COUNT) {
		send_error(s, "ER 501 Argument syntax error.\r\n");
		return nullptr;
	}

	//TODO: подбор игры

	game* game = games[0];
	int result = game->add_player(pl);
	if (result < 0) {
		send_error(s, "ER 403 Game is full. Please try later or choose another game.\r\n");
		return nullptr;
	}

	SOCKET s_notifications = socket(AF_INET, SOCK_STREAM, 0);
	if (WSAGetLastError() != 0) {
		wcout << L"Creating notifications socket error: " << WSAGetLastError() << endl;
		return nullptr;
	}

	
	wcout << L"[DEBUG] Before connect to socket notifications: " << inet_ntoa(tcpaddr.sin_addr) << endl;
	
	sockaddr_in addr = tcpaddr;
	addr.sin_port = htons(1007);
	connect(s_notifications, (SOCKADDR*)&addr, sizeof(addr));
	if (WSAGetLastError() != 0) {
		send_error(s, "ER 510 Port 1007 is not available for notifications.\r\n");
		return nullptr;
	}

	pl->set_socket_notifications(s_notifications);
	wcout << L"[DEBUG] SOCKET for notifications is opened." << endl;
	

	
	char buf[100] = "OK 250 You added to game.\r\n";
	send_message(s, buf, strlen(buf));

	int count = game->get_nodes_count();
	sprintf(buf, "MAP: %d %d\r\n", count, game->get_level_size());
	send_message(s, buf, strlen(buf));

	for (int i = 0; i < count; i++)	{
		node* n = game->get_node(i);
		sprintf(buf, "%d %d %d %d %d %d %d %d %d\r\n", n->get_id(), n->get_up(), n->get_down(), n->get_left(), n->get_right(),
			n->get_up_left(), n->get_up_right(), n->get_down_left(), n->get_down_right());
		send_message(s, buf, strlen(buf));
	}

	count = game->get_players_count();
	sprintf(buf, "PLS: %d\r\n", count);
	send_message(s, buf, strlen(buf));

	if (count > 0) {
		int i = 0;
		for(auto it = game->get_players_iterator_begin(); 
			it !=  game->get_players_iterator_end(); ++it)
		{
			sprintf(buf, "%d %s %d %d\r\n", it->first->get_uid(), it->first->get_name(), 1, it->second->get_id());
			send_message(s, buf, strlen(buf));

		}
	}

	*server_state = STATE_IN_GAME;

	return game;
}

//обрабатывает команду движения игрока из заданной позиции
int lsm_server::process_move_message(SOCKET s, char* msg, int size, int* server_state, player* pl, game* g)
{
	wcout << L"[DEBUG] Start process move message" << endl;
	if (size <= COMMAND_LEN + 1) {
		send_error(s, "ER 501 Argument syntax error.\r\n");
		return -1;
	}

	const int dir_size = 11;
	char direction[dir_size];
	memset(direction, '\0', dir_size);
	wcout << L"[DEBUG] Will find here" << msg + COMMAND_LEN + 1 << endl;

	char* pos = strstr(msg + COMMAND_LEN + 1, " ");
	char* c = msg + COMMAND_LEN + 1;
	int i = 0;
	while (c != pos)
	{
		direction[i] = *c;
		i++;
		c++;
	}
	//strncpy(direction, msg + COMMAND_LEN + 1, msg + COMMAND_LEN + 1 - pos);
	//int from_node = atoi(pos + 1);

	wcout << L"[DEBUG] Get direction" << direction << endl;


	node* n = g->get_player_node(pl);
	int new_node = -1;
	if (strncmp(direction, "UP", dir_size) == 0)
		new_node = n->get_up();
	else if (strncmp(direction, "DOWN", dir_size) == 0)
		new_node = n->get_down();
	else if (strncmp(direction, "LEFT", dir_size) == 0)
		new_node = n->get_left();
	else if (strncmp(direction, "RIGHT", dir_size) == 0)
		new_node = n->get_right();
	else if (strncmp(direction, "UPLEFT", dir_size) == 0)
		new_node = n->get_up_left();
	else if (strncmp(direction, "UPRIGHT", dir_size) == 0)
		new_node = n->get_up_right();
	else if (strncmp(direction, "DOWNLEFT", dir_size) == 0)
		new_node = n->get_down_left();
	else if (strncmp(direction, "DOWNRIGHT", dir_size) == 0)
		new_node = n->get_down_right();

	if (new_node == -1) {
		send_error(s, "ER 582 You cannot move in the intend direction from current node.\r\n");
		return -1;
	}


	int result = g->set_player_node(pl, new_node);

	//int result = g->remove_player_from_node(pl, from_node);
	if (result < 0) {
		send_error(s, "ER 400 Internal server error.\r\n");
		return -1;
	}
	result = g->add_player_to_node(pl, new_node);

	char message[MSG_SIZE];
	memset(message, '\0', MSG_SIZE);
	sprintf(message, "OK 245 %d\r\n", new_node);
	send_message(s, message, strlen(message));
	g->notify_players_move(pl->get_uid(), n->get_id(), new_node);

	return 0;
}

int process_shot_message(SOCKET s, char* msg, int size, int* server_state, player* pl, game* g)
{
	
}

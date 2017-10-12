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
		sprintf(buf, "OK 220 Welcome, %s! Your UID is %04d\r\n", pl->get_name(), uid);

		send_message(s, buf, strlen(buf));
		*server_state = STATE_WORKING;
		return pl;
	}
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

game_map* lsm_server::process_star_message(SOCKET s, char* msg, int size, int* server_state, player* pl)
{
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

	game_map* game = games[0];
	int result = game->add_player(pl);
	if (result < 0) {
		send_error(s, "ER 403 Game is full. Please try later or choose another game.\r\n");
		return nullptr;
	}

	//TODO: код на подключение к сокету 1007 клиента и в случа неуспеха удаление игрока
	
	char buf[100] = "OK 250 You added to game.\r\n";
	send_message(s, buf, strlen(buf));

	int count = game->get_nodes_count();
	sprintf(buf, "MAP: %d\r\n", count);
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
		game->reset_players_iterator();
		player* pl = game->current_player();
		sprintf(buf, "%d %s %d %d\r\n", pl->get_uid(), pl->get_name(), 1, game->current_node()->get_id());
		send_message(s, buf, strlen(buf));
		while (game->move_next()) 		{
			pl = game->current_player();
			sprintf(buf, "%d %s %d %d\r\n", pl->get_uid(), pl->get_name(), 1, game->current_node()->get_id());
			send_message(s, buf, strlen(buf));
		}
	}

	*server_state = STATE_IN_GAME;

	return game;
}

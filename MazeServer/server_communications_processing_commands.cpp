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

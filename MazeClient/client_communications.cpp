#include "client_communications.h"
int get_ip(char* ip_buf, int buf_size)
{
	wcout << L"Enter IPv4 of game_server:" << endl;
	fgets(ip_buf, buf_size, stdin);
	fflush(stdin);
	int len = strlen(ip_buf);

	ip_buf[min(buf_size, len) - 1] = '\0';
	return 1;
}

int get_nickname(char* buf, int buf_size)
{
	wcout << L"Enter nickname:" << endl;
	fgets(buf, buf_size, stdin);
	fflush(stdin);
	int len = strlen(buf);

	buf[min(buf_size, len) - 1] = '\0';
	return 1;
}


int get_command(SOCKET s, char* buf, int buf_size)
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

bool is_ok_command(char* message, int msg_size)
{
	if (msg_size < 2)
		return false;
	if (strncmp(message, "OK ", 3) == 0)
		return true;
	return false;
}

int send_command(SOCKET s, char* buf, int buf_size)
{
	char msg[MSG_SIZE];
	memset(msg, '\0', MSG_SIZE);
	strncpy(msg, buf, min(buf_size, MSG_SIZE - 1));
	int result = send(s, msg, strlen(msg), 0);
	if (result <= 0)
		return -1;
	return 0;
}

DWORD WINAPI process_notifications(LPVOID data)
{
	SOCKET s = static_cast<notifications_data*>(data)->s;
	game* g = static_cast<notifications_data*>(data)->g;

	int res;
	char buf[MSG_SIZE];
	while(true)
	{
		res = get_command(s, buf, MSG_SIZE);
		if (res < 0)
			break;
		//system("cls");

		if (strncmp(buf, "MOVE ", COMMAND_LEN + 1) == 0)
		{
			int uid, from, to;
			char* start = buf + COMMAND_LEN + 1;
			char* end = strstr(buf + COMMAND_LEN + 1, " ");
			*end = '\0';
			uid = atoi(start);

			start = end + 1;
			end = strstr(start, " ");
			*end = '\0';
			from = atoi(start);

			start = end + 1;
			to = atoi(start);

			g->set_player_node(uid, to);
			g->output_players_at_node(from, 0, 0);
			g->output_players_at_node(to, 0, 0);

		}
		else if (strncmp(buf, "QUIT ", COMMAND_LEN + 1) == 0)
		{
			int uid, from;
			char* start = buf + COMMAND_LEN + 1;
			char* end = strstr(buf + COMMAND_LEN + 1, " ");
			*end = '\0';
			uid = atoi(start);

			start = end + 1;
			from = atoi(start);

			g->remove_player(uid);
			system("cls");
		}
		else if (strncmp(buf, "JOIN ", COMMAND_LEN + 1) == 0)
		{
			//UID name color position points
			int uid, color, position, points;
			char name[NICKNAME_LEN];
			memset(name, '\0', NICKNAME_LEN);

			char* start = buf + COMMAND_LEN + 1;
			char* end = strstr(buf + COMMAND_LEN + 1, " ");
			*end = '\0';
			uid = atoi(start);

			start = end + 1;
			end = strstr(start, " ");
			*end = '\0';
			strncpy(name, start, NICKNAME_LEN - 1);

			start = end + 1;
			end = strstr(start, " ");
			*end = '\0';
			color = atoi(start);

			start = end + 1;
			end = strstr(start, " ");
			*end = '\0';
			position = atoi(start);

			start = end + 1;
			points = atoi(start);


			player* pl = new player(name);
			pl->set_uid(uid);
			pl->set_color(color);
			pl->set_points(points);
			g->add_player_to_node(pl, position);
		}
		else if (strncmp(buf, "PNTS ", COMMAND_LEN + 1) == 0)
		{
			int uid, points;
			char* start = buf + COMMAND_LEN + 1;
			char* end = strstr(buf + COMMAND_LEN + 1, " ");
			*end = '\0';
			uid = atoi(start);

			start = end + 1;
			points = atoi(start);
			for (auto it = g->get_players_iterator_begin(); it != g->get_players_iterator_end(); ++it) {
				if (it->first->get_uid() == uid)
				{
					it->first->set_points(points);
					break;
				}
			}
			g->output_stat();
		}

		COORD coord;
		coord.X = 0;
		coord.Y = 0;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		//g->output_map();
		//g->output_stat();
	}

	delete g;
	closesocket(s);
	free(data);

	return 0;
}

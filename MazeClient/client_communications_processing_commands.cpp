#include "client_communications.h"

int process_start_command(SOCKET s, wstring message, int uid)
{
	//подготовка сокета----------------------------------------------------
	SOCKET s_notifications = socket(AF_INET, SOCK_STREAM, 0);
	if (WSAGetLastError() != 0) {
		wcout << L"Creating socket error: " << WSAGetLastError() << endl;
		system("pause");
		return -1;
	}

	sockaddr_in tcpaddr;
	tcpaddr.sin_family = AF_INET;
	tcpaddr.sin_port = htons(1007);
	tcpaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int size = sizeof(tcpaddr);

	bind(s_notifications, (SOCKADDR*)&tcpaddr, sizeof(tcpaddr));
	if (WSAGetLastError() != 0) {
		wcout << L"Binding error:" << WSAGetLastError() << endl;
		goto error_end;
	}

	listen(s_notifications, 5);

	//подготовка команды --------------------------------------
	int result = 0;
	system("cls");
	int game_number;
	try	{
		game_number = stoi(message.substr(6));
	}
	catch(invalid_argument)
	{
		wcout << L"Can not recognize the number of game" << endl;
		goto error_end;
	}
	catch(out_of_range)
	{
		wcout << L"Can not use this number of game (too big)" << endl;
		goto error_end;
	}

	//отправляем команду STAR
	char buf[MSG_SIZE];
	memset(buf, '\0', MSG_SIZE);
	sprintf(buf, "STAR %d\r\n", game_number);
	result = send_command(s, buf, strlen(buf));
	if (result < 0)
	{
		wcout << L"Connection error" << endl;
		goto error_end;
	}

	
	SOCKET fd = accept(s_notifications, (sockaddr*)&tcpaddr, &size);

	result = get_command(s, buf, MSG_SIZE);
	if (result < 0) {
		wcout << L"Connection error" << endl;
		closesocket(fd);
		goto error_end;
	}

	if (!is_ok_command(buf, strlen(buf)))	{
		wcout << buf;
		closesocket(fd);
		goto error_end;
	}

	game* g = new game(1);

	//считать карту------------------------------------------------------------------------------
	result = get_command(s, buf, MSG_SIZE);
	if (result < 0)
		goto connection_error;

	char intermediate_buf[MSG_SIZE];
	int nodes_count, level_size;
	sscanf(buf, "%s %d %d", intermediate_buf, &nodes_count, &level_size);
	g->set_level_size(level_size);
	for (int i = 0; i < nodes_count; i++) {
		int num, up, down, left, right, up_left, up_right, down_left, down_right;
		result = get_command(s, buf, MSG_SIZE);
		if (result < 0)
			goto connection_error;
		sscanf(buf, "%d %d %d %d %d %d %d %d %d", &num, &up, &down, &left, &right, &up_left, &up_right, &down_left, &down_right);
		g->add_node(new node(num, up, down, left, right, up_left, up_right, down_left, down_right));
	}

	//считать игроков -------------------------------------------------------------
	result = get_command(s, buf, MSG_SIZE);
	if (result < 0)
		goto connection_error;

	//считать игроков
	int start_position;
	int players_count;
	sscanf(buf, "%s %d", intermediate_buf, &players_count);
	for (int i = 0; i < players_count; i++)
	{
		int id, color, position;
		char name[NICKNAME_LEN];
		result = get_command(s, buf, MSG_SIZE);
		if (result < 0)
			goto connection_error;
		sscanf(buf, "%d %s %d %d", &id, name, &color, &position);
		player* pl = new player(name);
		pl->set_uid(id);
		pl->set_color(color);
		g->add_player_to_node(pl, position);

		if (id == uid)
			start_position = position;
	}
	g->output_map();

	notifications_data* data = (notifications_data*)malloc(sizeof(notifications_data));
	data->s = fd;
	data->g = g;
	if (CreateThread(NULL, 0, process_notifications, data, 0, NULL) == NULL) {
		wcout << L"ER 401 Internal game error. Connection will be closed." << endl;
		closesocket(fd);
		goto error_end;
	}

	process_game_commands(s, uid, start_position);

	closesocket(s_notifications);

	memset(buf, '\0', MSG_SIZE);
	strncpy(buf, "ENDG\r\n", MSG_SIZE);
	send_command(s, buf, strlen(buf));
	get_command(s, buf, MSG_SIZE);

	//удалить игроков с карты!
	return 0;


connection_error:
	wcout << L"Connection error" << endl;
	delete g;
	memset(buf, '\0', MSG_SIZE);
	strncpy(buf, "ENDG\r\n", MSG_SIZE);
	send_command(s, buf, strlen(buf));
	get_command(s, buf, MSG_SIZE);
error_end:
	closesocket(s_notifications);
	system("pause");
	return -1;
}


void process_game_commands(SOCKET s, int uid, int start_node)
{
	char command;
	char message[MSG_SIZE];

	while (true)
	{
		command = _getch();
		if (iswdigit(command))
		{
			memset(message, '\0', MSG_SIZE);
			if (command == '2')
				sprintf(message, "MOVE %s \r\n", "DOWN");
			else if (command == '8')
				sprintf(message, "MOVE %s \r\n", "UP");
			else if (command == '4')
				sprintf(message, "MOVE %s \r\n", "LEFT");
			else if (command == '6')
				sprintf(message, "MOVE %s \r\n", "RIGHT");
			else if (command == '7')
				sprintf(message, "MOVE %s \r\n", "UPLEFT");
			else if (command == '9')
				sprintf(message, "MOVE %s \r\n", "UPRIGHT");
			else if (command == '1')
				sprintf(message, "MOVE %s \r\n", "DOWNLEFT");
			else if (command == '3')
				sprintf(message, "MOVE %s \r\n", "DOWNRIGHT");
			else
				continue;

			send_command(s, message, strlen(message));
			get_command(s, message, MSG_SIZE);
		}
		else if (command == 'q')
		{
			memset(message, '\0', MSG_SIZE);
			sprintf(message, "ENDG\r\n");
			send_command(s, message, strlen(message));
			get_command(s, message, MSG_SIZE);
			return;
		}
		else if (command == ' ')
		{
			memset(message, '\0', MSG_SIZE);
			sprintf(message, "SHOT\r\n");
			send_command(s, message, strlen(message));
		}
	}
}

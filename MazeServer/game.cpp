#include "game.h"


game::game(int _level_size) {
	level_size = _level_size;
	max_players_count = STANDARD_PLAYERS_COUNT_ON_MAP;

	set_available_colors();
	name = (char*)malloc(sizeof(char)*(GAMENAME_LEN + 1));
	memset(name, '\0', GAMENAME_LEN + 1);
}
game::~game() {
	free(name);
}

void game::set_game_name(char* _name) {
	strncpy(name, _name, GAMENAME_LEN);
}

char* game::get_game_name() {
	return name;
}

//int game::get_id() {
//	return id;
//}
int game::get_level_size() {
	return level_size;
}

void game::set_level_size(int size)
{
	level_size = size;
}

void game::add_node(node* _node) {
	if (_node == nullptr || _node->get_id() != get_nodes_count())
		throw "Incorrect numbering of node";
	nodes.push_back(_node);

}
//получить узел по индексу
node* game::get_node(int index) {
	return nodes.at(index);
}
//получить число узлов в карте
int game::get_nodes_count() {
	return nodes.size();
}

void game::output_map()
{
	int level_size = get_level_size();
	int count = get_nodes_count();

	//обход построчно
	for (int i = 0; i < count / level_size; i++)
	{
		for (int k = 0; k < 5; k++) {
			for (int j = 0; j < level_size; j++) {
				if (j == 0) {
					if (level_size > 1) {
						output_node_row(get_node(i*level_size + j), nullptr, get_node(i*level_size + j + 1), k,
							get_players_at_node(i*level_size + j));
					}
					else {
						output_node_row(get_node(i*level_size + j), nullptr, nullptr, k, get_players_at_node(i*level_size + j));
					}
				}
				else if (j == level_size - 1) {
					output_node_row(get_node(i*level_size + j), get_node(i*level_size + j - 1), nullptr, k,
						get_players_at_node(i*level_size + j));
				}
				else {
					output_node_row(get_node(i*level_size + j), get_node(i*level_size + j - 1),
						get_node(i*level_size + j + 1), k, get_players_at_node(i*level_size + j));
				}
			}
			wcout << endl;
		}
	}
}

//╚╗╚═╝ ╚═╝╔╝
//═╝       ╚═
//═╗       ╔═
//═╝╔═╗ ╔═╗╚═
//╔═╝ ║ ║ ╚═╗
//соседи слева и справа даны для того, чтобы корректно отображать диагонали
void game::output_node_row(node* _node, node* _left, node* _right, int row, vector<player*> pls)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)(White));

	//_setmode(_fileno(stdout), _O_U16TEXT);
	//_setmode(_fileno(stdin), _O_U16TEXT);
	//_setmode(_fileno(stderr), _O_U16TEXT);
	switch (row)
	{
	case 0:
		if (_node->get_up_left() != -1)
			wcout << L"═╗╚";
		else if (_left != nullptr && _left->get_up_right() != -1)
			wcout << L"╝╔═";
		else
			wcout << L" ╔═";

		wcout << L"═";

		if (_node->get_up() != -1)
			wcout << L"╝ ╚";
		else
			wcout << L"═══";

		wcout << L"═";

		if (_node->get_up_right() != -1)
			wcout << L"╝╔═";
		else if (_right != nullptr && _right->get_up_left() != -1)
			wcout << L"═╗╚";
		else
			wcout << L"═╗ ";

		break;

	case 1:
		if (_node->get_left() != -1)
			wcout << L"═╝  ";
		else
			wcout << L" ║  ";

		output_players_at_node_one_row(hStdOut, _node->get_id(), true);

		if (_node->get_right() != -1)
			wcout << L"  ╚═";
		else
			wcout << L"  ║ ";

		break;
	case 2:
		if (_node->get_left() != -1)
			wcout << L"═╗  ";
		else
			wcout << L" ║  ";

		output_players_at_node_one_row(hStdOut, _node->get_id(), false);

		if (_node->get_right() != -1)
			wcout << L"  ╔═";
		else
			wcout << L"  ║ ";
		break;

	case 3:
		if (_node->get_down_left() != -1)
			wcout << L"═╝╔";
		else if (_left != nullptr && _left->get_down_right() != -1)
			wcout << L"╗╚═";
		else
			wcout << L" ╚═";

		wcout << L"═";

		if (_node->get_down() != -1)
			wcout << L"╗ ╔";
		else
			wcout << L"═══";

		wcout << L"═";

		if (_node->get_down_right() != -1)
			wcout << L"╗╚═";
		else if (_right != nullptr && _right->get_down_left() != -1)
			wcout << L"═╝╔";
		else
			wcout << L"═╝ ";
		break;

	case 4:
		if (_node->get_down_left() != -1)
			wcout << L"╔═╝";
		else if (_left != nullptr && _left->get_down_right() != -1)
			wcout << L"╚═╗";
		else
			wcout << L"   ";

		wcout << L" ";

		if (_node->get_down() != -1)
			wcout << L"║ ║";
		else
			wcout << L"   ";

		wcout << L" ";

		if (_node->get_down_right() != -1)
			wcout << L"╚═╗";
		else if (_right != nullptr && _right->get_down_left() != -1)
			wcout << L"╔═╝";
		else
			wcout << L"   ";

		break;
	default:
		break;
	}
}

void game::output_players_at_node_one_row(HANDLE hStdOut, int node, bool is_upper_row) {
	vector<player*> pls = get_players_at_node(node);

	switch (is_upper_row) {
	case true: {
		if (pls.size() > 0)
		{
			int size = pls.size();
			switch (size) {
			case 1:
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[0]->get_color()));
				wcout << L"***";
				break;
			case 2:
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[0]->get_color())); //первая строка закрашивается цветом первого игрока
				wcout << L"***";
				break;
			case 3:
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[0]->get_color()));
				wcout << L"*";
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[1]->get_color()));
				wcout << L"*";
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[2]->get_color()));
				wcout << L"*";
				break;
			case 4:
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[0]->get_color()));
				wcout << L"*";
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[1]->get_color()));
				wcout << L"*";
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[2]->get_color()));
				wcout << L"*";
				break;
			case 5:
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[0]->get_color()));
				wcout << L"*";
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[1]->get_color()));
				wcout << L"*";
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[2]->get_color()));
				wcout << L"*";
				break;
			case 6:
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[0]->get_color()));
				wcout << L"*";
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[1]->get_color()));
				wcout << L"*";
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[2]->get_color()));
				wcout << L"*";
				break;
			}
			SetConsoleTextAttribute(hStdOut, (WORD)(White));
		}
		else
			wcout << L"   ";
		break;
	}

	case false: {
		if (pls.size() > 0)
		{
			int size = pls.size();
			switch (size) {
			case 1:
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[0]->get_color()));
				wcout << L"***";
				break;
			case 2:
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[1]->get_color())); //вторая строка закрашивается цветом второго игрока
				wcout << L"***";
				break;
			case 3:
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[0]->get_color()));
				wcout << L"*";
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[1]->get_color()));
				wcout << L"*";
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[2]->get_color()));
				wcout << L"*";
				break;
			case 4:
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[3]->get_color()));
				wcout << L" * ";
				break;
			case 5:
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[3]->get_color()));
				wcout << L"* ";
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[4]->get_color()));
				wcout << L"*";
				break;
			case 6:
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[3]->get_color()));
				wcout << L"*";
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[4]->get_color()));
				wcout << L"*";
				SetConsoleTextAttribute(hStdOut, (WORD)(pls[5]->get_color()));
				wcout << L"*";
				break;
			}
			SetConsoleTextAttribute(hStdOut, (WORD)(White));
		}
		else
			wcout << L"   ";
		break;
	}
	}
}


void game::output_players_at_node(int node_num, int x, int y) {
	int row = node_num / get_level_size();
	int column = node_num % get_level_size();

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = x + 11 * column + 4; //9 - ширина одного узла
	coord.Y = y + 5 * row + 1; // 4 - высота одного узла

	SetConsoleCursorPosition(hStdOut, coord);
	output_players_at_node_one_row(hStdOut, node_num, true);

	coord.Y++;
	SetConsoleCursorPosition(hStdOut, coord);
	output_players_at_node_one_row(hStdOut, node_num, true);

}

void game::output_stat() {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)(White));

	COORD coord;
	coord.X = 0; //9 - ширина одного узла
	coord.Y = (get_nodes_count()/get_level_size())*5; // 4 - высота одного узла
	SetConsoleCursorPosition(hStdOut, coord);

	wcout << L"┌";
	for (int i = 0; i < 5; i++)
		wcout << L"┄";
	wcout << L"┬";
	for (int i = 0; i < NICKNAME_LEN + 1; i++)
		wcout << L"┄";
	wcout << L"┬";
	for (int i = 0; i < 5; i++)
		wcout << L"┄";
	wcout << L"┐" << endl;

	for (auto it = players.begin(); it != players.end(); ++it) {
		wcout << L"┊";
		wcout.width(5);
		wcout << it->first->get_uid();
		wcout << L"┊";
		SetConsoleTextAttribute(hStdOut, (WORD)(it->first->get_color()));
		wcout.width(NICKNAME_LEN + 1);
		wcout << it->first->get_name();
		SetConsoleTextAttribute(hStdOut, (WORD)(White));
		wcout << L"┊";
		wcout.width(5);
		wcout << it->first->get_points();
		wcout << L"┊" << endl;
	}
	wcout << L"└";
	for (int i = 0; i < 5; i++)
		wcout << L"┄";
	wcout << L"┴";
	for (int i = 0; i < NICKNAME_LEN + 1; i++)
		wcout << L"┄";
	wcout << L"┴";
	for (int i = 0; i < 5; i++)
		wcout << L"┄";
	wcout << L"┘" << endl;
}


//получить число игроков на карте
int game::get_players_count() {
	return players.size();
}

int game::get_max_players_count() {
	return max_players_count;
}

map<player*, node*>::iterator game::get_players_iterator_begin() {
	return players.begin();
}
map<player*, node*>::iterator game::get_players_iterator_end() {
	return players.end();
}

//добавить игрока в игру, в случа неуспеха возвращается -1, иначе - 0
int game::add_player(player* pl, int color)
{
	//TODO: добвить точки респауна

	return add_player_to_node(pl, 0, color);
}

int game::add_player_to_node(player* pl, int node_num, int color)
{
	if (players.size() >= max_players_count)
		return -1;

	players.insert_or_assign(pl, get_node(node_num));

	if (pl->get_color() == -1)
	{
		if (color <= 0)
		{
			auto color = pop_available_color();
			pl->set_color(color);
		}
		else {
			pl->set_color(color);
			std::remove(available_colors.begin(), available_colors.end(), color);
			//available_colors.erase(color);
		}
	}
	return 0;
}

int game::remove_player(player* pl)
{
	closesocket(pl->get_socket_notifications());
	pl->set_socket_notifications(-1);

	players.erase(pl);
	available_colors.push_back((ConsoleColor)pl->get_color());
	pl->set_color(-1);

	return 0;
}

int game::remove_player(int uid)
{
	player* pl = nullptr;
	for (auto it = players.begin(); it != players.end(); ++it)
		if (it->first->get_uid() == uid)
			pl = it->first;

	if (pl == nullptr)
		return 0;

	remove_player(pl);
	return 0;
}

int game::set_player_node(player* pl, int new_node)
{
	if (new_node >= get_nodes_count())
		return -1;

	try {
		players[pl] = get_node(new_node);
	}
	catch (exception &e)
	{
		return -1;
	}
	return 0;
}

int game::set_player_node(int uid, int new_node)
{
	for (auto it = players.begin(); it != players.end(); ++it)
		if (it->first->get_uid() == uid)
			return set_player_node(it->first, new_node);
	return -1;
}


node* game::get_player_node(player* pl) {
	return players[pl];
}
node* game::get_player_node(int uid) {
	for (auto it = players.begin(); it != players.end(); ++it)
		if (it->first->get_uid() == uid)
			return it->second;
	return nullptr;
}

int game::notify_players_move(int uid, int from, int to)
{
	char message[MSG_SIZE];
	memset(message, '\0', MSG_SIZE);

	sprintf(message, "MOVE %d %d %d\r\n", uid, from, to);

	for (auto it = players.begin(); it != players.end(); ++it)
		send(it->first->get_socket_notifications(), message, strlen(message), 0);

	return 0;
}

int game::notify_players_quit(int uid, int from)
{
	char message[MSG_SIZE];
	memset(message, '\0', MSG_SIZE);

	sprintf(message, "QUIT %d %d\r\n", uid, from);
	for (auto it = players.begin(); it != players.end(); ++it)
		send(it->first->get_socket_notifications(), message, strlen(message), 0);

	return 0;
}


int game::notify_players_join(player* pl)
{
	char message[MSG_SIZE];
	memset(message, '\0', MSG_SIZE);

	int uid = pl->get_uid();
	node* n = players[pl];
	sprintf(message, "JOIN %d %s %d %d %d\r\n", uid, pl->get_name(), pl->get_color(), n->get_id(), pl->get_points());
	for (auto it = players.begin(); it != players.end(); ++it)
		if (it->first->get_uid() != uid) //уведомление не придет самому игроку
			send(it->first->get_socket_notifications(), message, strlen(message), 0);
	return 0;
}

int game::notify_players_pnts(player* pl)
{
	char message[MSG_SIZE];
	memset(message, '\0', MSG_SIZE);

	int uid = pl->get_uid();
	node* n = players[pl];
	sprintf(message, "PNTS %d %d\r\n", uid, pl->get_points());
	for (auto it = players.begin(); it != players.end(); ++it)
		send(it->first->get_socket_notifications(), message, strlen(message), 0);
	return 0;
}


vector<player*> game::get_players_at_node(int nid)
{
	vector<player*> pls;
	for (auto it = players.begin(); it != players.end(); ++it)
		if (it->second->get_id() == nid)
			pls.push_back(it->first);
	return pls;
}

void game::set_available_colors() {
	for (int i = 2; i <= 14; i++)
		available_colors.push_back((ConsoleColor)i);
}

int game::pop_available_color() {
	auto back = available_colors.back();
	available_colors.pop_back();
	return back;
}
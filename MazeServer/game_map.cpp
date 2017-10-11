#include "game_map.h"

player::player(char* _name){
	name = static_cast<char*>(malloc(NICKNAME_LEN));
	strncpy(name, _name, NICKNAME_LEN);
	name[NICKNAME_LEN] = '\0';
}

player::~player() {
	free(name);
}

char* player::get_name() {
	return name;
}

void player::set_uid(int _uid) {
	uid = _uid;
}

int player::get_uid() {
	return uid;
}


node::node(int _id, int _up , int _down, int _left, int _right,
	int _up_left, int _up_right, int _down_left, int _down_right) {
	id = _id;
	up = _up;
	down = _down;
	left = _left;
	right = _right;
	up_left = _up_left;
	up_right = _up_right;
	down_left = _down_left;
	down_right = _down_right;
}
node::~node() {}

void node::add_player(player* _player) {
	if (_player != nullptr)
		players.push_back(_player);
}

player* node::remove_player(int uid) {
	player* pl = nullptr;

	for (list<player*>::iterator it = players.begin(); it != players.end(); ++it)
		if ((*it)->get_uid() == uid) {
			pl = *it;
			break;
		}
	players.remove(pl);
	return pl;
}

bool node::contains_players() {
	return players.size() > 0;
}


int node::get_id() { return id; }
int node::get_up() { return up; }
int node::get_down() { return down; }
int node::get_left() { return left; }
int node::get_right() { return right; }
int node::get_up_left() { return up_left; }
int node::get_up_right() { return up_right; }
int node::get_down_left() { return down_left; }
int node::get_down_right() { return down_right; }

game_map::game_map(int _id, int _level_size) {
	id = id;
	level_size = _level_size;
}
game_map::~game_map() {}

int game_map::get_id() {
	return id;
}
int game_map::get_level_size() {
	return level_size;
}


void game_map::add_node(node* _node) {
	if (_node == nullptr || _node->get_id() != nodes_count())
		throw "Incorrect numbering of node";
	nodes.push_back(_node);

}
//получить узел по индексу
node* game_map::get_node(int index) {
	return nodes.at(index);
}
//получить число узлов в карте
int game_map::nodes_count() {
	return nodes.size();
}

void game_map::output_map()
{
	int level_size = get_level_size();
	int count = nodes_count();

	//обход построчно
	for (int i = 0; i < count / level_size; i++)
	{
		for (int k = 0; k < 5; k++) {
			for (int j = 0; j < level_size; j++) {
				if (j == 0) {
					if (level_size > 1) {
						output_node_row(get_node(i*level_size + j), nullptr, get_node(i*level_size + j + 1), k);
					}
					else {
						output_node_row(get_node(i*level_size + j), nullptr, nullptr, k);
					}
				}
				else if (j == level_size - 1) {
					output_node_row(get_node(i*level_size + j), get_node(i*level_size + j - 1), nullptr, k);
				}
				else {
					output_node_row(get_node(i*level_size + j), get_node(i*level_size + j - 1),
						get_node(i*level_size + j + 1), k);
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
void game_map::output_node_row(node* _node, node* _left, node* _right, int row)
{
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
			wcout << L"═╝";
		else
			wcout << L" ║";

		if (_node->contains_players())
			wcout << L"  ***  ";
		else
			wcout << L"       ";

		if (_node->get_right() != -1)
			wcout << L"╚═";
		else
			wcout << L"║ ";

		wcout << L"";
		break;
	case 2:
		if (_node->get_left() != -1)
			wcout << L"═╗";
		else
			wcout << L" ║";

		if (_node->contains_players())
			wcout << L"  ***  ";
		else
			wcout << L"       ";

		if (_node->get_right() != -1)
			wcout << L"╔═";
		else
			wcout << L"║ ";
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

void game_map::initialize(int id)
{
	level_size = 3;
	add_node(new node(0, -1, 3, -1, -1, -1, -1, -1, 4));
	add_node(new node(1, -1, 4, -1, 2));
	add_node(new node(2, -1, 5, 1));
	add_node(new node(3, 0, 6));
	add_node(new node(4, 1, -1, -1, -1, 0, -1, -1, 8));
	add_node(new node(5, 2, 8));
	add_node(new node(6, 3, 9, -1, 7));
	add_node(new node(7, -1, -1, 6, 8, -1, -1, 9, 11));
	add_node(new node(8, 5, 11, 7, -1, 4));
	add_node(new node(9, 6, -1, -1, 10, -1, 7));
	add_node(new node(10, -1, -1, 9, 11));
	add_node(new node(11, 8, -1, 10, -1, 7));
}
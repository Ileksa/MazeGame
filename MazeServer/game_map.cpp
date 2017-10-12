#include "game_map.h"


game_map::game_map(int _level_size) {
	//id = id;
	level_size = _level_size;
	players_count = 0;
	max_players_count = STANDARD_PLAYERS_COUNT_ON_MAP;
}
game_map::~game_map() {}

//int game_map::get_id() {
//	return id;
//}
int game_map::get_level_size() {
	return level_size;
}


void game_map::add_node(node* _node) {
	if (_node == nullptr || _node->get_id() != get_nodes_count())
		throw "Incorrect numbering of node";
	nodes.push_back(_node);

}
//получить узел по индексу
node* game_map::get_node(int index) {
	return nodes.at(index);
}
//получить число узлов в карте
int game_map::get_nodes_count() {
	return nodes.size();
}

void game_map::output_map()
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

//получить число игроков на карте
int game_map::get_players_count() {
	return players_count;
}

//добавить игрока в игру, в случа неуспеха возвращается -1, иначе - 0
int game_map::add_player(player* pl)
{
	if (players_count >= max_players_count)
		return -1;

	players_count++;
	get_node(0)->add_player(pl);
	return 0;
}

int game_map::remove_player(player* pl)
{
	reset_players_iterator();
	player* p = current_player();
	while (p->get_uid() != pl->get_uid() && move_next()) {
		p = current_player();
	}
	
	if (p->get_uid() == pl->get_uid())
	{
		get_node(index_node)->remove_player(p->get_uid());

		players_count--;
		return 0;
	}
	return -1;
}

//
//protected:
//	int index_in_node = -1;
//	int index_node = -1;
//public:
//возвращает игро
player* game_map::current_player()
{
	return get_node(index_node)->get_player(index_in_node);
}

node* game_map::current_node() {
	return get_node(index_node);
}
//возвращает true, если удалось успешно передвинуться
bool game_map::move_next()
{
	index_in_node++;
	if (index_in_node == get_node(index_node)->get_players_count())
	{
		index_node++;
		index_in_node = 0;
	}
	else
		return true;

	for (; index_node < get_nodes_count(); index_node++)
		if (get_node(index_node)->contains_players())
			break;

	if (index_node == get_nodes_count())
		return false;

	return true;

}
//функция устанавливает указатель на первого игрока; если игроков вообще нет, возвращает false
bool game_map::reset_players_iterator()
{
	index_in_node = 0;
	index_node = 0;
	while (index_node < get_nodes_count() && !get_node(index_node)->contains_players())
		index_node++;
	
	if (index_node == get_nodes_count())
		return false;
	return true;
}
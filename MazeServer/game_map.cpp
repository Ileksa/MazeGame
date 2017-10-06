#include "game_map.h"

int player::get_uid() {
	return uid;
}

void player:: set_uid(int _uid) {
	uid = _uid;
}

player::player(int _uid) {
	uid = _uid;
}

player::~player() {}

node::node(int _id, int _up, int _down, int _left, int _right,
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

void node::add_player(player* _player){
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

int node::get_id() {}
int node::get_up() { return up; }
int node::get_down() { return down; }
int node::get_left() { return left; }
int node::get_right() { return right; }
int node::get_up_left() { return up_left; }
int node::get_up_right() { return up_right; }
int node::get_down_left() { return down_left; }
int node::get_down_right() { return down_right; }

game_map::game_map(int _id) {
	id = id;
}
game_map::~game_map() {}

int game_map::get_id() {
	return id;
}

void add_node(node* _node) {
	if (_node == nullptr || _node->)
}
//получить узел по индексу
node* get_node(int index);
//получить число узлов в карте
int node_count();
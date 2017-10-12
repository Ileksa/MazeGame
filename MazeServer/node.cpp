#include "node.h"
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

void node::add_player(int uid) {
	players.push_back(uid);
}

void node::remove_player(int uid) {
	players.remove(uid);
}

bool node::contains_players() {
	return players.size() > 0;
}

bool node::contains_player(int uid) {
	for (list<int>::iterator it = players.begin(); it != players.end(); ++it)
		if (*it == uid)
			return true;
	return false;
}


int node::get_players_count() {
	return players.size();
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
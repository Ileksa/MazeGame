#include "player.h"
player::player(char* _name) {
	name = static_cast<char*>(malloc(NICKNAME_LEN + 1));
	strncpy(name, _name, NICKNAME_LEN + 1);
	name[NICKNAME_LEN] = '\0';

	color = -1;
	socket_notifications = -1;
	set_points(0);
	//free(name);
}

player::~player() {
	wcout << L"[DEBUG] player dctor" << endl;
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

int player::get_color() {
	return color;
}
void player::set_color(int _color) {
	color = _color;
}

void player::set_socket_notifications(SOCKET s) {
	socket_notifications = s;
}
SOCKET player::get_socket_notifications() {
	return socket_notifications;
}

void player::add_point() {
	points++;
}

void player::set_points(int value) {
	points = value;
}

int player::get_points() {
	return points;
}
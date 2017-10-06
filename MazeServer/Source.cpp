#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "game_map.h"
using namespace std;

int main() {


	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);

	//wcout << L"╚\n";

	

	system("pause");
	return 0;
}
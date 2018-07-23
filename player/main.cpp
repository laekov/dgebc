#include "../engine/engine.h"
#include <iostream>
using namespace std;

int main()
{
	DGEBC::Engine e;
	cout << "score: " << e.score("233") << endl;
	cout << "combine: " << e.combine("233", "666") << endl;
	cout << "mutate: " << e.mutate("233") << endl;
	return 0;
}

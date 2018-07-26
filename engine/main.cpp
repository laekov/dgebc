#include <QDebug>
#include <cstdlib>
#include "engine.h"

int main()
{
	srand(time(0));
	while(1)
	{
		DGEBC::Engine e;
		std::string gene = e.initial();
		// qDebug() << gene.c_str();
		qDebug() << e.score(gene);
	}
}

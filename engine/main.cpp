#include <QDebug>

#include "engine.h"

int main()
{
	DGEBC::Engine e;
	std::string gene = e.initial();
	qDebug() << gene.c_str();
	qDebug() << e.score(gene);
}

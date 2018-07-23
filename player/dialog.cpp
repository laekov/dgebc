#include "dialog.h"
#include <QVBoxLayout>
#include <QPushButton>

Dialog::Dialog(QWidget *parent): QDialog(parent)
{
	gl = new QGridLayout();
	rebuildLayout();
	QVBoxLayout *lo = new QVBoxLayout();
	QPushButton *btn = new QPushButton("Add");
	lo->addLayout(gl);
	lo->addWidget(btn);
	setLayout(lo);
	connect(btn, &QPushButton::clicked, [this]()
	{
		std::string gene = getGene();
		sons.push_back(engine.visualize(gene));
		rebuildLayout();
	});
}

void Dialog::rebuildLayout()
{
	for(QLayout *w: sons)
		gl->removeItem(w);
	
	int n = sons.size();
	int m = ceil(sqrt(n));
	for(int i = 0; i < n; i++)
		gl->addLayout(sons[i], i / m, i % m);
}

std::string Dialog::getGene()
{
	return engine.initial();
}

Dialog::~Dialog()
{
}

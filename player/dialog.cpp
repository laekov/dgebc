#include "dialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QTimer>
#include "../engine/mainwindow.h"

Dialog::Dialog(QWidget *parent): QDialog(parent)
{
	n = QInputDialog::getInt(NULL, "player", "Rows:", 3, 1, 10);
	m = QInputDialog::getInt(NULL, "player", "Columns:", 4, 1, 10);
	gl = new QGridLayout();
	for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
		{
			QLayout *lo = engine.visualize(getGene());
			sons << lo;
			gl->addLayout(lo, i, j);
		}
	rebuildLayout();
	QVBoxLayout *lo = new QVBoxLayout();
	QPushButton *btn = new QPushButton("Add");
	lo->addLayout(gl);
	lo->addWidget(btn);
	setLayout(lo);
	connect(btn, &QPushButton::clicked, [this]()
	{
		rebuildLayout();
	});
	QTimer *timer = new QTimer(this);
	connect(timer, &QTimer::timeout, [this]()
	{
		rebuildLayout();
	});
	timer->start(100);
}

void Dialog::rebuildLayout()
{
	for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
		{
			QLayout *&lo = sons[i * m + j];
			MainWindow *mw = (MainWindow *) lo->itemAt(0)->widget();
			if(mw->finished)
			{
				mw->finished = 0;
				mw->setGene(getGene());
			}
			// if(!lo || !lo->count())
			// {
				// gl->removeItem(lo);
				// delete lo;
				// lo = 
				// gl->addLayout(lo, i, j);
				
				// break;
				
			// }
		}
}

std::string Dialog::getGene()
{
	return engine.initial();
}

Dialog::~Dialog()
{
}

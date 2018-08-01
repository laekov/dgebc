#include "dialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QTimer>
#include <algorithm>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "../engine/mainwindow.h"

Dialog::Dialog(QWidget *parent): QDialog(parent)
{
	server = QUrl(QInputDialog::getText(NULL, "player", "Server address:", QLineEdit::Normal, "http://e1.sc.team:2333/get_gene"));
	QNetworkReply *reply = qnm.get(QNetworkRequest(server));
	waiting = 1;
	connect(reply, &QNetworkReply::finished, [this, reply]()
	{
		genes.clear();
		QStringList entries = QString(reply->readAll()).split(";", QString::SkipEmptyParts);
		for(const QString &s: entries)
		{
			QStringList subs = s.split(",", QString::SkipEmptyParts);
			for(int i = 1; i + 1 < subs.size(); i += 2)
			{
				std::string gene = subs[i].toStdString();
				double score = subs[i + 1].toDouble();
				genes << qMakePair(score, gene);
			}
		}
		waiting = 0;
		std::sort(genes.begin(), genes.end());
		std::reverse(genes.begin(), genes.end());
		rebuildLayout();
	});
	n = QInputDialog::getInt(NULL, "player", "Rows:", 4, 1, 10);
	m = QInputDialog::getInt(NULL, "player", "Columns:", 6, 1, 10);
	gl = new QGridLayout();
	for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
		{
			QLayout *lo = engine.visualize("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
			sons << lo;
			gl->addLayout(lo, i, j);
		}
	rebuildLayout();
	QVBoxLayout *lo = new QVBoxLayout();
	QPushButton *btn = new QPushButton("Reset");
	lo->addLayout(gl);
	lo->addWidget(btn);
	setLayout(lo);
	connect(btn, &QPushButton::clicked, [this]()
	{
		rebuildLayout(true);
	});
	QTimer *timer = new QTimer(this);
	connect(timer, &QTimer::timeout, [this]()
	{
		rebuildLayout();
	});
	timer->start(100);
}

void Dialog::rebuildLayout(bool force)
{
	for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
		{
			if(!genes.size()) break;
			int id = i * m + j;
			QLayout *&lo = sons[id];
			MainWindow *mw = (MainWindow *) lo->itemAt(0)->widget();
			if(mw->finished || force)
			{
				mw->finished = 0;
				mw->setGene(getGene(i, j));
			}
		}
	if(genes.size() < 10 && !waiting)
	{
		QNetworkReply *reply = qnm.get(QNetworkRequest(server));
		waiting = 1;
		connect(reply, &QNetworkReply::finished, [this, reply]()
		{
			genes.clear();
			QStringList entries = QString(reply->readAll()).split(";", QString::SkipEmptyParts);
			for(const QString &s: entries)
			{
				QStringList subs = s.split(",", QString::SkipEmptyParts);
				for(int i = 1; i + 1 < subs.size(); i += 2)
				{
					std::string gene = subs[i].toStdString();
					double score = subs[i + 1].toDouble();
					// qDebug() << score << gene.c_str();
					genes << qMakePair(score, gene);
				}
			}
			waiting = 0;
			std::sort(genes.begin(), genes.end());
			std::reverse(genes.begin(), genes.end());
			rebuildLayout();
		});
	}
}

std::string Dialog::getGene(int i, int j)
{
	auto it = genes.begin() + genes.size() * (i * m + j) / n / m;
	std::string ans = it->second;
	genes.erase(it);
	return ans;
}

Dialog::~Dialog()
{
}

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QList>
#include <QGridLayout>
#include <QNetworkAccessManager>
#include <QUrl>
#include <string>
#include "../engine/engine.h"

class Dialog: public QDialog
{
	Q_OBJECT
public:
	Dialog(QWidget *parent = 0);
	~Dialog();
private:
	QUrl server;
	QNetworkAccessManager qnm;
	bool waiting;
	int n, m;
	QList<QLayout *> sons;
	QVector<QPair<double, std::string> > genes;
	QGridLayout *gl;
	DGEBC::Engine engine;
	void rebuildLayout(bool force = false);
	std::string getGene(int i, int j);
};

#endif // !DIALOG_H

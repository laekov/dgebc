#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QList>
#include <QGridLayout>
#include <string>
#include "../engine/engine.h"

class Dialog: public QDialog
{
	Q_OBJECT
public:
	Dialog(QWidget *parent = 0);
	~Dialog();
private:
	QList<QLayout *> sons;
	QGridLayout *gl;
	DGEBC::Engine engine;
	void rebuildLayout();
	std::string getGene();
};

#endif // !DIALOG_H

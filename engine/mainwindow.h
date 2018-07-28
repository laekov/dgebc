#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#  include <QtWidgets>
#else
#  include <QtGui>
#endif

#include "render.h"
#include "world.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(std::string gene, QWidget *parent = 0);
    ~MainWindow();

signals:
    
public slots:

private:
    static const int SPEED_PAUSE = 0;
    static const int SPEED_SLOW = 1;
    static const int SPEED_NORMAL = 2;
    static const int SPEED_FAST = 4;
    static const int SPEED_VERY_FAST = 8;
    static const int SPEED_SUPER_FAST = 16;
    static const int SPEED_SUPER_DUPER_FAST = 1024;

    Render *render;
    int speed;
    World *world;

    void createMenu();
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *e);

signals:
    void changeSpeed(int);

private slots:
    void setSpeed(int newSpeed);
    void showAbout();

};

#endif // MAINWINDOW_H

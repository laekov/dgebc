#include "mainwindow.h"

#include "render.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {
    world = new World();
    render = new Render(world, this);
    connect(world, SIGNAL(creteNewCar()), render, SLOT(createCarCallList()));
    connect(world, SIGNAL(freeCallListNumber(uint)), render,
            SLOT(deleteCallList(uint)));
    setCentralWidget(render);
    resize(800, 600);
    setWindowTitle(tr("Carbox2d"));
    createMenu();
    speed = SPEED_NORMAL;
    startTimer(1000/30);
    render->updateGL();
    render->createCarCallList();
}

MainWindow::~MainWindow() {
    world->deleteLater();
    render->deleteLater();
}

//private

void MainWindow::createMenu() {
    QMenuBar *menuBar = new QMenuBar();
    QMenu *speedMenu = new QMenu(tr("S&peed"));
    QSignalMapper *speedSignalMapper = new QSignalMapper(this);
    QActionGroup *speedGroup = new QActionGroup(this);
    QAction* action = speedMenu->addAction(tr("&Pause"),
                                           speedSignalMapper, SLOT(map()));
    speedSignalMapper->setMapping(action, SPEED_PAUSE);
    speedGroup->addAction(action);
    action->setCheckable(true);
    action = speedMenu->addAction(tr("&Slow (0.5x)"),
                                           speedSignalMapper, SLOT(map()));
    speedSignalMapper->setMapping(action, SPEED_SLOW);
    speedGroup->addAction(action);
    action->setCheckable(true);
    action = speedMenu->addAction(tr("&Normal"),
                                  speedSignalMapper, SLOT(map()));
    speedGroup->addAction(action);
    action->setCheckable(true);
    action->setChecked(true);
    speedSignalMapper->setMapping(action, SPEED_NORMAL);
    action = speedMenu->addAction(tr("&Fast (2x)"),
                                  speedSignalMapper, SLOT(map()));
    speedGroup->addAction(action);
    action->setCheckable(true);
    speedSignalMapper->setMapping(action, SPEED_FAST);
    action = speedMenu->addAction(tr("&Very fast (4x)"),
                                  speedSignalMapper, SLOT(map()));
    speedGroup->addAction(action);
    action->setCheckable(true);
    speedSignalMapper->setMapping(action, SPEED_VERY_FAST);
    action = speedMenu->addAction(tr("S&uper fast (8x)"),
                                  speedSignalMapper, SLOT(map()));
    speedGroup->addAction(action);
    action->setCheckable(true);
    speedSignalMapper->setMapping(action, SPEED_SUPER_FAST);
    action = speedMenu->addAction(tr("Super &duper fast!!!"),
                                  speedSignalMapper, SLOT(map()));
    speedGroup->addAction(action);
    action->setCheckable(true);
    speedSignalMapper->setMapping(action, SPEED_SUPER_DUPER_FAST);
    QMenu *help = new QMenu(tr("&Help"));
    help->addAction(tr("&About Carbox2d"), this, SLOT(showAbout()));
    help->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
    menuBar->addMenu(speedMenu);
    menuBar->addMenu(help);
    this->setMenuBar(menuBar);
    connect(speedSignalMapper, SIGNAL(mapped(int)),
            this, SLOT(setSpeed(int)));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    event->key();
}

void MainWindow::timerEvent(QTimerEvent *e __attribute__ ((unused))) {
    for (int i = 0; i < speed; i++)
        world->step();
    render->updateGL();
}

//private slots

void MainWindow::setSpeed(int newSpeed) {
    speed = newSpeed;
    render->setSpeed(speed);
}

void MainWindow::showAbout() {
    QMessageBox::about(this, tr("About carbox2d"),
                       tr("<h2>Carbox2d 0.2.3</h2>"
                          "<p>Evolution simulator like "
                          "<a href='http://boxcar2d.com/'>http://boxcar2d.com/</a></p>"
                          "<p>Copyright &copy; 2012 Zagidulin Artyom</p>"
                          "<p>This program is free software: you can redistribute it and/or modify "
                          "it under the terms of the GNU General Public License as published by "
                          "the Free Software Foundation, either version 3 of the License, or "
                          "(at your option) any later version.</p>"
                          "<p>This program is distributed in the hope that it will be useful, "
                          "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                          "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
                          "GNU General Public License for more details.</p>"
                          "<p>You should have received a copy of the GNU General Public License "
                          "along with this program.  If not, see <a href='http://www.gnu.org/licenses'>"
                          "http://www.gnu.org/licenses</a>.</p>"
                          "<p>Get source code: <a href='https://github.com/macrosoft/carbox2d'>https://github.com/macrosoft/carbox2d</a></p>"
                          "<p>Feedback: <a href='mailto:p0chta@yandex.ru'>p0chta@yandex.ru</a></p>"));
}

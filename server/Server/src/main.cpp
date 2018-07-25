#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QString>
#include "httplistener.h"
#include "httprequesthandler.h"
#include "server.h"

using namespace stefanfrings;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // load configruation file
    QSettings *listenerSettings = new QSettings("../Server/etc/config.ini", QSettings::IniFormat, &app);
    listenerSettings->beginGroup("listener");

    // start HTTP server
    HttpListener *listener = new HttpListener(listenerSettings, new Server(&app), &app);

    return app.exec();
}

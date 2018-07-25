#ifndef SERVER_H
#define SERVER_H

#include <QMutex>
#include <QNetworkAccessManager>
#include <QTimer>
#include "httprequesthandler.h"
#include "worker.h"

using namespace stefanfrings;

class Server : public HttpRequestHandler
{
    Q_OBJECT

public:
    // constructor
    Server(QObject *parent = 0);
    ~Server();

    // override response method
    void service(HttpRequest &request, HttpResponse &response);

    // constant time interval
    static const int ONE_MINUTE = 60000;
    static const int ONE_SECOND = 1000;

private:
    // http
    QNetworkAccessManager *manager;
    QTimer *timer;

    // active workers
    QMutex mutex;
    QMap<QUrl, Worker> allActiveWorkers;

private slots:
    void heartBeat();
//    void killDeadWorker(QUrl url);
//    void keepActiveWorker(QUrl url);
};

#endif // SERVER_H

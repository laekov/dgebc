#ifndef SERVER_H
#define SERVER_H

#include "httprequesthandler.h"
#include "worker.h"

using namespace stefanfrings;

class Server : public HttpRequestHandler
{
    Q_OBJECT

public:
    // constructor
    Server(QObject *parent = 0);

    // override response method
    void service(HttpRequest &request, HttpResponse &response);

private:
    // active workers
    QMap<QUrl, Worker> allActiveWorkers;
};

#endif // SERVER_H

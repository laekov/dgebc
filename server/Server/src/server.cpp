#include <iostream>
#include <QMapIterator>
#include "server.h"

Server::Server(QObject *parent) : HttpRequestHandler(parent)
{

}

void Server::service(HttpRequest &request, HttpResponse &response)
{
    QString path = request.getPath().data();

    if (path == "/register_worker")
    {
        // a new worker emerges
        QString sourceAddress = request.getPeerAddress().toString();
        QUrl url = QUrl(sourceAddress.split(":").takeLast());
        Worker w = Worker(url);
        allActiveWorkers.insert(url, w);
    }
    else if (path == "/get_workers")
    {
        for (QMap<QUrl, Worker>::iterator it = allActiveWorkers.begin(); it != allActiveWorkers.end(); it++)
        {
            response.write(QString("%1,").arg(QString(it.value().url.toString())).toLatin1(), false);
        }

        response.write("", true);
    }
    else if (path == "/dump")
    {
        QString sourceAddress = request.getPeerAddress().toString();
        QUrl url = QUrl(sourceAddress.split(":").takeLast());
        QByteArray gene = request.getParameter(QString("gene").toLatin1());
        QByteArray score = request.getParameter(QString("score").toLatin1());

        if (allActiveWorkers.contains(url))
        {
            // update an old worker
            Worker &w = allActiveWorkers[url];
            w.gene = gene;
            w.score = score;
        }
        else
        {
            // a new worker emerges
            Worker w = Worker(url, gene, score);
            allActiveWorkers.insert(url, w);
        }
    }
    else if (path == "/get_gene")
    {
        for (QMap<QUrl, Worker>::iterator it = allActiveWorkers.begin(); it != allActiveWorkers.end(); it++)
        {
            response.write(QString("%1,%2,%3;")
                           .arg(QString(it.value().url.toString()))
                           .arg(QString(it.value().gene))
                           .arg(QString(it.value().score)).toLatin1(), false);
        }

        response.write("", true);
    }
    else if (path == "/debug")
    {
        for (QMap<QUrl, Worker>::iterator it = allActiveWorkers.begin(); it != allActiveWorkers.end(); it++)
        {
            response.write(QString("ip = %1,    gene = %2,    score = %3\n")
                           .arg(QString(it.value().url.toString()))
                           .arg(QString(it.value().gene))
                           .arg(QString(it.value().score)).toLatin1(), false);
        }

        response.write("", true);
    }
    else
    {
        response.write("Server: unknown url", true);
    }
}

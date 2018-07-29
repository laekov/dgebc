#include <iostream>
#include <QEventLoop>
#include <QMapIterator>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "server.h"

Server::Server(QObject *parent) : HttpRequestHandler(parent)
{
    manager = new QNetworkAccessManager(this);

    timer = new QTimer(this);
    timer->setSingleShot(false);
    connect(timer, SIGNAL(timeout()), this, SLOT(heartBeat()));
    timer->start(10000);

    mutex.unlock();
}

Server::~Server()
{
    delete manager;
    delete timer;
}

void Server::service(HttpRequest &request, HttpResponse &response)
{
    QString path = request.getPath().data();

    if (path == "/register_worker")
    {
        // a new worker emerges
        QString sourceAddress = request.getPeerAddress().toString();
		QString addr = sourceAddress.split(":").takeLast();
        QString port = QString(
				request.getParameter(QString("port").toLatin1()));
        QUrl url = QUrl();
		url.setHost(addr);
		url.setPort(port.toInt());
		std::cerr << url.port() << "\n";
        Worker w = Worker(url);
        mutex.lock();
        allActiveWorkers.insert(url, w);
        mutex.unlock();
    }
    else if (path == "/get_workers")
    {
        mutex.lock();

        for (QMap<QUrl, Worker>::iterator it = allActiveWorkers.begin(); it != allActiveWorkers.end(); it++)
        {
            response.write(QString("%1,").arg(QString(it.value().url.toString())).toLatin1(), false);
        }

        mutex.unlock();
        response.write("", true);
    }
    else if (path == "/dump")
    {
        QString sourceAddress = request.getPeerAddress().toString();
        QUrl url = QUrl(sourceAddress.split(":").takeLast());
        QByteArray gene = request.getParameter(QString("gene").toLatin1());
        QByteArray score = request.getParameter(QString("score").toLatin1());

        mutex.lock();

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

        mutex.unlock();
    }
    else if (path == "/get_gene")
    {
        mutex.lock();

        for (QMap<QUrl, Worker>::iterator it = allActiveWorkers.begin(); it != allActiveWorkers.end(); it++)
        {
            response.write(QString("%1,%2,%3;")
                           .arg(QString(it.value().url.toString()))
                           .arg(QString(it.value().gene))
                           .arg(QString(it.value().score)).toLatin1(), false);
        }

        mutex.unlock();
        response.write("", true);
    }
    else if (path == "/debug")
    {
        mutex.lock();

        for (QMap<QUrl, Worker>::iterator it = allActiveWorkers.begin(); it != allActiveWorkers.end(); it++)
        {
            response.write(QString("ip = %1,    gene = %2,    score = %3\n")
                           .arg(QString(it.value().url.toString()))
                           .arg(QString(it.value().gene))
                           .arg(QString(it.value().score)).toLatin1(), false);
        }

        mutex.unlock();
        response.write("", true);
    }
    else
    {
        response.write("Server: unknown url", true);
    }
}

void Server::heartBeat()
{
    qDebug() << "Server: heart beat size" << allActiveWorkers.size();

    QVector<QUrl> toDelete;

    for (QMap<QUrl, Worker>::iterator it = allActiveWorkers.begin(); it != allActiveWorkers.end(); it++)
    {
        qDebug() << "Server: heart beat @" << it.key().toString();

        QUrl url = QUrl(it.key().toString() + "/status");
        QNetworkRequest request;
        request.setUrl(QUrl(url));

        QNetworkReply *pReply = manager->get(request);
        QTimer stimer;
        stimer.setSingleShot(true);
        QEventLoop loop;
        connect(&stimer, &QTimer::timeout, &loop, &QEventLoop::quit);
        connect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        stimer.start(ONE_SECOND);
        loop.exec();

        if (stimer.isActive())
        {
            stimer.stop();

            if (pReply->readAll() != "233")
            {
                // timeout == dead
                qDebug() << "Server: kill worker (error)" << it.key().toString();
                toDelete.append(it.key());
                disconnect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
                pReply->abort();
            }
            else
            {
                qDebug() << "Server: worker" << it.key().toString() << "is active";
            }
        }
        else
        {
            // timeout == dead
            qDebug() << "Server: kill worker (timeout)" << it.key().toString();
            toDelete.append(it.key());
            disconnect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            pReply->abort();
        }

        pReply->deleteLater();
    }

    mutex.lock();

    foreach (QUrl url, toDelete)
    {
        allActiveWorkers.remove(url);
    }

    mutex.unlock();
}

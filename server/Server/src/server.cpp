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
    timer->start(ONE_MINUTE);

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
        QString port = QString(request.getParameter(QString("port").toLatin1()));
        QUrl url = QUrl();
        url.setHost(addr);
        url.setPort(port.toInt());
        qDebug() << "Server: /register_worker" << url;
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
			QUrl url = it.value().url;
			QString content = QString("%1 %2\n").arg(url.host()).arg(url.port());
            response.write(content.toLatin1(), false);
        }

        mutex.unlock();
        response.write("", true);
    }
    else if (path == "/dump")
    {
        QString sourceAddress = request.getPeerAddress().toString();
        QString addr = sourceAddress.split(":").takeLast();
        QString port = QString(request.getParameter(QString("port").toLatin1()));
        QUrl url = QUrl();
        url.setHost(addr);
        url.setPort(port.toInt());
        QByteArray gene = request.getParameter(QString("gene").toLatin1());
        QByteArray score = request.getParameter(QString("score").toLatin1());

        mutex.lock();

        if (allActiveWorkers.contains(url))
        {
            // update an old worker
            qDebug() << "Server: /dump (update active worker)" << url << gene << score;
            Worker &w = allActiveWorkers[url];
            w.appendCar(gene, score);
        }
        else
        {
            // a new worker emerges
            qDebug() << "Server: /dump (register new worker)" << url << gene << score;
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
            response.write(QString("%1:%2,")
                           .arg(QString(it.value().url.toString()))
                           .arg(QString(it.value().speed)).toLatin1(), false);

            foreach (Car c, it.value().cars)
            {
                response.write(QString("%1,%2,")
                               .arg(QString(c.gene))
                               .arg(QString(c.score)).toLatin1(), false);
            }

            response.write(";", false);
        }

        mutex.unlock();
        response.write("", true);
    }
    else if (path == "/debug")
    {
        mutex.lock();

        for (QMap<QUrl, Worker>::iterator it = allActiveWorkers.begin(); it != allActiveWorkers.end(); it++)
        {
            response.write(QString("[ip = %1,    speed = %2]\n")
                           .arg(QString(it.value().url.toString()))
                           .arg(QString(it.value().speed)).toLatin1(), false);

            foreach (Car c, it.value().cars)
            {
                response.write(QString("    gene = %1,    score = %2\n")
                               .arg(QString(c.gene))
                               .arg(QString(c.score)).toLatin1(), false);
            }
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
        QUrl url = QUrl("http:" + it.key().toString() + "/status");
        qDebug() << "Server: heart beat @" << url;

        QNetworkRequest request;
        request.setUrl(QUrl(url));

        QNetworkReply *pReply = manager->get(request);

        QTimer stimer;
        stimer.setSingleShot(true);
        QEventLoop eventLoop;
        connect(&stimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);
        connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
        stimer.start(ONE_SECOND);
        eventLoop.exec();

        if (stimer.isActive())
        {
            stimer.stop();
            QString replyMessage = pReply->readAll();
            qDebug() << "Server: receive response message [" << replyMessage << "]";
            QList<QString> split = replyMessage.split(",");

            if (split[0] != "g")
            {
                // error == dead
                qDebug() << "Server: kill worker (error)" << it.key().toString();
                toDelete.append(it.key());
                disconnect(pReply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
                pReply->abort();
            }
            else
            {
                qDebug() << "Server: worker" << it.key().toString() << "is active, calcaluting" << split[1] << "steps/sec";
                mutex.lock();
                allActiveWorkers[url].speed = split[1].toLatin1();
                mutex.unlock();
            }
        }
        else
        {
            // timeout == dead
            qDebug() << "Server: kill worker (timeout)" << it.key().toString();
            toDelete.append(it.key());
            disconnect(pReply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
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

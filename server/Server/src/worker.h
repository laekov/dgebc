#ifndef WORKER_H
#define WORKER_H

#include <QUrl>

struct Worker
{
    Worker();
    Worker(QUrl u);
    Worker(QUrl u, QByteArray g, QByteArray s);

    QUrl url;
    QByteArray gene;
    QByteArray score;
};

#endif // WORKER_H

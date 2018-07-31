#include "worker.h"

Worker::Worker()
{

}

Worker::Worker(QUrl u) :
    url(u),
    speed(QByteArray("-1"))
{

}

Worker::Worker(QUrl u, QByteArray g, QByteArray s) :
    url(u),
    gene(g),
    score(s),
    speed(QByteArray("-1"))
{

}

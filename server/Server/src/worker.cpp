#include "worker.h"

Worker::Worker()
{

}

Worker::Worker(QUrl u) :
    url(u)
{

}

Worker::Worker(QUrl u, QByteArray g, QByteArray s) :
    url(u),
    gene(g),
    score(s)
{

}

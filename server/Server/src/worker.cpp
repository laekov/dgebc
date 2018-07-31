#include "worker.h"

Car::Car()
{

}

Car::Car(QByteArray g, QByteArray s) :
    gene(g),
    score(s)
{

}

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
    speed(QByteArray("-1"))
{
    cars.append(Car(g, s));
}

void Worker::appendCar(QByteArray g, QByteArray s)
{
    cars.append(Car(g, s));

    if (cars.length() > BUFFER_SIZE)
    {
        cars.removeFirst();
    }
}

void Worker::appendCar(Car c)
{
    cars.append(c);

    if (cars.length() > BUFFER_SIZE)
    {
        cars.removeFirst();
    }
}

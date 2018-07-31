#ifndef WORKER_H
#define WORKER_H

#include <QByteArray>
#include <QList>
#include <QUrl>

struct Car
{
    Car();
    Car(QByteArray g, QByteArray s);

    QByteArray gene;
    QByteArray score;
};

struct Worker
{
    Worker();
    Worker(QUrl u);
    Worker(QUrl u, QByteArray g, QByteArray s);

    void appendCar(Car c);
    void appendCar(QByteArray g, QByteArray s);

    QUrl url;
    QByteArray speed;
    QList<Car> cars;

    static const int BUFFER_SIZE = 100;
};

#endif // WORKER_H

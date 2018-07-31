#include <qmath.h>
#include <QTime>
#include <QDebug>
#include "world.h"

World::World(bool isRender, std::string gene) {
	isDied = false;
	this->gene = gene;
	this->isRender = isRender;
	
    qsrand(QDateTime::currentDateTime().toTime_t());
    algorithm = new GeneticAlgorithm(gene);
    connect(algorithm, SIGNAL(freeCallListNumber(uint)),
            SIGNAL(freeCallListNumber(uint)));
    algorithm->init();
    algorithm->nextCar();
    uptime = 0;
    init();
}

void World::setGene(std::string gene)
{
	this->gene = gene;
	algorithm->setGene(gene);
    destroy();
    algorithm->init();
    algorithm->nextCar();
    init();
}

World::~World() {
    algorithm->deleteLater();
    destroy();
}

//public

void World::addSparksList(const int impulses, const b2Vec2 pos,
                          const QColor *color) {
    if (impulses > 32) {
        sparkStuct spark;
        spark.count = qMin(impulses/4, 32);
        spark.pos = pos;
        spark.color = color;
        sparkList.push_back(spark);
    }
}

GeneticAlgorithm *World::getAlgorithm() {
    return algorithm;
}

Car *World::getCar() {
    return car;
}

b2Vec2 World::getCenter() {
    return car->getPossition();
}

Track *World::getTrack() {
    return track;
}

b2Body *World::getSpark(const int index) {
    return sparks[index]->GetBody();
}

int World::getSparkCount() {
    return sparks.size();
}

float World::getUptime() {
    return uptime;
}

void World::step() {
	// if(isDied) return;
    car->update();
    updateSparks();
    b2world->Step(TIME_STEP, ITERATIONS, ITERATIONS);
    uptime += TIME_STEP;
}

//private

void World::destroy() {
    while (sparks.size()) {
        destroySpark(0);
    }
    sparks.clear();
    car->deletePhisicsBody();
    car->deleteLater();
    delete track;
    delete contactListener;
    delete b2world;
}

void World::destroySpark(const int index) {
    QColor *color = (QColor *)sparks[index]->GetUserData();
    delete color;
    b2world->DestroyBody(sparks[index]->GetBody());
    sparks.remove(index);
}

void World::init() {
    qsrand(QDateTime::currentDateTime().toTime_t());
    b2Vec2 gravity(0.0, GRAVITY);
    b2world = new b2World(gravity);
    b2world->SetContinuousPhysics(true);
    b2world->SetAutoClearForces(true);
    contactListener = new ContactListener(this);
    b2world->SetContactListener(contactListener);
    track = new Track(b2world);
    car = new Car(algorithm, b2world);
    connect(car, SIGNAL(stoped()), SLOT(carStoped()));
    emit creteNewCar();
    qsrand(car->getBody()->GetMass());
}

//slots

void World::carStoped() {
	// isDied = true;
	throw car->getMaxPossition();
    // algorithm->setScoreAndTime(car->getMaxPossition(), car->getTime());
    // algorithm->nextCar();
    // destroy();
    // init();
}

//private

void World::updateSparks() {
	return;
    while (sparkList.size()) {
        sparkStuct sparkEntry = sparkList.at(0);
        sparkList.remove(0);
        for (int i = 0; i < sparkEntry.count; i++) {
            if (sparks.size() >= MAX_SPARK_COUNT)
                continue;
            b2PolygonShape shape;
            shape.SetAsBox(float(qrand())/float(RAND_MAX)/30+0.02,
                           float(qrand())/float(RAND_MAX)/30+0.02, b2Vec2(0,0), 0);
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &shape;
            fixtureDef.filter.groupIndex = -1;
            fixtureDef.filter.maskBits = 2;
            fixtureDef.filter.categoryBits = 3;
            fixtureDef.density = 0.5;
            fixtureDef.restitution = 0.7;
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position = sparkEntry.pos;
            b2Body *spark = b2world->CreateBody(&bodyDef);
            b2Fixture *fixture = spark->CreateFixture(&fixtureDef);
            fixture->SetUserData(new QColor(*sparkEntry.color));
            float speed = qMax(3.0f, car->getSpeed());
            float x = (float(qrand())/float(RAND_MAX)*0.75 + 0.25)*speed*2 - speed;
            float y = (float(qrand())/float(RAND_MAX)*0.75 + 0.25)*speed;
            spark->SetLinearVelocity(b2Vec2(x, y));
            sparks.push_back(fixture);
        }
    }
    int i = 0;
    while (i < sparks.size()) {
        b2Vec2 vel = sparks[i]->GetBody()->GetLinearVelocity();
        if (qAbs(vel.x) < 0.01 && qAbs(vel.y) < 0.01) {
            destroySpark(i);
            continue;
        }
        i++;
    }
}

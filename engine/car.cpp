#include <qmath.h>

#include "car.h"

Car::Car(GeneticAlgorithm *pAlghorithm, b2World *world):
    algorithm (pAlghorithm),
    b2world(world) {
    b2BodyDef bodyDef;
    init(bodyDef);
    createCart();
    createWheels(bodyDef);
    updateTorque();
    baseSpringForce = 7.5*carBody->GetMass();
    update();
}

//public

void Car::breakFixture(const int index) {
    breakCartFixture[index] = false;
    int i = index%8;
    if (index < 8) {
        QColor *color = (QColor *)cartFixture[i]->GetUserData();
        carBody->DestroyFixture(cartFixture[i]);
        cartFixture[i] = NULL;
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = carBody->GetPosition();
        bodyDef.angle = carBody->GetAngle();
        b2Body *piece = b2world->CreateBody(&bodyDef);
        b2Fixture *f = piece->CreateFixture(&cartShapes[i], 2);
        f->SetUserData(color);
        pieces.push_back(f);
    } else {
        QColor *color = (QColor *)axleFixture[i]->GetUserData();
        carBody->DestroyFixture(axleFixture[i]);
        axleFixture[i] = NULL;
        b2Fixture *f = axle[i]->CreateFixture(&axleShapes[i], 2);
        f->SetUserData(color);
        wheelOn[i] = BROKEN_WHEELS;
        wheelsCount--;
    }
}

void Car::deletePhisicsBody() {
    QColor *color;
    for (int i = 0; i < 8; i++) {
        if (wheelOn[i] != NO_WHEELS || wheelOn[i] == BROKEN_WHEELS) {
            if (wheelOn[i] != BROKEN_WHEELS)
                b2world->DestroyJoint(spring[i]);
            b2world->DestroyJoint(motor[i]);
            for (b2Fixture *f = axle[i]->GetFixtureList(); f;
                 f = f->GetNext()) {
                if (f->GetUserData())
                    delete (QColor *)f->GetUserData();
            }
            b2world->DestroyBody(axle[i]);
            color = (QColor *)wheel[i]->GetUserData();
            delete color;
            b2world->DestroyBody(wheel[i]->GetBody());
        }
    }
    for (b2Fixture *f = carBody->GetFixtureList(); f; f = f->GetNext()) {
        if (f->GetUserData())
            delete (QColor *)f->GetUserData();
    }
    b2world->DestroyBody(carBody);
    QVectorIterator<b2Fixture *> piece(pieces);
    while (piece.hasNext()) {
        b2Fixture *p = piece.next();
        color = (QColor *)p->GetUserData();
        delete color;
        b2world->DestroyBody(p->GetBody());
    }
}

b2Body *Car::getAxleBody(const int index) {
    return wheelOn[index] == NO_WHEELS? NULL: axle[index];
}

b2Fixture *Car::getAxleFixture(const int index) {
    return wheelOn[index] == NO_WHEELS? NULL: axleFixture[index];
}

b2Body *Car::getBody() {
    return carBody;
}

b2Fixture *Car::getCartFixture(const int index) {
    return cartFixture[index];
}

b2Body *Car::getWheel(const int index) {
    return wheelOn[index] == NO_WHEELS? NULL: wheel[index]->GetBody();
}

float Car::getMaxPossition() {
    return TRACK_LENGTH < maxPossition? TRACK_LENGTH: maxPossition;
}

b2Body *Car::getPiece(const int index) {
    return pieces[index]->GetBody();
}

int Car::getPiecesCount() {
    return pieces.size();
}

b2Vec2 Car::getPossition() {
    return carBody->GetPosition();
}

int Car::getRemainingTime() {
    return (MAX_ITTERATION - itteration)/60;
}

float Car::getSpeed() {
    b2Vec2 vel = carBody->GetLinearVelocity();
    return qSqrt(qPow(vel.x, 2) + qPow(vel.y, 2));
}

float Car::getTime() {
    return itteration/60.0f;
}

float Car::getTorque() {
    return torque;
}

void Car::setBreakFixture(const int index, const bool val) {
    breakCartFixture[index] = val;
}

void Car::update() {
    for (int i = 0; i < 8; i++) {
        if (wheelOn[i] >= 0) {
            spring[i]->SetMaxMotorForce(baseSpringForce + 40*baseSpringForce*
                                        qPow(spring[i]->GetJointTranslation(),
                                             2));
            spring[i]->SetMotorSpeed(-20*spring[i]->GetJointTranslation());
        }
        if (brokeNum < 7 && breakCartFixture[i]) {
            brokeNum++;
            breakFixture(i);
            for (int j = 0; j < 8; j++) {
                if (wheelOn[j] == i) {
                    b2world->DestroyJoint(spring[j]);
                    breakFixture(8 + j);
                    motor[j]->SetMotorSpeed(0);
                    motor[j]->SetMaxMotorTorque(0);
                }
            }
            updateTorque();
        }
        if (breakCartFixture[i + 8]) {
            b2world->DestroyJoint(spring[i]);
            breakFixture(i+8);
            motor[i]->SetMotorSpeed(0);
            motor[i]->SetMaxMotorTorque(0);
            updateTorque();
        }
    }
    itteration++;
    float possition = carBody->GetPosition().x;
    maxPossition = qMax(maxPossition, possition);
    if (maxPossition - prevPossition > 1) {
        slow = 0;
        prevPossition = maxPossition;
    } else {
        if (carBody->GetLinearVelocity().x < 1)
            slow++;
    }
    float maxSlow = possition > 10? 300: 180;
    if (slow >= maxSlow || possition >= TRACK_LENGTH ||
            itteration > MAX_ITTERATION || possition < -10 || brokeNum >= 7)
        emit stoped();
}

void Car::updateTorque() {
    float totalMass = carBody->GetMass();
    torque = wheelsCount? totalMass*MASS_MULT*15/qPow(2,float(wheelsCount) - 1):
                          0;
    for (int i = 0; i < 8; i++) {
        if (wheelOn[i] >= 0) {
            motor[i]->SetMaxMotorTorque(torque);
        }
    }
}

void Car::createCart() {
    b2Vec2 triangle[3];
    triangle[0].Set(0,0);
    for (int i = 0; i < 8; i++) {
        float angle = algorithm->getCartAngle(i);
        float magnitude = algorithm->getMagnitude(i);
        float x = magnitude*qCos(angle);
        float y = magnitude*qSin(angle);
        float nextMagnitude = algorithm->getMagnitude((i + 1)%8);
        float nextAngle = algorithm->getCartAngle((i + 1)%8);
        float nextX = nextMagnitude*qCos(nextAngle);
        float nextY = nextMagnitude*qSin(nextAngle);
        triangle[0].Set(0 ,0);
        triangle[1].Set(x, y);
        triangle[2].Set(nextX, nextY);
        cartShapes[i].Set(triangle, 3);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &cartShapes[i];
        fixtureDef.density = 2;
        fixtureDef.friction = 10;
        fixtureDef.restitution = 0.05;
        fixtureDef.filter.groupIndex = -1;
        cartFixture[i] = createFixture(&fixtureDef, carBody,
                                       algorithm->getColorCart(i));
        setBreakFixture(i, false);
    }
}

b2Fixture *Car::createFixture(const b2FixtureDef* def, b2Body *body,
                         const QColor color) {
    b2Fixture *fixture = body->CreateFixture(def);
    QColor *c = new QColor(color);
    fixture->SetUserData(c);
    return fixture;
}

void Car::createWheels(b2BodyDef &bodyDef) {
    b2PrismaticJointDef prismaticJointDef;
    prismaticJointDef.lowerTranslation = -0.1;
    prismaticJointDef.upperTranslation = 0.25;
    prismaticJointDef.enableLimit = true;
    prismaticJointDef.enableMotor = true;
    for (int i = 0; i < 8; i++) {
        wheelOn[i] = algorithm->getWheelOn(i);
        setBreakFixture(8 + i, false);
        if (wheelOn[i] == NO_WHEELS)
            continue;
        wheelsCount++;
        float angle = algorithm->getCartAngle(wheelOn[i]);
        float magnitude = algorithm->getMagnitude(wheelOn[i]);
        float x = magnitude*qCos(angle);
        float y = magnitude*qSin(angle);
        float axleAngle = algorithm->getAxleAngle(i);
        axleShapes[i].SetAsBox(0.2, 0.1, b2Vec2(x, y), axleAngle);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &axleShapes[i];
        fixtureDef.density = 2;
        fixtureDef.friction = 10;
        fixtureDef.restitution = 0.05;
        fixtureDef.filter.groupIndex = -1;
        axleFixture[i] = createFixture(&fixtureDef, carBody,
                                       algorithm->getColorAxle(i));
        axle[i] = b2world->CreateBody(&bodyDef);
        b2PolygonShape polygonShape;
        polygonShape.SetAsBox(0.2, 0.05, b2Vec2(x - 0.3*qCos(axleAngle),
                              y - 0.3*qSin(axleAngle)), axleAngle);
        fixtureDef.shape = &polygonShape;
        fixtureDef.density = 20;
        createFixture(&fixtureDef, axle[i], algorithm->getColorAxle(i));
        prismaticJointDef.Initialize(carBody, axle[i],
                                     axle[i]->GetWorldCenter(),
                                     b2Vec2(qCos(axleAngle), qSin(axleAngle)));
        spring[i] = (b2PrismaticJoint *)
                b2world->CreateJoint(&prismaticJointDef);
        b2CircleShape circleShape;
        circleShape.m_radius = algorithm->getWheelRadius(i);
        b2FixtureDef wheelFixtureDef;
        wheelFixtureDef.shape = &circleShape;
        wheelFixtureDef.density = 0.5;
        wheelFixtureDef.friction = 10;
        wheelFixtureDef.restitution = 0.1;
        wheelFixtureDef.filter.groupIndex = -1;
        b2BodyDef wheelBodyDef;
        wheelBodyDef.position.Set(carBody->GetPosition().x + x -
                                  qCos(axleAngle)/2, carBody->GetPosition().y +
                                  y - qSin(axleAngle)/2);
        wheelBodyDef.allowSleep = false;
        wheelBodyDef.type = b2_dynamicBody;
        b2Body *bodyWheel = b2world->CreateBody(&wheelBodyDef);
        wheel[i] = createFixture(&wheelFixtureDef, bodyWheel, Qt::black);
        b2RevoluteJointDef revoluteJointDef;
        revoluteJointDef.enableMotor = true;
        revoluteJointDef.Initialize(axle[i], bodyWheel,
                                    bodyWheel->GetWorldCenter());
        motor[i] = (b2RevoluteJoint *)b2world->CreateJoint(&revoluteJointDef);
        motor[i]->SetMotorSpeed(-6*M_PI);
    }
}

void Car::init(b2BodyDef &bodyDef) {
    bodyDef.position.Set(0, 4);
    bodyDef.angle = 0;
    bodyDef.type = b2_dynamicBody;
    carBody = b2world->CreateBody(&bodyDef);
    brokeNum = 0;
    wheelsCount = 0;
    maxPossition = 0;
    prevPossition = 0;
    itteration = 0;
    slow = 0;
}


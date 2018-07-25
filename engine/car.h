#ifndef CAR_H
#define CAR_H

#include <QVector>
#include "Box2D.h"
#include "geneticalgorithm.h"

class Car: public QObject {
    Q_OBJECT
public:
    Car(GeneticAlgorithm *pAlghorithm, b2World *world);

    void breakFixture(const int index);
    void deletePhisicsBody();
    b2Body *getAxleBody(const int index);
    b2Fixture *getAxleFixture(const int index);
    b2Body *getBody();
    b2Fixture *getCartFixture(const int index);
    b2Body *getWheel(const int index);
    float getMaxPossition();
    b2Body *getPiece(const int index);
    int getPiecesCount();
    b2Vec2 getPossition();
    int getRemainingTime();
    float getSpeed();
    float getTime();
    float getTorque();
    void setBreakFixture(const int index, const bool val);
    void update();
    void updateTorque();

signals:
    void stoped();

private:
    static constexpr const float MASS_MULT = 1.5;
    static constexpr const int MAX_ITTERATION = 5*60*60;
    static constexpr const float TRACK_LENGTH = 1500.0f;
    static constexpr const int NO_WHEELS = -1;
    static constexpr const int BROKEN_WHEELS = -2;

    void createCart();
    b2Fixture *createFixture(const b2FixtureDef* def, b2Body *body,
                             const QColor color);
    void createWheels(b2BodyDef &bodyDef);
    void init(b2BodyDef &bodyDef);

    GeneticAlgorithm *algorithm;
    b2Body *axle[8];
    b2Fixture *axleFixture[8];
    b2PolygonShape axleShapes[8];
    float baseSpringForce;
    b2World *b2world;
    bool breakCartFixture[16];
    int brokeNum;
    b2Body *carBody;
    b2Fixture *cartFixture[8];
    b2PolygonShape cartShapes[8];
    int itteration;
    float maxPossition;
    b2RevoluteJoint *motor[8];
    float prevPossition;
    int slow;
    b2PrismaticJoint *spring[8];
    float torque;
    b2Fixture *wheel[8];
    int wheelsCount;
    int wheelOn[8];
    QVector<b2Fixture *> pieces;
};

#endif // CAR_H

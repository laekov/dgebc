#include "contactlistener.h"


ContactListener::ContactListener(World *pWorld): world(pWorld) {
    massData = new b2MassData();
    manifold = new b2WorldManifold();
}

ContactListener::~ContactListener() {
    delete massData;
    delete manifold;
}

void ContactListener::PostSolve(b2Contact* contact,
                                const b2ContactImpulse* impulse) {
    b2Fixture *fixture = contact->GetFixtureB();
    int indexFixture = -1;
    b2Fixture *motoFixture;
    Car *car = world->getCar();
    for (int i = 0; i < 8; i++) {
        motoFixture = car->getCartFixture(i);
        if (fixture == motoFixture) {
            indexFixture = i;
            break;
        }
        motoFixture = car->getAxleFixture(i);
        if (fixture == motoFixture) {
            indexFixture = i + 8;
            break;
        }
    }
    if (indexFixture < 0)
        return;
    int pointCount = contact->GetManifold()->pointCount;
    float maxImpulses = 0;
    contact->GetWorldManifold(manifold);
    for (int i = 0; i < pointCount; i++) {
        if (maxImpulses < impulse->normalImpulses[i])
            maxImpulses = impulse->normalImpulses[i];
        world->addSparksList(maxImpulses, manifold->points[i],
                             (QColor *)fixture->GetUserData());
    }
    motoFixture->GetMassData(massData);
    float strength = 50*massData->mass;
    if (strength < maxImpulses)
        car->setBreakFixture(indexFixture, true);
}

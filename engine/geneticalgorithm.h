#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <QColor>
#include <QObject>
#include <QVector>
#include "Box2D.h"

class GeneticAlgorithm: public QObject {
    Q_OBJECT
public:
    GeneticAlgorithm();

    float getAvgScore(const int index);
    float getAxleAngle(const int index);
    unsigned int getCarCallListNuber();
    unsigned int getCarParentCallListNumber(const int parent);
    int getCarNum();
    float getCartAngle(const int index);
    QColor getColorAxle(const int index);
    QColor  getColorCart(const int index);
    int getGenerationNum();
    float getMagnitude(const int index);
    float getMaxScore(const int index);
    int getOffspringsCount(const int index);
    float getScore(const int index);
    int getTime(const int index);
    int getWheelOn(const int index);
    float getWheelRadius(const int index);
    void init();
    void nextCar();
    void nextGenetation();
    void setCarCallList(const unsigned int callListNumber);
    void setScoreAndTime(float score, float time);

signals:
    void freeCallListNumber(unsigned int);

private:
    static constexpr const int CROMES_SIZE = 40;
    static constexpr const float MIN_CART = 0.1;
    static constexpr const float MAX_CART = 3;
    static constexpr const float MIN_WHEEL = 0.1;
    static constexpr const float MAX_WHEEL = 1.5;
    static constexpr const float MIN_ANGLE = 0.08;
    static constexpr const int POP_SIZE = 32;
    static constexpr const float WHEEL_PROB0 = 0.5; //(0:1]
    static constexpr const int START_WHEELS_GEN = 16;
    static constexpr const int MAX_MUTATION_RATE = 5.0;
    static constexpr const int RED = 0;
    static constexpr const int GREEN = 1;
    static constexpr const int BLUE = 2;

    bool compareCar(const float scoreA, const float timeA, const int scoreB,
                    const float timeB);
    void copyChrome(const int parent, const int offspring);
    void copyChromes();
    void createCache();
    void crossover(const int parentA, const int parentB, const int offsprinA,
                   const int offspringB);
    QColor getColor(const int index);
    int getRandomChrome(bool queue[], const int excluding = -1);
    void mutation();
    void setColors(const int parentA, const int offspringA, const int  parentB,
                   const int offspringB, const int index);
    void setParentCallLists(const int index, const int parentACL,
                            const int parentBCL);

    QVector <float> avgScore;
    unsigned int callLists[POP_SIZE];
    float chromes[POP_SIZE][CROMES_SIZE];
    float oldChromes[POP_SIZE][CROMES_SIZE];
    float cacheMagnitudes[POP_SIZE][8];
    float cacheAngles[POP_SIZE][8];
    int callListIndex;
    unsigned short int colors[POP_SIZE][16][3];
    QVector<float> maxScore;
    float mutationRate;
    unsigned short int oldColors[POP_SIZE][16][3];
    unsigned int parentsCallLists[POP_SIZE][2];
    float scores[POP_SIZE];
    int offspringsCount[POP_SIZE];
    float times[POP_SIZE];
    int currentCar;
    int generationNum;
};

#endif // GENETICALGORITHM_H

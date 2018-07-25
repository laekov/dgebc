#ifndef RENDER_H
#define RENDER_H

#include <QGLWidget>

#include "world.h"

class Render : public QGLWidget {
    Q_OBJECT
public:
    explicit Render(World *pWorld, QWidget *parent);
    ~Render();
    void setSpeed(int newSpeed);

public slots:
    void createCarCallList();
    void deleteCallList(const unsigned int index);

private:
    float boundTop;
    float boundLeft;
    float boundBottom;
    float boundRight;
    float *avgScore;
    float *maxScore;
    float tableBoard[152];
    int genrationNum;
    QFont font;
    int speed;
    QPointF pos;
    float viewRadius;
    World *world;

    void drawBody(b2Body *body);
    void drawCar();
    void drawCircle(const b2Vec2& center, float32 radius, const float32 angle,
                    const QColor &color);
    void drawGraph();
    void drawGrid();
    void drawParents();
    void drawPolygon(const b2Vec2* vertices, int32 vertexCount,
                     const QColor &color);
    void drawSparks();
    void drawTable();
    void drawText();
    float getLeftBound();
    float getRightBound();
    float getBottomBound();
    float getTopBound();
    void initializeGL();
    void move();
    void paintGL();
    void prepareTableBoard();
    void resizeGL(const int nWidth, const int nHeight);
    void setBounds(const float nLeft, const float nRight, const float nBottom,
                   const float nTop);
    void updateGraph();
};

#endif // RENDER_H

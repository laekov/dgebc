#include "render.h"

#include <QTime>

Render::Render(World *pWorld, QWidget *parent) :
    QGLWidget(parent) {
    world = pWorld;
    viewRadius = 10;
    pos = QPointF(0, 0);
    font.setFamily("Arial");
    avgScore = NULL;
    maxScore = NULL;
    genrationNum = 0;
    setMinimumSize(640, 480);
    prepareTableBoard();
    setSpeed(2);
}

Render::~Render() {
}

void Render::setSpeed(int newSpeed) {
    speed = newSpeed;
}

//slots

void Render::createCarCallList() {
    GeneticAlgorithm *algorithm = world->getAlgorithm();
    unsigned int oldCallListNubmer = algorithm->getCarCallListNuber();
    if (oldCallListNubmer)
        glDeleteLists(oldCallListNubmer, 1);
    unsigned int listNubmer = glGenLists(1);
    algorithm->setCarCallList(listNubmer);
    glNewList(listNubmer, GL_COMPILE);
    drawCar();
    glEndList();
}

void Render::deleteCallList(const unsigned int index) {
    glDeleteLists(index, 1);
}

//private

void Render::drawBody(b2Body *body) {
    QColor *color;
    for (b2Fixture *f = body->GetFixtureList(); f; f = f->GetNext()) {
        color = (QColor *)f->GetUserData();
        const b2Transform& xf = f->GetBody()->GetTransform();
        switch (f->GetType()) {
        case b2Shape::e_circle: {
            b2CircleShape* circle = (b2CircleShape*)f->GetShape();
            b2Vec2 center = b2Mul(xf, circle->m_p);
            float32 radius = circle->m_radius;
            drawCircle(center, radius, body->GetAngle(), *color);
        }
        break;
        case b2Shape::e_polygon: {
            b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
            int32 vertexCount = poly->m_count;
            b2Assert(vertexCount <= b2_maxPolygonVertices);
            b2Vec2 vertices[b2_maxPolygonVertices];
            for (int32 i = 0; i < vertexCount; ++i) {
                vertices[i] = b2Mul(xf, poly->m_vertices[i]);
            }
            drawPolygon(vertices, vertexCount, *color);
        }
        break;
        default: break;
        }
    }
}

void Render::drawCar() {
    Car *car = world->getCar();
    b2Body *axle, *wheel;
    for (int i = 0; i < 8; i++) {
        wheel = car->getWheel(i);
        if (wheel)
            drawBody(wheel);
        axle = car->getAxleBody(i);
        if (axle)
            drawBody(axle);
    }
    int piecesCount = car->getPiecesCount();
    for (int i = 0; i < piecesCount; i++) {
        drawBody(car->getPiece(i));
    }
    drawBody(car->getBody());
}

void Render::drawCircle(const b2Vec2& center, float32 radius,
                         const float32 angle, const QColor &color) {
    const float32 k_segments = 32.0f;
    const float32 k_increment = 2.0f * b2_pi / k_segments;
    float32 theta = 0.0f;
    QColor transparent = color;
    transparent.setAlpha(127);
    qglColor(transparent);
    glBegin(GL_TRIANGLE_FAN);
    for (int32 i = 0; i < k_segments; ++i) {
        b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
        glVertex2f(v.x, v.y);
        theta += k_increment;
    }
    glEnd();
    theta = 0.0f;
    qglColor(color);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    for (int32 i = 0; i < k_segments; ++i) {
        b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
        glVertex2f(v.x, v.y);
        theta += k_increment;
    }
    glEnd();
    b2Vec2 p = center + radius*b2Vec2(cosf(angle), sinf(angle));
    glBegin(GL_LINES);
        glVertex2f(center.x, center.y);
        glVertex2f(p.x, p.y);
    glEnd();
    glLineWidth(1);
}

void Render::drawGrid() {
    glDisable(GL_LINE_SMOOTH);
    glBegin(GL_LINES);
    for (int y = getBottomBound(); y <= getTopBound(); y++) {
        if (y % 20 == 0)
            glColor3f(1,0,0.5);
        else
            glColor3f(0.8,0.8,0.8);
        glVertex2f(getLeftBound(), y);
        glVertex2f(getRightBound(), y);
     }
    for (int x = getLeftBound(); x <= getRightBound(); x++) {
       if (x % 100 == 0)
            glColor3f(1,0,0.5);
        else
            glColor3f(0.8,0.8,0.8);
        glVertex2f(x, getBottomBound());
        glVertex2f(x, getTopBound());
     }
    glEnd();
    glEnable(GL_LINE_SMOOTH);
}

void Render::drawGraph() {
    GeneticAlgorithm *algorithm = world->getAlgorithm();
    int count = algorithm->getGenerationNum();
    if (!count)
        return;
    if (genrationNum != count) {
        genrationNum = count;
        updateGraph();
    }
    count++;
    glLineWidth(2);
    qglColor(Qt::black);
    glVertexPointer(2, GL_FLOAT, 0, avgScore);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_LINE_STRIP, 0, count);
    qglColor(Qt::red);
    glVertexPointer(2, GL_FLOAT, 0, maxScore);
    glDrawArrays(GL_LINE_STRIP, 0, count);
    glDisableClientState(GL_VERTEX_ARRAY);
    glLineWidth(1);
}

void Render::drawParents() {
    if (world->getAlgorithm()->getCarParentCallListNumber(0)) {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        float aspectRatio = (float)width()/height();
        float left, right, bottom, top;
        if (aspectRatio < 1) {
            left = -20;
            right = 20;
            bottom = -20/aspectRatio;
            top = 20/aspectRatio;
        }
        else {
            left = -20*aspectRatio;
            right = 20*aspectRatio;
            bottom = -20;
            top = 20;
        }
        glOrtho(left, right, bottom, top, 1.0, -1.0);
        float offset = world->getAlgorithm()->getCarParentCallListNumber(1)?
                    0.0f: 10.f;
        float panel[8] = {right - 20.5f + offset, bottom + 10.5f,
                          right - 20.5f + offset, bottom + 0.5f,
                          right - 0.5f, bottom + 0.5f,
                          right - 0.5f, bottom + 10.5f};
        qglColor(QColor(255, 255, 200, 96));
        glVertexPointer(2, GL_FLOAT, 0, panel);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_QUADS, 0, 4);
        qglColor(Qt::gray);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glTranslated(right - 15.5  + offset, bottom + 1.5f, 0.0f);
        glCallList(world->getAlgorithm()->getCarParentCallListNumber(0));
        glTranslated(10.0f, 0.0f, 0.0f);
        if (world->getAlgorithm()->getCarParentCallListNumber(1))
            glCallList(world->getAlgorithm()->getCarParentCallListNumber(1));
        glPopMatrix(); //GL_MODELVIEW
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }
}

void Render::drawPolygon(const b2Vec2* vertices, int32 vertexCount,
                 const QColor &color) {
    QColor transparent = color;
    transparent.setAlpha(qMin(127, color.alpha()));
    if (!transparent.alpha())
        return;
    qglColor(transparent);
    glBegin(GL_TRIANGLE_FAN);
    for (int32 i = 0; i < vertexCount; ++i) {
        glVertex2f(vertices[i].x, vertices[i].y);
    }
    glEnd();
    glLineWidth(2);
    qglColor(color);
    glBegin(GL_LINE_LOOP);
    for (int32 i = 0; i < vertexCount; ++i) {
        glVertex2f(vertices[i].x, vertices[i].y);
    }
    glEnd();
    glLineWidth(1);
}

void Render::drawSparks() {
    int sparksCount = world->getSparkCount();
    for (int i = 0; i < sparksCount; i++) {
        drawBody(world->getSpark(i));
    }
}

void Render::drawTable() {
    int stepY = qMin((height() - 1)/33, 14);
    qglColor(QColor(255, 255, 200, 192));
    glBegin(GL_QUADS);
        glVertex2f(5, 1);
        glVertex2f(135, 1);
        glVertex2f(135, 33*stepY);
        glVertex2f(5, 33*stepY);
    glEnd();
    qglColor(Qt::gray);
    glVertexPointer(2, GL_FLOAT, 0, tableBoard);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_LINES, 0, 76);
    glDisableClientState(GL_VERTEX_ARRAY);
    qglColor(Qt::black);
    font.setPixelSize(12);
    QFontMetrics fm(font);
    int y = 2 + fm.ascent();
    renderText(15, y, "#", font);
    renderText(40, y, "Score", font);
    renderText(90, y, "Time", font);
    y += stepY;
    GeneticAlgorithm *algorithm = world->getAlgorithm();
    for (int i = 0; i < 32; i++) {
        qglColor(Qt::black);
        if (!algorithm->getCarNum() && algorithm->getGenerationNum()) {
            switch (algorithm->getOffspringsCount(i)) {
            case 0: qglColor(Qt::red);
                break;
            case 1: qglColor(Qt::black);
                break;
            case 2: qglColor(Qt::darkGreen);
                break;
            }
        }
        renderText(15, y + i*stepY, QString::number(i), font);
        float score = algorithm->getScore(i);
        if (score >= 0) {
            renderText(40, y + i*stepY, QString::number(score, '0', 1), font);
            int ss = algorithm->getTime(i);
            QTime time(0, ss/60, ss%60);
            renderText(90, y + i*stepY, time.toString("m:ss"), font);
        }
    }
}

void Render::drawText() {
    font.setPixelSize(24);
    QFontMetrics fmScore(font);
    Car *car = world->getCar();
    QString str;
    qglColor(Qt::red);
    str = tr("Score: %1").arg(QString::number(car->getMaxPossition(), '0', 1));
    renderText(this->width()/2 - fmScore.width(str)/2, height()/6*5, str,
               font);
    font.setPixelSize(14);
    QFontMetrics fm(font);
    int ss = 5*60 - car->getTime();
    QTime time(0, ss/60, ss%60);
    str = tr("Time: %1").arg(time.toString("m:ss"));
    renderText(width() - fm.width(str) - 5, 20, str, font);
    str = tr("Torque: %1").arg(QString::number(car->getTorque(), '0', 1));
    renderText(width() - fm.width(str) - 5, 34, str, font);
    str = tr("Speed: %1").arg(QString::number(car->getSpeed(), '0', 1));
    renderText(width() - fm.width(str) - 5, 48, str, font);
    font.setPixelSize(16);
    QFontMetrics fmGeneration(font);
    int generationNumber = world->getAlgorithm()->getGenerationNum();
    str = tr("Generation: %1").arg(QString::number(generationNumber));
    renderText(width()/2 - fmGeneration.width(str)/2, 20, str, font);
    qglColor(Qt::blue);
    if (speed == 0) {
        str = tr("[PAUSE]");
        font.setPixelSize(32);
        QFontMetrics fmPause(font);
        renderText(width()/2 - fmPause.width(str)/2, height()/2, str, font);
    } else if (speed != 2) {
        str = tr("Playback speed: %1x").arg(QString::number(speed/2.0, '0', 1));
        font.setPixelSize(14);
        renderText(width() - fm.width(str) - 5, 62, str, font);
    }
}

float Render::getLeftBound() {
    return boundLeft + pos.x();
}

float Render::getRightBound() {
    return boundRight + pos.x();
}

float Render::getBottomBound() {
    return boundBottom + pos.y();
}

float Render::getTopBound() {
    return boundTop + pos.y();
}

void Render::initializeGL() {
    qglClearColor(QColor(255,255,200));
}


void Render::move() {
    b2Vec2 newPos = world->getCenter();
    newPos.y += 1;
    float deltaX = pos.x() - newPos.x;
    float deltaY = pos.y() - newPos.y;
    glTranslated(deltaX, deltaY, 0);
    pos = QPointF(newPos.x, newPos.y);
}

void Render::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    move();
    drawGrid();
    drawCar();
    drawSparks();
    drawBody(world->getTrack()->getBody());
    drawText();
    drawParents();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, width(), height(), 0, 1.0, -1.0);
    drawTable();
    drawGraph();
    glPopMatrix(); //GL_MODELVIEW
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void Render::prepareTableBoard() {
    int stepY = qMin((height() - 1)/33, 14);
    int y = 0;
    for (int i = 0; i < 34; i++) {
        y = 1+i*stepY;
        tableBoard[i*4] = 5;
        tableBoard[i*4 + 1] = y;
        tableBoard[i*4 + 2] = 135;
        tableBoard[i*4 + 3] = y;
    }
    tableBoard[136] = 5;
    tableBoard[137] = 1;
    tableBoard[138] = 5;
    tableBoard[139] = y;
    tableBoard[140] = 35;
    tableBoard[141] = 1;
    tableBoard[142] = 35;
    tableBoard[143] = y;

    tableBoard[144] = 85;
    tableBoard[145] = 1;
    tableBoard[146] = 85;
    tableBoard[147] = y;
    tableBoard[148] = 135;
    tableBoard[149] = 1;
    tableBoard[150] = 135;
    tableBoard[151] = y;
}

void Render::resizeGL(const int nWidth, const int nHeight) {
    int width = nWidth;
    int height = nHeight? nHeight: 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspectRatio = (float)width/height;
    if (width <= height) {
        setBounds(-viewRadius, viewRadius, -viewRadius/aspectRatio,
                  viewRadius/aspectRatio);
     } else {
        setBounds(-viewRadius*aspectRatio, viewRadius*aspectRatio,
                  -viewRadius, viewRadius);
    }
    glOrtho(getLeftBound(), getRightBound(), getBottomBound(), getTopBound(),
            1.0, -1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    updateGraph();
}

void Render::setBounds(const float nLeft, const float nRight,
                       const float nBottom, const float nTop) {
    boundLeft = nLeft;
    boundRight = nRight;
    boundBottom = nBottom;
    boundTop = nTop;
}


void Render::updateGraph() {
    GeneticAlgorithm *algorithm = world->getAlgorithm();
    int count = algorithm->getGenerationNum();
    if (!count)
        return;
    if (avgScore) {
        delete []avgScore;
        delete []maxScore;
    }
    int width = this->width() - 400;
    int height = 200;
    float dx = float(width)/count;
    float max = algorithm->getMaxScore(count);
    if (max < 1)
        max = 1;
    count++;
    avgScore = new float[count*2];
    maxScore = new float[count*2];
    for (int i = 0; i < count; i++) {
        avgScore[i*2] = 200 + i*dx;
        avgScore[i*2 + 1] = 230 - height*(algorithm->getAvgScore(i)/max);
        maxScore[i*2] = 200 + i*dx;
        maxScore[i*2 + 1] = 230 - height*(algorithm->getMaxScore(i)/max);
    }
}

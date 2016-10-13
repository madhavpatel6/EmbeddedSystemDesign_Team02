#ifndef UNITTEST_H
#define UNITTEST_H

#include <QtTest/QtTest>

class TestRayTrace: public QObject
{
    Q_OBJECT
private slots:
    void samePointMaximum();
    void samePointNotMaximum();
    void diagonalTrace();
    void horizontalTrace();
    void verticalTrace();
    void angledTrace();
private:
    bool compareVector(QVector<QPoint> answer, QVector<QPoint> expected);
};


#endif // UNITTEST_H

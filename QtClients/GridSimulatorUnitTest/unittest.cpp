#include "unittest.h"
#include "../GridSimulator/grid.h"
#include "../GridSimulator/gridhelper.h"

using namespace Grid;
using namespace GridHelper;

bool TestRayTrace::compareVector(QVector<QPoint> answer, QVector<QPoint> expected) {
    if(answer.size() != expected.size())
        return false;
    for(int i = 0; i < answer.size(); i++) {
        if(answer[i] != expected[i])
            return false;
    }
    return true;
}

void TestRayTrace::samePointMaximum() {
    GridType testGrid;
    initializeGrid(testGrid);
    RayTraceReturnType ret = raytrace3(0,0,0,0,false, testGrid);
    QCOMPARE(ret.decrementPoints.size(), (1));
    QCOMPARE(ret.decrementPoints[0], QPoint(0,0));
    QVector<QPoint> expected = {QPoint(0,0)};
    QVERIFY(compareVector(ret.decrementPoints, expected));
}

void TestRayTrace::samePointNotMaximum() {
    GridType testGrid;
    initializeGrid(testGrid);
    RayTraceReturnType ret = raytrace3(0,0,0,0,true, testGrid);
    QCOMPARE(ret.decrementPoints.size(), (1));
    QCOMPARE(ret.decrementPoints[0], QPoint(0,0));
    QVector<QPoint> expected = {QPoint(0,0)};
    QVERIFY(compareVector(ret.decrementPoints, expected));
}

void TestRayTrace::diagonalTrace() {
    GridType testGrid;
    initializeGrid(testGrid);
    RayTraceReturnType ret = raytrace3(0,0,5,5,false, testGrid);
    QCOMPARE(ret.decrementPoints.size(), 15);
    QCOMPARE(ret.incrementPoints.size(), 1);
    QVector<QPoint> expected = {QPoint(0,0), QPoint(0,1), QPoint(1,0), QPoint(1,1), QPoint(1,2), QPoint(2,1), QPoint(2,2), QPoint(2,3), QPoint(3,2), QPoint(3,3), QPoint(3,4), QPoint(4,3), QPoint(4,4), QPoint(4,5), QPoint(5,4)};
    QVERIFY(compareVector(ret.decrementPoints, expected));
    QVERIFY(ret.incrementPoints[0] == QPoint(5,5));
}

void TestRayTrace::horizontalTrace() {
    GridType testGrid;
    initializeGrid(testGrid);
    RayTraceReturnType ret = raytrace3(0,0,5,0,false, testGrid);
    QCOMPARE(ret.decrementPoints.size(), 5);
    QCOMPARE(ret.incrementPoints.size(), 1);
    QVector<QPoint> expected = {QPoint(0,0), QPoint(1,0), QPoint(2,0), QPoint(3,0), QPoint(4,0)};
    QVERIFY(compareVector(ret.decrementPoints, expected));
    QVERIFY(ret.incrementPoints[0] == QPoint(5,0));
}

void TestRayTrace::verticalTrace() {
    GridType testGrid;
    initializeGrid(testGrid);
    RayTraceReturnType ret = raytrace3(0,0,0,5,false, testGrid);
    QCOMPARE(ret.decrementPoints.size(), 5);
    QCOMPARE(ret.incrementPoints.size(), 1);
    QVector<QPoint> expected = {QPoint(0,0), QPoint(0,1), QPoint(0,2), QPoint(0,3), QPoint(0,4)};
    QVERIFY(compareVector(ret.decrementPoints, expected));
    QVERIFY(ret.incrementPoints[0] == QPoint(0,5));
}

void TestRayTrace::angledTrace() {
    GridType testGrid;
    initializeGrid(testGrid);
    RayTraceReturnType ret = raytrace3(0,0,2,5,false, testGrid);
    QCOMPARE(ret.decrementPoints.size(), 7);
    QCOMPARE(ret.incrementPoints.size(), 1);
    QVector<QPoint> expected = {QPoint(0,0), QPoint(0,1), QPoint(1,1), QPoint(1,2), QPoint(1,3), QPoint(1,4), QPoint(2,4)};
    QVERIFY(compareVector(ret.decrementPoints, expected));
    QVERIFY(ret.incrementPoints[0] == QPoint(2,5));
}


QTEST_MAIN(TestRayTrace)
//#include "testraytrace.moc"

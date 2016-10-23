




#include <iostream>
#include <string>

#include "a_star_functions.h"


/* These are the tests we want to run */

using namespace std;

string passFail(bool item){
	return (item ? "PASS: " : "FAIL: ");
}

void populateWorldTests(){
	Point t1 = {.x = 0, .y = 0};
	Point t2   = {.x = 4, .y = 0};
	Point t3 = {.x = 2, .y = 2};
	Point t4  = {.x = 0, .y = 2};

	addObstacle(t1, t2, t3, t4);

	Point t12 = {.x = 3, .y = 3};
	Point t22   = {.x = 5, .y = 3};
	Point t32 = {.x = 5, .y = 9};
	Point t42  = {.x = 3, .y = 5};

	addObstacle(t1, t2, t3, t4);

	Point v1 = {.x = -10, .y = -10};
	Point v2   = {.x = 10, .y = -10};
	Point v3 = {.x = 10, .y = 10};
	Point v4  = {.x = -10, .y = 10};

	addVertex(v1);
	addVertex(v2);
	addVertex(v3);
	addVertex(v4);

	Point testPoint = {.x=0, .y=0};

	/* Check arena bounds set correctly */
	cout << passFail(isInArena(testPoint) == true) << " isInArena in" << endl;;
	testPoint.x = 8;
	testPoint.y = 0;
	cout << passFail(isInArena(testPoint) == true) << " isInArena in " << endl;
	testPoint.x = -9;
	testPoint.y = -9;
	cout << passFail(isInArena(testPoint) == true) << " isInArena in " << endl;
	testPoint.x = 10;
	testPoint.y = 10;
	cout << passFail(isInArena(testPoint) == true) << " isInArena in " << endl;
	testPoint.x = 0;
	testPoint.y = 0;
	cout << passFail(isInArena(testPoint) == true) << " isInArena in " << endl;

	testPoint.x = 100;
	testPoint.y = -10;
	cout << passFail(isInArena(testPoint) == false) << " isInArena out " << endl;
	testPoint.x = -10;
	testPoint.y = -11;
	cout << passFail(isInArena(testPoint) == false) << " isInArena out " << endl;
	testPoint.x = 12;
	testPoint.y = 12;
	cout << passFail(isInArena(testPoint) == false) << " isInArena out " << endl;
	testPoint.x = 16;
	testPoint.y = 0;
	cout << passFail(isInArena(testPoint) == false) << " isInArena out " << endl;

	/* Check target location and verification working */
	testPoint.x = 1;
	testPoint.y = 1;
	cout << passFail(isEmpty(testPoint) == false) << " isEmpty should not be" << endl;
	testPoint.x = 1;
	testPoint.y = 0;
	cout << passFail(isEmpty(testPoint) == false) << " isEmpty should not be" << endl;
	testPoint.x = 2;
	testPoint.y = 1;
	cout << passFail(isEmpty(testPoint) == false) << " isEmpty should not be" << endl;
	testPoint.x = 4;
	testPoint.y = 6;
	cout << passFail(isEmpty(testPoint) == false) << " isEmpty should not be" << endl;
	cout << passFail(isOccupied(testPoint) == true) << " isOccupied should be" << endl;



	testPoint.x = 8;
	testPoint.y = -3;
	cout << passFail(isEmpty(testPoint) == true) << " isEmpty should be" << endl;
	testPoint.x = 4;
	testPoint.y = 9;
	cout << passFail(isEmpty(testPoint) == true) << " isEmpty should be" << endl;	
	cout << passFail(isOccupied(testPoint) == false) << " isOccupied should not be" << endl;

}

void findPathTests(){
	initWorld();

	Point t1 = {.x = 0, .y = 0};
	Point t2   = {.x = 4, .y = 0};
	Point t3 = {.x = 2, .y = 2};
	Point t4  = {.x = 0, .y = 2};

	addObstacle(t1, t2, t3, t4);

	Point t12 = {.x = 3, .y = 3};
	Point t22   = {.x = 5, .y = 3};
	Point t32 = {.x = 5, .y = 9};
	Point t42  = {.x = 3, .y = 5};

	addObstacle(t1, t2, t3, t4);

	Point v1 = {.x = -10, .y = -10};
	Point v2   = {.x = 10, .y = -10};
	Point v3 = {.x = 10, .y = 10};
	Point v4  = {.x = -10, .y = 10};

	addVertex(v1);
	addVertex(v2);
	addVertex(v3);
	addVertex(v4);

	Point sta1 = {.x = 00, .y = 00};
	Point sto1 = {.x = 99, .y = 99};
	Point sta2 = {.x = -1, .y = -1};
	Point sto2 = {.x = -9, .y = -9};

	bool result = (calculateH(sta1, sto1) > calculateH(sta2, sto2));

	cout << passFail(result) << " calculateH approxDistance" << endl;

	cout << passFail(findPath(sta2, sto2)) << " path reported as successfully found" << endl;

}

int main(){

	cout << "--------------    PopulateWorldTests    --------------"<< endl;
	populateWorldTests();

	cout << "--------------    FindPathTests         --------------"<< endl;
	findPathTests(); 

	
	printPath();
	
}





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
	Point t2   = {.x = 0, .y = 2};
	Point t3 = {.x = 2, .y = 0};
	Point t4  = {.x = 2, .y = 2};

	addTarget(t1, t2, t3, t4);

	Point v1 = {.x = -10, .y = -10};
	Point v2   = {.x = 10, .y = 10};
	Point v3 = {.x = -10, .y = 10};
	Point v4  = {.x = 10, .y = -10};

	addVertex(v1);
	addVertex(v2);
	addVertex(v3);
	addVertex(v4);

	Point testPoint = {.x=0, .y=0};

	/* Check arena bounds set correctly */
	cout << passFail(isInArena(testPoint) == true) << " isInArena in" << endl;;

	testPoint.x = 100;
	testPoint.y = 100;
	cout << passFail(isInArena(testPoint) == true) << " isInArena out " << endl;

	/* Check target location and verification working */
	testPoint.x = 1;
	testPoint.y = 1;
	cout << passFail(isOccupied(testPoint) == true) << " isOccupied is" << endl;

	cout << passFail(isEmpty(testPoint) == false) << " isEmpty isnt" << endl;

	testPoint.x = 8;
	testPoint.y = -1;
	cout << passFail(isOccupied(testPoint) == false) << " isOccupied isnt" << endl;

	cout << passFail(isEmpty(testPoint) == true) << " isEmpty is" << endl;
}

void findPathTests(){
	Point sta1 = {.x = 00, .y = 00};
	Point sto1 = {.x = 99, .y = 99};
	Point sta2 = {.x = 55, .y = 55};
	Point sto2 = {.x = 56, .y = 56};

	bool result = (calculateH(sta1, sto1) > calculateH(sta2, sto2));

	cout << passFail(result) << " calculateH approxDistance" << endl;

	cout << passFail(findPath(sta1, sto1)) << " path reported as successfully found" << endl;

}

int main(){

	cout << "--------------    PopulateWorldTests    --------------"<< endl;
	populateWorldTests();

	cout << "--------------    FindPathTests         --------------"<< endl;
	findPathTests(); 
}
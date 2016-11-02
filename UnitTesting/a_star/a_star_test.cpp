




#include <iostream>
#include <string>

#include <chrono>

#include "a_star_functions.h"


/* These are the tests we want to run */

using namespace std;

string passFail(bool item){
	return (item ? "PASS: " : "FAIL!: ");
}
void intersectingTests(){
	Point t1 = {.x = 0, .y = 0};
	Point t2   = {.x = 0, .y = 4};
	Point t3 = {.x = 0, .y = -100};
	Point t4  = {.x = 0, .y = 0};

	bool isVertex, result;

	cout << "    vertial lines" << endl;
	// intersecting where we stop when we get to the other one
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is" << endl;
	cout << passFail(isVertex == true) << " is Vertex is" << endl;

	// intersecting where we stop where we pass all the way thru one
	t4.y = 4;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is" << endl;
	cout << passFail(isVertex == true) << " is Vertex is" << endl;

	// intersecting where we stop where we pass part way thru one
	t4.y = 2;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is" << endl;
	cout << passFail(isVertex == false) << " is Vertex is" << endl;

	// not intersecting we stop before we get to the other one
	t4.y = -1;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == false) << " isIntersecting is not" << endl;
	cout << passFail(isVertex == false) << " is Vertex is not" << endl;

	// intersecting we pass all the way thru the other one
	t4.y = 10;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is not" << endl;
	cout << passFail(isVertex == false) << " is Vertex is not" << endl;

	// test non coincident vertical lines
	t4.x = 10;
	t3.x = 10;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == false) << " isIntersecting is not" << endl;
	cout << passFail(isVertex == false) << " is Vertex is not" << endl;

	cout << "    horizontal lines" << endl;
	t1.x = 3;
	t1.y = 10;

	t2.x = 10;
	t2.y = 10;

	t3.x = -100;
	t3.y = 10;

	t4.x = 3;
	t4.y = 10;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is" << endl;
	cout << passFail(isVertex == true) << " is Vertex is" << endl;

	t4.x = 4;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is" << endl;
	cout << passFail(isVertex == false) << " is Vertex is not" << endl;
	t4.x = 10;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is" << endl;
	cout << passFail(isVertex == true) << " is Vertex is" << endl;

	t4.x = 11;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is" << endl;
	cout << passFail(isVertex == false) << " is Vertex is not" << endl;

	cout << "    diagonal parallel lines"<< endl;
	t1.x = 1;
	t1.y = 1;

	t2.x = 10;
	t2.y = 10;

	t3.x = -100;
	t3.y = -100;

	// we go up to it but dont make it all the way
	t4.x = -1;
	t4.y = -1;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == false) << " isIntersecting is not" << endl;
	cout << passFail(isVertex == false) << " is Vertex is not" << endl;

	// we got up to it and stop at the vertex
	t4.x = 1;
	t4.y = 1;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is" << endl;
	cout << passFail(isVertex == true) << " is Vertex is" << endl;

	// we continue thru it a little bit
	t4.x = 3;
	t4.y = 3;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is" << endl;
	cout << passFail(isVertex == false) << " is Vertex is not" << endl;

	// we go all the way thru and stop at vertex
	t4.x = 10;
	t4.y = 10;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is" << endl;
	cout << passFail(isVertex == true) << " is Vertex is" << endl;

	// we go all the way thru and keep going
	t4.x = 11;
	t4.y = 11;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is" << endl;
	cout << passFail(isVertex == false) << " is Vertex is not" << endl;

	cout << "    non parallel lines" << endl;
	t1.x = -10;
	t1.y = -10;

	t2.x = 10;
	t2.y = 10;

	t3.x = -10;
	t3.y =  10;

	// approaching line
	t4.x = -1;
	t4.y =  1;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == false) << " isIntersecting is" << endl;
	cout << passFail(isVertex == false) << " is Vertex is not" << endl;

	// touching line
	t4.x = 0;
	t4.y = 0;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is" << endl;
	cout << passFail(isVertex == true) << " is Vertex is" << endl;

	// going thru line
	t4.x =  1;
	t4.y = -1;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is" << endl;
	cout << passFail(isVertex == false) << " is Vertex is not" << endl;

	// we have to test things like vertical with horizontal and diagonal with vertical and so on
	cout << "    combinations of different types of lines" << endl; 

	t3.x =  0;
	t3.y =  10;

	// approaching line
	t4.x =  0;
	t4.y =  1;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == false) << " isIntersecting is not" << endl;
	cout << passFail(isVertex == false) << " isVertex is not" << endl;

	// touching line
	t4.x =  0;
	t4.y =  0;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is" << endl;
	cout << passFail(isVertex == true) << " isVertex is" << endl;

	// passing thru
	t4.x =  0;
	t4.y =  -1;
	result = isIntersecting(t1, t2, t3, t4, &isVertex);
	cout << passFail(result == true) << " isIntersecting is" << endl;
	cout << passFail(isVertex == false) << " isVertex is not" << endl;
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

	addTarget(t12, t22, t32, t42);

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
	cout << passFail(isInArena(testPoint) == true) << " isInArena in" << endl;
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

void findPathTestsBigArenaSmallPath(){
	initWorld();

	Point t1 = {.x = 0, .y = 0};
	Point t2   = {.x = 4, .y = 0};
	Point t3 = {.x = 2, .y = 2};
	Point t4  = {.x = 0, .y = 2};

	addObstacle(t1, t2, t3, t4);

	Point t12 = {.x = 4, .y = -1};
	Point t22   = {.x =6, .y = -1};
	Point t32 = {.x = 6, .y =1};
	Point t42  = {.x = 4, .y = 1};

	addObstacle(t12, t22, t32, t42);

	Point v1 = {.x = -100, .y = -100};
	Point v2   = {.x = 100, .y = -100};
	Point v3 = {.x = 100, .y = 100};
	Point v4  = {.x = -100, .y = 100};

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

	printPath();

	resetPath();

	cout << passFail(findPath(sto2, sta2)) << " path reported as successfully found" << endl;

	printPath();

	resetPath();

	Point sta3 = {.x = -1, .y = -1};
	Point sto3 = {.x = 5, .y = 5};

	cout << passFail(findPath(sta3, sto3)) << " path reported as successfully found" << endl;

	printPath();

	resetPath();

	Point o1 = {.x = 10, .y = 10};
	Point o2   = {.x =18, .y = 10};
	Point o3 = {.x = 18, .y =18};
	Point o4  = {.x = 10, .y = 18};

	addObstacle(o1, o2, o3, o4);

	Point sta4 = {.x = -1, .y = -1};
	Point sto4 = {.x = 14, .y = 14};
	cout << passFail(findPath(sta4, sto4) == false) << " impossible reported as rejected" << endl;

	resetPath();

	Point sta5 = {.x = -1, .y = -1};
	Point sto5 = {.x = 105, .y = 105};
	cout << passFail(findPath(sta5, sto5) == false) << " impossible reported as rejected" << endl;


	printWorld();

}

void findFarPathsTests(){
	initWorld();

	Point t1 = {.x = 0, .y = 0};
	Point t2   = {.x = 4, .y = 0};
	Point t3 = {.x = 2, .y = 2};
	Point t4  = {.x = 0, .y = 2};

	addObstacle(t1, t2, t3, t4);

	Point t12 = {.x = 4, .y = -1};
	Point t22   = {.x =6, .y = -1};
	Point t32 = {.x = 6, .y =1};
	Point t42  = {.x = 4, .y = 1};

	addObstacle(t12, t22, t32, t42);

	Point v1 = {.x = -100, .y = -100};
	Point v2   = {.x = 100, .y = -100};
	Point v3 = {.x = 100, .y = 100};
	Point v4  = {.x = -100, .y = 100};

	addVertex(v1);
	addVertex(v2);
	addVertex(v3);
	addVertex(v4);

	resetPath();

	Point sta1 = {.x = -1, .y = -1};
	Point sto1   = {.x = 30, .y = 30};	
	cout << passFail(findPath(sta1, sto1)) << " path reported as successfully found" << endl;
	printPath();


}

int main(){

	cout << "--------------    intersectingTests     --------------"<< endl;
	intersectingTests();

	cout << "--------------    PopulateWorldTests    --------------"<< endl;
	populateWorldTests();

	cout << "--------------    findPathTestsBigArenaSmallPath------"<< endl;
	findPathTestsBigArenaSmallPath(); 

	auto startTime = chrono::steady_clock::now();
	cout << "--------------    findFarPathsTests------"<< endl;
	findFarPathsTests(); 
	auto endTime = chrono::steady_clock::now();
    cout << "elapsed " << chrono::duration_cast<chrono::microseconds>(endTime - startTime).count() << endl;
		


}

void openListHeapTests(){

    // Point v1 = {.x = 0, .gScore = 0};
    Point v1 = {.x = -100, .y = -100, .gScore = 0};
    Point v2 = {.x = -100, .y = -100, .gScore = 10};
    Point v3 = {.x = -100, .y = -100, .gScore = 1};
    Point v4 = {.x = -100, .y = -100, .gScore = 2};

    openHeapAdd(v1);
    openHeapAdd(v2);
	openHeapAdd(v3);
	openHeapAdd(v4);

	Point temp = openHeapRemove();
	cout  <<temp.gScore +  temp.hScore<< endl;
	temp = openHeapRemove();
	cout << temp.gScore +  temp.hScore<< endl;
	temp = openHeapRemove();
	cout << temp.gScore +  temp.hScore<< endl;
	temp = openHeapRemove();
	cout << temp.gScore +  temp.hScore<< endl;



	printOpenHeap();
}

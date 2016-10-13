
#include "a_star_functions.h"

#define listLength 100
#define maxVerticies 25

static Point openList[listLength];
static Point closedList[listLength];
static Point finalPath[listLength];

static Point verticies[maxVerticies];


/* initializes grid and open, closed, and final point lists */
void initWorld(){

}

/* add a target to the world */
void addTarget(Point one, Point two, Point three, Point four){

}

/* add an obstacle to the world */
void addObstacle(Point one, Point two, Point three, Point four){

}

/* add a vertex to the world, more than 3 makes an arena */
void addVertex(Point one){

}

/* determines if a point is in the arena */
bool isInArena(Point one){
	return false;
}

/* determines if a point is traversable */
bool isEmpty(Point one){
	return false;
}

/* opposite of isEmpty */
bool isOccupied(Point one){
	return ! isEmpty(one);
}

/* gets 4 points adjacent to the specified one */
bool getAdjacent(Point target, Point &one, Point &two, Point &three, Point &four){
	return false;
}

/* estimates distance to destination by counting city blocks */
int calculateH(Point start, Point stop){
	return -1;
}

bool addToClosedList(Point one){
	return false;
}

bool addWalkableLocations(){
	return false;
}

Point findBestStep(){
	Point ex = {.x = 0, .y = 0};
	return ex;
}

bool finalizePath(){
	return false;
}

bool findPath(Point start, Point stop){
	return false;
}


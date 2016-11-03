
#include <stdbool.h>
#include <limits.h>

#include <stdlib.h>
#include <math.h>

#define ADJ_LEN 3

typedef struct Point{
	float x;
	float y;
	unsigned char gScore;
	unsigned char hScore;
	unsigned short parent;
} Point;

//typedef enum {
//    forward, back, left, right;    
//}Direction;
//
//typedef struct Move{
//	Direction dir;
//    float distance;
//} Move;


/* initializes grid and open, closed, and final point lists */
void initWorld();

/* add a target to the world */
void addTarget(Point one, Point two, Point three, Point four);

/* add an obstacle to the world */
void addObstacle(Point one, Point two, Point three, Point four);

/* add a vertex to the world, more than 3 makes an arena */
void addVertex(Point one);

/* determines if a point is in the arena */
bool isInArena(Point one);

/* determines if a point is traversable */
bool isEmpty(Point one);

/* opposite of isEmpty */
bool isOccupied(Point one);

/* gets 4 points adjacent to the specified one */
bool getAdjacent(Point target, Point *one, Point *two, Point *three, Point *four, int parentIndex);

/* estimates distance to destination by counting city blocks */
int calculateH(Point start, Point stop);

bool addToClosedList(Point one, Point end);

void addWalkableLocations(Point current, Point stop, int parentIndex);

Point findBestStep();

bool finalizePath();

bool findPath(Point start, Point stop);

bool isInObstacle(Point one, int obIndex);
bool isInTarget(Point one, int obIndex);
bool isOutAllTargets(Point one);
bool isOutAllObstacles(Point one);

void resetPath();

void printPath();
void printWorld();
bool isIntersecting(Point rayStart, Point rayEnd, Point three, Point four, bool *isVertex);

void openHeapAdd(Point one);
Point openHeapRemove();
void printOpenHeap();
int indexSmallerChild(int index);

bool can_I_go_targ(Point start);
Point getNextLoc(Point start);




/* steps for A_Star */

// add current location to closed list

// while(destination is not in closed list)

	// add walkable locations to open list (calculating F = G + H scores for them)

	// add lowest F score to the closed list

// work backward from your target 



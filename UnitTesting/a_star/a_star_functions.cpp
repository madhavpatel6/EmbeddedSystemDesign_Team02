
#include "a_star_functions.h"

#include "iostream"
using namespace std;

#define listLength 100
#define maxVerticies 25
#define pointsToObject 4


static Point openList[listLength];
static int openLen;
static Point closedList[listLength];
static int closedLen;
static Point finalPath[listLength];
static int finalLen;


static Point targets[listLength][pointsToObject];
static int targetLen;
static Point obstacles[listLength][pointsToObject];
static int obstacleLen;
static Point verticies[maxVerticies];
static int vertexLen;


/* initializes grid and open, closed, and final path lists */
void initWorld(){
	openLen = 0;
	closedLen = 0;
	finalLen = 0;

	targetLen = 0;
	obstacleLen = 0;
	vertexLen = 0;
}

/* add a target to the world */
void addTarget(Point one, Point two, Point three, Point four){
	targets[targetLen][0] = one;
	targets[targetLen][1] = two;
	targets[targetLen][2] = three;
	targets[targetLen][3] = four;

	targetLen++;
}

/* add an obstacle to the world */
void addObstacle(Point one, Point two, Point three, Point four){
	obstacles[obstacleLen][0] = one;
	obstacles[obstacleLen][1] = two;
	obstacles[obstacleLen][2] = three;
	obstacles[obstacleLen][3] = four;

	obstacleLen++;
}

/* add a vertex to the world, more than 3 makes an arena */
void addVertex(Point one){
	verticies[vertexLen] = one;

	vertexLen++;
}

/* ray starts at -inf -inf and goes to rayend, intersects with 3 4*/
bool isIntersectingRay(Point rayEnd, Point three, Point four, bool *isVertex){
	*isVertex = false;
	Point rayStart = {.x = INT_MIN, .y = INT_MIN};

	// cout << rayEnd.x << " " << rayEnd.y << "\n";

	// cout << three.x << " " << three.y << "\n";
	// cout << four.x << " " << four.y << "\n";

	//m1 is only inf if one is -inf y
	float m1 = ((float) rayEnd.y - (float)rayStart.y) / ((float)rayEnd.x - (float)rayStart.x);
	float b1 = rayEnd.y - m1 * rayEnd.x;

	float m2, b2, x, y;
	if(four.x - three.x == 0 ){
		x = four.x;
		y = m1 * x + b1;
		// cout << "vertical line" << endl;
	}else{
		m2 = (four.y - three.y) / (four.x - three.x);
	    b2 = three.y - m2 * three.x;
	    // cout << "m1 " << m1 << " b1 " << b1 << endl;
	    // cout << "m2 " << m2 << " b2 " << b2 << endl;
	    // cout << "b2-b1 " << b2-b1 << " m1-m2 " << m1-m2 << endl;
	    x = (b2 - b1) / (m1 - m2);
		y = m1 * x + b1;
	}


	// cout << "m1 " << m1 << " b1 " << b1 << endl;
	// cout << "m2 " << m2 << " b2 " << b2 << endl;

	// cout << "x " << x << " y " << y << endl;

	//return true if that intersecting point is in both of the line segments
	if ( x <= rayEnd.x && y <= rayEnd.y &&
			x >= min(three.x, four.x) &&
			x <= max(three.x, four.x) &&
			y >= min(three.y, four.y) &&
			y <= max(three.y, four.y) ){

		if ( (x == min(three.x, four.x) ||
				x == max(three.x, four.x)) &&
				(y == min(three.y, four.y)||
				y == max(three.y, four.y) ) ){
			*isVertex = true;
					
		}

		return true;
	}else{
		return false;
	}
}

/* determines if a point is in the arena */
bool isInArena(Point one){
	int count = 0;
	int vertCount = 0;
	int i;

	bool crap;

	//return true if point is a vertex
	for(i = 0; i < vertexLen; i++){
		if( verticies[i].x == one.x  && verticies[i].y == one.y){
			return true;
		}
	}
	// count intersections of ray going to -inf -inf	
	for(i = 0; i < vertexLen; i++){
		Point start = verticies[i];
		Point end = (i == vertexLen-1 ? verticies[0] : verticies[i+1]);
		// cout << "start " << start.x << " " << start.y << endl;
		// cout << "end " << end.x << " " << end.y << endl;
		if( isIntersectingRay(one, start, end, &crap)){
			if(crap == true){
				vertCount++;
			}else{
				// cout << "end " << end.x << " " << end.y << endl;
				count++;
			}
		}
	}
	// cout << count << " " << vertCount << endl;
	if((count + (vertCount / 2)) % 2 == 0){
		return false;
	}else{
		return true;
	}
}

bool isInObstacle(Point one, int obIndex){
	int count = 0;
	int vertCount = 0;
	int i;

	bool crap;


	//return true if point is a vertex
	for(i = 0; i < pointsToObject; i++){
		if( obstacles[obIndex][i].x == one.x  && obstacles[obIndex][i].y == one.y){
			return true;
		}
	}
	// count intersections of ray going to -inf -inf	
	for(i = 0; i < pointsToObject; i++){
		Point start = obstacles[obIndex][i];
		Point end = i == pointsToObject-1 ? obstacles[obIndex][0] : obstacles[obIndex][i+1];
		if( isIntersectingRay(one, start, end, &crap)){
			if(crap == true){
				vertCount++;
			}else{
				// cout << "end " << end.x << " " << end.y << endl;
				count++;
			}
		}
	}

	// cout << "coutnt " <<count<< endl;
	if((count + (vertCount / 2)) % 2 == 0){
		return false;
	}else{
		return true;
	}

}
bool isInTarget(Point one, int obIndex){
	int count = 0;
	int vertCount = 0;
	int i;

	bool crap;


	//return true if point is a vertex
	for(i = 0; i < pointsToObject; i++){
		if( targets[obIndex][i].x == one.x  && targets[obIndex][i].y == one.y){
			return true;
		}
	}
	// count intersections of ray going to -inf -inf	
	for(i = 0; i < pointsToObject; i++){
		Point start = targets[obIndex][i];
		Point end = i == pointsToObject-1 ? targets[obIndex][0] : targets[obIndex][i+1];
		if( isIntersectingRay(one, start, end, &crap)){
			if(crap == true){
				vertCount++;
			}else{
				// cout << "end " << end.x << " " << end.y << endl;
				count++;
			}
		}
	}

	// cout << "coutnt " <<count<< endl;
	if((count + (vertCount / 2)) % 2 == 0){
		return false;
	}else{
		return true;
	}
}
bool isOutAllTargets(Point one){
	int i;
	for(i = 0; i < targetLen; i++){
		if(isInTarget(one, i)){
			return false;
		}
	}
	return true;
}
bool isOutAllObstacles(Point one){
	int i;
	for(i = 0; i < obstacleLen; i++){
		if(isInObstacle(one, i) == true){
			return false;
		}
	}
	return true;
}

/* determines if a point is traversable */
bool isEmpty(Point one){
	return isOutAllObstacles(one) && isOutAllTargets(one) && isInArena(one);
}

/* opposite of isEmpty */
bool isOccupied(Point one){
	return ! isEmpty(one);
}

/* gets 4 points adjacent to the specified one */
bool getAdjacent(Point target, Point *one, Point *two, Point *three, Point *four, int parentIndex){
	one->x = target.x + 1;
	one->y = target.y;
	one->gScore = target.gScore + 1;
	one->parent = parentIndex;
	two->x = target.x;
	two->y = target.y + 1;
	two->gScore = target.gScore + 1;
	two->parent = parentIndex;
	three->x = target.x - 1;
	three->y = target.y;
	three->gScore = target.gScore + 1;
	three->parent = parentIndex;
	four->x = target.x;
	four->y = target.y - 1;
	four->gScore = target.gScore + 1;
	four->parent = parentIndex;

	return true;
}

/* estimates distance to destination by counting city blocks */
int calculateH(Point start, Point stop){
	return abs(start.x - stop.x) + abs(start.y - stop.y);
}

bool addToClosedList(Point one, Point end){
	closedList[closedLen].x  = one.x;
	closedList[closedLen].y  = one.y;
	closedList[closedLen].gScore  = one.gScore;
	closedList[closedLen].hScore = calculateH(one, end);
	closedList[closedLen].parent = one.parent;
	
	closedLen++;

	return false;
}
bool addToOpenList(Point one, Point end){
	openList[openLen].x  = one.x;
	openList[openLen].y  = one.y;
	openList[openLen].gScore  = one.gScore;
	openList[openLen].hScore = calculateH(one, end);
	openList[openLen].parent = one.parent;
	
	openLen++;

	return false;
}

bool isInOpenList(Point one){
	for(int i = 0 ; i < closedLen; i++){
		// this may need to be within boundry not straigh equality later
		if(openList[i].x == one.x &&
			openList[i].y == one.y ){
			return true;
		}
	}
	return false;
}

bool isInClosedList(Point one){
	for(int i = 0 ; i < closedLen; i++){
		// this may need to be within boundry not straigh equality later
		if(closedList[i].x == one.x &&
			closedList[i].y == one.y ){
			return true;
		}
	}
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

	start.gScore = 0;
	start.hScore = calculateH(start, stop);
	Point current = start;

	// add current location to closed list
	addToClosedList(current, stop);

	int count = 0;
	int parentIndex = 0;

	// while(destination is not in closed list)
	while( ! isInClosedList(stop) && count <= 100){
		// cout << current.x << " " << current.y << " "<< current.gScore <<" " << current.hScore<<endl;
		// add walkable locations to open list (calculating F = G + H scores for them)
		Point one, two, three, four;
		getAdjacent(current, &one, &two, &three, &four, parentIndex); // determines f scores

		if(isEmpty(one) && !isInOpenList(one) ){//&& !isInClosedList(one)){
			addToOpenList(one, stop);
		}
		if(isEmpty(two)&& !isInOpenList(two) ){//&& !isInClosedList(one)){
			addToOpenList(two, stop);
		}
		if(isEmpty(three)&& !isInOpenList(three) ){//&& !isInClosedList(one)){
			addToOpenList(three, stop);
		}
		if(isEmpty(four)&& !isInOpenList(four)){
			addToOpenList(four, stop);
		}
		// cout << "openLen " << openLen << endl;

		int minFscore = INT_MAX; // i hope openList has something in it
		int minFScoreIndex = -1;
		for(int j = 0; j < openLen; j++){
			if( !isInClosedList(openList[j]) && 
				openList[j].gScore + openList[j].hScore < minFscore){
				minFscore = openList[j].gScore + openList[j].hScore;
				minFScoreIndex = j;
			}

		}
		// cout << "chosen OpenIndex " << minFScoreIndex<< endl;

		// add lowest F score to the closed list
		// openList[minFScoreIndex].parent = parentIndex;
		parentIndex = closedLen;
		addToClosedList(openList[minFScoreIndex], stop);
		current = openList[minFScoreIndex];
		// parentIndex = minFScoreIndex;
		count ++;
	}


	// for( int i =0; i < closedLen; i++){
	// 	cout << i<<" patStep "<< closedList[i].x << " " << closedList[i].y <<" " << closedList[i].parent<<endl;
	// }
	int i, index;
	for(i = closedLen-1; i > 0; i = closedList[i].parent){
		//cout << i<<" patStep "<< closedList[i].x << " " << closedList[i].y <<" " << closedList[i].parent<<endl;
		finalPath[index] = closedList[i];
		index++;
	}
	finalPath[index] = closedList[i];
	finalLen = index +1;
	return count < 100;
}

void printPath(){
	for( int i =0; i < finalLen; i++){
		cout << i<<" patStep "<< finalPath[i].x << " " << finalPath[i].y <<" " << finalPath[i].parent<<endl;
	}
}

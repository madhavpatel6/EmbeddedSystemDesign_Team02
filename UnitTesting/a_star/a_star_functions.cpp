
#include "a_star_functions.h"

#include "iostream"
using namespace std;

#define listLength 5000
#define MAXLOOPCOUNT 1100
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

bool isIntersecting(Point rayStart, Point rayEnd, Point three, Point four, bool *isVertex){
	*isVertex = false;

	//two vertical special case
	if(rayStart.x == rayEnd.x && three.x == four.x && rayEnd.x == three.x){
		//vertical coincident lines

		//if ray starts or ends at the edges of y they are a vertex and will be double counted later
		*isVertex = (rayEnd.y   == min(three.y, four.y) || rayEnd.y   == max(three.y, four.y) ||
					 rayStart.y == min(three.y, four.y) || rayStart.y == max(three.y, four.y));
		return rayEnd.y   >= min(three.y, four.y) && rayEnd.y   <= max(three.y, four.y) ||
			   rayStart.y >= min(three.y, four.y) && rayStart.y <= max(three.y, four.y);
	}else if(rayStart.x == rayEnd.x && three.x == four.x){
		// vertical non coincident lines
		*isVertex = false;
		return false;
	}

	float m1 = (rayEnd.y - rayStart.y) / (rayEnd.x - rayStart.x);
	float m2 = (four.y   - three.y   ) / (four.x   - three.x   );

	float b1 = rayEnd.y - m1 * rayEnd.x;
    float b2 = three.y  - m2 * three.x;

    // parallel lines special case
    if(m1 == m2 && b1 == b2){
    	*isVertex = (rayEnd.y   == min(three.y, four.y) || rayEnd.y   == max(three.y, four.y) ||
					 rayStart.y == min(three.y, four.y) || rayStart.y == max(three.y, four.y)) &&
    				(rayEnd.x   == min(three.x, four.x) || rayEnd.x   == max(three.x, four.x) ||
					 rayStart.x == min(three.x, four.x) || rayStart.x == max(three.x, four.x));
    	return (rayEnd.y   >= min(three.y, four.y) && rayEnd.y   <= max(three.y, four.y) ||
			    rayStart.y >= min(three.y, four.y) && rayStart.y <= max(three.y, four.y)) && 
    		   (rayEnd.x   >= min(three.x, four.x) && rayEnd.x   <= max(three.x, four.x) ||
			    rayStart.x >= min(three.x, four.x) && rayStart.x <= max(three.x, four.x));
    }else if ((m1 == m2)){
    	*isVertex = false;
    	return false;
    }
    float x, y;

    // find the intersection point
    if( rayEnd.x == rayStart.x){
    	x = rayEnd.x;
    	y = m2 * x + b2;
    }else if(three.x == four.x){
    	x = three.x;
    	y = m1 * x + b1;
    }else{
    	x = (b2 - b1) / (m1 - m2);
		y = m1 * x + b1;
    }


	*isVertex = ( (x == min(three.x, four.x) ||
				   x == max(three.x, four.x)     ) &&
				  (y == min(three.y, four.y)  ||
				   y == max(three.y, four.y)     )  );

	return  x >= min(rayStart.x, rayEnd.x) &&
			x <= max(rayStart.x, rayEnd.x) &&
			y >= min(rayStart.y, rayEnd.y) &&
			y <= max(rayStart.y, rayEnd.y) &&

			x >= min(three.x, four.x) &&
			x <= max(three.x, four.x) &&
			y >= min(three.y, four.y) &&
			y <= max(three.y, four.y);
}


/* determines if a point is in the arena */
bool isInArena(Point one){
	int count = 0;
	int vertCount = 0;
	int i;

	bool crap;
	bool crap2;
	int count2 = 0; 
	int vertCount2 = 0;

	Point s1 = {.x = INT_MIN, .y = INT_MIN};
	Point s2 = {.x = INT_MIN, .y = one.y};

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
		if( isIntersecting(s1, one, start, end, &crap)){
			if(crap == true){
				vertCount++;
			}else{
				// cout << "end " << end.x << " " << end.y << endl;
				count++;
			}
		}
		if( isIntersecting(s2, one, start, end, &crap2)){
			// cout << crap2<< endl;
			if(crap2 == true){
				vertCount2++;
			}else{
				// cout << "end " << end.x << " " << end.y << endl;
				count2++;
			}
		}
	}
	// cout << count << " " << vertCount << " " << count2 << " " << vertCount2 << endl;
	if((count + (vertCount / 2)) % 2 == 0 || (count2 + (vertCount2 / 2)) % 2 == 0){
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
	bool crap2;
	int count2 = 0; 
	int vertCount2 = 0;

	Point s1 = {.x = INT_MIN, .y = INT_MIN};
	Point s2 = {.x = INT_MIN, .y = one.y};

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
		if( isIntersecting(s1, one, start, end, &crap)){
			if(crap == true){
				vertCount++;
			}else{
				// cout << "end " << end.x << " " << end.y << endl;
				count++;
			}
		}
		if( isIntersecting(s2, one, start, end, &crap2)){
			if(crap2 == true){
				vertCount2++;
			}else{
				// cout << "end " << end.x << " " << end.y << endl;
				count2++;
			}
		}

	}

	// cout << "count  " <<count<< " " << vertCount<< endl;
	// cout << "count2 " <<count2<< " " << vertCount2<< endl;
	if((count + (vertCount / 2)) % 2 == 0 || (count2 + (vertCount2 / 2)) % 2 == 0){
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
	bool crap2;
	int count2 = 0; 
	int vertCount2 = 0;

	Point s1 = {.x = INT_MIN, .y = INT_MIN};
	Point s2 = {.x = INT_MIN, .y = one.y};


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
		if( isIntersecting(s1, one, start, end, &crap)){
			if(crap == true){
				vertCount++;
			}else{
				// cout << "end " << end.x << " " << end.y << endl;
				count++;
			}

		}
		if( isIntersecting(s2, one, start, end, &crap2)){
			if(crap2 == true){
				vertCount2++;
			}else{
				// cout << "end " << end.x << " " << end.y << endl;
				count2++;
			}
		}
	}

	// cout << "coutnt " <<count<< endl;
	if((count + (vertCount / 2)) % 2 == 0 || (count2 + (vertCount2 / 2)) % 2 == 0){
		return false;
	}else{
		return true;
	}
}
bool isOutAllTargets(Point one){
	int i;
	for(i = 0; i < targetLen; i++){
		if(isInTarget(one, i) == true){
			return false;
		}
	}
	return true;
}
bool isOutAllObstacles(Point one){
	int i;
	for(i = 0; i < obstacleLen; i++){
		if(isInObstacle(one, i) == true){
			// cout << "in ob" << i<<endl;
			return false;
		}
	}
	return true;
}

/* determines if a point is traversable */
bool isEmpty(Point one){
	// cout << isOutAllObstacles(one) << isOutAllTargets(one) << isInArena(one)<<endl;
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
	return fabs(start.x - stop.x) + fabs(start.y - stop.y);
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

void addWalkableLocations(Point current, Point stop, int parentIndex){
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
}

bool finalizePath(){
	int i;
	int index = 0;
	for(i = closedLen-1; i > 0; i = closedList[i].parent){
		//cout << i<<" patStep "<< closedList[i].x << " " << closedList[i].y <<" " << closedList[i].parent<<endl;
		finalPath[index] = closedList[i];
		index++;
	}
	finalPath[index] = closedList[i];
	finalLen = index +1;
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
	while( !isInClosedList(stop) && count <= MAXLOOPCOUNT){
		// possible faster while condition (current.x != stop.x || current.y != stop.y) && count <= MAXLOOPCOUNT){

		// add walkable locations to open list (calculating F = G + H scores for them)
		addWalkableLocations(current, stop, parentIndex);

		int minFscore = INT_MAX; // i hope openList has something in it
		int minFScoreIndex = -1;
		for(int j = 0; j < openLen; j++){
			if( !isInClosedList(openList[j]) && 
					openList[j].gScore + openList[j].hScore < minFscore){
				minFscore = openList[j].gScore + openList[j].hScore;
				minFScoreIndex = j;
			}

		}
		// cout << openLen << " " << count << " " << closedLen<< endl;
		parentIndex = closedLen;
		addToClosedList(openList[minFScoreIndex], stop);
		current = openList[minFScoreIndex];

		count ++;
	}

	finalizePath();
	return count < MAXLOOPCOUNT;
}

void printPath(){
	for( int i =0; i < finalLen; i++){
		cout << i<<" patStep "<< finalPath[i].x << " " << finalPath[i].y <<" " << finalPath[i].parent<<endl;
	}
}

void resetPath(){
	// for(int i = 0; i < openLen; i++){

	// }
	// for(int i = 0; i < closedLen; i++){
		
	// }
	// for(int i = 0; i < finalLen; i++){
		
	// }

	openLen = 0;
	closedLen = 0;
	finalLen = 0;
}

void printWorld(){
	for(int i = 0 ; i < targetLen; i++){
		for(int j = 0; j < pointsToObject; j++){
			cout << "("<<targets[i][j].x << "," << targets[i][j].y << ") " ;
		}
		cout <<endl;
	}
	for(int i = 0 ; i < obstacleLen; i++){
		for(int j = 0; j < pointsToObject; j++){
			cout << "("<<obstacles[i][j].x << "," << obstacles[i][j].y << ") " ;
		}
		cout <<endl;
	}
}

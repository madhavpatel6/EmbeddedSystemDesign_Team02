

typedef struct Point{
	int x;
	int y;
	int gScore;
	int hScore;
} Point;


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
bool getAdjacent(Point target, Point &one, Point &two, Point &three, Point &four);

/* estimates distance to destination by counting city blocks */
int calculateH(Point start, Point stop);

bool addToClosedList(Point one);

bool addWalkableLocations();

Point findBestStep();

bool finalizePath();

bool findPath(Point start, Point stop);


/* steps for A_Star */

// add current location to closed list

// while(destination is not in closed list)

	// add walkable locations to open list (calculating F = G + H scores for them)

	// add lowest F score to the closed list

// work backward from your target 



#pragma once

#include <ncltech\CommonUtils.h>
#include <list>


class MapNavigation
{
public:
	MapNavigation();

	void writeToFile(struct MapNode*);
	void readFromFile();
	void checkConnectedNeighbourNodes();
	MapNode* getGrid(int a, int b) { return grid[a][b]; };
	void convertPositionToGridSpace(GameObject*, int& row, int& col);
	nclgl::Maths::Vector3 convertGridToVectorSpace(int a, int b);
	void generateAStar();
	void generatePath();
	void addToOpenList(MapNode* a) { openList.push_back(a); }
	std::list<MapNode*>& getOpenList() { return openList; }
	void addToClosedList(MapNode* a) { closedList.push_back(a); }
	void addToWayPoints(nclgl::Maths::Vector3 a) { wayPoints.push_back(a); }
	nclgl::Maths::Vector3 getWayPoint(int a) { return wayPoints[a];}
	void usePath();
	
private:
	int index = 0;
	MapNode* grid[100][100];
	std::list <MapNode*> openList;
	std::list <MapNode*> closedList;
	std::vector <nclgl::Maths::Vector3> wayPoints;
};

struct MapNode
{
	int objX = 1; int objZ = 1;
	int getObjectiveX() { return objX; }
	int getObjectiveZ() { return objZ; }
	void setPosition(int x, int z) {posX = x; posZ = z;}
	int getPosition() { return posX, posZ; }
	void addConnectedNodes(MapNode *a) { connectedNodes.push_back(a); }
	std::list <MapNode*> getConnectedNodes() { return connectedNodes; }
	std::list <MapNode*> connectedNodes;
	int CalcCostForMapNode();
	void setParent(MapNode* a) { parent = a; }
	MapNode* getParent() { return parent; }
	
	int type;
	int posX;
	int posZ;
	int movementCost;
	int estimateCost;
	MapNode* parent;
};
#pragma once

#include <ncltech\CommonUtils.h>
#include <queue>

struct MapNode
{
	int objX = 18; int objZ = 18;
	int getObjectiveX() { return objX; }
	int getObjectiveZ() { return objZ; }
	void setPosition(int x, int z) { posX = x; posZ = z; }
	int getPosition() { return posX, posZ; }
	void addConnectedNodes(MapNode a) { connectedNodes.insert(a); }
	std::set<MapNode> getConnectedNodes() { return connectedNodes; }
	std::set<MapNode> connectedNodes;
	int CalcCostForMapNode();
	void setParent(MapNode* a) { parent = a; }
	MapNode getParent() { return *parent; }
	bool operator<(const MapNode& right) const 
	{
		if (this == &right)
		{
			return false;
		}
		if (totalCost == right.totalCost)
			return ID < right.ID;
		else return totalCost < right.totalCost;
		}
	int type;
	int posX;
	int posZ;
	int movementCost;
	int estimateCost;
	int totalCost;
	MapNode* parent;
	int ID;
};


class MapNavigation
{
public:
	MapNavigation();

	void writeToFile(struct MapNode);
	void readFromFile();
	void checkConnectedNeighbourNodes();
	MapNode& getGrid(int a, int b) { return grid[a][b]; };
	void convertPositionToGridSpace(GameObject*, int& row, int& col);
	nclgl::Maths::Vector3 convertGridToVectorSpace(int a, int b);
	void generateAStar();
	void generatePath();
	void addToOpenList(MapNode a) { openList.insert(a); }
	void addToClosedList(MapNode a) { closedList.insert(a); }
	void addToWayPoints(nclgl::Maths::Vector3 a) { wayPoints.push_back(a); }
	nclgl::Maths::Vector3 getWayPoint(int a) { return wayPoints[a];}
	void usePath();
	
private:
	int index = 0;
	MapNode grid[100][100];
	
	std::set <MapNode> openList;
	std::set <MapNode> closedList;
	std::vector <nclgl::Maths::Vector3> wayPoints;
};


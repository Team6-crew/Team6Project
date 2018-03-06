#include "MapNavigation.h"
#include <iostream>
#include "fstream"
#include "Definitions.h"
#include "GameLogic.h"

int gridSq = 20;

using namespace nclgl::Maths;

class MapNavigation;

MapNavigation::MapNavigation()
{
	for (long i = 0; i < gridSq; i++)
	{
		for (long j = 0; j < gridSq; j++)
		{
			grid[i][j] = new MapNode;
			grid[i][j]->type = 0;
			grid[i][j]->setPosition(i, j);
			grid[i][j]->movementCost = 0;
			grid[i][j]->estimateCost = 0;
		}
	}
	
	readFromFile();
	/*MapNode* a = new MapNode();
	writeToFile(a);*/
}

void MapNavigation::writeToFile(MapNode* a)
{
		std::ofstream myfile;
		myfile.open("map.txt");
		if (myfile.is_open())
		{
			for (int i = 0; i < gridSq; i++)
			{	
				for (int j = 0; j < gridSq; j++)
				{
					myfile <<grid[i][j]->type << " ";
				}
				myfile << "\n";
			}
			
		}
		myfile.close();
}

void MapNavigation::readFromFile()
{
	std::list<int> nums;
	

	std::ifstream myReadFile;
	myReadFile.open("map.txt");
	string num;
	int col = 0;
	int row = 0;
	
			if (myReadFile.is_open())
			{
				while (!myReadFile.eof())
				{
					int node;
					remove(num.begin(), num.end(), '\n');
					getline(myReadFile, num,' ');
					{
						if (myReadFile.eof())
							break;
						std::string::size_type sz;
						node = std::stof(num, &sz);
						nums.push_back(node);
					}
				}
				myReadFile.close();
			}	
				std::list<int>::iterator it = nums.begin(); it != nums.end();
				for (int i = 0; i < gridSq; i++)
				{
					for (int j = 0; j < gridSq; j++)
					{
						{
							getGrid(i, j)->type = *it;
							//cout << getGrid(i, j)->type << " ";
							it++;
						}
					}
				}
			

	checkConnectedNeighbourNodes();
	generateAStar();
	generatePath();
	
	
}

void MapNavigation::checkConnectedNeighbourNodes()
{
	for (int i = 0; i < gridSq; i++)
	{
		for (int j = 0; j < gridSq; j++)
		{
			if (i > 0 && j > 0 && i < gridSq - 1 && j < gridSq - 1) //not border
			{
				if (getGrid(i - 1, j)->type != 1) // horible checks to see if next to a border
				{
					if (getGrid(i - 1, j - 1)->type != 1)
					{
						if (i + 1 != gridSq)
						{
							if (j + 1 != gridSq) //has 4 neighbours
							{
								if (getGrid(i - 1, j)->type == 0) // left
								{
									getGrid(i, j)->addConnectedNodes(getGrid(i - 1, j));
									cout << i << " " << j << " neighbours " << i - 1 << " " << j << "\n";
								}
								if (getGrid(i, j - 1)->type == 0) //down
								{
									getGrid(i, j)->addConnectedNodes(getGrid(i, j - 1));
									cout << i << " " << j << " neighbours " << i << " " << j - 1 << "\n";
								}
								if (getGrid(i + 1, j)->type == 0) //right
								{
									getGrid(i, j)->addConnectedNodes(getGrid(i + 1, j));
									cout << i << " " << j << " neighbours " << i + 1 << " " << j << "\n";
								}
								if (getGrid(i, j + 1)->type == 0) //up
								{
									getGrid(i, j)->addConnectedNodes(getGrid(i, j + 1));
									cout << i << " " << j << " neighbours " << i << " " << j + 1 << "\n";
								}

							}

						}
					}
				}
			}
			
			// 3neighbours
			 if ((i || j != 0) && (i || j < gridSq - 1))
			{
				if (i == 0 && j > 0) //wall left
				{
					if (getGrid(i, j - 1)->type == 0) //down
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i, j - 1));
						cout << i << " " << j << " neighbours " << i << " " << j - 1 << "\n";
					}
					if (getGrid(i + 1, j)->type == 0) //right
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i + 1, j));
						cout << i << " " << j << " neighbours " << i + 1 << " " << j << "\n";
					}
					if (getGrid(i, j + 1)->type == 0) //up
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i, j + 1));
						cout << i << " " << j << " neighbours " << i << " " << j + 1 << "\n";
					}
				}
				if (i ==0 && j > 0) // wall up
				{
					if (getGrid(i, j - 1)->type == 0) //down
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i, j - 1));
						cout << i << " " << j << " neighbours " << i << " " << j - 1 << "\n";
					}
					if (getGrid(i + 1, j)->type == 0) //right
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i + 1, j));
						cout << i << " " << j << " neighbours " << i + 1 << " " << j << "\n";
					}
					if (getGrid(i, j + 1)->type == 0) //up
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i, j + 1));
						cout << i << " " << j << " neighbours " << i << " " << j + 1 << "\n";
					}

				}
				if (i < gridSq-1 && j == gridSq-1) // wall right
				{
					if (getGrid(i, j - 1)->type == 0) //down
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i, j + 1));
						cout << i << " " << j << " neighbours " << i << " " << j - 1 << "\n";
					}
					if (getGrid(i - 1, j)->type == 0) // left
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i - 1, j));
						cout << i << " " << j << " neighbours " << i - 1 << " " << j << "\n";
					}
					if (getGrid(i, j - 1)->type == 0) //up
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i, j - 1));
						cout << i << " " << j << " neighbours " << i << " " << j + 1 << "\n";
					}
				}
				if ( i == gridSq-1 && j > 0 && j <gridSq-1) // wall down
				{
					if (getGrid(i, j+1)->type == 0) //right
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i + 1, j));
						cout << i << " " << j << " neighbours " << i << " " << j+1 << "\n";
					}
					if (getGrid(i, j-1)->type == 0) // left
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i, j-1));
						cout << i << " " << j << " neighbours " << i << " " << j-1 << "\n";
					}
					if (getGrid(i-1,j)->type == 0) //up
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i-1,j));
						cout << i << " " << j << " neighbours " << i-1 << " " << j << "\n";
					}
				}
			}

			else  //2 neighbours max
			{
				if(i==0 && j==0) //top left corner
				{
					if (getGrid(i + 1, j)->type == 0) //right
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i + 1, j));
						cout << i << " " << j << " neighbours " << i + 1 << " " << j << "\n";
					}

					if (getGrid(i, j+1)->type == 0) //down
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i, j+1));
						cout << i << " " << j << " neighbours " << i << " " << j+1 << "\n";
					}
				}
				if (i == 19 && j == gridSq - 1) //top right corner
				{
					if (getGrid(i-1, j)->type == 0)
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i - 1, j));
						cout << i << " " << j << " neighbours " << i-1 << " " << j << "\n"; //left
					}
					if (getGrid(i, j+1)->type == 0)
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i, j+1));
						cout << i << " " << j << " neighbours " << i << " " << j + 1 << "\n"; //down
					}
					
				}
				if (i == gridSq -1 && j == 0) //bottom left corner
				{
					if (getGrid(i, j-1)->type == 0) //up
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i, j-1));
						cout << i << " " << j << " neighbours " << i << " " << j-1 << "\n";
					}

					if (getGrid(i, j + 1)->type == 0) //right
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i, j + 1));
						cout << i << " " << j << " neighbours " << i << " " << j + 1 << "\n";
					}
				}
				if (i == gridSq -1 && j == gridSq - 1) //bottom right corner
				{
					if (getGrid(i, j-1)->type == 0) //up
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i, j-1));
						cout << i << " " << j << " neighbours " << i << " " << j - 1 << "\n"; 
					}
					if (getGrid(i-1,j )->type == 0) //left
					{
						getGrid(i, j)->addConnectedNodes(getGrid(i-1,j));
						cout << i << " " << j << " neighbours " << i-1 << " " << j << "\n"; //down
					}

				}
			}
		}
	}
}
void MapNavigation::convertPositionToGridSpace(GameObject* a, int& row, int& col)
{
	
	int posX = int(a->Physics()->GetPosition().x);
	int posZ = int(a->Physics()->GetPosition().z);

	row = (posX + (WORLD_SIZE / 2)) * gridSq / WORLD_SIZE;
	col = (posZ + (WORLD_SIZE / 2)) * gridSq / WORLD_SIZE;
}

nclgl::Maths::Vector3 MapNavigation::convertGridToVectorSpace(int a, int b)
{

	float posX =  WORLD_SIZE/gridSq * a - (WORLD_SIZE /2);
	float posZ = (WORLD_SIZE / gridSq) * b - (WORLD_SIZE /2);

	return Vector3(posX, 1.5, posZ);
	
}

void MapNavigation::generateAStar()
{
	int startX, startZ;
	convertPositionToGridSpace(GameLogic::Instance()->getAIPlayer(0), startX, startZ); //need to find start location
	getGrid(startX, startZ)->movementCost = 0;
	getGrid(startX, startZ)->estimateCost = 0;
	MapNode* goal = new MapNode();
	goal = getGrid(goal->getObjectiveX(),goal->getObjectiveZ());
	int cost = 10000;
	addToOpenList(getGrid(startX, startZ));										// add start to the openlist
	Vector3 goalPos = convertGridToVectorSpace(goal->getObjectiveX(),goal->getObjectiveZ());

	MapNode* bestConnectedNode = openList.front()->connectedNodes.front();	//best node is the best neighbour of the parent
	MapNode* parent = bestConnectedNode;									//to start we need the parent to be best node
	
	while (!openList.empty())
	{
		
		if (parent == goal)													//we have the goal break and generate path
		{
			generatePath();
		}
		else
				parent = bestConnectedNode;
				for (std::list<MapNode*>::iterator it = parent->connectedNodes.begin(); it != parent->connectedNodes.end(); ) // check connected nodes
				{
					addToOpenList(*it);
						if ((*it)->CalcCostForMapNode() < cost)				// if it's cheaper
						{
							cost = (*it)->CalcCostForMapNode();				//cost is now that node
							bestConnectedNode = (*it);
							bestConnectedNode->setParent(*it);
						}
						else ++it;
						if ((bestConnectedNode) == openList.front())
						{
							continue;
						}
						else addToOpenList(bestConnectedNode);
				}

					
					addToClosedList(bestConnectedNode); //add it to the closedlist
					openList.remove(openList.front());
					openList.unique();
	}	
}

void MapNavigation::generatePath()
{
	for (std::list<MapNode*>::iterator it = closedList.begin(); it != closedList.end();it++)
	{
		nclgl::Maths::Vector3 pos = convertGridToVectorSpace((*it)->posX,(*it)->posZ);
		 
		addToWayPoints(pos);
	}
}

nclgl::Maths::Vector3 seek(nclgl::Maths::Vector3 goal)
{
	nclgl::Maths::Vector3 DesVelo = ((goal - GameLogic::Instance()->getAIPlayer(0)->Physics()->GetPosition()).Normalise());

	return(DesVelo)* 4;
}
void MapNavigation::usePath()
{
	std::vector<nclgl::Maths::Vector3> a = wayPoints;
	if (index == 0)
	{
		index = (index + 1) % wayPoints.size();
	}
	nclgl::Maths::Vector3 goal = getWayPoint(index);
	nclgl::Maths::Vector3 AIBallPos = GameLogic::Instance()->getAIPlayer(0)->Physics()->GetPosition();

		float distanceToGoal = (AIBallPos - goal).Length();
		if (distanceToGoal < 4)
		{
			if (index != wayPoints.size())
			{
				std::cout << "Changing to Node" << index << "\n";
				index++;
			}
			
		}
	
		GameLogic::Instance()->getAIPlayer(0)->Physics()->SetForce(seek(goal));
}



int MapNode::CalcCostForMapNode()
{
	int estX = abs(this->posX - getObjectiveX());
	int estZ = abs(this->posZ- getObjectiveZ());
	int totalCost = this->movementCost + this->estimateCost + estX + estZ;
	return totalCost;
}

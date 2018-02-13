#pragma once
#include "State.h"
#include "BallAI.h"

GameObject* BallAI::AIBall;

class RoamingState : public State
{
	public:
	static State * GetInstance()
	{
		static RoamingState theInstance;
		return &theInstance;
	}
public:
	float maxVel = 20.0f;
	virtual void enter(GameObject* owner) 
	{
		std::ifstream myReadFile;
		myReadFile.open("../pos.txt");
		string line;

		float xQ;
		float yQ;
		float zQ;
		Vector3 pos;
		int nodes = 0;
		if (myReadFile.is_open())
		{
			while (!myReadFile.eof())
			{
				string vector3;
				getline(myReadFile, line);
				remove(line.begin(), line.end(), ' '); //remove any spaces
				getline(myReadFile, vector3, '(');
				string x;
				getline(myReadFile, x, ',');
				string y;
				getline(myReadFile, y, ',');
				string z;
				getline(myReadFile, z, ')');

				if (myReadFile.eof())
					break;

				std::string::size_type sz;

				xQ = std::stof(x, &sz);
				yQ = std::stof(y, &sz);
				zQ = std::stof(z, &sz);



				pos = (Vector3(xQ, yQ, zQ));
				setNodes(pos);
				++nodes;
				cout << "Node " << nodes << " added \n";
			}
			myReadFile.close();
		}
	};
	virtual void update(GameObject* owner)
	{
		owner->Physics()->SetForce(followPath(owner));

	}
	std::vector<Vector3> getNodes() { return Nodes; }
	int CurrentNode = 0;

	Vector3 seek(GameObject* owner, Vector3 TargetPos)
	{
		Vector3 DesVelo = ((TargetPos - owner->Physics()->GetPosition()).Normalise() * maxVel);

		return(DesVelo - owner->Physics()->GetLinearVelocity());
	}

	Vector3 followPath(GameObject* owner)
	{
		//following a path created by a collection of nodes. You can make your own path by pressing 1 when playing.
		//This will print a vector3 to screen and add it to a txt file. This file is read before run time
		//Give it a shot and add as many nodes as you would like
		std::vector<Vector3> nodesList = getNodes();
		goal = nodesList[CurrentNode];

		Vector3 ballPos = owner->Physics()->GetPosition();
		// Switch to length square
		float distanceToGoal = (ballPos - goal).Length();

		if (distanceToGoal <= 10)
		{
			cout << "Changing to Node" << CurrentNode << "\n";
			Vector3 Node = goal;
			CurrentNode++;
			nodesList.erase(nodesList.begin()); //delete the first node
			setNodes(Node); //adds it back to the list
		}
		return seek(owner,goal);


	}
	virtual void exit(GameObject* owner) {};
	
private:
	Vector3 goal;
	void setNodes(Vector3 Node) { Nodes.push_back(Node); }
	std::vector<Vector3> Nodes;
};


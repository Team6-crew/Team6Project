//#pragma once
//#include <string>
//#include <nclgl\Vector3.h>
//#include <nclgl\Vector4.h>
//
//#include <vector>
//
//enum Type {
//	GEOMETRY_CUBOID,
//	GEOMETRY_SPHERE,
//	GEOMETRY_RAMP,
//};
//
//class LevelWriter
//{
//	struct MapObject
//	{
//		std::string name;
//		Type type;
//		nclgl::Maths::Vector3 position;
//		nclgl::Maths::Vector3 scale;
//		float inverseMass;		
//		nclgl::Maths::Vector4  colour;
//		nclgl::Maths::Vector3 rotAxis;
//		float rotationDegrees;
//	};
//
//public:
//	LevelWriter();
//	~LevelWriter();
//
//	void WriteLevel(const std::string& filename, Scene* scene);
//
//protected:
//	void WriteMapObject();
//	bool Write(const std::string& filename);
//
//	std::vector<MapObject> mapObjects;
//
//protected:
//	Type StringToType(const std::string& line);
//};
//

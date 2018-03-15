#pragma once
#include "GameObject.h"
#include "Spring_Constraint.h"
#include "CommonUtils.h"
#include <nclgl\Matrix3.h>
#include <nclgl\Graphics\Renderer\RenderNodeFactory.h>
#include <nclgl\Graphics\Renderer\OpenGL\OGLMesh.h>

class Softbody
{
public:
	Softbody(const std::string& name,
		nclgl::Maths::Vector3& pos,
		float rdius,
		bool phys_enbld = false,
		float invrse_m = 0.0f,			//requires physics_enabled = true
		bool clldble = true,				//requires physics_enabled = true
		bool drgble = true,
		nclgl::Maths::Vector4& color = nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f))
	{
		position = pos;
		radius = rdius;
		physics_enabled = phys_enbld;
		inverse_mass = invrse_m;
		collidable = clldble;
		dragable = drgble;
		colour = color;
	};

	~Softbody() {
		
	}
	;

	void buildsoftbody() {
		int k = 0;
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 20; ++j) {
				if (j != 0 && j != 10) {
					GameObject* sphere = CommonUtils::BuildSoftSphereObject(
						"",
						nclgl::Maths::Vector3(position.x + radius * sin(j * 18 * PI / 180) * cos(i * PI / 180 * 18), position.y + radius * sin(j * PI / 180 * 18) * sin(i * PI / 180 * 18) + 1, position.z + radius * cos(j * PI / 180 * 18)),
						0.05,
						physics_enabled,
						inverse_mass,
						collidable,
						dragable,
						colour);
					sphere->Physics()->SetElasticity(0.5f);
					sphere->Physics()->SetFriction(0.05f);
					sphere->Physics()->SetInverseInertia(nclgl::Maths::Matrix3(100, 0, 0, 100, 0, 0, 100, 0, 0));
					++k;
					softball[k] = sphere;
				}
			}
		}

		GameObject* endsphere1 = CommonUtils::BuildSoftSphereObject(
			"",
			nclgl::Maths::Vector3(position.x + radius - 1, position.y + radius, position.z - radius * 1),
			0.05,
			physics_enabled,
			inverse_mass,
			collidable,
			dragable,
			colour);
		endsphere1->Physics()->SetElasticity(0.5f);
		endsphere1->Physics()->SetFriction(0.05f);
		endsphere1->Physics()->SetInverseInertia(nclgl::Maths::Matrix3(100, 0, 0, 100, 0, 0, 100, 0, 0));
		softball[181] = endsphere1;

		GameObject* endsphere2 = CommonUtils::BuildSoftSphereObject(
			"",
			nclgl::Maths::Vector3(position.x + radius - 1, position.y + radius, position.z + radius * 1),
			0.05,
			physics_enabled,
			inverse_mass,
			collidable,
			dragable,
			colour);
		endsphere2->Physics()->SetElasticity(0.5f);
		endsphere2->Physics()->SetFriction(0.05f);
		endsphere2->Physics()->SetInverseInertia(nclgl::Maths::Matrix3(100, 0, 0, 100, 0, 0, 100, 0, 0));
		softball[0] = endsphere2;


		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 8; ++j) {
				c1[(i * 20) + j] = new Spring_Constraint(
					softball[1 + j + (i * 18)]->Physics(),					//Physics Object A
					softball[1 + j + (i * 18) + 1]->Physics(),					//Physics Object B
					softball[1 + j + (i * 18)]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
					softball[1 + j + (i * 18) + 1]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre 
				PhysicsEngine::Instance()->AddConstraint(c1[(i * 20) + j]);

				c1[(i * 20) + j + 80] = new Spring_Constraint(
					softball[10 + j + (i * 18)]->Physics(),					//Physics Object A
					softball[10 + j + (i * 18) + 1]->Physics(),					//Physics Object B
					softball[10 + j + (i * 18)]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
					softball[10 + j + (i * 18) + 1]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre 
				PhysicsEngine::Instance()->AddConstraint(c1[(i * 20) + j + 80]);
			}

			c1[160 + i] = new Spring_Constraint(
				softball[0]->Physics(),					//Physics Object A
				softball[1 + (i * 18)]->Physics(),					//Physics Object B
				softball[0]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
				softball[1 + (i * 18)]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre 
			PhysicsEngine::Instance()->AddConstraint(c1[160 + i]);

			c1[170 + i] = new Spring_Constraint(
				softball[0]->Physics(),					//Physics Object A
				softball[18 + (i * 18)]->Physics(),					//Physics Object B
				softball[0]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
				softball[18 + (i * 18)]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre 
			PhysicsEngine::Instance()->AddConstraint(c1[170 + i]);


			c1[180 + i] = new Spring_Constraint(
				softball[9 + (i * 18)]->Physics(),					//Physics Object A
				softball[181]->Physics(),					//Physics Object B
				softball[9 + (i * 18)]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
				softball[181]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre 
			PhysicsEngine::Instance()->AddConstraint(c1[180 + i]);

			c1[190 + i] = new Spring_Constraint(
				softball[10 + (i * 18)]->Physics(),					//Physics Object A
				softball[181]->Physics(),					//Physics Object B
				softball[10 + (i * 18)]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
				softball[181]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre 
			PhysicsEngine::Instance()->AddConstraint(c1[190 + i]);
		}
		//for (int i = 0; i < 10; ++i) {
		//	c1[160 + i] = new Spring_Constraint(
		//		softball[0]->Physics(),					//Physics Object A
		//		softball[1 + (i * 18)]->Physics(),					//Physics Object B
		//		softball[0]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
		//		softball[1 + (i * 18)]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre 
		//	PhysicsEngine::Instance()->AddConstraint(c1[160 + i]);

		//	c1[170 + i] = new Spring_Constraint(
		//		softball[0]->Physics(),					//Physics Object A
		//		softball[18 + (i * 18)]->Physics(),					//Physics Object B
		//		softball[0]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
		//		softball[18 + (i * 18)]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre 
		//	PhysicsEngine::Instance()->AddConstraint(c1[170 + i]);
		//}


		//for (int i = 0; i < 10; ++i) {
		//	c1[180 + i] = new Spring_Constraint(
		//		softball[9 + (i * 18)]->Physics(),					//Physics Object A
		//		softball[181]->Physics(),					//Physics Object B
		//		softball[9 + (i * 18)]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
		//		softball[181]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre 
		//	PhysicsEngine::Instance()->AddConstraint(c1[180 + i]);

		//	c1[190 + i] = new Spring_Constraint(
		//		softball[10 + (i * 18)]->Physics(),					//Physics Object A
		//		softball[181]->Physics(),					//Physics Object B
		//		softball[10 + (i * 18)]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
		//		softball[181]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre 
		//	PhysicsEngine::Instance()->AddConstraint(c1[190 + i]);
		//}
		//Test
		k = 0;
		for (int i = 1; i <= 18; ++i) {
			for (int j = 0; j < 9; ++j) {
				c2[k] = new Spring_Constraint(
					softball[i + (j * 18)]->Physics(),					//Physics Object A
					softball[i + ((j + 1) * 18)]->Physics(),					//Physics Object B
					softball[i + (j * 18)]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
					softball[i + ((j + 1) * 18)]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre 
				PhysicsEngine::Instance()->AddConstraint(c2[k]);
				++k;
			}

			c2[k] = new Spring_Constraint(
				softball[i]->Physics(),					//Physics Object A
				softball[181 - i]->Physics(),					//Physics Object B
				softball[i]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
				softball[181 - i]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre 
			PhysicsEngine::Instance()->AddConstraint(c2[k]);
			++k;
		}


		//for (int i = 1; i <= 18; ++i) {
		//	c2[k] = new Spring_Constraint(
		//		softball[i]->Physics(),					//Physics Object A
		//		softball[181 - i]->Physics(),					//Physics Object B
		//		softball[i]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
		//		softball[181 - i]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre 
		//	PhysicsEngine::Instance()->AddConstraint(c2[k]);
		//	++k;
		//}

		k = 0;

		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 9; ++j) {
				c3[k] = new Spring_Constraint(
					softball[j + (i * 18) + 1]->Physics(),					//Physics Object A
					softball[j + (i * 18) + 10]->Physics(),					//Physics Object B
					softball[j + (i * 18) + 1]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
					softball[j + (i * 18) + 10]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre 
				PhysicsEngine::Instance()->AddConstraint(c3[k]);
				++k;
			}
		}

		c3[k] = new Spring_Constraint(
			softball[0]->Physics(),					//Physics Object A
			softball[181]->Physics(),					//Physics Object B
			softball[0]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
			softball[181]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre 
		PhysicsEngine::Instance()->AddConstraint(c3[k]);

		//rendering mesh


	}

	void RenderSoftbody() {
		rnode = RenderNodeFactory::Instance()->MakeRenderNode();

		AddTexture();
		dummy = RenderNodeFactory::Instance()->MakeRenderNode(m, colour);
		dummy->SetTransform(nclgl::Maths::Matrix4::Scale(nclgl::Maths::Vector3(radius, radius, radius)));
		rnode->AddChild(dummy);

		rnode->SetTransform(nclgl::Maths::Matrix4::Translation(position));
		rnode->SetBoundingRadius(radius);

		//if (softball[0]->GetScene() && softball[0]->Render()) GraphicsPipeline::Instance()->RemoveRenderNode(rnode);

		GraphicsPipeline::Instance()->AddRenderNode(rnode);
	};

	void RemoveRender() {
		GraphicsPipeline::Instance()->RemoveRenderNode(rnode);
		delete rnode;
		delete m;
	};

	float GetRadius() { return radius; };


	GameObject* softball[182];

private:
	nclgl::Maths::Vector3 position;
	float radius;
	bool physics_enabled;
	float inverse_mass;
	bool collidable;
	bool dragable;
	nclgl::Maths::Vector4 colour;
	OGLMesh* m;
	RenderNodeBase* rnode;
	RenderNodeBase* dummy;
	Spring_Constraint* c1[210];			//"circles" around centre
	Spring_Constraint* c2[210];			//"rings" along sphere
	Spring_Constraint* c3[210];			// Joining opposite points

	void AddTexture() {
		m = new OGLMesh();
		m->numVertices = 774;
		m->type = GL_TRIANGLE_STRIP;

		m->vertices = new nclgl::Maths::Vector3[m->numVertices];
		//m->textureCoords = new nclgl::Maths::Vector2[m->numVertices];
		m->colours = new nclgl::Maths::Vector4[m->numVertices];
		//m->normals			= new nclgl::Maths::Vector3[m->numVertices];
		//m->tangents			= new nclgl::Maths::Vector3[m->numVertices];



		for (int i = 0; i < 9; ++i) {
			m->vertices[3 * i] = softball[0]->Physics()->GetPosition();
			m->vertices[3 * i + 1] = softball[i * 18 + 1]->Physics()->GetPosition();
			m->vertices[3 * i + 2] = softball[(i + 1) * 18 + 1]->Physics()->GetPosition();

			m->vertices[3 * i + 30] = softball[0]->Physics()->GetPosition();
			m->vertices[3 * i + 31] = softball[i * 18 + 18]->Physics()->GetPosition();
			m->vertices[3 * i + 32] = softball[(i + 1) * 18 + 18]->Physics()->GetPosition();

			m->vertices[716 + 3 * i] = softball[181]->Physics()->GetPosition();
			m->vertices[716 + 3 * i + 1] = softball[i * 18 + 9]->Physics()->GetPosition();
			m->vertices[716 + 3 * i + 2] = softball[(i + 1) * 18 + 9]->Physics()->GetPosition();

			m->vertices[3 * i + 746] = softball[181]->Physics()->GetPosition();
			m->vertices[3 * i + 747] = softball[i * 18 + 10]->Physics()->GetPosition();
			m->vertices[3 * i + 748] = softball[(i + 1) * 18 + 10]->Physics()->GetPosition();
		}

		m->vertices[27] = softball[0]->Physics()->GetPosition();
		m->vertices[28] = softball[163]->Physics()->GetPosition();
		m->vertices[29] = softball[18]->Physics()->GetPosition();

		/*for (int i = 0; i < 9; ++i) {
			m->vertices[3 * i + 30] = softball[0]->Physics()->GetPosition();
			m->vertices[3 * i + 31] = softball[i * 18 + 18]->Physics()->GetPosition();
			m->vertices[3 * i + 32] = softball[(i + 1) * 18 + 18]->Physics()->GetPosition();
		}*/

		m->vertices[57] = softball[0]->Physics()->GetPosition();
		m->vertices[58] = softball[1]->Physics()->GetPosition();
		m->vertices[59] = softball[180]->Physics()->GetPosition();

		//Finished "end" of sphere





		int k;
		int l;
		for (int i = 1; i < 9; ++i) {
			k = 1;
			l = 0;
			m->vertices[60 + 82 * (i - 1)] = softball[i + 1]->Physics()->GetPosition();
			m->vertices[62 + 82 * (i - 1)] = softball[i]->Physics()->GetPosition();
			for (int j = 0; j < 9; ++j) {
				m->vertices[61 + (4 * j) + 82 * (i - 1)] = softball[i + 1 + k * 18]->Physics()->GetPosition();
				m->vertices[61 + (4 * j) + 2 + 82 * (i - 1)] = softball[i + k * 18]->Physics()->GetPosition();
				m->vertices[61 + (4 * j) + 3 + 82 * (i - 1)] = m->vertices[61 + (4 * j) + 82 * (i - 1)];
				m->vertices[61 + (4 * j) + 5 + 82 * (i - 1)] = m->vertices[61 + (4 * j) + 2 + 82 * (i - 1)];
				++k;

				m->vertices[100 + (4 * j) + 82 * (i - 1)] = softball[18 - i + l * 18]->Physics()->GetPosition();
				m->vertices[100 + (4 * j) + 2 + 82 * (i - 1)] = softball[18 - i + 1 + l * 18]->Physics()->GetPosition();
				m->vertices[100 + (4 * j) + 3 + 82 * (i - 1)] = m->vertices[100 + (4 * j) + 82 * (i - 1)];
				m->vertices[100 + (4 * j) + 5 + 82 * (i - 1)] = m->vertices[100 + (4 * j) + 2 + 82 * (i - 1)];

				++l;
			}
			m->vertices[97 + 82 * (i - 1)] = softball[18 - i]->Physics()->GetPosition();
			m->vertices[99 + 82 * (i - 1)] = softball[19 - i]->Physics()->GetPosition();
			m->vertices[101 + 82 * (i - 1)] = softball[36 - i]->Physics()->GetPosition();

			/*for (int j = 0; j < 9; ++j) {
				m->vertices[100 + (4 * j) + 82 * (i - 1)] = softball[18 - i + l * 18]->Physics()->GetPosition();
				m->vertices[100 + (4 * j) + 2 + 82 * (i - 1)] = softball[18 - i + 1 + l * 18]->Physics()->GetPosition();
				m->vertices[100 + (4 * j) + 3 + 82 * (i - 1)] = m->vertices[100 + (4 * j) + 82 * (i - 1)];
				m->vertices[100 + (4 * j) + 5 + 82 * (i - 1)] = m->vertices[100 + (4 * j) + 2 + 82 * (i - 1)];

				++l;
			}*/
			m->vertices[136 + 82 * (i - 1)] = softball[180 - i]->Physics()->GetPosition();
			m->vertices[138 + 82 * (i - 1)] = softball[180 - i + 1]->Physics()->GetPosition();
			m->vertices[139 + 82 * (i - 1)] = softball[180 - i]->Physics()->GetPosition();
			m->vertices[140 + 82 * (i - 1)] = softball[i + 1]->Physics()->GetPosition();
			m->vertices[141 + 82 * (i - 1)] = softball[180 - i]->Physics()->GetPosition();
		}


		//Finish main "body" of sphere and now creating mesh for other "end"

		m->vertices[714] = softball[9]->Physics()->GetPosition();
		m->vertices[715] = softball[172]->Physics()->GetPosition();


		/*for (int j = 0; j < 9; ++j) {
			m->vertices[716 + 3 * j] = softball[181]->Physics()->GetPosition();
			m->vertices[716 + 3 * j + 1] = softball[j * 18 + 9]->Physics()->GetPosition();
			m->vertices[716 + 3 * j + 2] = softball[(j + 1) * 18 + 9]->Physics()->GetPosition();

			m->vertices[3 * j + 746] = softball[181]->Physics()->GetPosition();
			m->vertices[3 * j + 747] = softball[j * 18 + 10]->Physics()->GetPosition();
			m->vertices[3 * j + 748] = softball[(j + 1) * 18 + 10]->Physics()->GetPosition();
		}*/

		m->vertices[743] = softball[181]->Physics()->GetPosition();
		m->vertices[744] = softball[171]->Physics()->GetPosition();
		m->vertices[745] = softball[10]->Physics()->GetPosition();

		/*for (int j = 0; j < 9; ++j) {
			m->vertices[3 * j + 746] = softball[181]->Physics()->GetPosition();
			m->vertices[3 * j + 747] = softball[j * 18 + 10]->Physics()->GetPosition();
			m->vertices[3 * j + 748] = softball[(j + 1) * 18 + 10]->Physics()->GetPosition();
		}*/
		m->vertices[773] = softball[181]->Physics()->GetPosition();


		for (int i = 0; i < m->numVertices; ++i) {
			m->vertices[i] = m->vertices[i] - position;
			m->colours[i] = colour;
		}

		/*nclgl::Maths::Matrix4 pushPos = nclgl::Maths::Matrix4::Translation(Vector3(0.5, 0.5, 0));
		nclgl::Maths::Matrix4 popPos = nclgl::Maths::Matrix4::Translation(Vector3(-0.5, -0.5, 0));
		nclgl::Maths::Matrix4 rotation = nclgl::Maths::Matrix4::Rotation(radius, Vector3(0, 0, 1));
		nclgl::Maths::Matrix4 concat = pushPos * rotation * popPos;

		for (int i = 0; i < m->numVertices; ++i) {
		nclgl::Maths::Vector4 rotvec = concat * nclgl::Maths::Vector4(m->textureCoords[i].x, m->textureCoords[i].y, 0, 1);
		m->textureCoords[i].x = rotvec.x;
		m->textureCoords[i].y = rotvec.y;
		}*/

		m->GenerateNormals();
		m->GenerateTangents();
		m->BufferData();

	};
};


#include "Model.h"
#include "Physics.h"

using namespace GAME;
using namespace MATH;
float dummyAngle = 0.0;

Vec3 GAME::Model::getCenterOfUsefulVerData()
{
	Vec3 holder = Vec3(0.0, 0.0, 0.0);
	for (int i = 0; i > usefulVertTotal; i++)
	{
		holder.x = holder.x + usefullVertData.at(i).x;
		holder.y = holder.y + usefullVertData.at(i).y;
		holder.z = holder.z + usefullVertData.at(i).z;
	}
	holder = holder / 4;
	return holder;
}

float GAME::Model::getLowDotProductUsefulVertData()
{
	int ticks = 0;
	Vec3 holder0 = Vec3(100.0, 100.0, 100.0);
	Vec3 holder1 = Vec3(100.0, 100.0, 100.0);
	Vec3 varAminusB = Vec3(0.0,0.0,0.0);
	float dotProd = 0.0;
	float lowestDot = VMath::dot(holder0,holder1);

	for (int yCount = 1; yCount != 7; yCount++)
	{
		ticks++;
		if (yCount <= 3) {
			holder0.x = usefullVertData.at(0).x;
			holder0.y = usefullVertData.at(0).y;
			holder0.z = usefullVertData.at(0).z;
			holder1.x = usefullVertData.at(yCount).x;
			holder1.y = usefullVertData.at(yCount).y;
			holder1.z = usefullVertData.at(yCount).z;
			//std::cout << "xCount = " << 0 << " yCount = " << yCount << std::endl;
		}
		else if (yCount > 3 && yCount < 6) {
			holder0.x = usefullVertData.at(1).x;
			holder0.y = usefullVertData.at(1).y;
			holder0.z = usefullVertData.at(1).z;
			holder1.x = usefullVertData.at(yCount -2).x;
			holder1.y = usefullVertData.at(yCount -2).y;
			holder1.z = usefullVertData.at(yCount -2).z;
			//std::cout << "xCount = " << 1 << " yCount = " << yCount - 2 << std::endl;
		}
		else {
			holder0.x = usefullVertData.at(2).x;
			holder0.y = usefullVertData.at(2).y;
			holder0.z = usefullVertData.at(2).z;
			holder1.x = usefullVertData.at(3).x;
			holder1.y = usefullVertData.at(3).y;
			holder1.z = usefullVertData.at(3).z;
			//std::cout << "xCount = " << 2 << " yCount = " << 3 << std::endl;
		}

		dotProd = VMath::dot(holder0, holder1);

		if (dotProd < lowestDot)
		{
			lowestDot = dotProd;
			varAminusB = holder0 - holder1;
		}
	}
	
	this->simplexDeterminingPointLowDot = varAminusB;
	return lowestDot;
}

	float GAME::Model::getBigDotProductUsefulVertData()
{
		int ticks = 0;
		Vec3 holder0 = Vec3(0, 0, 0);
		Vec3 holder1 = Vec3(0, 0, 0);
		Vec3 varAminusB = Vec3(0, 0, 0);
		float dotProd = 0.0;
		float highestDot = VMath::dot(holder0, holder1);

		for (int yCount = 1; yCount != 7; yCount++)
		{
			ticks++;
			if (yCount <= 3) {
				holder0.x = usefullVertData.at(0).x;
				holder0.y = usefullVertData.at(0).y;
				holder0.z = usefullVertData.at(0).z;
				holder1.x = usefullVertData.at(yCount).x;
				holder1.y = usefullVertData.at(yCount).y;
				holder1.z = usefullVertData.at(yCount).z;
				//std::cout << "xCount = " << 0 << " yCount = " << yCount << std::endl;
			}
			else if (yCount > 3 && yCount < 6) {
				holder0.x = usefullVertData.at(1).x;
				holder0.y = usefullVertData.at(1).y;
				holder0.z = usefullVertData.at(1).z;
				holder1.x = usefullVertData.at(yCount - 2).x;
				holder1.y = usefullVertData.at(yCount - 2).y;
				holder1.z = usefullVertData.at(yCount - 2).z;
				//std::cout << "xCount = " << 1 << " yCount = " << yCount - 2 << std::endl;
			}
			else {
				holder0.x = usefullVertData.at(2).x;
				holder0.y = usefullVertData.at(2).y;
				holder0.z = usefullVertData.at(2).z;
				holder1.x = usefullVertData.at(3).x;
				holder1.y = usefullVertData.at(3).y;
				holder1.z = usefullVertData.at(3).z;
				//std::cout << "xCount = " << 2 << " yCount = " << 3 << std::endl;
			}

			dotProd = VMath::dot(holder0, holder1);

			if (dotProd > highestDot)
			{
				highestDot = dotProd;
				varAminusB = holder0 - holder1;
			}
		}

		this->simplexDeterminingPointHighDot = varAminusB;
		return highestDot;
}


Model::Model(const std::vector<Mesh*> meshes_, const Shader*  shader_): meshes(meshes_), shader(shader_) {
	useGravity = true;
	usefulVertTotal = 4;
	usefullVertData = std::vector<Vec3>(usefulVertTotal);
}

bool Model::OnCreate() {

	return true;
}

void Model::OnDestroy() {}


//void Model::Render() const {
//	// Multiply by rotation matrix
//	//Matrix4 modelMatrix = MMath::translate(pos) * MMath::rotate(dummyAngle, Vec3(0.0, 1.0, 0.0));
//	Quaternion tempOrientation = orientation;
//	Matrix4 rotationMatrix = tempOrientation.toMatrix();
//	Matrix4 modelMatrix = MMath::translate(pos) * rotationMatrix;
//	dummyAngle++;
//	GLint modelMatrixID = glGetUniformLocation(shader->getProgram(), "modelMatrix");
//	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, modelMatrix);
//	for (Mesh* mesh : meshes) {
//		mesh->Render();
//	}
//}

void Model::Render() /* const */  {

	//attempting to only get useful verts
	Quaternion dummy(orientation);
	Matrix4 rotationMatrix = dummy.toMatrix();
	Matrix4 modelMatrix = MMath::translate(pos) * rotationMatrix;
	int vertCount = meshes.at(0)->vertices.size();
	int ycount = 0; //this is the count to know where to insert data into a vector
	for (int x = 0; x <= vertCount - 1; x++)
	{	
		if (x == 0 || x == 1 || x == 2 || x == 4)
		{
			float tX = meshes.at(0)->vertices.at(x).x;
			float tY = meshes.at(0)->vertices.at(x).y;
			float tZ = meshes.at(0)->vertices.at(x).z;

			Vec4 vertexPosObjectSpace4(tX, tY, tZ, 1);
			Vec4 vertexPosWorldSpace4 = modelMatrix * vertexPosObjectSpace4;
			Vec3 vData = (0,0,0);
			vData.x = vertexPosWorldSpace4.x;
			vData.y = vertexPosWorldSpace4.y;
			vData.z = vertexPosWorldSpace4.z;
			addVertData(vData, ycount);
			//vertexPosWorldSpace4.print();
			//std::vector<Vec3> test = usefullVertData;
			ycount++;
		}	
	}
	ycount = 0;
	GLint modelMatrixID = glGetUniformLocation(shader->getProgram(), "modelMatrix");
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, modelMatrix);
	for (Mesh* mesh : meshes) {
		mesh->Render();
	}
}

void Model::Update(const float deltaTime) {
	Physics::SimpleNewtonMotion( *this, deltaTime);
	boundingSphere = pos;
	//lowestUsefulDotProduct = getLowDotProductUsefulVertData();
	//biggestUsefulDotProduct = getBigDotProductUsefulVertData();
}

Model::~Model() {}

Vec3 GAME::PhysicsModel::MaxPointAlongDirection(Vec3 direction)
{
	float max = -INFINITY;
	int index = 0;
	for (int i = 0; i < usefullVertData.size(); i++) {
		float dot = MATH::VMath::dot(usefullVertData.at(i), direction);
		if (dot > max) {
			max = dot;
			index = i;
		}
	}
	return usefullVertData.at(index);
}

#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include "MMath.h"
#include "Mesh.h"
#include "Shader.h"
#include "Quaternion.h"
#include "VMath.h"

namespace GAME {
using namespace MATH;

class PhysicsModel {
	friend class Physics;		/// This a test of the idea I had about using
protected:						/// a static Physics class that is a friend 
	Vec3 pos;					/// of just the data in the PhysicsModel class
	Vec3 vel;
	Vec3 accel;
	Quaternion orientation; // For Kevin (in Unity), this is Transform.Rotation
	Vec3 angularVel; // Adding a Vector to hold angular velocity
	float mass;
	bool useGravity;
	Sphere boundingSphere;
	int usefulVertTotal;
	
public:
	Vec3 simplexDeterminingPointLowDot;
	Vec3 simplexDeterminingPointHighDot;
	float lowestUsefulDotProduct;
	float biggestUsefulDotProduct;
	std::vector<Vec3> usefullVertData;
	Vec3 MaxPointAlongDirection(Vec3 direction);
};


class Model: public PhysicsModel {
protected:
	Matrix4 modelMatrix;
	const std::vector<Mesh*> meshes;
	const Shader*  shader;
public:
	Vec3 posRef;
	Vec3 getCenterOfUsefulVerData();
	float getLowDotProductUsefulVertData();
	float getBigDotProductUsefulVertData();
	
	inline void setPos(const Vec3& pos_){ pos = pos_; }
	inline void setVel(const Vec3& vel_){ vel = vel_; }
	inline void setOrientation(const Quaternion& orientation_) { orientation = orientation_; }
	inline void setAngularVel(const Vec3& angularVel_) { angularVel = angularVel_; }
	inline void setAccel(const Vec3& accel_) { accel = accel_; }
	inline void setMass(const float& mass_) { mass = mass_; }
	inline void setGrav(const bool& grav_) { useGravity = grav_; }
	inline Vec3 getPosition() { return pos; }
	inline Vec3 getVelocity() { return vel; }
	inline const std::vector<Mesh*> getMeshes() { return meshes; }
	inline Matrix4 getMatrix() { return modelMatrix; }
	inline Quaternion getOrientation() { return orientation; }
	inline void addVertData(Vec3 vertData_, int vertexCount) { 
		usefullVertData.at(vertexCount).x = vertData_.x;
		usefullVertData.at(vertexCount).y = vertData_.y;
		usefullVertData.at(vertexCount).z = vertData_.z;
	}
	inline void setBoundingSphere(const float r) { boundingSphere = pos;  boundingSphere.r = r; }
	Model(const Model&) = delete;
	Model(Model&&) = delete;
	Model& operator = (const Model&) = delete;
	Model& operator = (Model&&) = delete;

	Model(const std::vector<Mesh*> meshes_, const Shader* shader_);
	bool OnCreate();
	void OnDestroy();
	//void Render() const;
	void Render();
	void Update(const float deltaTime);
	virtual ~Model();
	
};


} /// end of namespace

#endif
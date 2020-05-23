#ifndef PHYSICS_H
#define PHYSICS_H
#include "Model.h"
#include "VMath.h"
#include "Quaternion.h"
#include <iostream>
using namespace MATH;

namespace GAME {
	
	class Physics {
	public:
		static void SimpleNewtonMotion(PhysicsModel &model, const float deltaTime);
		static void SimpleAttraction(PhysicsModel &model, const float deltaTime);
		static bool PlaneSphereCollision(const Model &model, const Plane &p);
		static void PlaneSphereCollisionResponse(PhysicsModel &model, const Plane &p);
		static bool SphereSphereCollision(const PhysicsModel &model1, const PhysicsModel &model2);
		static void SphereSphereCollisionResponse(PhysicsModel &model1, PhysicsModel &model2);
		static bool GJKattempt(PhysicsModel &model1, PhysicsModel &model2);
		static std::vector<Vec3> getMinkowskiDiff(PhysicsModel &model1, PhysicsModel &model2);
	};
}
#endif
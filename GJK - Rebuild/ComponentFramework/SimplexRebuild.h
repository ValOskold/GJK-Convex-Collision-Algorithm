#pragma once
#include <vector>
#include "MMath.h"
#include <iostream>
#include "Quaternion.h"
#include "Model.h"
#include "Physics.h"
namespace GAME {
	using namespace MATH;

	class SimplexRebuild
	{
		//std::vector<Vec3> iPoints;
		//std::vector<Vec3> jPoints;

		PhysicsModel iPModel;
		PhysicsModel jPModel;
		//When i create this object ill need get a Minkowski starting simplex
		std::vector<Vec3> MinkowskiSimplex;
		//The constructor will take a MinkowskiSimplex
	public:
		
		bool collision;
		SimplexRebuild(PhysicsModel& i, PhysicsModel& j);
		//I need a support Function and i need a DoSimplex function
		//im not sure if the support function should be a void or a vec3
	private:
		Vec3 Support(Vec3 point);
		bool DoSimplex(std::vector<Vec3> points,Vec3 point);
		//For Support and DoSimplex to doAnything i need to begin constructing a tetrohedron so ill need a variable for that
		std::vector<Vec3> finalSimplex;
		Vec3 newSearchDir;
		
	};

}
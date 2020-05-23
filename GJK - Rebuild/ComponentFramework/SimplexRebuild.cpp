#include "SimplexRebuild.h"
using namespace GAME;

GAME::SimplexRebuild::SimplexRebuild(PhysicsModel& i, PhysicsModel& j) {
	//Setup simplex
	MinkowskiSimplex = Physics::getMinkowskiDiff(i,j);

	//Save ij as variables
	//jPoints = j.usefullVertData;
	//iPoints = i.usefullVertData;
	iPModel = i;
	jPModel = j;

	//Any point in A - any point in B should be satasfactory aka the MinkowskiSimplex should be satasfactory to start
	Vec3 S = Support(MinkowskiSimplex.at(0)); //what direction do i give this guy? For now im picking the first value in our MinkowskiSimplex
	finalSimplex.push_back(S); //Now we have a point
	newSearchDir = -S;
	bool loopbool = true;
	//loop now
	while (loopbool)
	{
		finalSimplex.push_back(Support(newSearchDir)); //Now we looked in the opposite direction of a random point in the simplex, so now we have two points aka a line!

		//planetest
		float test = GAME::VMath::dot(finalSimplex.at(finalSimplex.size()-1), newSearchDir);
		//DoSimplex(finalSimplex, newSearchDir);
		if (GAME::VMath::dot(finalSimplex.at(finalSimplex.size()-1), newSearchDir) <= 0) {
			//this object cannot contain the origin there is no collision
			collision = false;
			return;
		}
		else {
			if (DoSimplex(finalSimplex,newSearchDir)) {
				collision = true;
			}
		}
		if (finalSimplex.size() >= 4) {
			return;
		}
	}
	//TODO FIX BUG THAT CAUSES ALREADY SELECTED POINTS TO BE REUSED IN THE FinalSimplex
}

Vec3 GAME::SimplexRebuild::Support(Vec3 searchDirection)
{
	//My first point needs to be the furthest point in some searchDirection, that way we can guarentee its on the edge of the simplex.
	Vec3 p1 = iPModel.MaxPointAlongDirection(searchDirection);
	Vec3 p2 = jPModel.MaxPointAlongDirection(-searchDirection);
	//This should be the same effect as preforming the Minjowski Difference, so this should return some point in our MinkowskiSimplex
	Vec3 p3 = p1 - p2;

	//Before i actually preform any planetest im going to want two points in my tetrohedron
	return p3;
}

bool GAME::SimplexRebuild::DoSimplex(std::vector<Vec3> points, Vec3 point)
{
	Vec3 A = finalSimplex.at(finalSimplex.size() - 1); //vec A is the newest added point
	Vec3 AO = -A;

	if (finalSimplex.size() == 4) {
		//it has to be a tetroherdron
		//so now if our volume contains the origin then there is collision! return true
		//else return false

		//this is easy to do in ones mind, because if you think of faces with fronts and backs if the point is in the center all faces should return the point is on its back
		//if any face had the origin to its front we know it does not contain it.

		//Im going to try to make all the faces and do a test on each of them in a similar way i test anyother dot

		Vec3 B = finalSimplex.at(2);
		Vec3 C = finalSimplex.at(1);
		Vec3 D = finalSimplex.at(0);

		Vec3 AB = B - A;
		Vec3 AC = C - A;
		Vec3 AD = D - A;
		//Vec3 BD = D - B;
		Vec3 CD = D - C;
		Vec3 CB = B - C;
		//the four faces we will test on
		Vec3 ABC = MATH::VMath::cross(AB, AC);
		Vec3 ABD = MATH::VMath::cross(AB, AD);
		Vec3 ACD = MATH::VMath::cross(AC, AD);
		Vec3 CBD = MATH::VMath::cross(CB, CD); //we have already searched this face last itteration it was the triangle case previously, but im not sure how to handle that fact

		//float Tst1 = MATH::VMath::dot(ABC, AO);
		//float Tst2 = MATH::VMath::dot(ABD, AO);
		//float Tst3 = MATH::VMath::dot(ACD, AO);
		//float Tst4 = MATH::VMath::dot(CBD, AO);
		
		float tst1 = MATH::VMath::dot(MATH::VMath::dot(AB, AC), AO);
		float tst2 = MATH::VMath::dot(MATH::VMath::dot(AC, AD), AO);
		float tst3 = MATH::VMath::dot(MATH::VMath::dot(AD, AB),AO);
		float tst4 = MATH::VMath::dot(MATH::VMath::dot(CD, CB), AO);
		if ((tst1 <= 0) && (tst2 <= 0) && (tst3 <= 0) && (tst4 <= 0)) {
			printf("Boop");
			return true;
		}
		

		//if (MATH::VMath::dot(AD, AB) < 0) { //ABC * AO > 0
		//	if (MATH::VMath::dot(AC, AD) < 0) { //ABD * AO > 0
		//		if (MATH::VMath::dot(AB, AC) < 0) { //ACD * AO > 0
		//			//if (MATH::VMath::dot(CBD, AO) < 0) { //CBD * AO > 0
		//				collision = true;
		//			//}
		//		}
		//	}
		//}

		if (MATH::VMath::dot(MATH::VMath::dot(AB,AC),AO) > 0) {//(AB * AC) * AO> 0
			//We are in the face ABC
			if (MATH::VMath::dot(MATH::VMath::dot(AB, ABC), AO) > 0) {//(AB * ABC) * AO> 0
				//printf("1");
				return true;
			}
			if (MATH::VMath::dot(MATH::VMath::dot(ABC, AC), AO) > 0) {//(ABC * AC) * AO> 0
				printf("2");
				return true;
			}
		}
		if (MATH::VMath::dot(MATH::VMath::dot(AC, AD), AO) > 0) {//(AC * AD) * AO> 0
			//We are in the face ACD
			if (MATH::VMath::dot(MATH::VMath::dot(AD, ACD), AO) > 0) {//(AD * ACD) * AO> 0
				//printf("1");
				return true;
			}
			if (MATH::VMath::dot(MATH::VMath::dot(ACD, AC), AO) > 0) {//(ACD * AC) * AO> 0
				printf("2");
				return true;
			}
		}
		if (MATH::VMath::dot(MATH::VMath::dot(AD, AB), AO) > 0) {//(AD * AB) * AO> 0
			//We are in the face of ABD
			if (MATH::VMath::dot(MATH::VMath::dot(AD, ABD), AO) > 0) {//(AB * ABD) * AO> 0
				//printf("1");
				return true;
			}
			if (MATH::VMath::dot(MATH::VMath::dot(ABD, AB), AO) > 0) {//(ABD * AC) * AO> 0
				printf("2");
				return true;
			}
		}
		//WE ARE MISSING ONE MORE CHECK FOR THE LAST FACE
		return false;
	}
	else if (finalSimplex.size() == 3) {
		//it has to be a triangle 
		//ABC is the new structure A is the newist point
		//ABC = AB-> cross AC->
		Vec3 B = finalSimplex.at(finalSimplex.size() - 2);
		Vec3 AB = B - A;
		Vec3 C = finalSimplex.at(0);
		Vec3 AC = C - A;
		Vec3 ABC = MATH::VMath::cross(AB, AC);
		//float testcase = MATH::VMath::dot(ABC, AC);
		//testcase = MATH::VMath::dot(testcase, AO);
		//
		//if (testcase > 0) { //if ABC X AC Dot AO > 0
		//	if (MATH::VMath::dot(AC, AO) > 0) { //if AC * AO > 0
		//		newSearchDir = MATH::VMath::cross(AC, AO);
		//		newSearchDir = MATH::VMath::cross(newSearchDir, AC);
		//		//(AC X AO) X AC
		//	}
		//	else {
		//		if (MATH::VMath::dot(AB, AO) > 0) { //star aka AB * AO > 0
		//			newSearchDir = MATH::VMath::cross(AB, AO);
		//			newSearchDir = MATH::VMath::cross(newSearchDir, AB);
		//			//(AB X AO) X AB
		//		}
		//		else {
		//			newSearchDir = AO;
		//		}//end of star
		//	}// end of else
		//}// end of first if
		//else {
		//	if (MATH::VMath::dot(AB, ABC) > 0) { //AB * ABC > 0
		//		if (MATH::VMath::dot(AB, AO) > 0) { //star aka AB * AO > 0
		//			newSearchDir = MATH::VMath::cross(AB, AO);
		//			newSearchDir = MATH::VMath::cross(newSearchDir, AB);
		//			//(AB X AO) X AB
		//		}
		//		else {
		//			newSearchDir = AO;
		//		}//end of star
		//	}//end of IF
		//	else {
		//		if (MATH::VMath::dot(ABC, AO) > 0) { //ABC * AO > 0
		//			newSearchDir = ABC;
		//		}
		//		else {
		//			newSearchDir = -ABC;
		//		}
		//	}//end of else
		//}// end of else after first if
	
		//
		if (MATH::VMath::dot(MATH::VMath::dot(AB, ABC), AO) > 0) {
			newSearchDir = MATH::VMath::cross(AB, AO);
			newSearchDir = MATH::VMath::cross(newSearchDir, AB);
		}
		else if (MATH::VMath::dot(MATH::VMath::dot(ABC, AC), AO) > 0) {
			newSearchDir = MATH::VMath::cross(AC, AO);
			newSearchDir = MATH::VMath::cross(newSearchDir, AC);
		}
		else if (MATH::VMath::dot(ABC,AO) > 0) {
			newSearchDir = ABC;
		}
		else {
			newSearchDir = -ABC;
		}

	}//end of the triangle case
	else {
		//it has to be a line segment
		Vec3 B = finalSimplex.at(0); //because a is our newest point b has to be our first point
		Vec3 AB = B - A;

		//float testcase = MATH::VMath::dot(AB, AO);
		//if (testcase > 0) { //IF AB * AO > 0
			newSearchDir = MATH::VMath::cross(AB, AO);
			newSearchDir = MATH::VMath::cross(newSearchDir, AB);
		////(AB X AO) X AB
		//}
		//else {
		//	newSearchDir = AO;
		//}
	}//end of the line case
	return false;
}

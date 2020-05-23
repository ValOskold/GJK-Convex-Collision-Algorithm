
#include "VMath.h"
using namespace MATH;
 
Vec3 VMath::reflect(const Vec3 &v, const Vec3 &n){
	float d = dot(v, n);
	Vec3 result = (v - (2.0f * d) * n);
	return result;
}

float VMath::distance(const Vec3 &a, const Vec3 &b){
	Vec3 r  = a - b;
	return(mag(r));
}

float VMath::distance(const Vec3 &v, const Plane &p){
	return v.x*p.x + v.y*p.y + v.z*p.z + p.d;
}

float VMath::distance(const Sphere &s, const Plane &p) {
	float centerToPlane = dot(p, s) - p.d;
	return (centerToPlane - s.r);
}

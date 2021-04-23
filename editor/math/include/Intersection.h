#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include "AABB.h"

using namespace std;

namespace BSMath
{
	
class Intersection
{
public:	
	static bool AabbIntersects(const Aabb &a,const  Aabb &b)  
	{
		if ((a.getMinimum().x > b.getMaximum().x) || (b.getMinimum().x > a.getMaximum().x))
      		return false;
   		if ((a.getMinimum().y > b.getMaximum().y) || (b.getMinimum().y > a.getMaximum().y))
      		return false;
   		if ((a.getMinimum().z > b.getMaximum().z) || (b.getMinimum().z > a.getMaximum().z))
      		return false;
   		
   		return true;	
	};
	
	static bool RayIntersects(const Ray<double>& ray, const Vector &v0, const Vector &v1, const Vector &v2, 
								const bool bCull, const float)  
	{
		Vector pvec, tvec, qvec;
	
	   	Vector edge1 = v1 - v0;
	    Vector edge2 = v2 - v0;
	
	   	pvec.cross(ray.getDirection(), edge2);
	
	   // if close to 0 ray is parallel
	   	float det = edge1 * pvec;
	   	if ( (bCull) && (det < 0.0001f) )
	      return false;
	   else if ( (det < 0.0001f) && (det > -0.0001f) )
	      return false;
	
	   // distance to plane, < 0 means beyond plane
	   tvec = ray.getOrigin() - v0;
	   float u = tvec * pvec;
	   if (u < 0.0f || u > det)
	      return false;
	
	   qvec.cross(tvec, edge1);
	   float v = ray.getDirection() * qvec;
	   if (v < 0.0f || u+v > det)
	      return false;
	   
	 /* Any math crack here? :) 
	  * if (t) 
	   {
	      *t = edge2 * qvec;
	      float fInvDet = 1.0f / det;
	      *t *= fInvDet;
	   }*/

   		return true;
	};
	
};	

	
}


#endif /*INTERSECTION_H_*/

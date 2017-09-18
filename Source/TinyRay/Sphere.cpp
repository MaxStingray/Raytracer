/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#include <math.h>
#include "Sphere.h"

Sphere::Sphere()
{
	//the default sphere is centred around the origin of the world coordinate
	//default radius is 2.0

	m_centre.SetZero();
	m_radius = 2.0;
	m_primtype = PRIMTYPE_Sphere;
}

Sphere::Sphere(double x, double y, double z, double r)
{
	m_centre.SetVector(x, y, z);
	m_radius = r;
	m_primtype = PRIMTYPE_Sphere;
}

Sphere::~Sphere()
{
}

RayHitResult Sphere::IntersectByRay(Ray& ray)
{
	RayHitResult result = Ray::s_defaultHitResult;

	double t = FARFAR_AWAY;
	double posVal;
	double negVal;
	double rayDotProd;
	Vector4D normal;
	Vector4D sphereVector;
	Vector4D intersection_point;

	sphereVector = ray.GetRayStart() - m_centre;
	rayDotProd = ray.GetRay().DotProduct(sphereVector);

	posVal = ((ray.GetRay()*-1).DotProduct(sphereVector) + sqrt((rayDotProd * rayDotProd) - ray.GetRay().DotProduct(ray.GetRay())*((sphereVector.DotProduct(sphereVector)) - (m_radius*m_radius))))
		/ ray.GetRay().DotProduct(ray.GetRay());

	negVal = ((ray.GetRay()*-1).DotProduct(sphereVector) - sqrt((rayDotProd * rayDotProd) - ray.GetRay().DotProduct(ray.GetRay())*((sphereVector.DotProduct(sphereVector)) - (m_radius*m_radius))))
		/ ray.GetRay().DotProduct(ray.GetRay());

	if (posVal > 0 && negVal > 0)
	{
		if (posVal > 0 && posVal < negVal)
		{
			t = posVal;
		}
		else if (negVal > 0 && negVal < posVal && posVal > 0)
		{
			t = negVal;
		}
		else if (negVal < 0)
		{
			t = posVal;
		}
		else
		{
			t = negVal;
		}
	}

	//Calculate the exact location of the intersection using the result of t
	intersection_point = ray.GetRayStart() + ray.GetRay()*t;

	//Calculate normal
	normal[0] = intersection_point[0] - m_centre[0];
	normal[1] = intersection_point[1] - m_centre[1];
	normal[2] = intersection_point[2] - m_centre[2];

	if (t>0.0 && t < FARFAR_AWAY)
	{
		result.t = t;
		result.data = this;
		result.point = intersection_point;
		result.normal = normal;
	}

	return result;
}

/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
* 
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#include "Triangle.h"

Triangle::Triangle()
{
	m_vertices[0] = Vector4D(-1.0, 0.0, -5.0);
	m_vertices[1] = Vector4D(0.0, 1.0, -5.0);
	m_vertices[2] = Vector4D(1.0, 0.0, -5.0);
	m_normal = Vector4D(0.0, 0.0, 1.0);
	m_primtype = PRIMTYPE_Triangle;
}

Triangle::Triangle(Vector4D pos1, Vector4D pos2, Vector4D pos3)
{
	SetTriangle(pos1, pos2, pos3);

	m_primtype = PRIMTYPE_Triangle;
}


Triangle::~Triangle()
{
}

void Triangle::SetTriangle(Vector4D v0, Vector4D v1, Vector4D v2)
{
	m_vertices[0] = v0;
	m_vertices[1] = v1;
	m_vertices[2] = v2;

	//Calculate Normal
	Vector4D NormalA = m_vertices[1] - m_vertices[0];
	Vector4D NormalB = m_vertices[2] - m_vertices[0];
	Vector4D Norm = NormalA.CrossProduct(NormalB);
	Norm.Normalise();
	m_normal = Norm;
}


RayHitResult Triangle::IntersectByRay(Ray& ray)
{
	RayHitResult result = Ray::s_defaultHitResult;
	double t = FARFAR_AWAY;
	Vector4D intersection_point;
	
	// this section uses the technique hosted at http://www.blackpawn.com/texts/pointinpoly/ translated from pseudocode

	t = -(ray.GetRayStart().DotProduct(m_normal) - (m_vertices[0].DotProduct(m_normal))) / ray.GetRay().DotProduct(m_normal);

	//Calculate the exact location of the intersection using the result of t
	intersection_point = ray.GetRayStart() + ray.GetRay()*t;

	Vector4D v0 = m_vertices[2] - m_vertices[0];
	Vector4D v1 = m_vertices[1] - m_vertices[0];
	Vector4D v2 = intersection_point - m_vertices[0];

	double dot00 = v0.DotProduct(v0);
	double dot01 = v0.DotProduct(v1);
	double dot02 = v0.DotProduct(v2);
	double dot11 = v1.DotProduct(v1);
	double dot12 = v1.DotProduct(v2);

	double temp = 1 / (dot00 * dot11 - dot01 * dot01);

	double u = (dot11 * dot02 - dot01 * dot12) * temp;
	double v = (dot00 * dot12 - dot01 * dot02) * temp;

	if (u < 0 || v < 0 || u + v >= 1)
	{
		t = FARFAR_AWAY;
		intersection_point = ray.GetRayStart() + ray.GetRay()*t;
	}

	if (t > 0 && t < FARFAR_AWAY) { //ray intersection
		result.t = t;
		result.normal = this->m_normal;
		result.point = intersection_point;
		result.data = this;
		return result;
	}

	return result;
}

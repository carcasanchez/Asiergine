
#pragma once
#include ".\MathGeoLib\include\Math.h"
#include "Color.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const math::float3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	math::float4x4 transform;
	bool axis,wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class Cube_prim : public Primitive
{
public :
	Cube_prim();
	Cube_prim(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
	void Size(float x, float y, float z);

public:
	math::OBB geometry;
};

/*
// ============================================
class Sphere_prim : public Primitive
{
public:
	Sphere_prim();
	Sphere_prim(float radius);
	void InnerRender() const;
public:
	math::Sphere geometry;
};

// ============================================
class Cylinder_prim : public Primitive
{
public:
	Cylinder_prim();
	Cylinder_prim(float radius, float height);
	void InnerRender() const;
public:
	math::Cylinder geometry;
};

// ============================================
class Line_prim : public Primitive
{
public:
	Line_prim();
	Line_prim(float x, float y, float z);
	void InnerRender() const;
public:v
	math::Line geometry;
};

// ============================================
class Plane_prim : public Primitive
{
public:
	Plane_prim();
	Plane_prim(float x, float y, float z, float d);
	void InnerRender() const;
public:
	math::Plane geometry;
};

// ============================================
class Capsule_prim : public Primitive
{
public:
	Capsule_prim();
	Capsule_prim(float x, float y, float z, float d);
	void InnerRender() const;
public:
	math::Capsule geometry;
};*/
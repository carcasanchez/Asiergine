#include "Geometry.h"

void Geometry::DrawGeometry()
{
}

Geometry::Geometry()
{
}

Geometry::~Geometry()
{
	delete[] vertices;
	delete[] indices;
}

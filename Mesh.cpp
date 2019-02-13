#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::setVertices(std::vector<TriangleVertex> vertices)
{
	_vertices = vertices;
}

void Mesh::setHitBox()
{

}

std::vector <TriangleVertex> & Mesh::getVertices()
{
	return _vertices;
}

int Mesh::getVertCount()
{
	return _vertices.size();
}

OBB Mesh::getHitBox()
{
	return _hitbox;
}

#pragma once
#include <vector>
#include "TriangleVertex.h"
#include "OBB.h"

class Mesh
{
private:
	std::vector <TriangleVertex> _vertices;
	OBB _hitbox;

public:
	Mesh();
	~Mesh();

	void setVertices(std::vector <TriangleVertex> vertices);
	void setHitBox();

	std::vector <TriangleVertex> getVertices();
	int getVertCount();
	OBB getHitBox();
};
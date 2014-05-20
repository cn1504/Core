#pragma once
#include "Core.h"
#include "Mesh.h"

namespace Core
{

	class Shape
	{
	public:
		virtual ~Shape() {}
		virtual Mesh* GenerateMesh() = 0;
		virtual float CalculateVolume(glm::vec3 scale) = 0;
		virtual btCollisionShape* GetCollisionShape(glm::vec3 scale) = 0;
	};

}
#include "Transform.h"

namespace Core
{

	Transform::Transform()
	{
		Position = glm::vec3(0.0f, 0.0f, 0.0f);
		Rotation = glm::quat();
		Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		Up = glm::vec3(0.0f, 1.0f, 0.0f);
		Forward = glm::vec3(0.0f, 0.0f, 1.0f);
	}


	Transform::~Transform()
	{
	}


	glm::mat4 Transform::ToMatrix()
	{
		return glm::translate(glm::mat4(), Position) * glm::toMat4(Rotation) * glm::scale(glm::mat4(), Scale);
	}

}
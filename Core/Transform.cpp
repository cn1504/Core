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


	glm::vec3 Transform::WSUp()
	{
		return Rotation * Up;
	}


	glm::vec3 Transform::WSForward()
	{
		return Rotation * Forward;
	}


	glm::mat4 Transform::ToMatrix()
	{
		auto t = glm::translate(glm::mat4(), Position);
		auto r = glm::toMat4(Rotation);
		auto s = glm::scale(Scale);
		
		return t * r * s;
	}

}
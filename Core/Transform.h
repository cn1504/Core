#pragma once
#include "Core.h"

namespace Core
{

	class Transform
	{
	public:
		glm::vec3 Position;
		glm::quat Rotation;
		glm::vec3 Scale;

		glm::vec3 Up;
		glm::vec3 WSUp();
		glm::vec3 Forward;
		glm::vec3 WSForward();

		Transform();
		~Transform();

		glm::mat4 ToMatrix();
	};

}
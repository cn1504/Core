#include "Spinner.h"

namespace Test
{

	Spinner::Spinner()
	{
	}


	Spinner::~Spinner()
	{
	}


	void Spinner::Load()
	{
		mat = Entity->GetComponent<Core::Material>();
	}


	void Spinner::Update()
	{
		Entity->Transform.Rotation = Entity->Transform.Rotation * glm::quat(glm::vec3(1.0f, 1.0f, 0.0f) * Core::Time::Delta);
		mat->DiffuseColor = glm::vec3((glm::sin(Core::Time::Elapsed) + 1.0f) * 0.5f, 0.5f, 0.0f);
	}

}
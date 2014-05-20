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
		FB = Entity->GetComponent<Core::FreeBody>();
		TimeSinceLastImpulse = 0.0f;
		i = 0;
	}


	void Spinner::Update()
	{
		TimeSinceLastImpulse += Core::Time::Delta;
		while (TimeSinceLastImpulse > 1.0f)
		{
			if (i == 0)
				FB->ApplyImpulse(glm::vec3(0, 0.98, 0) * FB->GetMass(), glm::vec3(0.3, 0.0, 0.3));
			if (i == 1)
				FB->ApplyImpulse(glm::vec3(0, -0.98, 0) * FB->GetMass(), glm::vec3(0.3, 0.0, 0.3));
			if (i == 2)
				FB->ApplyImpulse(glm::vec3(0, -0.98, 0) * FB->GetMass(), glm::vec3(0.3, 0.0, -0.3));
			if (i == 3)
				FB->ApplyImpulse(glm::vec3(0, 0.98, 0) * FB->GetMass(), glm::vec3(0.3, 0.0, -0.3));

			i = (i + 1) % 4;

			TimeSinceLastImpulse -= 1.0f;
		}
	}

}
#include "Core.h"
#include "Component.h"

namespace Core
{

	class LightSource
		: public Component
	{
	public:
		glm::vec3 Color;

		LightSource(glm::vec3 color);
		virtual ~LightSource();

		virtual void Load();
		virtual void Update();
	};

}
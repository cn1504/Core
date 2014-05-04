#include "Core.h"
#include "FreeBody.h"

namespace Core
{

	class DynamicsWorld
	{
	private:
		std::vector<FreeBody*> FreeBodies;

	public:
		DynamicsWorld();
		~DynamicsWorld();

		void AddBody(FreeBody* body);
	};

}
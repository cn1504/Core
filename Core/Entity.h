#pragma once
#include "Core.h"
#include "Transform.h"
#include "Component.h"

namespace Core
{
	class Renderable;

	class Entity
	{
	private:
		std::vector<Component*> Components;
		Renderable* renderable;

	public:
		Transform Transform;

		Entity();
		~Entity();

		void Load();
		void Update();
		void AddComponent(Component* c);

		bool IsRenderable();
		Renderable* GetRenderable();

		template<class T>
		T* GetComponent();
	};


	template<class T>
	T* Entity::GetComponent()
	{
		for (auto c : Components)
		{
			T* t = dynamic_cast<T*>(c);
			if (t != nullptr)
				return t;
		}
		return nullptr;
	}

}
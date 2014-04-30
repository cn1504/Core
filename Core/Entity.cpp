#include "Entity.h"
#include "Renderable.h"
#include <typeinfo>

namespace Core
{

	Entity::Entity()
	{
		renderable = nullptr;
	}


	Entity::~Entity()
	{
		for (auto c : Components)
			delete c;
	}


	void Entity::Load()
	{
		for (auto c : Components)
			c->Load();
	}


	void Entity::Update()
	{
		for (auto c : Components)
			c->Update();
	}


	void Entity::AddComponent(Component* c)
	{
		c->Entity = this;
		Components.push_back(c);

		Renderable* r = dynamic_cast<Renderable*>(c);
		if (r != nullptr)
			renderable = r;
	}


	bool Entity::IsRenderable()
	{
		return renderable != nullptr;
	}


	Renderable* Entity::GetRenderable()
	{
		return renderable;
	}

}
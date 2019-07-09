#pragma once
#include <vector>
#include <memory>
#include <string>
#include <typeinfo>

#include "World.h"
#include "Component.h"
#include "System.h"

class Component;

enum KeyEntityIDs
{
	GAMEMANAGER,
	CAMERA,
	PLAYER
};

class Entity
{
public:
	Entity();
	~Entity();

	// Identifiers
	unsigned int EntityID;	// What the entity is
	std::string Name;		// The name of the entity

	// Stores all components attached to this entity
	std::vector<std::shared_ptr<Component>> Components;

	// Gets an attached component
	template <typename T>
	std::shared_ptr<T> GetComponent(void)
	{
		std::string TNAME = typeid(T).name();
		for (unsigned int i = 0; i < Components.size(); i++)
		{
			if (Components[i]->Name == TNAME)
				return std::dynamic_pointer_cast<T>(Components[i]);
		}

		return nullptr;
	}

	// Attach a Component
	template <typename T>
	void AddComponent()
	{
		std::shared_ptr<T> NewComponent = std::shared_ptr<T>(new T());
		NewComponent->Name = typeid(T).name();
		NewComponent->OwnerHandle = std::shared_ptr<Entity>(this);
		Components.push_back(NewComponent);
	}

	// Remove a Component. I doubt I'm going to use it, but it's here for completeness
	template <typename T>
	void RemoveComponent();

	// Create a new entity
	static std::shared_ptr<Entity> CreateEntity(unsigned int EntityID, std::string Name)
	{
		std::shared_ptr<Entity> NewEntity = std::shared_ptr<Entity>(new Entity());
		std::pair<unsigned int, std::shared_ptr<Entity>> EntityPair;

		NewEntity->EntityID = EntityID;
		NewEntity->Name = Name;

		return NewEntity;
	}
};

// Writing std::shared_ptr<Entity> can be a pain in the arse
typedef std::shared_ptr<Entity> pEntity;


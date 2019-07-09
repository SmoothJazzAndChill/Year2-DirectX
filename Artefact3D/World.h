#pragma once
#include <vector>
#include <map>
#include <memory>
#include <Windows.h>

#include "Entity.h"
#include "Component.h"
#include "System.h"

class Component;
class System;
class InputComponent;

class World
{
public:
	World();
	~World();

	// Setup the game world
	void Setup();

	// Game Tick
	void Update();

	// Fixed Game Tick
	void FixedUpdate();

	// Find the first Entity with Name or ID
	std::shared_ptr<Entity> Find(std::string Name);
	std::shared_ptr<Entity> Find(unsigned int ID);

	// Find all Entities with Name or ID
	std::vector<std::shared_ptr<Entity>> FindAll(std::string Name);
	std::vector<std::shared_ptr<Entity>> FindAll(unsigned int ID);

	// Find all entities that have a specific component attached to them
	template <typename C>
	std::vector<std::shared_ptr<Entity>> FindEntitiesWithComponent()
	{
		std::vector<pEntity> Found;

		for (std::map<unsigned int, std::shared_ptr<Entity>>::iterator i = Entities.begin(); i != Entities.end(); ++i)
		{
			if ((i->second)->GetComponent<C>())
			{
				Found.push_back((i->second));
			}
		}

		return Found;
	}

	template <typename S>
	static std::shared_ptr<S> CreateSystem()
	{
		return std::shared_ptr<S>(new S);
	}

	// Queues input ready to be processed
	void QueueInput(MSG* msg);

private:
	// Where everything is stored
	std::vector<std::shared_ptr<System>> Systems;
	std::map<unsigned int, std::shared_ptr<Entity>> Entities;
	std::vector<std::shared_ptr<Component>> Components;

	// Easy access to the component that stores the input
	std::shared_ptr<InputComponent> Input;

	// Grab everything
	void GrabComponents(); //Don't know if this works, haven't needed to use it yet
	void GetSystems();
};


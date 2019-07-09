#include "World.h"

#include "InputManager.h"
#include "CameraMovement.h"

#include "Input.h"
#include "Transform.h"
#include "Camera.h"

World::World()
{
	// Try to modularise this at some point

	// Create a gamemanager entity
	pEntity GameManager = Entity::CreateEntity(GAMEMANAGER, "GameManager");
	GameManager->AddComponent<InputComponent>();
	Input = GameManager->GetComponent<InputComponent>();
	std::pair<unsigned int, pEntity> NewPair;
	NewPair.first = GAMEMANAGER;
	NewPair.second = GameManager;
	Entities.insert(NewPair);

	// Create a camera entity
	pEntity Camera = Entity::CreateEntity(CAMERA, "Camera");
	Camera->AddComponent<TransformComponent>();
	Camera->AddComponent<CameraComponent>();
	NewPair.first = CAMERA;
	NewPair.second = Camera;
	Entities.insert(NewPair);
}


World::~World()
{
}

void World::Setup()
{

	// Grab all of the components
	GrabComponents();
	GetSystems();

	// Start all the systems
	for (std::vector<std::shared_ptr<System>>::iterator i = Systems.begin(); i != Systems.end(); ++i)
	{
		(*i)->Start();
	}
}

void World::Update()
{
	for (std::vector<std::shared_ptr<System>>::iterator i = Systems.begin(); i != Systems.end(); ++i)
	{
		(*i)->Update();
	}
}

void World::FixedUpdate()
{
	for (std::vector<std::shared_ptr<System>>::iterator i = Systems.begin(); i != Systems.end(); ++i)
	{
		(*i)->FixedUpdate();
	}
}

std::shared_ptr<Entity> World::Find(std::string Name)
{
	for (std::map<unsigned int, std::shared_ptr<Entity>>::iterator i = Entities.begin(); i != Entities.end(); ++i)
	{
		if ((i->second)->Name == Name)
		{
			return (i->second);
		}
	}

	return nullptr;
}

std::shared_ptr<Entity> World::Find(unsigned int ID)
{
	for (std::map<unsigned int, std::shared_ptr<Entity>>::iterator i = Entities.begin(); i != Entities.end(); ++i)
	{
		if((i->first) == ID)
		{
			return (i->second);
		}
	}

	return nullptr;
}

std::vector<std::shared_ptr<Entity>> World::FindAll(std::string Name)
{
	std::vector<std::shared_ptr<Entity>> Found;

	for (std::map<unsigned int, std::shared_ptr<Entity>>::iterator i = Entities.begin(); i != Entities.end(); ++i)
	{
		if ((i->second)->Name == Name)
		{
			Found.push_back((i->second));
		}
	}

	return Found;
}

std::vector<std::shared_ptr<Entity>> World::FindAll(unsigned int ID)
{
	std::vector<std::shared_ptr<Entity>> Found;

	for (std::map<unsigned int, std::shared_ptr<Entity>>::iterator i = Entities.begin(); i != Entities.end(); ++i)
	{
		if ((i->first) == ID)
		{
			Found.push_back((i->second));
		}
	}

	return Found;
}

void World::GrabComponents()
{
	for (std::map<unsigned int, std::shared_ptr<Entity>>::iterator i = Entities.begin(); i != Entities.end(); ++i)
	{
		for (std::shared_ptr<Component> c : (i->second)->Components)
		{
			Components.push_back(c);
		}
	}
}

void World::GetSystems()
{
	std::shared_ptr<InputManagerSystem> S = std::shared_ptr<InputManagerSystem>(new InputManagerSystem());
	S->GameWorld = std::shared_ptr<World>(this);
	Systems.push_back(S);

	std::shared_ptr<CameraMovementSystem> C = std::shared_ptr<CameraMovementSystem>(new CameraMovementSystem());
	C->GameWorld = std::shared_ptr<World>(this);
	Systems.push_back(C);
}

void World::QueueInput(MSG* msg)
{
	Input->InputQueue.push(*msg);
}

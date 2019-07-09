#pragma once
#include <memory>
#include <vector>

#include "Entity.h"

class Entity;

class Component
{
public:
	Component() {}
	virtual ~Component() {}

	std::string Name;

	std::shared_ptr<Entity> OwnerHandle;

	// Gets a different component attached to the same entity
	template <typename T>
	std::shared_ptr<T> Sibling()
	{
		return OwnerHandle->GetComponent<T>();
	}
};


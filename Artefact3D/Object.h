#pragma once
#include <vector>
#include <memory>

#include "System.h"
#include "Entity.h"

class Object
{
public:
	Object();
	~Object();

	std::vector<std::shared_ptr<Entity>> Entities;
	std::vector<std::shared_ptr<System>> Systems;
};


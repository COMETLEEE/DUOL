#include "DUOLGraphicsLibrary/EntityBase.h"

namespace DUOLGraphicsLibrary
{

	bool EntityBase::IsInstanceOf(int id) const
	{
		return (static_cast<EntityID>(id) == EntityID::EntityBase);
	}

}
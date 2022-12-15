#include "DUOLEditor/UI/GUIBase.h"

namespace DUOLEditor
{
	uint64_t GUIBase::_nextID = 0;

	const uint64_t& GUIBase::GetID() const
	{
		return _id;
	}

	GUIBase::GUIBase() :
		_id(_nextID++)
	{
	}
}
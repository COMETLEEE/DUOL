#include "DUOLEditor/UI/GUIBase.h"

namespace DUOLEditor
{
	uint64_t GUIBase::_nextID = 0;

	const uint64_t& GUIBase::GetID() const
	{
		return _id;
	}

	const DUOLCommon::tstring& GUIBase::GetTStringID() const
	{
		return _tstringID;
	}

	GUIBase::GUIBase() :
		_id(_nextID++)
	{
		_tstringID = TEXT("##") + DUOLCommon::StringHelper::ToTString(_id);
	}
}
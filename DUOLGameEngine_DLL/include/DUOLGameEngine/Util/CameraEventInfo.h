#pragma once
#include "DUOLCommon/StringHelper.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGameEngine
{
	struct FrameInfo;

	enum class SequenceType { Linear, Catmullrom };

	struct CameraEventInfo
	{
		UINT64 _id;
		DUOLCommon::tstring _eventName;
		SequenceType _type;
		std::vector<FrameInfo*> _frameInfo;

	};

	struct FrameInfo
	{
		int _frame;
		DUOLMath::Vector3 _position;
		DUOLMath::Vector4 _rotation;
	};
}

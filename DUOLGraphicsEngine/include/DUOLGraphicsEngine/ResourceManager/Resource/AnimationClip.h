#pragma once
#include "DUOLGraphicsEngine/Export.h"
#include <vector>
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsEngine
{
	//�������� Ű������
	struct DUOLGRAPHICSENGINE_EXPORT KeyFrame
	{
		float _time;

		DUOLMath::Vector3	_localTransform;

		DUOLMath::Quaternion _localRotation;

		DUOLMath::Vector3	_localScale;
	};

	class DUOLGRAPHICSENGINE_EXPORT AnimationClip
	{
	public:
		AnimationClip() = default;
			
		float _frameRate;

		float _tickPerFrame;

		int _totalKeyFrame;

		int _startKeyFrame;

		int _endKeyFrame;

		//�� index< ������ index > 
		std::vector<std::vector<KeyFrame>> _keyFrameList;
	};

}

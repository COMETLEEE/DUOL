#pragma once
#include "DUOLGraphicsEngine/Export.h"
#include <vector>
#include "DUOLMath/DUOLMath.h"
#include <string>
typedef  unsigned __int64 uint64;

namespace DUOLGraphicsEngine
{

	struct DUOLGRAPHICSENGINE_EXPORT DUOLMath::Quaternion;
	struct DUOLGRAPHICSENGINE_EXPORT DUOLMath::Vector3;

	//�������� Ű������
	struct DUOLGRAPHICSENGINE_EXPORT KeyFrame
	{
	public:
		friend class boost::serialization::access;

		float _time;

		DUOLMath::Vector3	_localTransform;

		DUOLMath::Quaternion _localRotation;

		DUOLMath::Vector3	_localScale;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _time;

			ar& _localTransform;
			ar& _localRotation;
			ar& _localScale;
		}
	};

	class DUOLGRAPHICSENGINE_EXPORT AnimationClip
	{
	public:
		AnimationClip() = default;

		friend class boost::serialization::access;

		uint64 _animationID;

		std::string _animationName;

		float _frameRate;

		float _tickPerFrame;

		int _totalKeyFrame;

		int _startKeyFrame;

		int _endKeyFrame;

		//�� index< ������ index > 
		std::vector<std::vector<KeyFrame>> _keyFrameList;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _animationID;
			ar& _animationName;

			ar& _frameRate;
			ar& _tickPerFrame;
			ar& _totalKeyFrame;
			ar& _startKeyFrame;
			ar& _endKeyFrame;

			ar& _keyFrameList;
		}
	};

}

/**

	@file      EnemyData.h
	@brief     Enemy의 데이터.
	@details   ~ Enemy 클래스는 EnemyData에서 정보를 받아와 자신을 초기화 혹은 참조한다.
	@author    SinSeongHyoen
	@date      3.04.2023
	@copyright © SinSeongHyoen, 2023. All right reserved.

**/
#pragma once
#include <vector>

#include "DUOLCommon/StringHelper.h"
#include "DUOLMath/DUOLMath.h"
namespace DUOLClient
{
	enum class EnemyCode
	{
		Close,
		Far,
		WeakElite,
		Elite,
		Boss,
		Count
	};
	struct EnemyData
	{
		DUOLCommon::tstring _name;
		std::string _behaviorTreeName;
		DUOLCommon::tstring _fbxModelName;
		EnemyCode _enemyCode;

		std::vector<std::pair<DUOLCommon::tstring, float>> _floatParameterInitDatas;
		std::vector<std::pair<DUOLCommon::tstring, bool>> _boolParameterInitDatas;

		float _height; // 캡슐 높이
		DUOLCommon::tstring _animControllerName;
		DUOLMath::Vector3 _capsuleCenter;
		float _capsuleRadius;
		DUOLMath::Vector3 _navBaseOffset;

		std::vector<DUOLCommon::tstring> _eventFuncKey; // 공격 함수를 유닛들이 재활용 할 수 있도록 만든 것.
		DUOLCommon::tstring _hitFuncKey; // 피격 함수를 유닛들이 재활용 할 수 있도록 만든 것.
	};
}
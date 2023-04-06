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
		Elite,
		Count
	};
	struct EnemyData
	{
		DUOLCommon::tstring _name;
		std::string _behaviorTreeName;

		float _damage;
		float _maxHp;
		float _attackRange;
		float _patrolOffset;
		float _lookRange;
		float _maxSpeed;
		float _height;

		DUOLCommon::tstring _animControllerName;
		DUOLMath::Vector3 _capsuleCenter;
		DUOLMath::Vector3 _navBaseOffset;

		std::unordered_map<DUOLCommon::tstring, std::function<void(Enemy*)>> _attackFuncs; // 공격 함수를 유닛들이 재활용 할 수 있도록 만든 것.
	};


}
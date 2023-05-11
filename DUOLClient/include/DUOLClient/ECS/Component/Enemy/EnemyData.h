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
		Count
	};
	struct EnemyData
	{
		DUOLCommon::tstring _name;
		std::string _behaviorTreeName;
		DUOLCommon::tstring _fbxModelName;
		EnemyCode _enemyCode;

		float _damage; // 기본 데미지
		float _maxHp; // 최대 HP
		float _attackRange; // 공격 사정거리
		float _patrolOffset; // 순찰 범위
		float _lookRange; // 시야 거리
		float _maxSpeed; // 최대 속력
		float _height; // 캡슐 높이
		float _attackDelayTime; // 기본공격 할 때 딜레이 시간.
		float _attackCancelTime; // 기본공격 캔슬 프레임.
		float _chaseRange; // 추적 범위.
		float _superArmorMaxGauge; // 초 단위 게이지 
		float _superArmorTime; // 지속시간
		float _superArmorCoolTime; // 슈퍼아머 쿨타임.
		float _maxAcceleration; // 가속도 한계.

		DUOLCommon::tstring _animControllerName;
		DUOLMath::Vector3 _capsuleCenter;
		float _capsuleRadius;
		DUOLMath::Vector3 _navBaseOffset;

		std::vector<DUOLCommon::tstring> _eventFuncKey; // 공격 함수를 유닛들이 재활용 할 수 있도록 만든 것.
		DUOLCommon::tstring _hitFuncKey; // 피격 함수를 유닛들이 재활용 할 수 있도록 만든 것.
	};
}
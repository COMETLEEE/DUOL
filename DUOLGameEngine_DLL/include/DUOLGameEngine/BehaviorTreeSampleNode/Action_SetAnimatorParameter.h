/**

	@file      Action_SetAnimatorParameter.h
	@brief     액션 노드 샘플입니다.
	@details   ~ 애니메이터의 파라메터 값을 조정하는 노드 입니다.
	@author    SinSeongHyeon
	@date      8.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "behaviortree_cpp/action_node.h"
#include "DUOLCommon/StringHelper.h"
namespace DUOLGameEngine
{
	enum class ParameterType
	{
		INT,
		FLOAT,
		BOOL,
		NONE // 타입 해석에 실패 했을 경우..
	};
	class Animator;

	class Action_SetAnimatorParameter : public BT::SyncActionNode
	{
	public:
		Action_SetAnimatorParameter(const std::string& name, const BT::NodeConfig& config);

		virtual ~Action_SetAnimatorParameter() override;

	private:
		Animator* _animator;

		DUOLCommon::tstring _parameterKey;

		ParameterType _parameterType;

		DUOLCommon::tstring _parameterValue;

		int _value_int;
		float _value_float;
		bool _value_bool;
	public:
		virtual BT::NodeStatus tick() override;

		static BT::PortsList providedPorts();
	};
}

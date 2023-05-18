#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_SetAnimatorParameter.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/Util/BehaviorTreeTypeConvert.h"

DUOLGameEngine::Action_SetAnimatorParameter::Action_SetAnimatorParameter(const std::string& name,
	const BT::NodeConfig& config) :
	SyncActionNode(name, config),
	_animator(nullptr),
	_parameterType(ParameterType::NONE),
	_value_int(0),
	_value_float(0),
	_value_bool(false)
{
	_animator = getInput<DUOLGameEngine::Animator*>("Animator").value();

	_parameterKey = getInput<DUOLCommon::tstring>("ParameterKey").value();

	_parameterValue = getInput<DUOLCommon::tstring>("Value").value();

	const auto type = getInput<DUOLCommon::tstring>("Type").value();

	if (type == TEXT("BOOL") || type == TEXT("bool"))
	{
		_parameterType = ParameterType::BOOL;

		if (_parameterValue == TEXT("true") || _parameterValue == TEXT("True") || _parameterValue == TEXT("TRUE"))
			_value_bool = true;
		else
			_value_bool = false;
	}
	else if (type == TEXT("INT") || type == TEXT("int"))
	{
		_parameterType = ParameterType::INT;

		_value_int = std::stoi(_parameterValue);
	}
	else if (type == TEXT("FLOAT") || type == TEXT("float"))
	{
		_parameterType = ParameterType::FLOAT;

		_value_float = std::stof(_parameterValue);
	}
	else
	{
		DUOL_ASSERT(DUOL_CONSOLE, "Not a Type. Check out the behavior tree variables.");

		_parameterType = ParameterType::NONE;
	}
}

DUOLGameEngine::Action_SetAnimatorParameter::~Action_SetAnimatorParameter()
{
}

BT::NodeStatus DUOLGameEngine::Action_SetAnimatorParameter::tick()
{
	switch (_parameterType)
	{
	case ParameterType::INT:
	{
		_animator->SetInt(_parameterKey, _value_int);
	}
	break;

	case ParameterType::FLOAT:
	{
		_animator->SetFloat(_parameterKey, _value_float);
	}
	break;

	case ParameterType::BOOL:
	{
		_animator->SetBool(_parameterKey, _value_bool);
	}
	break;

	default:
		break;
	}


	return BT::NodeStatus::SUCCESS;
}

BT::PortsList DUOLGameEngine::Action_SetAnimatorParameter::providedPorts()
{
	BT::PortsList result = {
		BT::InputPort<DUOLGameEngine::Animator*>("Animator"),
		BT::InputPort<DUOLCommon::tstring>("ParameterKey"),
		BT::InputPort<DUOLCommon::tstring>("Type"),
		BT::InputPort<DUOLCommon::tstring>("Value"),
	};

	return result;
}

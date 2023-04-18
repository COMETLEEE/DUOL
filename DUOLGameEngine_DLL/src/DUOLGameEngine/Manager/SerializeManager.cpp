#include <fstream>

#include "DUOLGameEngine/Manager/SerializeManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"

#include "DUOLReflectionJson/JsonSerializer.h"

#include <rttr/type>
#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorControllerLayer.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"
#include "DUOLGameEngine/ECS/Object/AnimationClip.h"
#include "DUOLGameEngine/ECS/UI/OnClick.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"

using namespace rttr;

#pragma region MATH_REGISTRATION
RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLMath::Vector2>("Vector2")
	.constructor<>()
	(
	)
	.property("x", &DUOLMath::Vector2::x)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)

	)
	.property("y", &DUOLMath::Vector2::y)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	);

	rttr::registration::class_<DUOLMath::Vector3>("Vector3")
	.constructor<>()
	(
	)
	.property("x", &DUOLMath::Vector3::x)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)

		)
	.property("y", &DUOLMath::Vector3::y)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("z", &DUOLMath::Vector3::z)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	);

	rttr::registration::class_<DUOLMath::Vector4>("Vector4")
	.constructor<>()
	(
	)
	.property("x", &DUOLMath::Vector4::x)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("y", &DUOLMath::Vector4::y)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("z", &DUOLMath::Vector4::z)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("w", &DUOLMath::Vector4::w)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	);

	rttr::registration::class_<DUOLMath::Quaternion>("Quaternion")
		.constructor<>()
		(
		)
		.property("x", &DUOLMath::Quaternion::x)
		(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
			)
		.property("y", &DUOLMath::Quaternion::y)
		(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
			)
		.property("z", &DUOLMath::Quaternion::z)
		(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
			)
		.property("w", &DUOLMath::Quaternion::w)
		(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		);

	rttr::registration::class_<DUOLMath::Matrix>("Matrix")
		.constructor<>()()
		.property("_11", &DUOLMath::Matrix::_11)(metadata(DUOLCommon::MetaDataType::Serializable, true))
		.property("_12", &DUOLMath::Matrix::_12)(metadata(DUOLCommon::MetaDataType::Serializable, true))
		.property("_13", &DUOLMath::Matrix::_13)(metadata(DUOLCommon::MetaDataType::Serializable, true))
		.property("_14", &DUOLMath::Matrix::_14)(metadata(DUOLCommon::MetaDataType::Serializable, true))
		.property("_21", &DUOLMath::Matrix::_21)(metadata(DUOLCommon::MetaDataType::Serializable, true))
		.property("_22", &DUOLMath::Matrix::_22)(metadata(DUOLCommon::MetaDataType::Serializable, true))
		.property("_23", &DUOLMath::Matrix::_23)(metadata(DUOLCommon::MetaDataType::Serializable, true))
		.property("_24", &DUOLMath::Matrix::_24)(metadata(DUOLCommon::MetaDataType::Serializable, true))
		.property("_31", &DUOLMath::Matrix::_31)(metadata(DUOLCommon::MetaDataType::Serializable, true))
		.property("_32", &DUOLMath::Matrix::_32)(metadata(DUOLCommon::MetaDataType::Serializable, true))
		.property("_33", &DUOLMath::Matrix::_33)(metadata(DUOLCommon::MetaDataType::Serializable, true))
		.property("_34", &DUOLMath::Matrix::_34)(metadata(DUOLCommon::MetaDataType::Serializable, true))
		.property("_41", &DUOLMath::Matrix::_41)(metadata(DUOLCommon::MetaDataType::Serializable, true))
		.property("_42", &DUOLMath::Matrix::_42)(metadata(DUOLCommon::MetaDataType::Serializable, true))
		.property("_43", &DUOLMath::Matrix::_43)(metadata(DUOLCommon::MetaDataType::Serializable, true))
		.property("_44", &DUOLMath::Matrix::_44)(metadata(DUOLCommon::MetaDataType::Serializable, true));
}
#pragma endregion

namespace DUOLGameEngine
{
	SerializeManager::SerializeManager() :
		_path(TEXT("Asset/"))
	{
#pragma region FOR_SCENE
		// 'uint64_t' => 'DUOLGameEngine::GameObject*'
		rttr::type::register_converter_func([](const uint64_t& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::GameObject*>(id);
			});

		// 'uint64_t' => 'DUOLGameEngine::Transform*'
		rttr::type::register_converter_func([](const uint64_t& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::Transform*>(id);
			});

		// 'uint64_t' => 'DUOLGameEngine::ComponentBase*'
		rttr::type::register_converter_func([](const uint64_t& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::ComponentBase*>(id);
			});

		// 'int64_t' => 'DUOLGameEngine::ComponentBase*'
		rttr::type::register_converter_func([](const int64_t& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::ComponentBase*>(id);
			});

		// 'uint32_t' => 'DUOLGameEngine::ComponentBase*'
		rttr::type::register_converter_func([](const uint32_t& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::ComponentBase*>(id);
			});

		// 'int' => 'DUOLGameEngine::ComponentBase*'
		rttr::type::register_converter_func([](const int& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::ComponentBase*>(id);
			});

		// 'int64_t' => 'DUOLGameEngine::Transform*'
		rttr::type::register_converter_func([](const int64_t& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::Transform*>(id);
			});

		// 'uint32_t' => 'DUOLGameEngine::Transform*'
		rttr::type::register_converter_func([](const uint32_t& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::Transform*>(id);
			});

		// 'int' => 'DUOLGameEngine::Transform*'
		rttr::type::register_converter_func([](const int& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::Transform*>(id);
			});
#pragma endregion

#pragma region FOR_ANIMATOR_CONTROLLER
		// 'uint64_t' => 'DUOLGameEngine::AnimatorControllerLayer*'
		rttr::type::register_converter_func([](const uint64_t& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::AnimatorController*>(id);
			});

		// 'uint64_t' => 'DUOLGameEngine::AnimatorControllerLayer*'
		rttr::type::register_converter_func([](const uint64_t& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::AnimatorControllerLayer*>(id);
			});

		// 'uint64_t' => 'DUOLGameEngine::AnimatorStateMachine*'
		rttr::type::register_converter_func([](const uint64_t& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::AnimatorStateMachine*>(id);
			});

		// 'uint64_t' => 'DUOLGameEngine::AnimatorState*'
		rttr::type::register_converter_func([](const uint64_t& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::AnimatorState*>(id);
			});

		// 'uint64_t' => 'DUOLGameEngine::AnimatorStateTransition*'
		rttr::type::register_converter_func([](const uint64_t& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::AnimatorStateTransition*>(id);
			});

		// 'uint64_t' => 'DUOLGameEngine::AnimationClip*'
		rttr::type::register_converter_func([](const uint64_t& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::AnimationClip*>(id);
			});
#pragma endregion

#pragma region FOR_UI
		// 'uint64_t' => 'DUOLGameEngine::OnClick*'
		rttr::type::register_converter_func([](const uint64_t& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::OnClick*>(id);
			});

		// 'uint64_t' => 'DUOLGameEngine::OnClickCall*'
		rttr::type::register_converter_func([](const uint64_t& id, bool& ok)
			{
				ok = true;

				return reinterpret_cast<DUOLGameEngine::OnClickCall*>(id);
			});
#pragma endregion
	}

	SerializeManager::~SerializeManager()
	{
	}

	void SerializeManager::Initialize(const DUOLGameEngine::EngineSpecification& engineSpec)
	{
		// _path = engineSpec.projectPath;

		_path = TEXT("Asset/");

		DUOL_INFO(DUOL_FILE, "SerializeManager Initialize Success !");
	}

	void SerializeManager::UnInitialize()
	{
		// 딱히 할 일 없는 것 같다..
	}

	bool SerializeManager::SerializeScene(const DUOLGameEngine::Scene* scene, const DUOLCommon::tstring& filePath)
	{
		DUOLReflectionJson::JsonSerializer jsonSerializer;

		// 모든 게임 오브젝트에 대해서 합시다 ..
		rttr::instance sceneObject = *scene;

		std::string fileContents = jsonSerializer.ToJson(sceneObject);

		if (fileContents.empty())
			return false;

		// JSON File 로 Out
		std::ofstream ofs{ DUOLCommon::StringHelper::ToString(filePath) };

		ofs << fileContents;

		ofs.close();

		return true;
	}

	std::shared_ptr<DUOLGameEngine::Scene> SerializeManager::DeserializeScene(const DUOLCommon::tstring& filePath)
	{
		DUOLReflectionJson::JsonSerializer jsonSerializer;

		// 외부에서 매핑할 주소를 긁어올 수 있는 함수를 지정합니다.
		jsonSerializer.SetUUIDObjectFunc([](DUOLCommon::UUID uuid)
			{
				return ResourceManager::GetInstance()->GetResourceByUUID(uuid);
			});

		jsonSerializer.SetStringObjectFunc([](DUOLCommon::tstring& name)
			{
				return ResourceManager::GetInstance()->GetResourceByName(name);
			});

		auto scene = new Scene();

		rttr::instance sceneInstance = scene;

		jsonSerializer.FromJson(filePath, sceneInstance);

		std::shared_ptr<DUOLGameEngine::Scene> sharedScene = std::shared_ptr<DUOLGameEngine::Scene>(scene);

		return sharedScene;
	}

	bool SerializeManager::SerializeAnimatorController(const DUOLGameEngine::AnimatorController* animatorController, const DUOLCommon::tstring& filePath)
	{
		DUOLReflectionJson::JsonSerializer jsonSerializer;

		const DUOLCommon::tstring& fileName = animatorController->GetName();

		// 모든 게임 오브젝트에 대해서 합시다 ..
		rttr::instance controllerObject = *animatorController;

		std::string fileContents = jsonSerializer.ToJson(controllerObject);

		if (fileContents.empty())
			return false;

		std::ofstream ofs{ DUOLCommon::StringHelper::ToString(filePath) };

		ofs << fileContents;

		ofs.close();

		return true;
	}

	std::shared_ptr<DUOLGameEngine::AnimatorController> SerializeManager::DeserializeAnimatorController(
		const DUOLCommon::tstring& filePath)
	{
		DUOLReflectionJson::JsonSerializer jsonSerializer;

		// 애니메이션 클립을 긁어올 수 있는 함수를 지정한다.
		jsonSerializer.SetUUIDObjectFunc([](DUOLCommon::UUID uuid)
			{
				return ResourceManager::GetInstance()->GetResourceByUUID(uuid);
			});

		jsonSerializer.SetStringObjectFunc([](DUOLCommon::tstring& name)
			{
				return ResourceManager::GetInstance()->GetResourceByName(name);
			});

		auto animCon = new AnimatorController();

		rttr::instance controllerInstance = animCon;

		jsonSerializer.FromJson(filePath, controllerInstance);

		std::shared_ptr<DUOLGameEngine::AnimatorController> animatorCon = std::shared_ptr<DUOLGameEngine::AnimatorController>(animCon);

		return animatorCon;
	}

	bool SerializeManager::SerializeAnimationClip(const DUOLGameEngine::AnimationClip* animationClip, const DUOLCommon::tstring& filePath)
	{
		DUOLReflectionJson::JsonSerializer jsonSerializer;

		const DUOLCommon::tstring& fileName = animationClip->GetName();

		// 모든 게임 오브젝트에 대해서 합시다 ..
		rttr::instance controllerObject = *animationClip;

		std::string fileContents = jsonSerializer.ToJson(controllerObject);

		if (fileContents.empty())
			return false;
		
		std::ofstream ofs{ DUOLCommon::StringHelper::ToString(filePath) };

		ofs << fileContents;

		ofs.close();

		return true;
	}

	std::shared_ptr<DUOLGameEngine::AnimationClip> SerializeManager::DeserializeAnimationClip(
		const DUOLCommon::tstring& filePath)
	{
		DUOLReflectionJson::JsonSerializer jsonSerializer;

		auto animClip = new AnimationClip();

		rttr::instance clipInstance = animClip;

		jsonSerializer.FromJson(filePath, clipInstance);

		std::shared_ptr<DUOLGameEngine::AnimationClip> animationClip = std::shared_ptr<DUOLGameEngine::AnimationClip>(animClip);

		return animationClip;
	}
}
#include <fstream>

#include "DUOLGameEngine/Manager/SerializeManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"

#include "DUOLReflectionJson/JsonSerializer.h"

#include <rttr/type>
#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorControllerLayer.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"
#include "DUOLGameEngine/ECS/Object/AnimationClip.h"

using namespace rttr;

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
}

namespace DUOLGameEngine
{
	SerializeManager::SerializeManager()
	{
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
	}

	SerializeManager::~SerializeManager()
	{
	}

	void SerializeManager::Initialize(const DUOLGameEngine::EngineSpecification& engineSpec)
	{
		// _path = engineSpec.projectPath;

		_path = TEXT("Asset/");
	}

	void SerializeManager::UnInitialize()
	{
	}

	bool SerializeManager::SerializeScene(const DUOLGameEngine::Scene* scene)
	{
		DUOLReflectionJson::JsonSerializer jsonSerializer;

		const DUOLCommon::tstring& fileName = scene->GetName();

		// 모든 게임 오브젝트에 대해서 합시다 ..
		rttr::instance sceneObject = *scene;

		std::string fileContents = jsonSerializer.ToJson(sceneObject);

		if (fileContents.empty())
			return false;

		DUOLCommon::tstring filePath = _path + TEXT("Scene/") + fileName + TEXT(".dscene");

		// JSON File 로 Out
		std::ofstream ofs{ DUOLCommon::StringHelper::ToString(filePath) };

		ofs << fileContents;

		ofs.close();

		return true;
	}

	std::shared_ptr<DUOLGameEngine::Scene> SerializeManager::DeserializeScene(const DUOLCommon::tstring& filePath)
	{
		DUOLReflectionJson::JsonSerializer jsonSerializer;

		// 텅빈 씬
		auto scene = std::shared_ptr<DUOLGameEngine::Scene>(new Scene());

		rttr::instance sceneInstance = scene;

		jsonSerializer.FromJson(filePath, sceneInstance);

		return scene;
	}

	bool SerializeManager::SerializeAnimatorController(const DUOLGameEngine::AnimatorController* animatorController)
	{
		DUOLReflectionJson::JsonSerializer jsonSerializer;

		const DUOLCommon::tstring& fileName = animatorController->GetName();

		// 모든 게임 오브젝트에 대해서 합시다 ..
		rttr::instance controllerObject = *animatorController;

		std::string fileContents = jsonSerializer.ToJson(controllerObject);

		if (fileContents.empty())
			return false;

		DUOLCommon::tstring filePath = _path + TEXT("AnimatorController/") + fileName + TEXT(".dcontroller");

		std::ofstream ofs{ DUOLCommon::StringHelper::ToString(filePath) };

		ofs << fileContents;

		ofs.close();

		return true;
	}

	std::shared_ptr<DUOLGameEngine::AnimatorController> SerializeManager::DeserializeAnimatorController(
		const DUOLCommon::tstring& filePath)
	{
		DUOLReflectionJson::JsonSerializer jsonSerializer;

		auto animCon = new AnimatorController();

		rttr::instance controllerInstance = animCon;

		jsonSerializer.FromJson(filePath, controllerInstance);

		std::shared_ptr<DUOLGameEngine::AnimatorController> animationCon = std::shared_ptr<DUOLGameEngine::AnimatorController>(animCon);

		return animationCon;
	}
}
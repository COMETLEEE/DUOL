#include <fstream>

#include "DUOLGameEngine/Manager/SerializeManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

#include <rttr/type>
#include "DUOLReflectionJson/JsonSerializer.h"

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/document.h>


#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

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
		
	}

	SerializeManager::~SerializeManager()
	{
	}

	void SerializeManager::Initialize(const DUOLGameEngine::EngineSpecification& engineSpec)
	{
		// _path = engineSpec.projectPath;

		_path = TEXT("Asset/Scene/");
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

		DUOLCommon::tstring fileContents = jsonSerializer.ToJson(sceneObject);

		if (fileContents.empty())
			return false;

		DUOLCommon::tstring filePath = _path + fileName + TEXT(".dscene");

		// JSON File 로 Out
		std::wofstream ofs{ DUOLCommon::StringHelper::ToTString(filePath) };

		ofs << fileContents;

		ofs.close();

		return true;
	}

	DUOLGameEngine::Scene* SerializeManager::DeserializeScene(const DUOLCommon::tstring& filePath)
	{
		return nullptr;
	}
}
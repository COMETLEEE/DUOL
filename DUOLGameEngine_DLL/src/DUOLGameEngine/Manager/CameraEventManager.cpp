#include "DUOLGameEngine/Manager/CameraEventManager.h"

#include "DUOLCommon/Util/Hash.h"
#include "DUOLJson/JsonReader.h"

namespace DUOLGameEngine
{
	CameraEventManager::CameraEventManager()
	{
	}

	CameraEventManager::~CameraEventManager()
	{
	}

	void CameraEventManager::Update(float deltaTime)
	{
	}

	void CameraEventManager::Initialize()
	{
		LoadCameraEvent(TEXT("Asset/DataTable/CameraSequences.json"));
	}

	void CameraEventManager::UnInitialize()
	{
	}

	void CameraEventManager::LoadCameraEvent(const DUOLCommon::tstring& path)
	{
		auto jsonReader = DUOLJson::JsonReader::GetInstance();

		auto cameraEventTable = jsonReader->LoadJson(path);

		const TCHAR* eventName = TEXT("sequenceName");

		const TCHAR* sequenceType = TEXT("sequenceType");

		const TCHAR* cameraKeyFrameDatas = TEXT("cameraKeyFrameDatas");

		uint64 eventID;

		/*	for (auto& camerasequence : cameraEventTable->GetArray())
			*/
		CameraEventInfo* cameraevet = new CameraEventInfo();

		//if (cameraEventTable->HasMember(L"cameraKeyFrameSequences"))
		//{
		//	//auto eventTable = cameraEventTable->HasMember(L"cameraKeyFrameSequences");


		//	// name을 key로 등록해줍니다. 
		//	const DUOLCommon::tstring& name = cameraEventTable->GetString();
		//	cameraevet->_id = DUOLCommon::Hash::Hash64(name);
		//	cameraevet->_eventName = name;
		//}


	}

	void CameraEventManager::Play(UINT64 key)
	{
	}
}

#include <iostream>

#define NOMINMAX

#include <Windows.h>
#include <tchar.h>

#include "DUOLFBXImporter/DUOLFBXImporter.h"
#include "DUOLFBXImporter/ParserData/DUOLFBXData.h"
#include "Serialize/BinarySerialize.h"

#include "DUOLJson/JsonReader.h"
#include "DUOLCommon/StringHelper.h"

void LoadFBXTable()
{
	std::unique_ptr<DUOLParser::DUOLFBXParser> fbxparser = std::make_unique<DUOLParser::DUOLFBXParser>();
	std::shared_ptr<FBXModel> _fbxModel = std::make_shared<FBXModel>();
	std::unique_ptr<DUOLFBXSerialize::BinarySerialize> test = std::make_unique<DUOLFBXSerialize::BinarySerialize>();

	auto jsonReader = DUOLJson::JsonReader::GetInstance();

	auto modelTable = jsonReader->LoadJson(L"Asset/DataTable/MeshTable.json");

	const TCHAR* id = TEXT("ID");

	const TCHAR* resourcePath = TEXT("ResourcePath");

	for(auto& model : modelTable->GetArray())
	{
		if (model.HasMember(id) && model.HasMember(resourcePath))
		{
			const DUOLCommon::tstring& modelStringID = model[id].GetString();

			const DUOLCommon::tstring& modelPath = model[resourcePath].GetString();

			_fbxModel = fbxparser->LoadFBX(DUOLCommon::StringHelper::ToString(modelPath), DUOLCommon::StringHelper::ToString(modelStringID));

			test->SerializeDuolData(_fbxModel);
		}
	}
	test->ExportJsonFile();
}

int main()
{
	LoadFBXTable();

	std::cout << "바이너리화 성공" << std::endl;

	return 0;
}
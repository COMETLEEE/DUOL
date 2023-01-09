#include <iostream>
#include <Windows.h>
#include <tchar.h>

#include "DUOLFBXImporter/DUOLFBXImporter.h"
#include "DUOLFBXImporter/ParserData/DUOLFBXData.h"
#include "Serialize/BinarySerialize.h"

#include "DUOLJson/JsonReader.h"

bool LoadFBXTable()
{
	auto jsonReader = DUOLJson::JsonReader::GetInstance();

	auto modelTable = jsonReader->LoadJson("Asset/DataTable/MeshTable.json");

	const TCHAR* id = TEXT("ID");

	const TCHAR* resourcePath = TEXT("ResourcePath");

}

int main()
{
	DUOLParser::DUOLFBXParser* fbxparser = new DUOLParser::DUOLFBXParser();
	std::shared_ptr<FBXModel> _fbxModel = std::make_shared<FBXModel>();

	_fbxModel = fbxparser->LoadFBX("Asset/Mesh/Joy.fbx");

	std::unique_ptr<BinarySerialize> test = std::make_unique<BinarySerialize>();

	test->SerializeDuolData(_fbxModel);

	std::cout << "바이너리화 성공" << std::endl;
	return 0;
}
 
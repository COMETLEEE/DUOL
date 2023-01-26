#include <iostream>

#define NOMINMAX

#include <Windows.h>
#include <tchar.h>
#include <filesystem>

#include "DUOLFBXImporter/DUOLFBXImporter.h"
#include "DUOLFBXImporter/ParserData/DUOLFBXData.h"
#include "Serialize/BinarySerialize.h"

void LoadFBXTable()
{
	std::unique_ptr<DUOLParser::DUOLFBXParser> fbxparser = std::make_unique<DUOLParser::DUOLFBXParser>();
	std::shared_ptr<FBXModel> _fbxModel = std::make_shared<FBXModel>();
	std::unique_ptr<DUOLFBXSerialize::BinarySerialize> binaryExporter = std::make_unique<DUOLFBXSerialize::BinarySerialize>();

	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator("Asset/Mesh/UseMesh"))
	{
		std::string meshName = entry.path().filename().generic_string();
		std::string meshPath = entry.path().generic_string();

		std::cout <<  meshName << " 를 불러옵니다." << std::endl;

		std::size_t pos = meshName.find(".");
		meshName = meshName.substr(0, pos);

		_fbxModel = fbxparser->LoadFBX(meshPath, meshName);

		std::cout << meshName << " FBX를 불러왔습니다.\n"<<"바이너리화를 진행합니다." << std::endl;

		binaryExporter->SerializeDuolData(_fbxModel);

		std::cout <<"축하합니다.\n" << meshName << " 바이너리화 성공!" << std::endl;
	}
	binaryExporter->ExportJsonFile();
}

int main()
{
	LoadFBXTable();

	std::cout << "바이너리화 성공" << std::endl;

	return 0;
}
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

		std::size_t pos = meshName.find(".");
		meshName = meshName.substr(0, pos);

		_fbxModel = fbxparser->LoadFBX(meshPath, meshName);

		binaryExporter->SerializeDuolData(_fbxModel);
	}
	binaryExporter->ExportJsonFile();
}

int main()
{
	LoadFBXTable();

	std::cout << "바이너리화 성공" << std::endl;

	return 0;
}
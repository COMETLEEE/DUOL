#include <iostream>

#define NOMINMAX

#include <Windows.h>
#include <tchar.h>
#include <filesystem>
#include <thread>
#include <mutex>
#include <chrono>


#include "DUOLFBXImporter/DUOLFBXImporter.h"
#include "DUOLFBXImporter/ParserData/DUOLFBXData.h"
#include "Serialize/BinarySerialize.h"

DUOLFBXSerialize::BinarySerialize  binaryExporter;

void LoadThreadFBXTable(DUOLParser::DUOLFBXParser& fbxparser, std::vector<std::string>& meshname, std::vector<std::string>& meshpath, int startcount, int endcount)
{
	static std::mutex mutex;

	std::shared_ptr<FBXModel> _fbxModel = std::make_shared<FBXModel>();

	for (int size = startcount; size < endcount; ++size)
	{
		std::cout << meshname[size] << " 를 불러옵니다." << std::endl;

		std::size_t pos = meshname[size].find(".");
		std::string meshName = meshname[size].substr(0, pos);

		mutex.lock();

		_fbxModel = fbxparser.LoadFBX(meshpath[size], meshName);

		mutex.unlock();

		std::cout << meshName << " FBX를 불러왔습니다.\n" << "바이너리화를 진행합니다." << std::endl;

		binaryExporter.SerializeDuolData(move(_fbxModel));

		std::cout << "축하합니다.\n" << meshName << " 바이너리화 성공!" << std::endl;
		std::cout << std::endl;
	}
}

void LoadFBXTable()
{
	std::unique_ptr<DUOLParser::DUOLFBXParser> fbxparser = std::make_unique<DUOLParser::DUOLFBXParser>();
	std::shared_ptr<FBXModel> _fbxModel = std::make_shared<FBXModel>();
	std::unique_ptr<DUOLFBXSerialize::BinarySerialize> binaryExporter = std::make_unique<DUOLFBXSerialize::BinarySerialize>();

	auto path = std::filesystem::current_path();

	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator("Asset/Mesh/UseMesh"))
	{
		if(entry.path().extension().string() == ".fbm")
			continue;

		std::string meshName = entry.path().filename().generic_string();
		std::string meshPath = entry.path().generic_string();

		std::cout << meshName << " 를 불러옵니다." << std::endl;

		std::size_t pos = meshName.find(".");
		meshName = meshName.substr(0, pos);

		_fbxModel = fbxparser->LoadFBX(meshPath, meshName);

		std::cout << meshName << " FBX를 불러왔습니다.\n" << "바이너리화를 진행합니다." << std::endl;

		binaryExporter->SerializeDuolData(_fbxModel);

		std::cout << "축하합니다.\n" << meshName << " 바이너리화 성공!" << std::endl;
		std::cout << std::endl;
	}
	binaryExporter->ExportJsonFile();
}

int main()
{
	std::chrono::time_point<std::chrono::system_clock> StartTime{ std::chrono::system_clock::now() };

	LoadFBXTable();

	std::chrono::time_point<std::chrono::system_clock> EndTime{ std::chrono::system_clock::now() };
	std::chrono::duration<double> defaultSecond = EndTime - StartTime;

	std::cout << "종료 시간" << defaultSecond.count() << std::endl;

	return 0;
}
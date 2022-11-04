#pragma once
#include <fbxsdk.h>
#include <vector>
#include <memory>
#include "DUOLMath/DUOLMath.h"

#include "ParserBase/DUOLParserBase.h"

struct FBXModel;

namespace DuolData
{
	struct Mesh;
}

namespace DUOLParser
{
	class DUOLFBXParser : public DUOLParser::DUOLParserBase
	{
	public:
		DUOLFBXParser();
		~DUOLFBXParser();

	public:
		std::shared_ptr<FBXModel> LoadFBX(const std::string& path) override;
		void Initialize();
		void Destory();
		void LoadScene(std::string path);
		void ProcessNode(FbxNode* node);
		void ProcessMesh(FbxNode* node);

	private:
		std::shared_ptr<DuolData::Mesh> FindMesh(const std::string nodename);

		void GetNormal(fbxsdk::FbxMesh* mesh, std::shared_ptr<DuolData::Mesh>  meshinfo, int controlpointindex, int vertexindex);
		void GetUV(fbxsdk::FbxMesh* mesh, std::shared_ptr<DuolData::Mesh>  meshinfo, int controlpointindex, int vertexindex);

		void ProcessMaterial(FbxNode* node);
		void LoadMaterial(const fbxsdk::FbxSurfaceMaterial* surfacematerial);
		std::wstring GetTextureName(const fbxsdk::FbxSurfaceMaterial* surfacematerial, const char* materialproperty);

		bool ConvertOptimize(std::shared_ptr<DuolData::Mesh>);

	private:
		fbxsdk::FbxManager* _fbxManager = nullptr;

		fbxsdk::FbxScene* _fbxScene = nullptr;

		fbxsdk::FbxImporter* _fbxImporter = nullptr;

		std::vector<fbxsdk::FbxGeometry*> _fbxGeometryList;		// Geometry ¸ðÀ½

		std::shared_ptr<FBXModel> _fbxModel;

	};
}
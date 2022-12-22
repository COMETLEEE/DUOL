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
		std::shared_ptr<FBXModel> LoadFBX(const std::string& path);
		void Initialize();
		void Destory();
		void LoadScene(std::string path);

	private:
		// 순회를 도는 친구들
		void ProcessNode(FbxNode* node);
		void ProcessMesh(FbxNode* node);
		void ProcessBone(FbxNode* node);
		void ProcessAnimation(FbxNode* node);

		void LoadAnimation();

		// Mesh 찾는 함수
		std::shared_ptr<DuolData::Mesh> FindMesh(const std::string nodename);

		DUOLMath::Vector3 GetNormal(fbxsdk::FbxMesh* mesh, int controlpointindex, int vertexindex);
		DUOLMath::Vector2 GetUV(fbxsdk::FbxMesh* mesh, int controlpointindex, int vertexindex);
		void GetTangent(std::shared_ptr<DuolData::Mesh>  meshinfo, int meshindex);

		void LoadMesh(FbxNode* node, FbxMesh* currentmesh, std::shared_ptr<DuolData::Mesh> meshinfo, int meshindex);
		void LoadMaterial(const fbxsdk::FbxSurfaceMaterial* surfacematerial);

		void LoadSkeleton(fbxsdk::FbxNode* node, int nowindex, int parentindex);

		std::wstring GetTextureName(const fbxsdk::FbxSurfaceMaterial* surfacematerial, const char* materialproperty);
		int GetBoneIndex(std::string bonename);

		void ConvertOptimize(fbxsdk::FbxMesh* currentMesh, std::shared_ptr<DuolData::Mesh> mesh, int meshindex);

		DUOLMath::Vector4 ConvertVector4(fbxsdk::FbxVector4 v4);
		DUOLMath::Matrix ConvertMatrix(fbxsdk::FbxMatrix matrix);

		fbxsdk::FbxAMatrix GetGeometryTransformation(fbxsdk::FbxNode* node);

		//void DecomposeMatrix(DUOLMath::Matrix nodet);

	private:
		fbxsdk::FbxManager* _fbxManager = nullptr;

		fbxsdk::FbxScene* _fbxScene = nullptr;

		fbxsdk::FbxImporter* _fbxImporter = nullptr;

		std::vector<fbxsdk::FbxGeometry*> _fbxGeometryList;		// Geometry 모음

		std::shared_ptr<FBXModel> _fbxModel;
	};
}

inline DUOLMath::Vector4 DUOLParser::DUOLFBXParser::ConvertVector4(fbxsdk::FbxVector4 v4)
{
	// xyzw -> xzyw
	return 	DUOLMath::Vector4
	(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[2]),
		static_cast<float>(v4.mData[1]),
		static_cast<float>(v4.mData[3])
	);
}

inline DUOLMath::Matrix DUOLParser::DUOLFBXParser::ConvertMatrix(fbxsdk::FbxMatrix matrix)
{
	FbxVector4 r1 = matrix.GetRow(0);
	FbxVector4 r2 = matrix.GetRow(1);
	FbxVector4 r3 = matrix.GetRow(2);
	FbxVector4 r4 = matrix.GetRow(3);

	return DUOLMath::Matrix
	(
		ConvertVector4(r1),
		ConvertVector4(r3),
		ConvertVector4(r2),
		ConvertVector4(r4)
	);
}
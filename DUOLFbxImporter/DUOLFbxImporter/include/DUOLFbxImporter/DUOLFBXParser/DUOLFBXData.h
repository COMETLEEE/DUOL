#pragma once
#include <memory>
#include <string>
#include <vector>
#include "DUOLMath/DUOLMath.h"

namespace DuolData
{
	struct Face;

	struct Vertex
	{
		Vertex(DUOLMath::Vector3 paPos = { 0.f, 0.f, 0.f },
			DUOLMath::Vector2 paUV = { 0.f, 0.f },
			DUOLMath::Vector3 paNormal = { 0.f, 0.f, 0.f },
			DUOLMath::Vector3 paTangent = { 0.f, 0.f, 0.f })
			: position(paPos), normal(paNormal), uv(paUV), tangent(paTangent)
		{}

		DUOLMath::Vector3		position;			// 위치값
		DUOLMath::Vector2		uv;				// 텍스쳐 좌표
		DUOLMath::Vector3		normal;			// 노말값
		DUOLMath::Vector3		tangent;		// 탄젠트
		DUOLMath::Vector3		binormal;		// 바이 노말
	};

	struct Material
	{
		std::string materialName;

		bool isAlbedo = false;
		bool isNormal = false;
		bool isMetallic = false;
		bool isRoughness = false;
		bool isAO = false;
		bool isEmissive = false;

		std::wstring albedoMap;
		std::wstring normalMap;
		std::wstring metallicMap;
		std::wstring roughnessMap;
		std::wstring AOMap;
		std::wstring emissiveMap;

		std::wstring cubeMap;

		float metallic;
		float roughness;
	};

	// 한개의 메시를 저장하기 위한 클래스
	struct Mesh
	{
		int				meshIndex;

		std::string		nodeName;		// 노드 이름
		std::string		parentName;		// 부모 이름(부모 이름있으면 부모 O)
		bool			isparent;		// 부모가 있는지 확실하게 체크

		std::vector<unsigned int>				indices;

		std::shared_ptr<Mesh>					parentMesh;		// 부모가 있으면 넣어준다
		std::vector<std::shared_ptr<Mesh>>		childList;		// 자식들을 넣어준다

		std::vector<std::shared_ptr<Face>>		MeshFace;		// 이 Mesh를 이루는 Face의 정보

		std::vector<Vertex>						vertexList;		// 이 Mesh의 Vertex 정보

		std::string								materialName;	// 이 Mesh의 material 정보
	};

	// 하나의 Face에 관한 정보
	struct Face
	{
		int										vertexIndex[3];		// Face를 이루는 버텍스의 인덱스
		DUOLMath::Vector3			normal;				// Face의 노말값
		DUOLMath::Vector3			vertexNormal[3];	// vertex의 노말값
		DUOLMath::Vector2			vertexUV[3];		// vertex의 UV값
		int										TFace[3];			// Texture Coordinate		
	};
}

struct FBXModel
{
	std::string fileName;

	std::vector<std::shared_ptr<DuolData::Mesh>> fbxMeshList;

	std::vector<std::shared_ptr<DuolData::Material>> fbxmaterialList;

	bool isSkinnedAnimation;		// 스키닝 애니메이션 존재 여부
};
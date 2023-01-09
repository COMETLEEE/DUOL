#pragma once
#include <memory>
#include <string>
#include <vector>
#include "DUOLMath/DUOLMath.h"

namespace DuolData
{
	struct Animation;
	struct Face;

	struct Vertex
	{
		Vertex(DUOLMath::Vector3 paPos = { 0.f, 0.f, 0.f },
			DUOLMath::Vector2 paUV = { 0.f, 0.f },
			DUOLMath::Vector3 paNormal = { 0.f, 0.f, 0.f },
			DUOLMath::Vector3 paTangent = { 0.f, 0.f, 0.f })
			: position(paPos), normal(paNormal), uv(paUV), tangent(paTangent)
		{
			for (int i = 0; i < 8; i++)
			{
				boneWeight[i] = 0.f;
				boneIndices[i] = -1;
			}
		}

		DUOLMath::Vector3		position;			// 위치값
		DUOLMath::Vector2		uv;				// 텍스쳐 좌표
		DUOLMath::Vector3		normal;			// 노말값
		DUOLMath::Vector3		tangent;		// 탄젠트

		// Skinning
		int boneIndices[8];
		float boneWeight[8];
	};

	struct Material
	{
		std::string materialName;

		bool isAlbedo;
		bool isNormal;
		bool isMetallic;
		bool isRoughness;
		bool isAO;
		bool isEmissive;

		std::wstring albedoMap;
		std::wstring normalMap;
		std::wstring metallicMap;
		std::wstring roughnessMap;
		std::wstring AOMap;
		std::wstring emissiveMap;

		DUOLMath::Vector4 material_Ambient = { 0.f, 0.f, 0.f, 0.f };
		DUOLMath::Vector4 material_Diffuse = { 0.f, 0.f, 0.f, 0.f };
		DUOLMath::Vector4 material_Specular = { 0.f, 0.f, 0.f, 0.f };
		DUOLMath::Vector4 material_Emissive = { 0.f, 0.f, 0.f, 0.f };

		std::wstring cubeMap;

		float material_Transparency = 0.f;	// 투명도
		float material_Reflectivity = 0.f;	// 반사율

		float metallic;
		float roughness;
	};

	// 한개의 메시를 저장하기 위한 클래스
	struct Mesh
	{
		std::string		nodeName;		// 노드 이름
		std::string		parentName;		// 부모 이름(부모 이름있으면 부모 O)
		bool			isparent;		// 부모가 있는지 확실하게 체크
		bool			isSkinned = false;		// 스키닝 메쉬인지

		std::vector<std::vector<unsigned int>>	indices;		// 만약 Mesh가 쪼개져 있으면 index를 쪼개준다. 

		DUOLMath::Matrix						nodeTM;

		std::vector<Vertex>						vertexList;		// 이 Mesh의 Vertex 정보
		std::vector<Vertex>						tempVertexList;	// 이 Mesh의 Vertex 정보

		std::shared_ptr<Mesh>					parentMesh;		// 부모가 있으면 넣어준다
		std::vector<std::shared_ptr<Mesh>>		childList;		// 자식들을 넣어준다

		std::vector<std::string>				materialName;	// 이 Mesh의 material 정보
		std::vector<unsigned int>				materialIndex;
	};

	struct Bone
	{
		std::string			boneName;
		int					parentIndex;

		DUOLMath::Matrix offsetMatrix = DirectX::XMMatrixIdentity();

		// 좌우반전때매 회전을 시키기위해서 넣어준 행렬
		DUOLMath::Matrix nodeMatrix = DirectX::XMMatrixIdentity();
	};

	struct KeyFrame
	{
		float time;

		DUOLMath::Vector3	localTransform = { 0.f,0.f,0.f };
		DUOLMath::Quaternion localRotation = { 0.f,0.f,0.f ,0.0f };
		DUOLMath::Vector3	localScale = { 0.f,0.f,0.f };
	};

	struct AnimationClip
	{
		std::string animationName;

		float frameRate;
		float tickPerFrame;
		int totalKeyFrame;
		int startKeyFrame;
		int endKeyFrame;

		std::vector<std::vector<std::shared_ptr<KeyFrame>>> keyframeList;
	};
}

struct FBXModel
{
	std::string fileName;

	std::string modelName;

	std::vector<std::shared_ptr<DuolData::Mesh>> fbxMeshList;

	std::vector<std::shared_ptr<DuolData::Material>> fbxmaterialList;

	std::vector<std::shared_ptr<DuolData::Bone>> fbxBoneList;

	std::vector<std::shared_ptr<DuolData::AnimationClip>> animationClipList;

	bool isSkinnedAnimation = false;		// 스키닝 애니메이션 존재 여부
};
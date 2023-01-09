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

		DUOLMath::Vector3		position;			// ��ġ��
		DUOLMath::Vector2		uv;				// �ؽ��� ��ǥ
		DUOLMath::Vector3		normal;			// �븻��
		DUOLMath::Vector3		tangent;		// ź��Ʈ

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

		float material_Transparency = 0.f;	// ����
		float material_Reflectivity = 0.f;	// �ݻ���

		float metallic;
		float roughness;
	};

	// �Ѱ��� �޽ø� �����ϱ� ���� Ŭ����
	struct Mesh
	{
		std::string		nodeName;		// ��� �̸�
		std::string		parentName;		// �θ� �̸�(�θ� �̸������� �θ� O)
		bool			isparent;		// �θ� �ִ��� Ȯ���ϰ� üũ
		bool			isSkinned = false;		// ��Ű�� �޽�����

		std::vector<std::vector<unsigned int>>	indices;		// ���� Mesh�� �ɰ��� ������ index�� �ɰ��ش�. 

		DUOLMath::Matrix						nodeTM;

		std::vector<Vertex>						vertexList;		// �� Mesh�� Vertex ����
		std::vector<Vertex>						tempVertexList;	// �� Mesh�� Vertex ����

		std::shared_ptr<Mesh>					parentMesh;		// �θ� ������ �־��ش�
		std::vector<std::shared_ptr<Mesh>>		childList;		// �ڽĵ��� �־��ش�

		std::vector<std::string>				materialName;	// �� Mesh�� material ����
		std::vector<unsigned int>				materialIndex;
	};

	struct Bone
	{
		std::string			boneName;
		int					parentIndex;

		DUOLMath::Matrix offsetMatrix = DirectX::XMMatrixIdentity();

		// �¿�������� ȸ���� ��Ű�����ؼ� �־��� ���
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

	bool isSkinnedAnimation = false;		// ��Ű�� �ִϸ��̼� ���� ����
};
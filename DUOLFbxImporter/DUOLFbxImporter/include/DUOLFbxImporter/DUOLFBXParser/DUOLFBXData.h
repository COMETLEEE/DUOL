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

		DUOLMath::Vector3		position;			// ��ġ��
		DUOLMath::Vector2		uv;				// �ؽ��� ��ǥ
		DUOLMath::Vector3		normal;			// �븻��
		DUOLMath::Vector3		tangent;		// ź��Ʈ
		DUOLMath::Vector3		binormal;		// ���� �븻
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

	// �Ѱ��� �޽ø� �����ϱ� ���� Ŭ����
	struct Mesh
	{
		int				meshIndex;

		std::string		nodeName;		// ��� �̸�
		std::string		parentName;		// �θ� �̸�(�θ� �̸������� �θ� O)
		bool			isparent;		// �θ� �ִ��� Ȯ���ϰ� üũ

		std::vector<unsigned int>				indices;

		std::shared_ptr<Mesh>					parentMesh;		// �θ� ������ �־��ش�
		std::vector<std::shared_ptr<Mesh>>		childList;		// �ڽĵ��� �־��ش�

		std::vector<std::shared_ptr<Face>>		MeshFace;		// �� Mesh�� �̷�� Face�� ����

		std::vector<Vertex>						vertexList;		// �� Mesh�� Vertex ����

		std::string								materialName;	// �� Mesh�� material ����
	};

	// �ϳ��� Face�� ���� ����
	struct Face
	{
		int										vertexIndex[3];		// Face�� �̷�� ���ؽ��� �ε���
		DUOLMath::Vector3			normal;				// Face�� �븻��
		DUOLMath::Vector3			vertexNormal[3];	// vertex�� �븻��
		DUOLMath::Vector2			vertexUV[3];		// vertex�� UV��
		int										TFace[3];			// Texture Coordinate		
	};
}

struct FBXModel
{
	std::string fileName;

	std::vector<std::shared_ptr<DuolData::Mesh>> fbxMeshList;

	std::vector<std::shared_ptr<DuolData::Material>> fbxmaterialList;

	bool isSkinnedAnimation;		// ��Ű�� �ִϸ��̼� ���� ����
};
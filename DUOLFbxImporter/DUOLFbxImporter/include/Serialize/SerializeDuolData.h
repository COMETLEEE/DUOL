#pragma once
#include "DUOLMath/DUOLMath.h"

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

namespace SerializeData
{
	class Vertex
	{
	public:
		Vertex(DUOLMath::Vector3 paPos, DUOLMath::Vector2 paUV,
			DUOLMath::Vector3 paNormal, DUOLMath::Vector3 paTangent,
			int boneindices[8], float boneweight[8])
			: position(paPos), normal(paNormal), uv(paUV), tangent(paTangent)
		{
			for (int i = 0; i < 8; ++i)
			{
				boneIndices[i] = boneindices[i];
				boneWeight[i] = boneweight[i];
			}
		};
		Vertex() = default;
		~Vertex() {};

	protected:
		// Ŭ������ �� �Լ��� ������ �� �ֵ��� �ش� Ŭ������ friend�� ����
		friend class boost::serialization::access;

		DUOLMath::Vector3		position;		// ��ġ��
		DUOLMath::Vector2		uv;				// �ؽ��� ��ǥ
		DUOLMath::Vector3		normal;			// �븻��
		DUOLMath::Vector3		tangent;		// ź��Ʈ

		// Skinning
		int boneIndices[8];
		float boneWeight[8];

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			// ������ �����Ǹ� �ѵ�
			ar& position;
			ar& uv;
			ar& normal;
			ar& tangent;

			ar& boneIndices;
			ar& boneWeight;
		}

	};

	class Mesh
	{
	public:
		Mesh(std::string nodename, std::string parentname, bool isparent, bool isskinned, std::vector<std::vector<unsigned int>>	indices,
			DUOLMath::Matrix nodetm, std::vector<Vertex> vertexlist, std::vector<std::string> materialname, std::vector<unsigned int> materialindex)
			:nodeName(nodename), parentName(parentname), isParent(isparent), isSkinned(isskinned),
			indices(indices), nodeTM(nodetm), vertexList(vertexlist), materialName(materialname), materialIndex(materialIndex) {};
		Mesh() = default;
		~Mesh() {};

	protected:
		friend class boost::serialization::access;

		//int				meshIndex = 0;		// Mesh Index

		std::string		nodeName;		// ��� �̸�
		std::string		parentName;		// �θ� �̸�(�θ� �̸������� �θ� O)
		bool			isParent;		// �θ� �ִ��� Ȯ���ϰ� üũ
		bool			isSkinned = false;		// ��Ű�� �޽�����

		std::vector<std::vector<unsigned int>>	indices;		// ���� Mesh�� �ɰ��� ������ index�� �ɰ��ش�. 

		DUOLMath::Matrix						nodeTM;

		std::vector<Vertex>						vertexList;		// �� Mesh�� Vertex ����

		std::vector<Mesh>						childList;		// �ڽĵ��� �־��ش�

		std::vector<std::string>				materialName;	// �� Mesh�� material ����
		std::vector<unsigned int>				materialIndex;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			//ar& meshIndex;

			ar& nodeName;
			ar& parentName;
			ar& isParent;
			ar& isSkinned;

			ar& indices;
			ar& nodeTM;

			ar& vertexList;

			ar& childList;

			ar& materialName;
			ar& materialIndex;
		}
	};

	class Model
	{
	public:
		Model() = default;
		Model(std::vector<SerializeData::Mesh> modelmeshs)
			:modelMeshs(modelmeshs)
		{};
		~Model() {};

	protected:
		friend class boost::serialization::access;

		std::vector<SerializeData::Mesh>	modelMeshs;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& modelMeshs;
		}
	};

	class Material
	{
	public:
		Material(std::string materialname, bool isalbedo, bool isnormal, bool ismetallic, bool isroughness,
			std::wstring albedomap, std::wstring normalmap, std::wstring metallicmap,
			DUOLMath::Vector4 diffuse, DUOLMath::Vector4 emissve,
			float metallic, float roughness)
			:materialName(std::move(materialname)), isAlbedo(isalbedo), isNormal(isnormal), isMetallic(ismetallic), isRoughness(isroughness),
			albedoMap(albedomap), normalMap(normalmap), metallicMap(metallicmap), material_Diffuse(diffuse), material_Emissive(emissve),
			metallic(metallic), roughness(roughness)
		{};
		Material() = default;

	protected:
		friend class boost::serialization::access;

		//int materialID=0;

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

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
		//	ar& materialID;

			// ������ �����Ǹ� �ѵ�
			ar& materialName;

			ar& isAlbedo;
			ar& isNormal;
			ar& isMetallic;
			ar& isRoughness;
			//ar& isAO;
			//ar& isEmissive;

			ar& albedoMap;
			ar& normalMap;
			ar& metallicMap;
			//ar& roughnessMap;
			//ar& AOMap;
			//ar& emissiveMap;

			//ar& material_Ambient;
			ar& material_Diffuse;
			//ar& material_Specular;
			ar& material_Emissive;

			//ar& cubeMap;

			//ar& material_Transparency;
			//ar& material_Reflectivity;

			ar& metallic;
			ar& roughness;
		}
	};
}
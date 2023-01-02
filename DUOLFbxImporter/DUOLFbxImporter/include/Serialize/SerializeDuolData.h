#pragma once
#include "DUOLMath/DUOLMath.h"

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>

class Vertex
{
public:
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
protected:
	// 클래스가 이 함수에 접근할 수 있도록 해당 클래스를 friend로 선언
	friend class boost::serialization::access;

	DUOLMath::Vector3		position;			// 위치값
	DUOLMath::Vector2		uv;				// 텍스쳐 좌표
	DUOLMath::Vector3		normal;			// 노말값
	DUOLMath::Vector3		tangent;		// 탄젠트

	// Skinning
	int boneIndices[8];
	float boneWeight[8];

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		// 연산자 재정의를 한듯
		ar& position;
		ar& uv;
		ar& normal;
		ar& tangent;
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
		albedoMap(albedomap), normalMap(normalmap), metallicMap(metallicmap),material_Diffuse(diffuse), material_Emissive(emissve),
		metallic(metallic), roughness(roughness)
	{};
	Material() = default;

protected:
	friend class boost::serialization::access;

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

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		// 연산자 재정의를 한듯
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
#include "DUOLFBXParser/DUOLFBXData.h"
#include "../Serialize/BinarySerialize.h"
#include "../Serialize/SerializeDuolData.h"

#include <fstream>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/archive/binary_oarchive.hpp>

void BinarySerialize::SerializeDuolData(std::shared_ptr<FBXModel> fbxmodel)
{
	int materialcount = fbxmodel->fbxmaterialList.size();

	// file을 material 만큼 만들어준다. (중복체크는 나중에 생각하기)
	for (int count = 0; count < materialcount; ++count)
	{
		// 테스트용
		std::shared_ptr<DuolData::Material> fbxMaterial = fbxmodel->fbxmaterialList[count];

		std::string name = fbxMaterial->materialName;
		bool isAlbedo = fbxMaterial->isAlbedo;
		bool isNormal = fbxMaterial->isNormal;
		bool isMetallic = fbxMaterial->isMetallic;
		bool isRoughness = fbxMaterial->isRoughness;

		std::wstring albedoMap = fbxMaterial->albedoMap;
		std::wstring normalMap = fbxMaterial->normalMap;
		std::wstring metallicMap = fbxMaterial->metallicMap;

		DUOLMath::Vector4 material_Diffuse = fbxMaterial->material_Diffuse;
		DUOLMath::Vector4 material_Emissive = fbxMaterial->material_Emissive;

		float metallic = fbxMaterial->metallic;
		float roughness = fbxMaterial->roughness;

		// 여기서만 쓸꺼니깐 shared_ptr로 만들이유가 없어서 unique로 만들었다.
		Material material(name, isAlbedo, isNormal, isMetallic, isRoughness, albedoMap, normalMap,
			metallicMap, material_Diffuse, material_Emissive, metallic, roughness);

		std::string path = "Asset/Materials/Material[" + std::to_string(count);
		std::ofstream fw(path + "].DUOL", std::ios_base::binary);
		//std::string path = "Asset/Materials/" + name;
		//std::ofstream fw(path + ".DUOL", std::ios_base::binary);
		boost::archive::binary_oarchive outArchive(fw);

		outArchive << material;
	}
}

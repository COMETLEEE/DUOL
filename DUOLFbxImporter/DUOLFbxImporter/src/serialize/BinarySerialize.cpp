#include "DUOLFBXParser/DUOLFBXData.h"
#include "../Serialize/BinarySerialize.h"
#include "../Serialize/SerializeDuolData.h"

#include <fstream>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

void BinarySerialize::SerializeDuolData(std::shared_ptr<FBXModel> fbxmodel)
{
#pragma region Mesh

	int meshcount = fbxmodel->fbxMeshList.size();

	std::vector<SerializeData::Mesh> modelmeshs;

	for(int count =0; count<meshcount; ++count)
	{
		SerializeData::Mesh mesh;

		SetMeshData(fbxmodel->fbxMeshList[count], mesh);

		modelmeshs.emplace_back(mesh);
	}

	SerializeData::Model model(modelmeshs);

	std::string path = "Asset/BinaryData/Mesh/name";
		std::ofstream fw(path + ".DUOL", std::ios_base::binary);
	boost::archive::text_oarchive outArchive(fw);

	outArchive << model;

#pragma endregion

#pragma region Material
	int materialcount = fbxmodel->fbxmaterialList.size();

	// file을 material 만큼 만들어준다. (중복체크는 나중에 생각하기)
	for (int count = 0; count < materialcount; ++count)
	{
		MaterialSerialize(fbxmodel->fbxmaterialList[count],count);
	}

#pragma endregion

#pragma region Animation

#pragma endregion 
}

void BinarySerialize::SetMeshData(std::shared_ptr<DuolData::Mesh> fbxmesh, SerializeData::Mesh& mesh)
{
	std::string name = fbxmesh->nodeName;
	std::string parentname = fbxmesh->parentName;

	bool isparent = fbxmesh->isparent;
	bool isskinned = fbxmesh->isSkinned;

	std::vector<std::vector<unsigned int>> indices = fbxmesh->indices;

	DUOLMath::Matrix nodetm = fbxmesh->nodeTM;

	std::vector<std::string> materialname = fbxmesh->materialName;
	std::vector<unsigned int> materialindex = fbxmesh->materialIndex;

	std::vector<SerializeData::Vertex> vertex;

	for (auto duolvertex : fbxmesh->vertexList)
	{
		int boneindice[8];
		float boneweight[8];

		for(int i=0; i<8; i++)
		{
			boneindice[i] = duolvertex.boneIndices[i];
			boneweight[i] = duolvertex.boneWeight[i];
		}

		SerializeData::Vertex vertexinfo(duolvertex.position, duolvertex.uv, duolvertex.normal, duolvertex.tangent, boneindice, boneweight);

		vertex.emplace_back(vertexinfo);
	}

	SerializeData::Mesh meshinfo(name,parentname,isparent, isskinned,indices, nodetm,vertex,materialname,materialindex);

	mesh = meshinfo;
}

void BinarySerialize::MaterialSerialize(std::shared_ptr<DuolData::Material> fbxmaterial,int count)
{
	// 테스트용
	std::shared_ptr<DuolData::Material> fbxMaterial = fbxmaterial;

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

	SerializeData::Material material(name, isAlbedo, isNormal, isMetallic, isRoughness, albedoMap, normalMap,
		metallicMap, material_Diffuse, material_Emissive, metallic, roughness);

	std::string path = "Asset/BinaryData/Materials/Material[" + std::to_string(count);
	std::ofstream fw(path + "].DUOL", std::ios_base::binary);
	//std::string path = "Asset/Materials/" + name;
	//std::ofstream fw(path + ".DUOL", std::ios_base::binary);
	boost::archive::binary_oarchive outArchive(fw);

	outArchive << material;
}
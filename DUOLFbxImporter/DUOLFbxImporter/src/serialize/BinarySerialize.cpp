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

	for (auto meshinfo : fbxmodel->fbxMeshList)
	{
		SerializeData::Mesh mesh;

		SetMeshData(meshinfo, mesh);

		modelmeshs.emplace_back(mesh);
	}

	int bonecount = fbxmodel->fbxBoneList.size();

	std::vector<SerializeData::Bone> modelbone;

	for (auto boneinfo : fbxmodel->fbxBoneList)
	{
		SerializeData::Bone bone;

		SetBoneData(boneinfo, bone);

		modelbone.emplace_back(bone);
	}

	SerializeData::Model model(modelmeshs, modelbone,fbxmodel->isSkinnedAnimation);

	std::string path = "Asset/BinaryData/Mesh/Joy";
	std::ofstream fw(path + ".DUOL", std::ios_base::binary);
	boost::archive::binary_oarchive outArchive(fw);

	outArchive << model;

#pragma endregion

#pragma region Material
	int materialcount = fbxmodel->fbxmaterialList.size();

	// file을 material 만큼 만들어준다. (중복체크는 나중에 생각하기)
	for (int count = 0; count < materialcount; ++count)
	{
		MaterialSerialize(fbxmodel->fbxmaterialList[count], count);
	}

#pragma endregion

#pragma region Animation
	int animationcount = fbxmodel->animationClipList.size();

	for (auto animationinfo : fbxmodel->animationClipList)
	{
		SetAnimationData(animationinfo);
	}
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

		for (int i = 0; i < 8; i++)
		{
			boneindice[i] = duolvertex.boneIndices[i];
			boneweight[i] = duolvertex.boneWeight[i];
		}

		SerializeData::Vertex vertexinfo(duolvertex.position, duolvertex.uv, duolvertex.normal, duolvertex.tangent, boneindice, boneweight);

		vertex.emplace_back(vertexinfo);
	}

	SerializeData::Mesh meshinfo(name, parentname, isparent, isskinned, indices, nodetm, vertex, materialname, materialindex);

	mesh = meshinfo;
}

void BinarySerialize::SetBoneData(std::shared_ptr<DuolData::Bone> fbxbone, SerializeData::Bone& bone)
{
	std::string name = fbxbone->boneName;
	int parentindex = fbxbone->parentIndex;
	DUOLMath::Matrix offsetmatrix = fbxbone->offsetMatrix;
	DUOLMath::Matrix nodetm= fbxbone->nodeMatrix;

	SerializeData::Bone boneinfo(name, parentindex, offsetmatrix, nodetm);

	bone = boneinfo;
}

void BinarySerialize::MaterialSerialize(std::shared_ptr<DuolData::Material> fbxmaterial, int count)
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

void BinarySerialize::SetAnimationData(std::shared_ptr<DuolData::AnimationClip> fbxanimationclip)
{
	std::string name = fbxanimationclip->animationName;

	float framerate = fbxanimationclip->frameRate;
	float tickperframe = fbxanimationclip->tickPerFrame;
	int totalkeyframe = fbxanimationclip->totalKeyFrame;
	int startkeyframe = fbxanimationclip->startKeyFrame;
	int endkeyframe = fbxanimationclip->endKeyFrame;

	std::vector<std::vector<SerializeData::KeyFrame>> keyframes;

	for (auto fbxkeyframelist : fbxanimationclip->keyframeList)
	{
		std::vector<SerializeData::KeyFrame> keyframelist;

		for (auto fbxkeyframe : fbxkeyframelist)
		{
			float time = fbxkeyframe->time;
			DUOLMath::Vector3 localtm = fbxkeyframe->localTransform;
			DUOLMath::Quaternion localrotation = fbxkeyframe->localRotation;
			DUOLMath::Vector3 localscale = fbxkeyframe->localScale;

			SerializeData::KeyFrame key(time, localtm, localrotation, localscale);
			keyframelist.emplace_back(key);
		}
		keyframes.emplace_back(keyframelist);
	}

	SerializeData::AnimationClip animationclip(name, framerate, tickperframe, totalkeyframe, startkeyframe, endkeyframe, keyframes);

	std::size_t pos = name.find('.');

	std::string path = "Asset/BinaryData/Animation/" + name.substr(0,pos);
	std::ofstream fw(path + ".DUOL", std::ios_base::binary);
	boost::archive::binary_oarchive outArchive(fw);

	outArchive << animationclip;
}

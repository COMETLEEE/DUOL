#include "DUOLFBXImporter/ParserData/DUOLFBXData.h"
#include "Serialize/BinarySerialize.h"

#define RAPIDJSON_HAS_STDSTRING 1

#include "Serialize/SerializeDuolData.h"

#include <fstream>
#include <boost/archive/binary_oarchive.hpp>

#include "DUOLCommon/Util/Hash.h"
#include "DUOLJson/document.h"
#include "DUOLJson/fwd.h"
#include "DUOLJson/writer.h"
#include "DUOLJson/stringbuffer.h"
#include "DUOLJson/filereadstream.h"
#include <DUOLJson\istreamwrapper.h>
#include "DUOLCommon/StringHelper.h"

#include "DUOLJson/JsonReader.h"


void DUOLFBXSerialize::BinarySerialize::SerializeDuolData(std::shared_ptr<FBXModel> fbxmodel)
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

	SerializeData::Model model(modelmeshs, modelbone, fbxmodel->isSkinnedAnimation);

	std::string path = "Asset/BinaryData/Mesh/" + fbxmodel->modelName;
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
		SetAnimationData(animationinfo, fbxmodel->modelName);
	}
#pragma endregion
}

void DUOLFBXSerialize::BinarySerialize::SetMeshData(std::shared_ptr<DuolData::Mesh> fbxmesh, SerializeData::Mesh& mesh)
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

void DUOLFBXSerialize::BinarySerialize::SetBoneData(std::shared_ptr<DuolData::Bone> fbxbone, SerializeData::Bone& bone)
{
	std::string name = fbxbone->boneName;
	int parentindex = fbxbone->parentIndex;
	DUOLMath::Matrix offsetmatrix = fbxbone->offsetMatrix;
	DUOLMath::Matrix nodetm = fbxbone->nodeMatrix;

	SerializeData::Bone boneinfo(name, parentindex, offsetmatrix, nodetm);

	bone = boneinfo;
}

void DUOLFBXSerialize::BinarySerialize::MaterialSerialize(std::shared_ptr<DuolData::Material> fbxmaterial, int count)
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

	DUOLMath::Vector3 emissive = DUOLMath::Vector3(material_Emissive.x, material_Emissive.y, material_Emissive.z);

	float metallic = fbxMaterial->metallic;
	float roughness = fbxMaterial->roughness;
	float specular = fbxMaterial->specular;

	SerializeData::Material material(name, isAlbedo, isNormal, isMetallic, isRoughness, albedoMap, normalMap,
		metallicMap, material_Diffuse, emissive, metallic, roughness, specular);

	std::string path = "Asset/BinaryData/Materials/" + name;
	std::ofstream fw(path + ".DUOL", std::ios_base::binary);
	boost::archive::binary_oarchive outArchive(fw);

	std::string objectID = name;

	auto keyValue = DUOLCommon::Hash::Hash64(DUOLCommon::StringHelper::ToTString(name));

	materialMap.insert(std::make_pair(keyValue, objectID));

	outArchive << material;
}

void DUOLFBXSerialize::BinarySerialize::SetAnimationData(std::shared_ptr<DuolData::AnimationClip> fbxanimationclip, std::string modelname)
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

	SetAnimationName(name);

	std::string path = "Asset/BinaryData/Animation/" + modelname;
	path += "_" + name;

	std::ofstream fw(path + ".DUOL", std::ios_base::binary);
	boost::archive::binary_oarchive outArchive(fw);

	std::string objectID = modelname + "_" + name;

	auto keyValue = DUOLCommon::Hash::Hash64(DUOLCommon::StringHelper::ToTString(objectID));

	// 동작이 끝날때까지 모든 데이터의 키값과 이름을 테이블에 넣어놓는다. 
	animationMap.insert(std::make_pair(keyValue, objectID));

	outArchive << animationclip;
}

void DUOLFBXSerialize::BinarySerialize::SetAnimationName(std::string& animationname)
{
	bool isCheck = true;
	std::size_t pos;
	std::string name = animationname;
	int checkNumber = 0;

	while (isCheck)
	{
		pos = name.find("|");
		if (name.size() <= pos || pos == -1)
		{
			isCheck = false;
			continue;
		}
		checkNumber = pos;
		name = name.substr(pos + 1, name.size());
		pos = -1;
	}

	pos = name.find('.');
	name = name.substr(0, pos);

	animationname = name;
}

void DUOLFBXSerialize::BinarySerialize::SetJsonFile(const DUOLCommon::tstring path, std::string filename, std::map<uint64, std::string>& datamap)
{
	auto jsonReader = DUOLJson::JsonReader::GetInstance();

	using namespace rapidjson;

	std::ifstream file(path);
	IStreamWrapper isw{ file };

	Document document;
	Document::AllocatorType& allocator = document.GetAllocator();

	document.ParseStream(isw);

	rapidjson::Value datas(rapidjson::kArrayType);

	for (int count = 0; count < datamap.size(); count++)
	{
		// 비어있는 친구는 안넣어줌
		if(datamap[count]=="")
			continue;

		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember("ID", count, allocator);
		object.AddMember("Name", datamap[count], allocator);
		datas.PushBack(object, allocator);
	}

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	datas.Accept(writer);

	document[filename].PushBack(datas, allocator);

	jsonReader->WriteJson(path, document);

}

void DUOLFBXSerialize::BinarySerialize::PerfabJsonFile(const DUOLCommon::tstring path, std::string filename)
{
	auto jsonReader = DUOLJson::JsonReader::GetInstance();

	using namespace rapidjson;

	std::ifstream file(path);
	IStreamWrapper isw{ file };

	Document document;
	Document::AllocatorType& allocator = document.GetAllocator();

	document.ParseStream(isw);

	rapidjson::Value datas(rapidjson::kArrayType);

	//for (int count = 0; count < datamap.size(); count++)
	//{
	//	// 비어있는 친구는 안넣어줌
	//	if (datamap[count] == "")
	//		continue;

	//	rapidjson::Value object(rapidjson::kObjectType);
	//	object.AddMember("MeshID", count, allocator);
	//	object.AddMember("MaterialID", datamap[count], allocator);
	//	object.AddMember("AnimationID", datamap[count], allocator);
	//	datas.PushBack(object, allocator);
	//}

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	datas.Accept(writer);

	document[filename].PushBack(datas, allocator);

	jsonReader->WriteJson(path, document);
}

/**
 * \brief 전부 저장되어 있는 데이터를 Json으로 뺀다.
 */
void DUOLFBXSerialize::BinarySerialize::ExportJsonFile()
{
	SetJsonFile(L"Asset/DataTable/Material.json", "Material", materialMap);
	SetJsonFile(L"Asset/DataTable/Animation.json", "Animation", animationMap);
	PerfabJsonFile(L"Asset/DataTable/Perfab.json", "Perfab");
}

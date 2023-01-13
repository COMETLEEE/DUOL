#define RAPIDJSON_HAS_STDSTRING 1

#include "DUOLFBXImporter/ParserData/DUOLFBXData.h"
#include "Serialize/BinarySerialize.h"

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
	materialKey.clear();
	animationKey.clear();

	jsonReader = DUOLJson::JsonReader::GetInstance();

#pragma region Mesh

	int meshcount = fbxmodel->fbxMeshList.size();

	std::vector<SerializeData::Mesh> modelmeshs;

	// model KeyVale
	auto keyValue = DUOLCommon::Hash::Hash64(DUOLCommon::StringHelper::ToTString(fbxmodel->modelName));

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

	SerializeData::Model model(keyValue, modelmeshs, modelbone, fbxmodel->isSkinnedAnimation);

	std::string path = "Asset/BinaryData/Mesh/" + fbxmodel->modelName;
	std::ofstream fw(path + ".DUOL", std::ios_base::binary);
	boost::archive::binary_oarchive outArchive(fw);

	outArchive << model;

#pragma endregion

#pragma region Material
	int materialcount = fbxmodel->fbxmaterialList.size();

	// file�� material ��ŭ ������ش�. (�ߺ�üũ�� ���߿� �����ϱ�)
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

	// model key material key animation key ������ �ʿ��ϴ�.
	std::pair<std::vector<uint64>, std::vector<uint64>> keyValueData;
	keyValueData = std::make_pair(materialKey, animationKey);
	modelPrefab.emplace_back(std::make_pair(keyValue, keyValueData));
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
	// �׽�Ʈ��
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

	auto keyValue = DUOLCommon::Hash::Hash64(DUOLCommon::StringHelper::ToTString(name));

	SerializeData::Material material(keyValue, name, isAlbedo, isNormal, isMetallic, isRoughness, albedoMap, normalMap,
		metallicMap, material_Diffuse, emissive, metallic, roughness, specular);

	std::string path = "Asset/BinaryData/Materials/" + name;
	std::ofstream fw(path + ".DUOL", std::ios_base::binary);
	boost::archive::binary_oarchive outArchive(fw);

	std::string objectID = name;

	outArchive << material;

	materialList.emplace_back(std::make_pair(keyValue, objectID));

	materialKey.emplace_back(keyValue);
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

	SetAnimationName(name);

	std::string objectID = modelname + "_" + name;

	auto keyValue = DUOLCommon::Hash::Hash64(DUOLCommon::StringHelper::ToTString(objectID));

	SerializeData::AnimationClip animationclip(keyValue, name, framerate, tickperframe, totalkeyframe, startkeyframe, endkeyframe, keyframes);

	std::string path = "Asset/BinaryData/Animation/" + modelname;
	path += "_" + name;

	std::ofstream fw(path + ".DUOL", std::ios_base::binary);
	boost::archive::binary_oarchive outArchive(fw);

	outArchive << animationclip;

	// ������ ���������� ��� �������� Ű���� �̸��� ���̺��� �־���´�.
	animationList.emplace_back(std::make_pair(keyValue, objectID));

	animationKey.emplace_back(keyValue);
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

void DUOLFBXSerialize::BinarySerialize::SetJsonFile(const DUOLCommon::tstring path, std::vector< std::pair<uint64, std::string>>& datamap)
{
	std::ofstream file(path);

	using namespace rapidjson;

	if (datamap.size() != 0)
	{
		Document document;
		document.SetArray();
		Document::AllocatorType& allocator = document.GetAllocator();

		rapidjson::Value datas(kArrayType);

		for (int count = 0; count < datamap.size(); count++)
		{
			rapidjson::Value object(kObjectType);
			object.AddMember("ID", datamap[count].first, allocator);
			object.AddMember("Name", datamap[count].second, allocator);
			datas.PushBack(object, allocator);
		}
		document.PushBack(datas, allocator);

		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		document.Accept(writer);

		file << buffer.GetString();
		//	jsonReader->WriteJson(path, document);
	}

	file.close();
}



void DUOLFBXSerialize::BinarySerialize::PerfabJsonFile(const DUOLCommon::tstring path)
{
	//using namespace rapidjson;

	//std::ifstream file(path);
	//IStreamWrapper isw{ file };

	//Document document;
	//Document::AllocatorType& allocator = document.GetAllocator();

	//document.ParseStream(isw);

	//rapidjson::Value datas(rapidjson::kArrayType);

	//for (int count = 0; count < modelPrefab.size(); count++)
	//{
	//	rapidjson::Value object(rapidjson::kObjectType);
	//	object.AddMember("MeshID", modelPrefab[count].first, allocator);
	//	/*object.AddMember("MaterialID", modelPrefab[count].second.first, allocator);
	//	object.AddMember("AnimationID", modelPrefab[count].second.second, allocator);*/
	//	datas.PushBack(object, allocator);
	//}

	//StringBuffer buffer;
	//Writer<StringBuffer> writer(buffer);
	//datas.Accept(writer);

	//document["Perfab"].PushBack(datas, allocator);

	//jsonReader->WriteJson(path, document);

}

/**
 * \brief ���� ����Ǿ� �ִ� �����͸� Json���� ����.
 */
void DUOLFBXSerialize::BinarySerialize::ExportJsonFile()
{
	//Test(L"Asset/DataTable/test.json");
	SetJsonFile(L"Asset/DataTable/Material.json", materialList);
	SetJsonFile(L"Asset/DataTable/Animation.json",animationList);
	//PerfabJsonFile(L"Asset/DataTable/Perfab.json");
}

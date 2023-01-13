#pragma once
#include <map>
#include <memory>
#include "DUOLCommon/StringHelper.h"
#include "DUOLJson/JsonReader.h"

typedef  unsigned __int64 uint64;

namespace SerializeData
{
	class Mesh;
	class Bone;
}

namespace DuolData
{
	struct Mesh;
	struct Material;
	struct Bone;
	struct AnimationClip;
}

struct FBXModel;

namespace DUOLFBXSerialize
{
	class BinarySerialize
	{
	public:
		BinarySerialize() {};
		~BinarySerialize() {};

	private:
		// vector<pair> 가 나을수도
		std::vector<std::pair<uint64, std::string>> materialList;
		std::vector< std::pair<uint64, std::string>> animationList;

		// Mesh Id, Material Ids, Animation Ids
		std::vector<std::pair<uint64, std::pair<std::vector<uint64>, std::vector<uint64>>>> modelPrefab;

		std::vector<uint64> materialKey;
		std::vector<uint64> animationKey;

		DUOLJson::JsonReader* jsonReader;

	public:
		void SerializeDuolData(std::shared_ptr<FBXModel> fbxmodel);

		void ExportJsonFile();

	private:
		void SetMeshData(std::shared_ptr<DuolData::Mesh> fbxmesh, SerializeData::Mesh& mesh);
		void SetBoneData(std::shared_ptr<DuolData::Bone> fbxbone, SerializeData::Bone& bone);
		void MaterialSerialize(std::shared_ptr<DuolData::Material> fbxmaterial, int count);
		void SetAnimationData(std::shared_ptr < DuolData::AnimationClip> fbxanimationclip, std::string modelname);

		void SetAnimationName(std::string& animationname);
		void SetJsonFile(const DUOLCommon::tstring path, std::vector< std::pair<uint64, std::string>>& datamap);
		void PerfabJsonFile(const DUOLCommon::tstring path);
	};
}

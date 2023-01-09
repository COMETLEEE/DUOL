#pragma once
#include <memory>

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
}

struct FBXModel;

class BinarySerialize
{
public:
	BinarySerialize() {};
	~BinarySerialize() {};

public:
	void SerializeDuolData(std::shared_ptr<FBXModel> fbxmodel);

private:
	void SetMeshData(std::shared_ptr<DuolData::Mesh> fbxmesh, SerializeData::Mesh& mesh);
	void SetBoneData(std::shared_ptr<DuolData::Bone> fbxbone, SerializeData::Bone& bone);
	void MaterialSerialize(std::shared_ptr<DuolData::Material> fbxmaterial, int count);
	void SetAnimationData(std::shared_ptr < DuolData::AnimationClip> fbxanimationclip);

};

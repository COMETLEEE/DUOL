#pragma once
#include <memory>

namespace SerializeData
{
	class Mesh;
}

namespace DuolData
{
	struct Mesh;
	struct Material;
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
	//void SetMaterialData(std::shared_ptr<DuolData::Material> fbxmaterialcount);
	void MaterialSerialize(std::shared_ptr<DuolData::Material> fbxmaterial,int count);


};

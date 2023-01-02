#pragma once
#include <memory>

struct FBXModel;

class BinarySerialize
{
public:
	BinarySerialize(){};
	~BinarySerialize(){};

public:
	void SerializeDuolData(std::shared_ptr<FBXModel> fbxmodel);
};

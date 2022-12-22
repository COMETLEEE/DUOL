#include "DUOLGameEngine/ECS/Object/Avatar.h"

namespace DUOLGameEngine
{
	Avatar::Avatar(const DUOLCommon::tstring& name) :
		DUOLGameEngine::ObjectBase(name, ObjectType::Resource)
	{

	}

	Avatar::~Avatar()
	{

	}

	void Avatar::SetPrimitiveBones(std::vector<DUOLGraphicsEngine::Bone>* bones)
	{
		_bones = bones;

		// TODO : 그냥 미리 하이어라키를 빨리 뽑아낼 수 있도록 트리 만들어 놓기 ?
	}
}
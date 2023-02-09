#include "DUOLGameEngine/ECS/Object/Avatar.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::Avatar>("Avatar")
	.constructor<>()
	(

	);
}

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

		// TODO : �׳� �̸� ���̾��Ű�� ���� �̾Ƴ� �� �ֵ��� Ʈ�� ����� ���� ?
	}
}
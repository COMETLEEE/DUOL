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

		// TODO : �׳� �̸� ���̾��Ű�� ���� �̾Ƴ� �� �ֵ��� Ʈ�� ����� ���� ?
	}
}
#include "DUOLClient/ECS/Component/Enemy.h"

namespace DUOLClient
{
	Enemy::Enemy(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		Character(owner, name),
		_isHit(false),
		_attackRange(3.0f),
		_targetOffset(10.0f),
		_lookRange(20.0f)
	{
	}

	void Enemy::OnStart()
	{
	}

	void Enemy::Attack(Character* target)
	{
	}

	void Enemy::OnHit(Character* other)
	{

	}
}

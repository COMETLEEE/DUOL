#pragma once

#include <Windows.h>
#include <memory>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <functional>
#include "util/TypeDefine.h"

namespace Muscle
{
	class Collider;

	class MonoBehavior abstract
	{
	public:
		MonoBehavior() = default;

		virtual ~MonoBehavior() = default;

	protected:
		virtual void OnEnterCollision(std::shared_ptr<Collider> _Other) {};

		virtual void OnStayCollision(std::shared_ptr<Collider> _Other) {};

		virtual void OnExitCollision(std::shared_ptr<Collider> _Other) {};

	protected:
		virtual void Start() {};

		virtual void Update() {}; // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.

		virtual void LateUpdate() {}; // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.

		virtual void Render() {}; // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����
	};
}


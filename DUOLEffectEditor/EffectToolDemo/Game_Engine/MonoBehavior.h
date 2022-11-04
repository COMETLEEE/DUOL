#pragma once
namespace Muscle
{
	class Collider;

	class MonoBehavior abstract
	{
	public:
		MonoBehavior() = default;

		~MonoBehavior() = default;

	protected:
		virtual void OnEnterCollision(std::shared_ptr<Collider> _Other) {};

		virtual void OnStayCollision(std::shared_ptr<Collider> _Other) {};

		virtual void OnExitCollision(std::shared_ptr<Collider> _Other) {};

	protected:
		virtual void Start() {};

		virtual void Update() {}; // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.

		virtual void LateUpdate() {}; // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.

		virtual void Render() {}; // 모든 Object 렌더 돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 렌더
	};
}


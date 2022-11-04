#pragma once
namespace Muscle
{
	class BoxCollider : public Collider
	{

	public:
		BoxCollider(std::shared_ptr<GameObject> _GameObject);

		virtual ~BoxCollider();

	private:
		Vector3 _size;

	public:
		virtual void Finalize() override;

		void SetSize(Vector3 size);

		Vector3 GetSize();

		Matrix GetWorldRotTM();
	public:
		virtual void ColliderCheck(std::shared_ptr<Collider> _Other) abstract;

		virtual void Start() abstract;

		virtual void Render() abstract; // 모든 Object 렌더 돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 렌더

		virtual void UpdateMinMax() override {};
	};
}

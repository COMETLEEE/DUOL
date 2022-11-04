#pragma once

namespace Muscle
{
	class SkyBox : public IComponents
	{
	public:
		SkyBox(std::shared_ptr<GameObject> _GameObj); //부모를 게임 오브젝트를 인자로 받고 초기화

		~SkyBox() = default;

	public:
		std::shared_ptr<RenderingData_3D> _renderingData;

	private:
		std::shared_ptr<Camera> _camera;

		std::shared_ptr<Transform> _transform; // 트랜스폼 캐싱해서 사용하자

	public:
		void Initialize(const uint64& cubeMapID, std::shared_ptr<Camera> camera);

		virtual void Start() override;

		virtual void Update()override {};  // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.

		virtual void LateUpdate() override; // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.

		virtual void Render() override; // 모든 Object 렌더 돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 렌더

	};
}


#pragma once

namespace Muscle
{

	class DirectionalLight : public Light
	{
		public:
			DirectionalLight(std::shared_ptr<GameObject> _GameObject);
			~DirectionalLight();

			std::shared_ptr<DirectionalLightInfo> _directionalLightInfo;
		public:
			virtual void Start() override;

			virtual void Update() override; // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.

			virtual void Render() override; // 모든 Object 렌더 돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 렌더

			void SetDirectional(Vector3 directional);
	};
}

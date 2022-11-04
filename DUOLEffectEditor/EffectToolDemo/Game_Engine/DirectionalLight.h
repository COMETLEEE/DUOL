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

			virtual void Update() override; // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.

			virtual void Render() override; // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����

			void SetDirectional(Vector3 directional);
	};
}

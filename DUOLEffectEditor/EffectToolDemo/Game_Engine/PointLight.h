#pragma once
namespace Muscle
{
	class PointLight : public Light
	{
	public:
		PointLight(std::shared_ptr<GameObject> _GameObject);
		~PointLight();

	private:
		std::shared_ptr<PointLightInfo> _PointLightInfo;

	public:
		virtual void Start() override;

		virtual void LateUpdate() override;

		virtual void Update() override; // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.

		virtual void Render() override; // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����

		void SetRange(float range);
	};

}

#pragma once
namespace Muscle
{
	class SpotLight : public Light
	{
	public:
		SpotLight(std::shared_ptr<GameObject> _GameObject);
		~SpotLight();

	private:
		std::shared_ptr<SpotLightInfo> _SpotLightInfo;

	public:
		virtual void Start() override;

		virtual void LateUpdate() override;

		virtual void Update() override; // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.

		virtual void Render() override; // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����

		void SetRange(float range);
		void SethalfAngle(float angle);
	};
}


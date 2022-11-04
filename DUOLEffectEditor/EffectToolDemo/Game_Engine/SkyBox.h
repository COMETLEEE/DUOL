#pragma once

namespace Muscle
{
	class SkyBox : public IComponents
	{
	public:
		SkyBox(std::shared_ptr<GameObject> _GameObj); //�θ� ���� ������Ʈ�� ���ڷ� �ް� �ʱ�ȭ

		~SkyBox() = default;

	public:
		std::shared_ptr<RenderingData_3D> _renderingData;

	private:
		std::shared_ptr<Camera> _camera;

		std::shared_ptr<Transform> _transform; // Ʈ������ ĳ���ؼ� �������

	public:
		void Initialize(const uint64& cubeMapID, std::shared_ptr<Camera> camera);

		virtual void Start() override;

		virtual void Update()override {};  // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.

		virtual void LateUpdate() override; // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.

		virtual void Render() override; // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����

	};
}


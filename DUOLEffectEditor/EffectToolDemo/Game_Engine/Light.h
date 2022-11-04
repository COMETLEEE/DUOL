#pragma once


namespace Muscle
{

	class Light : public IComponents
	{
	public:
		Light(std::shared_ptr<GameObject> _GameObject);
		~Light();

	protected:
		std::shared_ptr<LightInfo> _lightInfo;

	public:
		virtual void Start() override;

		virtual void Update() override; // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.

		virtual void Render() abstract; // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����

	public:
#pragma region PBR Lighting Model
		void SetColor(Vector3 rgb);

		void SetLumen(float lumen);
#pragma endregion

#pragma region Legacy Lighting Model
		//void SetAmbient(Vector4 _Ambient) { _lightInfo->_ambient = _Ambient; } // �ֺ���

		//void SetDiffuse(Vector4 _Diffuse) { _lightInfo->_diffuse = _Diffuse; } // N dot L

		//void SetSpecular(Vector4 _Specular) { _lightInfo->_specular = _Specular; } //�ݻ籤
#pragma endregion
	};
}
#pragma once
/**

	@class   TextureManager
	@brief   �ε��� �ؽ����� �ּҸ� �����ϴ� �� �׷��Ƚ� �������� �������� ShaderResourceView�� ���� �ϰ� �ְ�, Path�� Ű ������ �̿��� �����ϰ� �ִ�.
	���⼭�� Ű ���� ������ �ִ�..
	@details ~

**/
class TextureLoader
{

private:
	/**
	 * \brief �ؽ��� ������ �ּ� ��.
	 */
	static std::vector<tstring> _loadTextureFilePaths; 
public:
	/**
	 * \brief �ؽ��ĸ� �ε�, �о���̴� �ϴ� �Լ�.
	 */
	static void* InsertTexture(tstring path);
	/**
	 * \brief �̹� �ε��� �ؽ����� �����͸� �������� �Լ�.
	 */
	static void* GetTexture(tstring path); // 
	/**
	 * \brief ����<�ؽ��� �ּ�>�� �������� �Լ�.
	 */
	static const std::vector<tstring>& GetTextureFilePaths();
};


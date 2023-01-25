#pragma once
#include <vector>
#include "StringHelper.h"
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
	static std::vector<DUOLCommon::tstring> _loadTextureFilePaths;
public:
	/**
	 * \brief �ؽ��ĸ� �ε�, �о���̴� �ϴ� �Լ�.
	 */
	static void* InsertTexture(DUOLCommon::tstring path);
	/**
	 * \brief �̹� �ε��� �ؽ����� �����͸� �������� �Լ�.
	 */
	static void* GetTexture(DUOLCommon::tstring path); //
	/**
	 * \brief �̹� �ε��� �ؽ����� �����͸� �������� �Լ�.
	*/
	static void* GetNoiseMap(std::tuple<float, int, float> key);
	/**
	 * \brief ����<�ؽ��� �ּ�>�� �������� �Լ�.
	 */
	static const std::vector<DUOLCommon::tstring>& GetTextureFilePaths();
};


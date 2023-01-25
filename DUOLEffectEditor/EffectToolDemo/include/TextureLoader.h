#pragma once
#include <vector>
#include "StringHelper.h"
/**

	@class   TextureManager
	@brief   로딩한 텍스쳐의 주소를 관리하는 곳 그래픽스 엔진에서 실질적인 ShaderResourceView를 관리 하고 있고, Path를 키 값으로 이용해 접근하고 있다.
	여기서는 키 값만 가지고 있다..
	@details ~

**/

class TextureLoader
{

private:
	/**
	 * \brief 텍스쳐 파일의 주소 값.
	 */
	static std::vector<DUOLCommon::tstring> _loadTextureFilePaths;
public:
	/**
	 * \brief 텍스쳐를 로드, 읽어들이는 하는 함수.
	 */
	static void* InsertTexture(DUOLCommon::tstring path);
	/**
	 * \brief 이미 로딩된 텍스쳐의 포인터를 가져오는 함수.
	 */
	static void* GetTexture(DUOLCommon::tstring path); //
	/**
	 * \brief 이미 로딩된 텍스쳐의 포인터를 가져오는 함수.
	*/
	static void* GetNoiseMap(std::tuple<float, int, float> key);
	/**
	 * \brief 벡터<텍스쳐 주소>를 가져오는 함수.
	 */
	static const std::vector<DUOLCommon::tstring>& GetTextureFilePaths();
};


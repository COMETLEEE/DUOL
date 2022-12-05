#include "TextureLoader.h"
#include "GraphicsManager.h"
#include "algorithm"
#include "IGameEngine.h"
std::vector<tstring> TextureLoader::_loadTextureFilePaths;

void* TextureLoader::InsertTexture(tstring path)
{
	if (_loadTextureFilePaths.end() == std::find(_loadTextureFilePaths.begin(), _loadTextureFilePaths.end(), path))
	{
		_loadTextureFilePaths.push_back(path);
		return Muscle::IGameEngine::Get()->GetGraphicsManager()->InsertTexture(path);
	}

	assert(false); // �̹� ���� �����ϱ� ������ ������ ����Ѵ�..

	return nullptr;
}

void* TextureLoader::GetTexture(tstring path)
{
	return Muscle::IGameEngine::Get()->GetGraphicsManager()->GetTexture(path);
}

const std::vector<tstring>& TextureLoader::GetTextureFilePaths()
{
	return _loadTextureFilePaths;
}

#include "ResourceManager.h"
#include "TextureLoader.h"

ResourceManager ResourceManager::_instance;

ResourceManager& ResourceManager::Get()
{
	return _instance;
}

void ResourceManager::Initialize()
{
#pragma region TextureLoad
	TextureLoader::InsertTexture(TEXT("Asset/Particle/Resource\\Image\\Particle.dds"));

	TextureLoader::InsertTexture(TEXT("Asset/Particle/Resource\\Image\\test1.png"));

	TextureLoader::InsertTexture(TEXT("Asset/Particle/Resource\\Image\\test2.png"));

	TextureLoader::InsertTexture(TEXT("Asset/Particle/Resource\\Image\\Arrow.png"));
#pragma endregion

}

void ResourceManager::Finalize()
{
}

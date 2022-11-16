#include "pch.h"
#include "ResourceManager.h"

ResourceManager ResourceManager::_instance;

ResourceManager& ResourceManager::Get()
{
	return _instance;
}

void ResourceManager::Initialize()
{
#pragma region TextureLoad
	TextureLoader::InsertTexture(TEXT("Resource\\Image\\Particle.dds"));

	TextureLoader::InsertTexture(TEXT("Resource\\Image\\test1.png"));

	TextureLoader::InsertTexture(TEXT("Resource\\Image\\test2.png"));
#pragma endregion

}

void ResourceManager::Finalize()
{
}

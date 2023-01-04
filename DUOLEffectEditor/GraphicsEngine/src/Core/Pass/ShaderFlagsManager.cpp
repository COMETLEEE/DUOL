#include "Core/Pass/ShaderFlagsManager.h"
#include <assert.h>

ShaderFlagsManager::ShaderFlagsManager()
{
	_basicParticeFlags["None"] = BasicParticleFlags::None;

	int size = sizeof(BasicParticleFlags_str) / sizeof(*BasicParticleFlags_str);

	for (int i = 0; i < size; i++)
	{
		_basicParticeFlags[BasicParticleFlags_str[i]] = static_cast<BasicParticleFlags>(1 << i);
	}
}

BasicParticleFlags ShaderFlagsManager::GetFlag(std::string flag_str)
{
	auto temp = _basicParticeFlags.find(flag_str);

	assert(temp != _basicParticeFlags.end()); // 잘못된 플래그로 접근..!!

	return temp->second;
}
ShaderFlagsManager& ShaderFlagsManager::Get()
{
	static ShaderFlagsManager instance;

	return instance;
}
#include "Core/Pass/ShaderFlagsManager.h"
#include <assert.h>

ShaderFlagsManager::ShaderFlagsManager()
{
	_basicParticeFlags["None"] = BasicParticle::Flags::None;

	int size = std::size(BasicParticle::Flags_str);

	for (int i = 0; i < size; i++)
	{
		_basicParticeFlags[BasicParticle::Flags_str[i]] = static_cast<BasicParticle::Flags>(1 << i);
	}
}

BasicParticle::Flags ShaderFlagsManager::GetFlag(std::string flag_str)
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
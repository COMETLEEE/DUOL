#include "HotKey.h"
#include "Windows.h"

HotKey& HotKey::Get()
{
	static HotKey _instance;

	return _instance;
}

void HotKey::RegisterHotKey(unsigned virtualKey, unsigned Modifiers, std::function<void()> func)
{
	::RegisterHotKey(nullptr, _keyID, Modifiers, virtualKey);

	_registedHotKey.emplace_back(std::make_pair(_keyID, func));

	_keyID++;
}

void HotKey::UpdateHotKey(unsigned int message)
{
	for (auto& iter : _registedHotKey)
	{
		if (iter.first == message)
		{
			iter.second();
			break;
		}
	}
}

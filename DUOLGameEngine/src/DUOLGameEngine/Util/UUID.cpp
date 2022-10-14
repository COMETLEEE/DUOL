#include "DUOLGameEngine/Util/UUID.h"
#include <random>

namespace DUOLGameEngine
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_MtEngine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	UUID::UUID() :
		_uuid(s_UniformDistribution(s_MtEngine))
	{

	}

	UUID::UUID(uint64_t uuid) :
		_uuid(uuid)
	{
		
	}
}
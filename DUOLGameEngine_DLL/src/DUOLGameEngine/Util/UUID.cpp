//#include "DUOLGameEngine/Util/UUID.h"
//#include <random>
//
//namespace DUOLGameEngine
//{
//	static std::random_device s_RandomDevice;
//	static std::mt19937_64 s_MtEngine(s_RandomDevice());
//	static std::uniform_int_distribution<uint64_t> s_UniformDistribution(std::numeric_limits<std::uint64_t>::min(), std::numeric_limits<std::uint64_t>::max());
//
//	UUID::UUID() // :
//		// _uuid(s_UniformDistribution(s_MtEngine))
//	{
//
//		std::random_device _RandomDevice;
//		std::mt19937_64 _MtEngine(_RandomDevice());
//		std::uniform_int_distribution<uint64_t> _UniformDistribution(std::numeric_limits<std::uint64_t>::min(), std::numeric_limits<std::uint64_t>::max());
//
//
//		_uuid = _UniformDistribution(_MtEngine);
//	}
//
//	UUID::UUID(uint64_t uuid) :
//		_uuid(uuid)
//	{
//		
//	}
//}
#pragma once
#include <cstdint>
#include <cstdlib>
#include <unordered_map>

namespace DUOLGameEngine
{
	/**
	 * \brief UUID 범용적으로 사용되는 고유 식별자입니다.
	 * 게임 엔진이 참조하는 모든 객체 및 에셋 객체에게 부여합니다.
	 */
	struct UUID
	{
	public:
		UUID();

		~UUID() = default;

		UUID(uint64_t uuid);

		UUID(const UUID&) = default;

		UUID& operator =(const UUID&) = default;

		UUID(UUID&&) = default;

		UUID& operator=(UUID&&) = default;

		operator uint64_t() const { return _uuid; }

	private:
		uint64_t _uuid;
	};
}

namespace std
{
	template <typename T> struct hash;

	/**
	 * \brief UUID를 통한 해쉬 함수 객체를 템플릿 완전 특수화합니다.
	 */
	template <>
	struct hash<DUOLGameEngine::UUID>
	{
		std::size_t operator() (const DUOLGameEngine::UUID& uuid) const noexcept
		{
			return (uint64_t)uuid;
		}
	};
}
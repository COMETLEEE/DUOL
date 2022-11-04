#pragma once
#include <cstdint>
#include <cstdlib>
#include <unordered_map>

namespace DUOLGameEngine
{
	/**
	 * \brief UUID ���������� ���Ǵ� ���� �ĺ����Դϴ�.
	 * ���� ������ �����ϴ� ��� ��ü �� ���� ��ü���� �ο��մϴ�.
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
	 * \brief UUID�� ���� �ؽ� �Լ� ��ü�� ���ø� ���� Ư��ȭ�մϴ�.
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
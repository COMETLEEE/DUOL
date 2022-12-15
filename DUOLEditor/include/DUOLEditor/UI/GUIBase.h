/**

    @file      GUIBase.h
    @brief     GUI component's interface class.
    @details   ~
    @author    COMETLEE
    @date      15.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <memory>

namespace DUOLEditor
{
	/**
	 * \brief GUI component's interface class.
	 */
	class GUIBase
	{
	public:
		virtual void Draw() = 0;

	protected:
		GUIBase();

		virtual ~GUIBase() = default;

	private:
		uint64_t _id;

		static uint64_t _nextID;

	public:
		const uint64_t& GetID() const;
	};
}
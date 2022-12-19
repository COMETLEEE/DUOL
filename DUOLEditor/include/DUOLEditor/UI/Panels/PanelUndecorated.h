#pragma once
#include "DUOLEditor/UI/Panels/PanelTransformableBase.h"

namespace DUOLEditor
{
	/**
	 * \brief transformable without decoration.
	 */
	class PanelUndecorated : public PanelTransformableBase
	{
	protected:
		virtual void Draw_Impl() override;

	private:
		int GetFlags();
	};
}
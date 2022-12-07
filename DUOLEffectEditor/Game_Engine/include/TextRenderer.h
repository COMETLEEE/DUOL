#pragma once
#include "IComponents.h"
#include "Export\RenderingData.h"

namespace Muscle
{
	class TextRenderer : public IComponents
	{
	public:
		TextRenderer(std::shared_ptr<GameObject> _GameObject);

		~TextRenderer();

	private:
		std::shared_ptr<MuscleGrapics::TextData> _textData;

	public:
		void SetPrintNDCPosition(DUOLMath::Vector2 pos);

		void SetPrintPosition(DUOLMath::Vector2 _pos);

		void SetText(tstring str);

		void SetFont(tstring str);
	public:
		virtual void Start() override;
		virtual void Update() override;
		virtual void Render() override;
	};
}


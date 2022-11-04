#pragma once
namespace Muscle
{
	class TextRenderer : public IComponents
	{
	public:
		TextRenderer(std::shared_ptr<GameObject> _GameObject);

		~TextRenderer();

	private:
		std::shared_ptr<TextData> _textData; 

	public:
		void SetPrintNDCPosition(Vector2 pos);

		void SetPrintPosition(Vector2 _pos);

		void SetText(tstring str);

		void SetFont(tstring str);
	public:
		virtual void Start() override;
		virtual void Update() override;
		virtual void Render() override;
	};
}


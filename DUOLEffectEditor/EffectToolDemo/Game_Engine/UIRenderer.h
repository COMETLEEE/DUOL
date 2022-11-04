#pragma once


namespace Muscle
{
	class UIRenderer : public IComponents
	{
	public:
		UIRenderer(std::shared_ptr<GameObject> _GameObject);

		~UIRenderer();

	private:
		std::shared_ptr<RenderingData_UI> _uiData;

		float _initialRight;

		float _rectFill; // �󸶳� ä����ΰ�..? 0 ~ 1 ���� ��, 1�� ���� ä�� ����.

	public:
		inline std::shared_ptr<RenderingData_UI>& GetUIData() { return _uiData; }

	public:
		// Left, Top, Right, Bottom
		const Vector4&& GetDrawNDCPosition();

		void SetDrawNDCPosition(const Vector2& leftTop, const Vector2& rightBottom);

		inline const uint64& GetTextureID() { return _uiData->_drawInfo->_textureID; }

		inline void SetTextureID(const uint64& textureID) { _uiData->_drawInfo->_textureID = textureID; }

		inline const uint32& GetSortOrder() { return _uiData->_drawInfo->_sortOrder; }

		inline void SetSortOrder(const uint32& sortOrder) { _uiData->_drawInfo->_sortOrder = sortOrder; }

		inline void SetUseAlphaBlend(bool value) { _uiData->_drawInfo->_useAlphaBlend = value; }

		inline void SetRectFill(float _fill) { _rectFill = _fill; }

	public:
		virtual void Start() override;

		virtual void Update() override;

		virtual void Render() override;
	};
}
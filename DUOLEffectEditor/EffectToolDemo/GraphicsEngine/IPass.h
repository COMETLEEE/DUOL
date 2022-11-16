#pragma once
/// <summary>
/// IPass를 상속 받은 Pass 제작 후
/// 리소스 매니저에 패스 추가.
/// Renderer에서 다형적으로 Draw동작 수형.
/// </summary>
template <typename T>
class IPass abstract
{
public:
	IPass() = default;

	virtual ~IPass() = default;

protected:
	virtual void SetConstants(std::shared_ptr<T>& renderingData) abstract;

public:
	virtual void Draw(std::shared_ptr<T>& renderingData) abstract;

};


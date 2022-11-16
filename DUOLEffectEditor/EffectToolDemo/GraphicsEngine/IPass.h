#pragma once
/// <summary>
/// IPass�� ��� ���� Pass ���� ��
/// ���ҽ� �Ŵ����� �н� �߰�.
/// Renderer���� ���������� Draw���� ����.
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


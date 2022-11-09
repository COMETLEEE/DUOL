#pragma once
/// <summary>
/// ���� ť�� �޾Ƽ� �ѹ��� �������� ���� ���ִ� ��ü.
/// ������ �Ŵ������ �����ϸ� �� ��...
/// 
/// </summary>
class Renderer
{
public:
	Renderer() = default;

	~Renderer();

public:
	void MoveRenderingData_Particle(std::queue<std::shared_ptr<RenderingData_Particle>>&& renderQueueParticle);

	void MoveRenderingData_3D(std::queue<std::shared_ptr<RenderingData_3D>>&& renderQueue3D);

	void MoveRenderingData_UI(std::queue<std::shared_ptr<RenderingData_UI>>&& renderQueueUI);

	void MoveTextData(std::queue<std::shared_ptr<TextData>>&& renderQueueText);

	void MovePerFrameData(std::shared_ptr<PerFrameData>&& perframeData);

	void ExecuteRender();

	static const std::shared_ptr<PerFrameData>& GetPerfreamData();

private:
	std::queue<std::shared_ptr<RenderingData_Particle>> _renderQueueParticle;

	std::queue<std::shared_ptr<RenderingData_3D>> _renderQueue3D;

	std::queue<std::shared_ptr<RenderingData_UI>> _renderQueueUI;

	std::queue<std::shared_ptr<TextData>> _renderQueueText;

	static std::shared_ptr<PerFrameData> _perframeData;

};


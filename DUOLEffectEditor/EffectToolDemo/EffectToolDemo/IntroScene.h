#pragma once

class IntroScene : public Muscle::IScene
{
public:
	IntroScene();

	~IntroScene();

private:
	std::shared_ptr<Muscle::UIRenderer> _uiRenderer;

	std::vector<uint64> _loadingImageIDs;

	std::string _nextSceneName; // ����� �ε��ϵ� �ݵ�� ��Ʈ�� ���� ���İ����Ѵ�..!

public:

	virtual void RapidUpdate() override;

	virtual void Start() override;

	virtual void Update() override;
};


#pragma once

class IntroScene : public Muscle::IScene
{
public:
	IntroScene();

	~IntroScene();

private:
	std::shared_ptr<Muscle::UIRenderer> _uiRenderer;

	std::vector<uint64> _loadingImageIDs;

	std::string _nextSceneName; // 어떤씬을 로딩하든 반드시 인트로 씬을 거쳐가야한다..!

public:

	virtual void RapidUpdate() override;

	virtual void Start() override;

	virtual void Update() override;
};


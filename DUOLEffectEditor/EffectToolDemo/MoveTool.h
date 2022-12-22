#pragma once
#include "IComponents.h"
#include "../../DUOLMath/include/DUOLMath/DUOLMath.h"

class MoveTool : public Muscle::IComponents
{
public:
	MoveTool(std::shared_ptr<Muscle::GameObject> _GameObject);

	virtual ~MoveTool();

public:
	void SetType(int num);// 1x, 2y, 3z

private:
	int _type;

	float _scale;

	DUOLMath::Vector2 _lastPos;

	bool _isSelected;

	DUOLMath::Vector3 _direction;

	DUOLMath::Vector3 _startPos;
public:
	virtual void Start() override;

	virtual void Update() override;

	void Select();

	void UnSelect();
};


#pragma once
#include <memory>
#include <stack>
#include "../../DUOLMath/include/DUOLMath/DUOLMath.h"
#include "Export/RenderingData.h"

#define EXCUTE(x) Commander::Get().Excute(x)
#define UNDO() Commander::Get().Undo()
#define REDO() Commander::Get().Redo()
#define COMMANDCLEAR() Commander::Get().Clear()
namespace Muscle
{
	class ParticleRenderer;
	class GameObject;
}
/**

	@class   Command
	@brief   Ŀ�ǵ� �������̽� Ŭ����
	@details ~

**/
class Command
{
public:
	virtual ~Command() = default;

	virtual void Excute() abstract;

	virtual void Undo() abstract;

	virtual void Redo() abstract;
};
/**

	@class   Commander
	@brief   Ŀ�ǵ带 �����Ű�� �����ϴ� Ŭ����
	@details ~

**/
class Commander
{
public:
	Commander() = default;

	~Commander();
public:
	static Commander& Get();

private:
	std::stack<Command*> _undoList; // Ctrl + z

	std::stack<Command*> _redoList; // Ctrl + y

public:
	void Excute(Command* command);

	void Undo();

	void Redo();

	void Clear();
};
/**

	@class   SelectObjectCommand
	@brief   ������Ʈ ���� Ŀ�ǵ�
	@details ~

**/
class SelectObjectCommand : public Command
{
public:
	SelectObjectCommand(const std::shared_ptr<Muscle::GameObject>& selectedObject);

	virtual ~SelectObjectCommand() = default;

	virtual void Excute() override;

	virtual void Undo() override;

	virtual void Redo() override;

private:
	std::shared_ptr<Muscle::GameObject> _curSelectedObject;

	std::shared_ptr<Muscle::GameObject> _prevSelectedObject;
};
/**

	@class   ObjectMoveCommand
	@brief   ������Ʈ�� �����̴� Ŀ�ǵ�.
	@details ~

**/
class ObjectTranslateCommand : public Command
{
public:
	ObjectTranslateCommand(const std::shared_ptr<Muscle::GameObject>& target, DUOLMath::Vector3 endPos);

	virtual ~ObjectTranslateCommand() = default;

	virtual void Excute() override;

	virtual void Undo() override;

	virtual void Redo() override;

private:
	std::shared_ptr<Muscle::GameObject> _target;

	DUOLMath::Vector3 _startPos;

	DUOLMath::Vector3 _endPos;

};
class ObjectRotateCommand : public Command
{
public:
	ObjectRotateCommand(const std::shared_ptr<Muscle::GameObject>& target, DUOLMath::Vector3 endRot);

	virtual ~ObjectRotateCommand() = default;

	virtual void Excute() override;

	virtual void Undo() override;

	virtual void Redo() override;

private:
	std::shared_ptr<Muscle::GameObject> _target;

	DUOLMath::Vector3 _startRot;

	DUOLMath::Vector3 _endRot;

};
class ObjectScaleCommand : public Command
{
public:
	ObjectScaleCommand(const std::shared_ptr<Muscle::GameObject>& target, DUOLMath::Vector3 endScale);

	virtual ~ObjectScaleCommand() = default;

	virtual void Excute() override;

	virtual void Undo() override;

	virtual void Redo() override;

private:
	std::shared_ptr<Muscle::GameObject> _target;

	DUOLMath::Vector3 _startScale;

	DUOLMath::Vector3 _endScale;

};
/**

	@class   RenderingDataUpdateCommand
	@brief   ������ �����͸� ������Ʈ �ϴ� Ŀ�ǵ�. Undo, Redo�� ���� �����Ѵ�.
	@details ~

**/
class RenderingDataUpdateCommand : public Command
{
public:
	RenderingDataUpdateCommand(const std::shared_ptr<Muscle::ParticleRenderer>& target, MuscleGrapics::RenderingData_Particle& prev, MuscleGrapics::RenderingData_Particle& cur);

	virtual ~RenderingDataUpdateCommand() = default;

	virtual void Excute() override;

	virtual void Undo() override;

	virtual void Redo() override;

private:
	std::shared_ptr<Muscle::ParticleRenderer> _target;

	MuscleGrapics::RenderingData_Particle _prev;

	MuscleGrapics::RenderingData_Particle _cur;
};

class ObjectDeleteCommand : public Command
{
public:
	ObjectDeleteCommand(const std::shared_ptr<Muscle::GameObject>& target);

	virtual ~ObjectDeleteCommand();

	virtual void Excute() override;

	virtual void Undo() override;

	virtual void Redo() override;

private:
	std::shared_ptr<Muscle::GameObject> _target;

	bool _isDelete;

};

class ObjectCreateCommand : public Command
{
public:
	ObjectCreateCommand(const std::shared_ptr<Muscle::GameObject>& target);

	virtual ~ObjectCreateCommand() override;

	virtual void Excute() override;

	virtual void Undo() override;

	virtual void Redo() override;

private:
	std::shared_ptr<Muscle::GameObject> _target;

	
};

class SetParentsCommand : public Command
{
public:
	SetParentsCommand(const std::shared_ptr<Muscle::GameObject>& child, const std::shared_ptr<Muscle::GameObject>& parents);

	virtual ~SetParentsCommand() = default;

	virtual void Excute() override;

	virtual void Undo() override;

	virtual void Redo() override;

	DUOLMath::Matrix _prevMatrix;
private:
	std::shared_ptr<Muscle::GameObject> _child;

	std::shared_ptr<Muscle::GameObject> _parents;

	std::shared_ptr<Muscle::GameObject> _preParents;
};
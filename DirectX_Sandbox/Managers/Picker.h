#pragma once
#include "ECS/CameraComponent.h"
#include "ECS/TransformComponent.h"
#include "Raycast.h"

class PickedEntity
{
	Entity* entity = nullptr;

	XMVECTOR initPos;
	XMVECTOR initRotation;
	XMVECTOR initScale;

public:
	Entity* GetEntity() { return entity; }
	const XMVECTOR& GetInitScale() const { return initScale; }

	void Select(Entity* entity)
	{
		this->entity = entity;
		auto entityTransform = entity->GetComponent<TransformComponent>();
		initPos = entityTransform->GetPosition();
		initRotation = entityTransform->GetRotation();
		initScale = entityTransform->GetScale();
	}

	void Deselect()
	{
		this->entity = nullptr;
	}

	void Revert()
	{
		if (entity == nullptr)
			return;

		auto entityTransform = entity->GetComponent<TransformComponent>();
		entityTransform->SetPosition(initPos);
		entityTransform->SetRotation(initRotation);
		entityTransform->SetScale(initScale);
	}
};

enum class TransformMode
{
	None,
	Move,
	Rotate,
	Scale
};

enum class TransformAxis
{
	All,
	X, Y, Z
};

class Picker
{
	int width, height;
	CameraComponent* camera = nullptr;
	TransformComponent* cameraTransform = nullptr;
	PickedEntity pickedEntity;

	TransformMode transformMode = TransformMode::None;
	TransformAxis transformAxis = TransformAxis::All;
	XMVECTOR mousePosInit;

public:
	Picker(int screenWidth, int screenHeight) : width(screenWidth), height(screenHeight), mousePosInit() {}
	void Initialize();
	void Update();
private:
	void Pick(int x, int y);
	void OnKeyPressed(const unsigned char key);
	void OnRMBClicked(int x, int y);

	XMVECTOR GetAxisVector() const;
	void ProcessMove();
	void ProcessScale();
};
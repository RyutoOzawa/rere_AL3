#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"ViewProjection.h"

class Reticle
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection viewprojection);

	void SetPos(Vector3 position);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
};


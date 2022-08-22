#include "Reticle.h"
#include<cassert>

void Reticle::Initialize(Model* model)
{
	//null�`�F�b�N
	assert(model);

	//�����̃f�[�^�Ń��f���f�[�^���i�[
	model_ = model;

	//���[���h���W������
	worldTransform_.Initialize();

	//worldTransform_.scale_ = { 2.0f, 1.0f, 2.0f};

	worldTransform_.MatUpdate();
}

void Reticle::Update()
{
	worldTransform_.MatUpdate();
}

void Reticle::Draw(ViewProjection viewprojection)
{
	model_->Draw(worldTransform_, viewprojection);
}

void Reticle::SetPos(Vector3 position)
{
	worldTransform_.translation_ = position;
}

#pragma once
#include "myLib.h"

namespace GameLib {
	//�I�u�W�F�N�g�ݒ�n

	// �I�u�W�F�N�g�̊�𒆐S�ɂ���֐�
	// ����
	// int w_ �I�u�W�F�N�g�̕�
	// int h_ �I�u�W�F�N�g�̍���
	// �߂�l
	// ML::Box2D�^ 
	ML::Box2D SetCenteredBox(int w_, int h_);

	//������S�̃I�u�W�F�N�g���w�肵���ʒu�ɔz�u
	// ����
	// int x_ ���Wx
	// int y_ ���Wy
	// int w_ �I�u�W�F�N�g�̕�
	// int h_ �I�u�W�F�N�g�̍���
	// �߂�l
	// ML::Box2D�^ 
	ML::Box2D SetBoxByCenter(int x_, int y_, int w_, int h_);

}
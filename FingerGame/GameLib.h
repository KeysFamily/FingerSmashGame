//-----------------------------------------------------------------------------
// ����⏕�p
// 
// �쐬�� ���J��E��N
// �@�\�F�I�u�W�F�N�g�𒆐S��ň����ۂ̋�`�f�[�^�̍쐬
//-----------------------------------------------------------------------------

#pragma once
#include "myLib.h"

namespace MyGameLib {

	// �I�u�W�F�N�g�̊�𒆐S�ɂ���֐�
	// ����
	// int w_ �I�u�W�F�N�g�̕�
	// int h_ �I�u�W�F�N�g�̍���
	// �߂�l
	// ML::Box2D ���S��(0,0)�̋�`�f�[�^
	ML::Box2D SetCenteredBox(int w_, int h_);

	//������S�̃I�u�W�F�N�g���w�肵���ʒu�ɔz�u
	// ����
	// int x_ ���Wx
	// int y_ ���Wy
	// int w_ �I�u�W�F�N�g�̕�
	// int h_ �I�u�W�F�N�g�̍���
	// �߂�l
	// ML::Box2D�^ ���S��(x_,y_)�̋�`�f�[�^
	ML::Box2D SetCenteredBox(int x_, int y_, int w_, int h_);

}
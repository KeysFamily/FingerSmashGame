#pragma once
#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
// �ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------


#include "GameEngine_Ver3_83.h"

class BHand : public BTask
{
	//�ύX�s����������������������������������������������������
public:
	typedef shared_ptr<BHand>		SP;
	typedef weak_ptr<BHand>		WP;
public:
	//�ύX������������������������������������������������������
	//���ʃ����o�ϐ�
	enum class Dir
	{
		Left,	//������
		Right	//�E����
	};
	int handMax;//�����̎�̍ő�l
	int hand;	//�������o����

	Dir dir;	//��̌�������
	//�����o�ϐ��̏�����
	BHand()
		: hand(0)
		, handMax(0)
	{}

	virtual ~BHand(){}

	void UpDate_Std();
	void Render_Std(DG::Image::SP img_, ML::Vec2 pos_);
	//���ʃ��\�b�h
	

};


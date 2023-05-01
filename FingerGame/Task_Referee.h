#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  Referee
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�R��");	//�O���[�v��
	const  string  defName("��");	//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
		//�u�ϐ��錾�������v
		
		DG::Image::SP imgPlayer;
		DG::Image::SP imgCPU;
		DG::Image::SP imgSelector;		//��U��U�̑I��
		DG::Image::SP imgFingerLabel;	//���Ȃ��̗\�z���鐔��
		DG::Image::SP imgExpected;		//�\�z���ꂽ����
		DG::Image::SP imgJudgement;		//���Ȃ��̏���or����
		DG::Image::SP imgController;	//������@�\��
		DG::Image::SP imgMiddleResult;	//������or�͂���
		DG::Image::SP imgTerminal;		//����or�I��
		DG::Image::SP imgTurn;			//�u���Ȃ�orCPU�v�̔Ԃł�
		DG::Image::SP imgFirSecAttack;	//���Ȃ��́u��Uor��U�v
		DG::Image::SP imgNumber;		//����
		DG::Image::SP imgDays;			//���ڂ̕\��


	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//�u�ϐ��錾�������v
		//�u�ǉ����\�b�h�������v

		//------------
		// �v���C���[�̃^�[�����ǂ����i�Q�[�����[�h1,�i�s���[�h1�œ���j
		// true:�v���C���[�̔�
		// false:�G�̔�
		// 
		//------------
		bool playerturn;	//�v���C���[�̔ԂȂ�true

		//------------
		// �Q�[�����[�h�i�i�s���[�h��1�̎��ɓ���j
		// 0�S�̂ł��������̍ő�̎擾
		// 
		// 1�����̏o���ڂ̑I��
		// 2�㏈���̒���ɑ���̏o���ڂ̐ݒ�
		// 
		// �i���o������Βǉ��j
		// ����
		//------------
		enum class GameMode {//GameMode�p
			Non,
			RefSet,		//�R���ݒ�
			PlayerSet,	//�v���C���[�ݒ�
			Judge,		//����
			EnemySet,	//�G�ݒ�
			Effect,		//���o
			MiddleResult,	//�r������
		};
		GameMode mode;			//�G�^�X�N�ƃv���C���[�^�X�N�̃��[�h���Ǘ�

		//------------
		// �i�s�J�E���^
		// 0.��U��U�̑I��
		// 1.�Q�[���̐i�s
		// 2.���s
		//------------
		//int progressMode;
		enum class Progress {
			Non,
			FirstSecond,//��U��U�̌���
			Attack,		//���Ȃ��́u��Uor��U�v
			Game,		//�Q�[��
			Result		//���U���g
		};
		Progress progressMode;

		//------
		// �v���C���[�̐�U��U�̑I��
		// 0.��U
		// 1.��U
		// 2.�R���s���[�^�ɔC����
		//------
		int attackSelect;

		int hand;		//�S�̂ŏグ����
		int smashHand;	//�\�z���ꂽ��̐�
		int handMax;	//�S�̂ŏグ�����

		bool equalFlag;//�\�z���ꂽ���Ǝ��ۂ̐��������Ƃ�
		//���o
		int effectCnt;//�G�t�F�N�g�J�E���^

		int dayCnt;//�f�C�J�E���^�i�^�[�����ƂɃC���N�������g�j

		enum class Dir { Right, Left };
		//
		// 
		// x_,y_	�ꕶ���ڂ̍��W[�������Ȃ�E�[,�E�����Ȃ獶�[]
		// w_,h_	�ꕶ��������̑傫��
		// n_		�\����������
		// dir		����
		//	left:000[1]��
		// Right:��[1]000
		//----
		void Render_Number(int x_, int y_, int w_, int h_, int n_, Dir dir_);

	};
}
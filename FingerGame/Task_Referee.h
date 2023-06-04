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

		enum class GameMode {	//�Q�[�����[�h
			Non,			//���ɂȂ�
			RefSet,			//�R���ݒ�
			PlayerSet,		//�v���C���[�ݒ�
			Judge,			//����
			EnemySet,		//�G�ݒ�
			Effect,			//���o
			MiddleResult,	//�r������
		};
		GameMode mode;			//�G�^�X�N�ƃv���C���[�^�X�N�̃��[�h���Ǘ�

		enum class Progress {	//�i�s���[�h
			Non,			//���ɂȂ�
			FirstSecond,	//��U��U�̌���
			Attack,			//���Ȃ��́u��Uor��U�v
			Game,			//�Q�[��
			Result			//���U���g
		};
		Progress progressMode;

#if false
		typedef std::map<Referee::Object::GameMode, std::function<void()>> GameModeMap;
		typedef std::map<Progress, std::function<void()>> ProgressMap;
#else
		using GameModeMap = std::map<Referee::Object::GameMode, std::function<void()>>;
		using ProgressMap = std::map<Progress, std::function<void()>>;
#endif
		
		bool isPlayerTurn;	//�v���C���[�̔Ԃ��H
		bool isPredictably;	//�\�z���ꂽ���Ǝ��ۂ̐����������H

		//------
		// �v���C���[�̐�U��U�̑I��
		// 0.��U
		// 1.��U
		// 2.�R���s���[�^�ɔC����
		//------
		int selectAttackType;

		int hand;			//�S�̂ŏグ����
		int smashHand;		//�\�z���ꂽ��̐�
		int handMax;		//�S�̂ŏグ�����

		//���o
		int effectCnt;		//�G�t�F�N�g�J�E���^

		int dayCnt;			//�f�C�J�E���^�i�^�[�����ƂɃC���N�������g�j

		enum class Dir { Right, Left };
		// x_,y_	�ꕶ���ڂ̍��W[�������Ȃ�E�[,�E�����Ȃ獶�[]
		// w_,h_	�ꕶ��������̑傫��
		// n_		�\����������
		// dir		����
		//	left:000[1]��
		// Right:��[1]000
		//----
		void Number_Render(int x_, int y_, int w_, int h_, int n_, Dir dir_);

	private:
		//�֐��Q�z��
		ProgressMap progressUpDateMap, progressRenderMap;
		void ProgressMap_Initialize();
		void ProgressMap_Finalize();

		//�i�s���[�h���Ƃ̏���
		void Progress_UpDate(Progress progress_);
		void FirstSecond_UpDate();
		void Attack_UpDate();
		void Game_UpDate();

		void Progress_Render(Progress progress_);
		void FirstSecond_Render();
		void Attack_Render();
		void Game_Render();
		void Result_Render();

		//�Q�[�����샂�[�h�̏���
		GameModeMap gamemodeUpDateMap, gamemodeRenderMap;
		void ModeMap_Initialize();
		void ModeMap_Finalize();

		void GameMode_UpDate(GameMode mode_);
		void Refset_UpDate();
		void Effect_UpDate();
		void Judge_UpDate();
		void MiddleResult_UpDate();

		void GameMode_Render(GameMode mode_);
		void PleyerSet_Render();
		void JudgeMiddleResult_Render();

	};
}
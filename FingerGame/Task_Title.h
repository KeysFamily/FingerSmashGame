#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  Title
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�^�C�g��");	//�O���[�v��
	const  string  defName("NoName");			//�^�X�N��
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
		DG::Image::SP   imgBG;		//�w�i
		DG::Image::SP	imgTitle;	//�^�C�g��
		DG::Image::SP   imgLabel;	//�X�^�[�g���x��
		DG::Image::SP   imgComp;	//�쐬�ҕ\��
		DG::Image::SP   imghelp;	//�w���v
		
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
		void  UpDate()			override;	//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		bool			isNextScene;	//���̃V�[���ɑ���ۂ�true
		bool			titleBound;		//�^�C�g���͒��˂���true
		bool			isTitleStop;	//�^�C�g���͎~�܂��Ă�����true
		bool			helpFlag;		//�w���v���\������true

		int				fadeCnt;		//�t�F�[�h�J�E���^
		float			fallSpeed;		//�^�C�g���̗������x
		float			titlePosY;		//�^�C�g����Y���W
		int				flashCnt;		//�_�ŃJ�E���^
	};
}
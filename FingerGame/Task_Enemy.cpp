//-------------------------------------------------------------------
//�G
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Enemy.h"

#include  "randomLib.h"


namespace  Enemy
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		
		this->img = DG::Image::Create("./data/image/Finger_256x85.png"); 
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->render2D_Priority[1] = 0.5f;
		
		this->enemyHand = 0;
		this->enemyHandMax = 2;

		EnemySetMap_Initialize();

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		this->EnemySetMap_Finalize();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//**
		//�y���[�h�ݒ�(����)�z
		// 0.���͑҂�/�o���ڂ̐ݒ�
		// (���o������Ή��o�҂�)
		// 1.���ʔ���
		// �����s�����Ȃ����0�ɖ߂�
		// �����s�����΃^�X�N�I��
		//**
		
		if (ge->qa_Ref->progressMode == Referee::Object::Progress::Game
			&& ge->qa_Ref->mode == Referee::Object::GameMode::EnemySet) {
			this->EnemySetMap_UpDate(ge->qa_Ref->isPlayerTurn);
			ge->qa_Ref->mode = Referee::Object::GameMode::Judge;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		if (ge->qa_Ref == nullptr) { return; }
		if (ge->qa_Ref->progressMode == Referee::Object::Progress::Game) {
			//�E��i��̂Ђ��������j
			if (this->enemyHandMax > 0) {
				ML::Box2D draw = ML::Box2D(ge->screen2DWidth, 300, -64 * 3, 85 * 3);
				ML::Box2D src = ML::Box2D(128 * (1 - (this->enemyHand / 2 + this->enemyHand % 2) % 2), 0, 64, 85);

				this->res->img->Rotation(ML::ToRadian(-10.0f), ML::Vec2(0.0f, 0.0f));
				this->res->img->Draw(draw, src, ML::Color(1.0f, 0.8f, 1.0f, 0.8f));
				this->res->img->Rotation(0.0f, ML::Vec2(0.0f, 0.0f));
			}

			//����(��̂Ђ�������Ȃ�)
			if (this->enemyHandMax == 2) {
				ML::Box2D draw = ML::Box2D(ge->screen2DWidth, 350, -64 * 3, 85 * 3);
				ML::Box2D src = ML::Box2D(64 + 128 * (1 - (this->enemyHand / 2)), 0, 64, 85);

				this->res->img->Rotation(ML::ToRadian(-10.0f), ML::Vec2(0.0f, 0.0f));
				this->res->img->Draw(draw, src, ML::Color(1.0f, 0.8f, 1.0f, 0.8f));
				this->res->img->Rotation(0.0f, ML::Vec2(0.0f, 0.0f));
			}
		}
	}


	//-------------------------------------------------------------------
	// �S�̂̎w�̐��ɂ�鏈��
	void Object::EnemySetMap_Initialize() {
		this->enemySetMap[4] = std::bind(&Object::EnemySetIfAllHand4, this);
		this->enemySetMap[3] = std::bind(&Object::EnemySetIfAllHand3, this);
		this->enemySetMap[2] = std::bind(&Object::EnemySetIfAllHand2, this);
	}
	void Object::EnemySetMap_Finalize() {
		this->enemySetMap.clear();
	}
	void Object::EnemySetMap_UpDate(bool isPlayerTrun_) {
		if (isPlayerTrun_) {
			this->EnemySetIfPlayerTurn();
		}
		else {
			if (this->enemySetMap.count(ge->qa_Ref->handMax) == 1) {
				this->enemySetMap[ge->qa_Ref->handMax]();
			}
			else {
				ML::MsgBox("�\�����ʏ��������o����܂���");
				ge->qa_Ref->smashHand = 0;
				this->enemyHand = 0;
			}
		}
	}

	void Object::EnemySetIfPlayerTurn() {
		this->enemyHand = GetRandom<int>(0, this->enemyHandMax);
	}
	void Object::EnemySetIfAllHand4() {
		//----------------------------------------------------------------------------
		//�S�̂̎w�̐���4�{�̎�(�݂���2����)�̎w�̏o����
		//(�A)	�w�X�}4����S���o��
		//(�C)	�w�X�}3���1�ȏ�o��
		//(�E)	�w�X�}2���2�{�܂ŁA�o���Ȃ��Ă��悢
		//(�G)	�w�X�}1���1�{�܂ŁA�o���Ȃ��Ă��悢
		//(�I)	�w�X�}0����o���Ȃ�
		//----------------------------------------------------------------------------

		ge->qa_Ref->smashHand = GetRandom<int>(0, 4);
		switch (ge->qa_Ref->smashHand) {
		case 4:
			this->enemyHand = this->enemyHandMax;
			break;
		case 3:
			this->enemyHand = GetRandom<int>(1, 2);
			break;
		case 2:
			this->enemyHand = GetRandom<int>(0, this->enemyHandMax);
			break;
		case 1:
			this->enemyHand = GetRandom<int>(0, 1);
			break;
		case 0:
			this->enemyHand = 0;
			break;
		}
	}
	void Object::EnemySetIfAllHand3() {
		//----------------------------------------------------------------------------
		//�S�̂̎w�̐���3�{�̎��̎w�̏o����
		//(�A)	�w�X�}3����S���o��
		//(�C)	�w�X�}2����G�w�c��2�{:1�ȏ�o����G�w�c��1�{:1�{�܂ŏo���Ȃ��Ă��悢
		//(�E)	�w�X�}1���1�{�܂ŁA�o���Ȃ��Ă��悢
		//(�G)	�w�X�}0����o���Ȃ��
		//----------------------------------------------------------------------------

		ge->qa_Ref->smashHand = GetRandom<int>(0, 3);
		switch (ge->qa_Ref->smashHand) {
		case 3:
			this->enemyHand = this->enemyHandMax;
			break;
		case 2:
			if (this->enemyHandMax == 2) {
				this->enemyHand = GetRandom<int>(1, enemyHandMax);
			}
			else {
				this->enemyHand = GetRandom<int>(0, enemyHandMax);
			}
			break;
		case 1:
			this->enemyHand = GetRandom<int>(0, 1);
			break;
		case 0:
			this->enemyHand = 0;
			break;
		}
	}
	void Object::EnemySetIfAllHand2() {
		//----------------------------------------------------------------------------
		// �S�̂̎w�̐���2�{�̎�(�݂���1����)�̎w�̏o����
		//(�A)	�w�X�}2����S���o��
		//(�C)	�w�X�}1����o���Ȃ��Ă��悢
		//(�E)	�w�X�}0����o���Ȃ�
		//----------------------------------------------------------------------------

		ge->qa_Ref->smashHand = GetRandom<int>(0, 2);
		switch (ge->qa_Ref->smashHand) {
		case 2:
			this->enemyHand = this->enemyHandMax;
			break;
		case 1:
			this->enemyHand = GetRandom<int>(0, this->enemyHandMax);
			break;
		case 0:
			this->enemyHand = 0;
			break;
		}
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}
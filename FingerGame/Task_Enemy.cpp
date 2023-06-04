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

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


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
		
		if (ge->qa_Ref->progressMode == Referee::Object::Progress::Game) {
			switch (ge->qa_Ref->mode) {
			case Referee::Object::GameMode::EnemySet://�G�̒l�Z�b�g
				if (!ge->qa_Ref->isPlayerTurn) {
					//�����Œl��\�z����
					ge->qa_Ref->smashHand = GetRandom<int>(0, ge->qa_Ref->handMax);
					//�\�z���ꂽ�l�͑S�̂̎w�̐��̍ő�l��(true�őS���o��)
					if (ge->qa_Ref->smashHand == ge->qa_Ref->handMax) {
						this->enemyHand = this->enemyHandMax;
					}
					//�\�z���ꂽ�l��0��(true�Ȃ�o���Ȃ�)
					else if (ge->qa_Ref->smashHand == 0) {
						this->enemyHand = 0;
					}
					//�\�z���ꂽ�l��3�Ŏ����̎�̐���2��(true�Ȃ�o��)
					else if (ge->qa_Ref->smashHand == 3 && this->enemyHandMax == 2
						 || ge->qa_Ref->smashHand == 2 && this->enemyHandMax == 2) {
						this->enemyHand = GetRandom<int>(1, 2);
					}
					//�\�z���ꂽ�l��3�Ŏ����̎�̐���1��(true�Ȃ�o���Ȃ��Ă��悢)
					else if (ge->qa_Ref->smashHand == 3 && this->enemyHandMax == 1) {
						this->enemyHand = GetRandom<int>(0, this->enemyHandMax);
					}
					//�o���Ȃ��Ă��悢
					else {
						this->enemyHand = GetRandom<int>(0, this->enemyHandMax);
					}
				}
				else {
					this->enemyHand = GetRandom<int>(0, this->enemyHandMax);
				}
				ge->qa_Ref->mode = Referee::Object::GameMode::Judge;
				//Sleep(500);
				break;
			default:
				break;
			}
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
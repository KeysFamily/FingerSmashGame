//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"

#include  "sound.h"

#include  "Task_Rule.h"
#include  "Task_Referee.h"
#include  "Task_Player.h"
#include  "Task_Enemy.h"

#include  "Task_Ending.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgBG = DG::Image::Create("./data/image/Classroom3470x720.jpg");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->imgBG.reset();
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
		this->render2D_Priority[1] = 0.9f;

		this->isPause = false;
		this->fadeCnt = 0;

		//���^�X�N�̐���
		Referee::Object::Create(true);
		Player::Object::Create(true);
		Enemy::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G(Game::defGroupName);
		ge->KillAll_G(Referee::defGroupName);//�R���^�X�N�̏I��
		ge->KillAll_G(Player::defGroupName);//�v���C���^�X�N�̏I��
		ge->KillAll_G(Enemy::defGroupName);	//�G�^�X�N�̏I��

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto next = Ending::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState( );
		//�|�[�Y����

		if (inp.B1.down) {
			se::Play("Select");
			this->isPause = !this->isPause;
			
			ge->StopAll_G(Referee::defGroupName, this->isPause);
			ge->StopAll_G(Player::defGroupName, this->isPause);
			ge->StopAll_G(Enemy::defGroupName, this->isPause);

			if (this->isPause) {
				Rule::Object::Create(true);
			}
			else {
				ge->KillAll_G(Rule::defGroupName);
			}
		}

		//�R���̏����擾
		ge->qa_Ref = ge->GetTask<Referee::Object>(Referee::defGroupName, Referee::defName);

		//���ʂ��\�����Ńv���C���[���G��������S�L�[�̓��͂��������Ƃ�
		if (ge->qa_Ref->progressMode == Referee::Object::Progress::Result
			&& (ge->GameClearFlag || ge->GameOverFlag) 
			&& inp.ST.down) {

			se::Play("Select");
			//���g�ɏ��ŗv��
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//���҂ӂ���			��
		//�Е��̂���ЂƂ�	�[��
		//�ǂ��炩0			��
		ML::Box2D drawBG(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D srcBG(0, 0, 1280, 720);//���̔w�i

		if (ge->qa_Ref != nullptr) {
			srcBG.Offset(1280 * (ge->qa_Ref->dayCnt % 3), 0);
		}
		else {
			srcBG.Offset(0, 0);
		}

		this->res->imgBG->Draw(drawBG, srcBG, ML::Color(0.8f, 1.0f, 1.0f, 1.0f));
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
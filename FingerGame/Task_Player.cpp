//-------------------------------------------------------------------
// �v���C������
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "sound.h"
namespace  Player
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
		this->render2D_Priority[1] = 0.5f;

		//���f�[�^������
		this->myHand = 0;
		this->myHandMax = 2;
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
		auto inp = ge->in1->GetState();
		if (ge->qa_Ref == nullptr) { return; }
		if (ge->qa_Ref->progressMode == Referee::Object::Progress::Game 
			&& ge->qa_Ref->mode == Referee::Object::GameMode::PlayerSet) {
			if (ge->qa_Ref->isPlayerTurn) {
				//�����̗\�z
				if (inp.LStick.BL.down) {//��
					se::Play("decision");
					--ge->qa_Ref->smashHand;
					if (ge->qa_Ref->smashHand < 0) {
						ge->qa_Ref->smashHand = ge->qa_Ref->handMax;
					}
				}
				else if(inp.LStick.BR.down) {//�E
					se::Play("decision");
					ge->qa_Ref->smashHand = (ge->qa_Ref->smashHand + 1) % (ge->qa_Ref->handMax + 1);
				}
			}
			//�����̐����̐��̐ݒ�
			if (inp.LStick.BD.down) {//��
				se::Play("decision");
				--this->myHand;
				if (this->myHand < 0) {
					this->myHand = this->myHandMax;
				}
			}
			else if (inp.LStick.BU.down) {//��
				se::Play("decision");
				this->myHand = (this->myHand + 1) % (this->myHandMax + 1);
			}
			if (inp.SE.down) {
				se::Play("Select");
				ge->qa_Ref->mode = Referee::Object::GameMode::Effect;//���̃��[�h��
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		if (ge->qa_Ref == nullptr) { return; }
		if (ge->qa_Ref->progressMode == Referee::Object::Progress::Game) {
			//����i��̂Ђ��������j
			if (this->myHandMax == 2) {
				ML::Box2D draw = ML::Box2D(0, 300, 64 * 3, 85 * 3);
				ML::Box2D src = ML::Box2D(128 * (1 - (this->myHand / 2)), 0, 64, 85);

				this->res->img->Rotation(ML::ToRadian(10.0f), ML::Vec2(0.0f, 0.0f));
				this->res->img->Draw(draw, src);
				this->res->img->Rotation(0.0f, ML::Vec2(0.0f, 0.0f));
			}

			//�E��i��̂Ђ�������Ȃ��j
			if (this->myHandMax > 0) {
				ML::Box2D draw = ML::Box2D(0, 350, 64 * 3, 85 * 3);
				ML::Box2D src = ML::Box2D(64 + 128 * (1 - (this->myHand / 2 + this->myHand % 2) % 2), 0, 64, 85);

				this->res->img->Rotation(ML::ToRadian(10.0f), ML::Vec2(0.0f, 0.0f));
				this->res->img->Draw(draw, src);
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
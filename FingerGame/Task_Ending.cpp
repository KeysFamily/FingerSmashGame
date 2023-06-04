//-------------------------------------------------------------------
//�G���f�B���O
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ending.h"

#include  "Task_Title.h"

namespace  Ending
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgBG = DG::Image::Create("./data/image/end1280x720.jpg");
		this->imgThanks = DG::Image::Create("./data/image/thank759x101.png");
		this->imgEndRoll = DG::Image::Create("./data/image/EndRoll900x540.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->imgBG.reset();
		this->imgThanks.reset();
		this->imgEndRoll.reset();
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

		//���^�X�N�̐���
		this->effectCnt = 0;
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto  nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();

		++this->effectCnt;
		if (this->effectCnt == 180 * 7) {
			//�G�t�F�N�g�J�n
			ge->CreateEffect(99, ML::Vec2());
		}
		if (this->effectCnt >= 180 * 7 + 50) {
			//���g�ɏ��ŗv��
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		
		ML::Box2D drawBG(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D srcBG(0, 0, 1280, 720);
		this->res->imgBG->Draw(drawBG, srcBG);

		if (this->effectCnt / 180 < 6) {
			ML::Box2D drawEndRoll = MyGameLib::SetCenteredBox(ge->screenWidth / 2, ge->screenHeight / 2, 900, 90);
			ML::Box2D srcEndRoll(0, this->effectCnt / 180 * 90, 900, 90);
			this->res->imgEndRoll->Draw(drawEndRoll, srcEndRoll, ML::Color(sin(ML::ToRadian(this->effectCnt - 180.0f * (this->effectCnt / 180))), 1, 1, 1));
		}
		else if (this->effectCnt <= 180*7) {
			ML::Box2D drawThanks = MyGameLib::SetCenteredBox(ge->screenWidth / 2, ge->screenHeight / 2, 759, 101);
			ML::Box2D srcThanks(0, 0, 759, 101);
			this->res->imgThanks->Draw(drawThanks, srcThanks, ML::Color(sin(ML::ToRadian((float)this->effectCnt)), 1, 1, 1));
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
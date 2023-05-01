//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Rule.h"

namespace  Rule
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgBG = DG::Image::Create("./data/effect/black.png");
		this->imgRuleTitle = DG::Image::Create("./data/image/Rule380x90.png");
		this->imgRule = DG::Image::Create("./data/image/help1223x659.png");
		this->imgController = DG::Image::Create("./data/image/helpView522x92.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->imgBG.reset();
		this->imgRuleTitle.reset();
		this->imgRule.reset();
		this->imgController.reset();
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
		this->render2D_Priority[1] = 0.1f;
		//���f�[�^������
		
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
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i�`��i���������w�i�j
		ML::Box2D drawBG(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D srcBG(0, 0, 100, 100);
		this->res->imgBG->Draw(drawBG, srcBG, ML::Color(0.5f, 1.0f, 1.0f, 1.0f));

		ML::Box2D drawTitle = GameLib::SetBoxByCenter(ge->screenWidth / 2, 45, 380, 90);
		ML::Box2D srcTitle(0, 0, 380, 90);
		this->res->imgRuleTitle->Draw(drawTitle, srcTitle);

		ML::Box2D drawRule = GameLib::SetBoxByCenter(ge->screenWidth / 2, ge->screenHeight / 2, 1000, 540);
		ML::Box2D srcRule(0, 0, 1223, 659);
		this->res->imgRule->Draw(drawRule, srcRule);

		ML::Box2D drawCon(0, 0, 520, 90);
		ML::Box2D srcCon(0, 0, 522, 92);
		drawCon.Offset((int)ge->screenWidth - drawCon.w, (int)ge->screenHeight - drawCon.h);
		this->res->imgController->Draw(drawCon, srcCon);
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
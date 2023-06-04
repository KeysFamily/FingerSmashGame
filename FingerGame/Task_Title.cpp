//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"

#include  "sound.h"

#include  "Task_Rule.h"

#include  "Task_Game.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgTitle = DG::Image::Create("./data/image/title.png");
		this->imgLabel = DG::Image::Create("./data/image/startLabel.png");
		this->imgComp = DG::Image::Create("./data/image/Create.png");
		this->imgBG = DG::Image::Create("./data/image/titleBG1280x720.jpg");
		this->imgHelpLabel = DG::Image::Create("./data/image/helpLabel920x92.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->imgTitle.reset();
		this->imgLabel.reset();
		this->imgComp.reset();
		this->imgBG.reset();
		this->imgHelpLabel.reset();
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

		this->isNextScene = false;
		this->isTitleBound = false;
		this->isTitleStop = false;
		this->isShowHelp = false;

		this->titlePosY = -196.0f;
		this->fallSpeed = 20;

		this->fadeCnt = 0;
		this->flashCnt = 0;

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
			auto  nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();
		//�w���v��ʂ̕\��
		if (this->isTitleStop) {
			if (inp.B1.down) {
				se::Play("Select");
				this->isShowHelp = !this->isShowHelp;
				if (this->isShowHelp) {
					Rule::Object::Create(true);
				}
				else {
					ge->KillAll_G(Rule::defGroupName);
				}
			}
		}

		//�o��G�t�F�N�g

		if (!this->isTitleStop) {
			//�^�C�g�����������Ȃ痎�����x�Ɋ�Â��āA�^�C�g���𗎉�������
			this->titlePosY += this->fallSpeed;
		}else{
			//�^�C�g�����~�܂��Ă����
			this->flashCnt = (this->flashCnt + 1) % 120;
		}


		if (this->titlePosY > 190 && !this->isTitleBound) {
			//���ڂ̃o�E���h
			this->isTitleBound = true;
			this->fallSpeed *= -0.3f;
		}
		//2��ڂł̓o�E���h���Ȃ�
		else if (this->titlePosY > 200 && !this->isTitleStop) {
			this->fallSpeed = 0.0f;
			this->isTitleStop = true;
			this->flashCnt = 0;
		}
		else {
			//�������x���グ��
			this->fallSpeed += ML::Gravity(64.0f);
		}


		//�^�C�g�����Î~���Ƀ{�^���������ꂽ��
		if (inp.ST.down && this->isTitleStop && !this->isShowHelp) {
			se::Play("Select");
			this->isNextScene = true;
			this->fadeCnt = 0;
			//�t�F�[�h���J�n����i90�t���[���j
			ge->CreateEffect(98, ML::Vec2());
		}

		if (isNextScene && ++this->fadeCnt > 45) {
			//���g�ɏ��ŗv��
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		
		//�w�i�`��
		ML::Box2D drawBG(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D srcBG(0, 0, 1280, 720);
		this->res->imgBG->Draw(drawBG, srcBG);
		
		//�w���v��\������ꍇ�͈ȉ���\�����Ȃ�
		if (this->isShowHelp) { return; }
		{
			//�^�C�g��
			ML::Box2D drawTitle(0, 0, 451 * 2, 98 * 2);
			drawTitle.Offset(ge->screenWidth / 2.0f - drawTitle.w / 2.0f, this->titlePosY);
			ML::Box2D srcTitle(0, 0, 451, 98);
			this->res->imgTitle->Draw(drawTitle, srcTitle);
		}
		//�^�C�g������~���Ă���Ε\��
		if(this->isTitleStop){
			//���x��
			ML::Box2D drawLabel(0, 0, 755, 101);
			drawLabel.Offset(ge->screenWidth / 2 - drawLabel.w / 2, 450);
			ML::Box2D srcLabel(0, 0, 755, 101);
			this->res->imgLabel->Draw(drawLabel, srcLabel, ML::Color(1.0f - (this->flashCnt / 60 % 2), 1.0f, 1.0f, 1.0f));
		
			//�����
			ML::Box2D drawComp(0, 0, 317, 83);
			drawComp.Offset((int)ge->screenWidth - drawComp.w, (int)ge->screenHeight - drawComp.h);
			ML::Box2D srcComp(0, 0, 317, 83);
			this->res->imgComp->Draw(drawComp, srcComp);
			
			//�w���v���x��
			ML::Box2D drawHelpLabel(0, 0, 850, 85);
			drawHelpLabel.Offset(0, (int)ge->screenHeight - drawHelpLabel.h);
			ML::Box2D srcHelpLabel(0, 0, 920, 92);
			this->res->imgHelpLabel->Draw(drawHelpLabel, srcHelpLabel);
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
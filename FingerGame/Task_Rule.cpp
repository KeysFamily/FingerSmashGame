//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Rule.h"

namespace  Rule
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->imgBG = DG::Image::Create("./data/effect/black.png");
		this->imgRuleTitle = DG::Image::Create("./data/image/Rule380x90.png");
		this->imgRule = DG::Image::Create("./data/image/help1223x659.png");
		this->imgController = DG::Image::Create("./data/image/helpView522x92.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		this->imgBG.reset();
		this->imgRuleTitle.reset();
		this->imgRule.reset();
		this->imgController.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		this->res = Resource::Create();
		this->render2D_Priority[1] = 0.1f;
		//f[^ϊ»
		
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		//wi`ζiΌ§Ύwij
		ML::Box2D drawBG(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D srcBG(0, 0, 100, 100);
		this->res->imgBG->Draw(drawBG, srcBG, ML::Color(0.5f, 1.0f, 1.0f, 1.0f));

		ML::Box2D drawTitle = MyGameLib::SetCenteredBox(ge->screenWidth / 2, 45, 380, 90);
		ML::Box2D srcTitle(0, 0, 380, 90);
		this->res->imgRuleTitle->Draw(drawTitle, srcTitle);

		ML::Box2D drawRule = MyGameLib::SetCenteredBox(ge->screenWidth / 2, ge->screenHeight / 2, 1000, 540);
		ML::Box2D srcRule(0, 0, 1223, 659);
		this->res->imgRule->Draw(drawRule, srcRule);

		ML::Box2D drawCon(0, 0, 520, 90);
		ML::Box2D srcCon(0, 0, 522, 92);
		drawCon.Offset((int)ge->screenWidth - drawCon.w, (int)ge->screenHeight - drawCon.h);
		this->res->imgController->Draw(drawCon, srcCon);
	}

	//
	//ΘΊΝξ{IΙΟXsvΘ\bh
	//
	//-------------------------------------------------------------------
	//^XNΆ¬ϋ
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWΙo^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYΙΈs΅½ηKill
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
	//\[XNXΜΆ¬
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
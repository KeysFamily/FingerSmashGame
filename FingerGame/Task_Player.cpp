//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "sound.h"
namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Finger_256x85.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		this->img.reset();
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
		this->render2D_Priority[1] = 0.5f;

		//f[^ϊ»
		this->myHand = 0;
		this->myHandMax = 2;
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
		auto inp = ge->in1->GetState();

		if (ge->qa_Ref->progressMode == Referee::Object::Progress::Game) {
			switch (ge->qa_Ref->mode)
			{
			case Referee::Object::GameMode::PlayerSet://θΜIπ
				if (ge->qa_Ref->playerturn) {
					//Μ\z
					if (inp.LStick.BL.down) {//Ά
						se::Play("decision");
						--ge->qa_Ref->smashHand;
						if (ge->qa_Ref->smashHand < 0) {
							ge->qa_Ref->smashHand = ge->qa_Ref->handMax;
						}
					}
					else if(inp.LStick.BR.down) {//E
						se::Play("decision");
						ge->qa_Ref->smashHand = (ge->qa_Ref->smashHand + 1) % (ge->qa_Ref->handMax + 1);
					}
				}
				//©ͺΜΜΜέθ
				if (inp.LStick.BD.down) {//Ί
					se::Play("decision");
					--this->myHand;
					if (this->myHand < 0) {
						this->myHand = this->myHandMax;
					}
				}
				else if (inp.LStick.BU.down) {//γ
					se::Play("decision");
					this->myHand = (this->myHand + 1) % (this->myHandMax + 1);
				}
				if (inp.SE.down) {
					se::Play("Select");
					ge->qa_Ref->mode = Referee::Object::GameMode::Effect;//Μ[hΙ
				}
				break;
			default:
				break;
			}
		}
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		if (ge->qa_Ref == nullptr) { return; }
		if (ge->qa_Ref->progressMode == Referee::Object::Progress::Game) {
			//Άθ
			if (this->myHandMax == 2) {
				ML::Box2D draw = ML::Box2D(0, 300, 64 * 3, 85 * 3);
				ML::Box2D src = ML::Box2D(128 * (1 - (this->myHand / 2)), 0, 64, 85);

				this->res->img->Rotation(ML::ToRadian(10.0f), ML::Vec2(0.0f, 0.0f));
				this->res->img->Draw(draw, src);
				this->res->img->Rotation(0.0f, ML::Vec2(0.0f, 0.0f));
			}

			//Eθ
			if (this->myHandMax > 0) {
				ML::Box2D draw = ML::Box2D(0, 350, 64 * 3, 85 * 3);
				ML::Box2D src = ML::Box2D(64 + 128 * (1 - (this->myHand / 2 + this->myHand % 2) % 2), 0, 64, 85);

				this->res->img->Rotation(ML::ToRadian(10.0f), ML::Vec2(0.0f, 0.0f));
				this->res->img->Draw(draw, src);
				this->res->img->Rotation(0.0f, ML::Vec2(0.0f, 0.0f));
			}
		}
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
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Rule.h"

namespace  Rule
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgBG = DG::Image::Create("./data/effect/black.png");
		this->imgRuleTitle = DG::Image::Create("./data/image/Rule380x90.png");
		this->imgRule = DG::Image::Create("./data/image/help1223x659.png");
		this->imgController = DG::Image::Create("./data/image/helpView522x92.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgBG.reset();
		this->imgRuleTitle.reset();
		this->imgRule.reset();
		this->imgController.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();
		this->render2D_Priority[1] = 0.1f;
		//★データ初期化
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//背景描画（半透明黒背景）
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

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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
//-------------------------------------------------------------------
//エンディング
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ending.h"
#include  "Task_Title.h"

namespace  Ending
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgBG = DG::Image::Create("./data/image/end1280x720.jpg");
		this->imgThanks = DG::Image::Create("./data/image/thank759x101.png");
		this->imgEndRoll = DG::Image::Create("./data/image/EndRoll900x540.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgBG.reset();
		this->imgThanks.reset();
		this->imgEndRoll.reset();
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

		//★タスクの生成
		this->endEffectCnt = 0;
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto  nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();

		//180*5 = 900

		++this->endEffectCnt;

		//if (inp.ST.down) {

		if (this->endEffectCnt == 180 * 7) {
			//エフェクト開始
			ge->CreateEffect(99, ML::Vec2());
		}
		if (this->endEffectCnt >= 180 * 7 + 50) {
			//自身に消滅要請
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		
		ML::Box2D drawBG(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D srcBG(0, 0, 1280, 720);
		this->res->imgBG->Draw(drawBG, srcBG);

		//-------
		// BGM
		// SE
		// textImage
		// handImage
		// thank
		//-------


		if (this->endEffectCnt / 180 < 6) {
			ML::Box2D drawEnd = GameLib::SetBoxByCenter(ge->screenWidth / 2, ge->screenHeight / 2, 900, 90);
			ML::Box2D srcEnd(0, this->endEffectCnt / 180 * 90, 900, 90);
			this->res->imgEndRoll->Draw(drawEnd, srcEnd, ML::Color(sin(ML::ToRadian(this->endEffectCnt - 180.0f * (this->endEffectCnt / 180))), 1, 1, 1));
		}
		else if (this->endEffectCnt <= 180*7) {
			ML::Box2D draw = GameLib::SetBoxByCenter(ge->screenWidth / 2, ge->screenHeight / 2, 759, 101);
			ML::Box2D src(0, 0, 759, 101);
			this->res->imgThanks->Draw(draw, src, ML::Color(sin(ML::ToRadian((float)this->endEffectCnt)), 1, 1, 1));
		}
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
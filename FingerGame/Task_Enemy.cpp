//-------------------------------------------------------------------
//敵
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Enemy.h"

#include  "randomLib.h"


namespace  Enemy
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		
		this->img = DG::Image::Create("./data/image/Finger_256x85.png"); 
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
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

		//★データ初期化
		this->render2D_Priority[1] = 0.5f;
		
		this->enemyHand = 0;
		this->enemyHandMax = 2;

		EnemySetMap_Initialize();

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		this->EnemySetMap_Finalize();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//**
		//【モード設定(共通)】
		// 0.入力待ち/出し目の設定
		// (演出があれば演出待ち)
		// 1.結果判定
		// →勝敗がつかなければ0に戻る
		// →勝敗がつけばタスク終了
		//**
		
		if (ge->qa_Ref->progressMode == Referee::Object::Progress::Game
			&& ge->qa_Ref->mode == Referee::Object::GameMode::EnemySet) {
			this->EnemySetMap_UpDate(ge->qa_Ref->isPlayerTurn);
			ge->qa_Ref->mode = Referee::Object::GameMode::Judge;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		if (ge->qa_Ref == nullptr) { return; }
		if (ge->qa_Ref->progressMode == Referee::Object::Progress::Game) {
			//右手（手のひらを見せる）
			if (this->enemyHandMax > 0) {
				ML::Box2D draw = ML::Box2D(ge->screen2DWidth, 300, -64 * 3, 85 * 3);
				ML::Box2D src = ML::Box2D(128 * (1 - (this->enemyHand / 2 + this->enemyHand % 2) % 2), 0, 64, 85);

				this->res->img->Rotation(ML::ToRadian(-10.0f), ML::Vec2(0.0f, 0.0f));
				this->res->img->Draw(draw, src, ML::Color(1.0f, 0.8f, 1.0f, 0.8f));
				this->res->img->Rotation(0.0f, ML::Vec2(0.0f, 0.0f));
			}

			//左手(手のひらを見せない)
			if (this->enemyHandMax == 2) {
				ML::Box2D draw = ML::Box2D(ge->screen2DWidth, 350, -64 * 3, 85 * 3);
				ML::Box2D src = ML::Box2D(64 + 128 * (1 - (this->enemyHand / 2)), 0, 64, 85);

				this->res->img->Rotation(ML::ToRadian(-10.0f), ML::Vec2(0.0f, 0.0f));
				this->res->img->Draw(draw, src, ML::Color(1.0f, 0.8f, 1.0f, 0.8f));
				this->res->img->Rotation(0.0f, ML::Vec2(0.0f, 0.0f));
			}
		}
	}


	//-------------------------------------------------------------------
	// 全体の指の数による処理
	void Object::EnemySetMap_Initialize() {
		this->enemySetMap[4] = std::bind(&Object::EnemySetIfAllHand4, this);
		this->enemySetMap[3] = std::bind(&Object::EnemySetIfAllHand3, this);
		this->enemySetMap[2] = std::bind(&Object::EnemySetIfAllHand2, this);
	}
	void Object::EnemySetMap_Finalize() {
		this->enemySetMap.clear();
	}
	void Object::EnemySetMap_UpDate(bool isPlayerTrun_) {
		if (isPlayerTrun_) {
			this->EnemySetIfPlayerTurn();
		}
		else {
			if (this->enemySetMap.count(ge->qa_Ref->handMax) == 1) {
				this->enemySetMap[ge->qa_Ref->handMax]();
			}
			else {
				ML::MsgBox("予期せぬ処理が検出されました");
				ge->qa_Ref->smashHand = 0;
				this->enemyHand = 0;
			}
		}
	}

	void Object::EnemySetIfPlayerTurn() {
		this->enemyHand = GetRandom<int>(0, this->enemyHandMax);
	}
	void Object::EnemySetIfAllHand4() {
		//----------------------------------------------------------------------------
		//全体の指の数が4本の時(互いに2つずつ)の指の出し方
		//(ア)	指スマ4･･･全部出す
		//(イ)	指スマ3･･･1つ以上出す
		//(ウ)	指スマ2･･･2本まで、出さなくてもよい
		//(エ)	指スマ1･･･1本まで、出さなくてもよい
		//(オ)	指スマ0･･･出さない
		//----------------------------------------------------------------------------

		ge->qa_Ref->smashHand = GetRandom<int>(0, 4);
		switch (ge->qa_Ref->smashHand) {
		case 4:
			this->enemyHand = this->enemyHandMax;
			break;
		case 3:
			this->enemyHand = GetRandom<int>(1, 2);
			break;
		case 2:
			this->enemyHand = GetRandom<int>(0, this->enemyHandMax);
			break;
		case 1:
			this->enemyHand = GetRandom<int>(0, 1);
			break;
		case 0:
			this->enemyHand = 0;
			break;
		}
	}
	void Object::EnemySetIfAllHand3() {
		//----------------------------------------------------------------------------
		//全体の指の数が3本の時の指の出し方
		//(ア)	指スマ3･･･全部出す
		//(イ)	指スマ2･･･敵指残り2本:1つ以上出す､敵指残り1本:1本まで出さなくてもよい
		//(ウ)	指スマ1･･･1本まで、出さなくてもよい
		//(エ)	指スマ0･･･出さない｡
		//----------------------------------------------------------------------------

		ge->qa_Ref->smashHand = GetRandom<int>(0, 3);
		switch (ge->qa_Ref->smashHand) {
		case 3:
			this->enemyHand = this->enemyHandMax;
			break;
		case 2:
			if (this->enemyHandMax == 2) {
				this->enemyHand = GetRandom<int>(1, enemyHandMax);
			}
			else {
				this->enemyHand = GetRandom<int>(0, enemyHandMax);
			}
			break;
		case 1:
			this->enemyHand = GetRandom<int>(0, 1);
			break;
		case 0:
			this->enemyHand = 0;
			break;
		}
	}
	void Object::EnemySetIfAllHand2() {
		//----------------------------------------------------------------------------
		// 全体の指の数が2本の時(互いに1つずつ)の指の出し方
		//(ア)	指スマ2･･･全部出す
		//(イ)	指スマ1･･･出さなくてもよい
		//(ウ)	指スマ0･･･出さない
		//----------------------------------------------------------------------------

		ge->qa_Ref->smashHand = GetRandom<int>(0, 2);
		switch (ge->qa_Ref->smashHand) {
		case 2:
			this->enemyHand = this->enemyHandMax;
			break;
		case 1:
			this->enemyHand = GetRandom<int>(0, this->enemyHandMax);
			break;
		case 0:
			this->enemyHand = 0;
			break;
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
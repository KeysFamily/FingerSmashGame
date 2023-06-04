//-------------------------------------------------------------------
//ゲーム本編
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgBG = DG::Image::Create("./data/image/Classroom3470x720.jpg");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgBG.reset();
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
		this->render2D_Priority[1] = 0.9f;

		this->isPause = false;
		this->fadeCnt = 0;

		//★タスクの生成
		Referee::Object::Create(true);
		Player::Object::Create(true);
		Enemy::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G(Game::defGroupName);
		ge->KillAll_G(Referee::defGroupName);//審判タスクの終了
		ge->KillAll_G(Player::defGroupName);//プレイヤタスクの終了
		ge->KillAll_G(Enemy::defGroupName);	//敵タスクの終了

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto next = Ending::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState( );
		//ポーズ処理

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

		//審判の情報を取得
		ge->qa_Ref = ge->GetTask<Referee::Object>(Referee::defGroupName, Referee::defName);

		//結果が表示中でプレイヤーか敵が勝利しSキーの入力があったとき
		if (ge->qa_Ref->progressMode == Referee::Object::Progress::Result
			&& (ge->GameClearFlag || ge->GameOverFlag) 
			&& inp.ST.down) {

			se::Play("Select");
			//自身に消滅要請
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//両者ふたつ			昼
		//片方のこりひとつ	夕方
		//どちらか0			夜
		ML::Box2D drawBG(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D srcBG(0, 0, 1280, 720);//昼の背景

		if (ge->qa_Ref != nullptr) {
			srcBG.Offset(1280 * (ge->qa_Ref->dayCnt % 3), 0);
		}
		else {
			srcBG.Offset(0, 0);
		}

		this->res->imgBG->Draw(drawBG, srcBG, ML::Color(0.8f, 1.0f, 1.0f, 1.0f));
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
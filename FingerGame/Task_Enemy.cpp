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
		//**
		//【モード設定(共通)】
		// 0.入力待ち/出し目の設定
		// (演出があれば演出待ち)
		// 1.結果判定
		// →勝敗がつかなければ0に戻る
		// →勝敗がつけばタスク終了
		//**
		
		if (ge->qa_Ref->progressMode == Referee::Object::Progress::Game) {
			switch (ge->qa_Ref->mode) {
			case Referee::Object::GameMode::EnemySet://敵の値セット
				if (!ge->qa_Ref->isPlayerTurn) {
					//乱数で値を予想する
					ge->qa_Ref->smashHand = GetRandom<int>(0, ge->qa_Ref->handMax);
					//予想された値は全体の指の数の最大値か(trueで全部出す)
					if (ge->qa_Ref->smashHand == ge->qa_Ref->handMax) {
						this->enemyHand = this->enemyHandMax;
					}
					//予想された値は0か(trueなら出さない)
					else if (ge->qa_Ref->smashHand == 0) {
						this->enemyHand = 0;
					}
					//予想された値は3で自分の手の数は2か(trueなら出す)
					else if (ge->qa_Ref->smashHand == 3 && this->enemyHandMax == 2
						 || ge->qa_Ref->smashHand == 2 && this->enemyHandMax == 2) {
						this->enemyHand = GetRandom<int>(1, 2);
					}
					//予想された値は3で自分の手の数は1か(trueなら出さなくてもよい)
					else if (ge->qa_Ref->smashHand == 3 && this->enemyHandMax == 1) {
						this->enemyHand = GetRandom<int>(0, this->enemyHandMax);
					}
					//出さなくてもよい
					else {
						this->enemyHand = GetRandom<int>(0, this->enemyHandMax);
					}
				}
				else {
					this->enemyHand = GetRandom<int>(0, this->enemyHandMax);
				}
				ge->qa_Ref->mode = Referee::Object::GameMode::Judge;
				//Sleep(500);
				break;
			default:
				break;
			}
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
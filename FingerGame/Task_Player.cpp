//-------------------------------------------------------------------
// プレイヤ処理
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "sound.h"
namespace  Player
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
		this->render2D_Priority[1] = 0.5f;

		//★データ初期化
		this->myHand = 0;
		this->myHandMax = 2;
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
		auto inp = ge->in1->GetState();
		if (ge->qa_Ref == nullptr) { return; }
		if (ge->qa_Ref->progressMode == Referee::Object::Progress::Game 
			&& ge->qa_Ref->mode == Referee::Object::GameMode::PlayerSet) {
			if (ge->qa_Ref->isPlayerTurn) {
				//数字の予想
				if (inp.LStick.BL.down) {//左
					se::Play("decision");
					--ge->qa_Ref->smashHand;
					if (ge->qa_Ref->smashHand < 0) {
						ge->qa_Ref->smashHand = ge->qa_Ref->handMax;
					}
				}
				else if(inp.LStick.BR.down) {//右
					se::Play("decision");
					ge->qa_Ref->smashHand = (ge->qa_Ref->smashHand + 1) % (ge->qa_Ref->handMax + 1);
				}
			}
			//自分の数字の数の設定
			if (inp.LStick.BD.down) {//下
				se::Play("decision");
				--this->myHand;
				if (this->myHand < 0) {
					this->myHand = this->myHandMax;
				}
			}
			else if (inp.LStick.BU.down) {//上
				se::Play("decision");
				this->myHand = (this->myHand + 1) % (this->myHandMax + 1);
			}
			if (inp.SE.down) {
				se::Play("Select");
				ge->qa_Ref->mode = Referee::Object::GameMode::Effect;//次のモードに
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		if (ge->qa_Ref == nullptr) { return; }
		if (ge->qa_Ref->progressMode == Referee::Object::Progress::Game) {
			//左手（手のひらを見せる）
			if (this->myHandMax == 2) {
				ML::Box2D draw = ML::Box2D(0, 300, 64 * 3, 85 * 3);
				ML::Box2D src = ML::Box2D(128 * (1 - (this->myHand / 2)), 0, 64, 85);

				this->res->img->Rotation(ML::ToRadian(10.0f), ML::Vec2(0.0f, 0.0f));
				this->res->img->Draw(draw, src);
				this->res->img->Rotation(0.0f, ML::Vec2(0.0f, 0.0f));
			}

			//右手（手のひらを見せない）
			if (this->myHandMax > 0) {
				ML::Box2D draw = ML::Box2D(0, 350, 64 * 3, 85 * 3);
				ML::Box2D src = ML::Box2D(64 + 128 * (1 - (this->myHand / 2 + this->myHand % 2) % 2), 0, 64, 85);

				this->res->img->Rotation(ML::ToRadian(10.0f), ML::Vec2(0.0f, 0.0f));
				this->res->img->Draw(draw, src);
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
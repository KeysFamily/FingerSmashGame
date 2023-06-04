//-------------------------------------------------------------------
//タイトル画面
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
	//リソースの初期化
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
	//リソースの解放
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
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();
		//★データ初期化
		this->render2D_Priority[1] = 0.5f;

		this->isNextScene = false;
		this->isTitleBound = false;
		this->isTitleStop = false;
		this->isShowHelp = false;

		this->titlePosY = -196.0f;
		this->fallSpeed = 20;

		this->fadeCnt = 0;
		this->flashCnt = 0;

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
			auto  nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();
		//ヘルプ画面の表示
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

		//登場エフェクト

		if (!this->isTitleStop) {
			//タイトルが落下中なら落下速度に基づいて、タイトルを落下させる
			this->titlePosY += this->fallSpeed;
		}else{
			//タイトルが止まっていれば
			this->flashCnt = (this->flashCnt + 1) % 120;
		}


		if (this->titlePosY > 190 && !this->isTitleBound) {
			//一回目のバウンド
			this->isTitleBound = true;
			this->fallSpeed *= -0.3f;
		}
		//2回目ではバウンドしない
		else if (this->titlePosY > 200 && !this->isTitleStop) {
			this->fallSpeed = 0.0f;
			this->isTitleStop = true;
			this->flashCnt = 0;
		}
		else {
			//落下速度を上げる
			this->fallSpeed += ML::Gravity(64.0f);
		}


		//タイトルが静止中にボタンが押されたら
		if (inp.ST.down && this->isTitleStop && !this->isShowHelp) {
			se::Play("Select");
			this->isNextScene = true;
			this->fadeCnt = 0;
			//フェードを開始する（90フレーム）
			ge->CreateEffect(98, ML::Vec2());
		}

		if (isNextScene && ++this->fadeCnt > 45) {
			//自身に消滅要請
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		
		//背景描画
		ML::Box2D drawBG(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D srcBG(0, 0, 1280, 720);
		this->res->imgBG->Draw(drawBG, srcBG);
		
		//ヘルプを表示する場合は以下を表示しない
		if (this->isShowHelp) { return; }
		{
			//タイトル
			ML::Box2D drawTitle(0, 0, 451 * 2, 98 * 2);
			drawTitle.Offset(ge->screenWidth / 2.0f - drawTitle.w / 2.0f, this->titlePosY);
			ML::Box2D srcTitle(0, 0, 451, 98);
			this->res->imgTitle->Draw(drawTitle, srcTitle);
		}
		//タイトルが停止していれば表示
		if(this->isTitleStop){
			//ラベル
			ML::Box2D drawLabel(0, 0, 755, 101);
			drawLabel.Offset(ge->screenWidth / 2 - drawLabel.w / 2, 450);
			ML::Box2D srcLabel(0, 0, 755, 101);
			this->res->imgLabel->Draw(drawLabel, srcLabel, ML::Color(1.0f - (this->flashCnt / 60 % 2), 1.0f, 1.0f, 1.0f));
		
			//製作者
			ML::Box2D drawComp(0, 0, 317, 83);
			drawComp.Offset((int)ge->screenWidth - drawComp.w, (int)ge->screenHeight - drawComp.h);
			ML::Box2D srcComp(0, 0, 317, 83);
			this->res->imgComp->Draw(drawComp, srcComp);
			
			//ヘルプラベル
			ML::Box2D drawHelpLabel(0, 0, 850, 85);
			drawHelpLabel.Offset(0, (int)ge->screenHeight - drawHelpLabel.h);
			ML::Box2D srcHelpLabel(0, 0, 920, 92);
			this->res->imgHelpLabel->Draw(drawHelpLabel, srcHelpLabel);
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
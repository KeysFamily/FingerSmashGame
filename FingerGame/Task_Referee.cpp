//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_Referee.h"
#include  "Task_Player.h"
#include  "Task_Enemy.h"
#include  "randomLib.h"
#include  "sound.h"

namespace Referee
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgPlayer = DG::Image::Create("./data/image/Player.png");
		this->imgCPU = DG::Image::Create("./data/image/CPU.png");
		this->imgSelector = DG::Image::Create("./data/image/OrderDicede637x364.png");
		this->imgFingerLabel = DG::Image::Create("./data/image/FingerNum536x90.png");
		this->imgExpected = DG::Image::Create("./data/image/Expected463x90.png");
		this->imgJudgement = DG::Image::Create("./data/image/Judgement408x182.png");
		this->imgController = DG::Image::Create("./data/image/Controller771x368.png");
		this->imgMiddleResult = DG::Image::Create("./data/image/judge199x172.png");
		this->imgTerminal = DG::Image::Create("./data/image/terminal270x180.png");
		this->imgFirSecAttack = DG::Image::Create("./data/image/TurnCall270x270.png");
		this->imgTurn = DG::Image::Create("./data/image/Turn720x180.png");
		this->imgNumber = DG::Image::Create("./data/image/number600x90.png");
		this->imgDays = DG::Image::Create("./data/image/days141x87.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgCPU.reset();
		this->imgPlayer.reset();
		this->imgSelector.reset();
		this->imgFingerLabel.reset();
		this->imgExpected.reset();
		this->imgJudgement.reset();
		this->imgController.reset();
		this->imgMiddleResult.reset();
		this->imgTerminal.reset();
		this->imgTurn.reset();
		this->imgFirSecAttack.reset();
		this->imgNumber.reset();
		this->imgDays.reset();
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
		this->render2D_Priority[1] = 0.7f;

		this->playerturn = false;
		this->mode = GameMode::RefSet;
		this->progressMode = Progress::FirstSecond;
		this->attackSelect = 0;

		this->hand = 0;
		this->smashHand = 0;
		this->handMax = 0;
		ge->GameClearFlag = ge->GameOverFlag = false;
		this->effectCnt = 0;
		this->equalFlag = false;

		this->dayCnt = 0;
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
		//コントローラの状態を取得
		auto inp = ge->in1->GetState();

		//プレイヤと敵の状態を取得
		auto pl = ge->GetTask<Player::Object>(Player::defGroupName, Player::defName);
		auto ene = ge->GetTask<Enemy::Object>(Enemy::defGroupName, Enemy::defName);
		if (pl == nullptr || ene == nullptr) { return; }
		
		switch (this->progressMode)
		{
		case Progress::FirstSecond://先攻後攻の選択
			//最小値最大値を超えないための処理
			if (inp.LStick.BU.down) {//上
				se::Play("decision");
				this->attackSelect = (this->attackSelect + 1) % 3;
			}
			else if(inp.LStick.BD.down) {//下
				se::Play("decision");
				--this->attackSelect;
				if (this->attackSelect < 0) {
					this->attackSelect = 2;
				}
			}
			//もし数字が確定したなら
			if (inp.SE.down) {
				se::Play("Select");
				if (this->attackSelect < 2) {
					this->playerturn = (this->attackSelect % 2 == 0);
				}
				else {
					//乱数で決定する
					this->playerturn = (GetRandom<int>(0, 9) % 2 == 0);
				}
				//ゲームモードに移行
				this->progressMode = Progress::Attack;
			}
			break;
		case Progress::Attack://あなたは「」の表示
			if (this->effectCnt < 30) { this->effectCnt += 2; }
			if (inp.SE.down) {
				se::Play("Select");
				this->progressMode = Progress::Game;
			}
			break;
		case Progress::Game://ゲームを進行する
			//数字の調整
			
			switch (this->mode)
			{
			case GameMode::RefSet:
				//全体であげられる手の取得（予想できる数の最大値を決める）
				this->handMax = pl->myHandMax + ene->enemyHandMax;
				this->smashHand = 0;
				pl->myHand = 0;
				ene->enemyHand = 0;
				this->mode = GameMode::PlayerSet;
				this->effectCnt = 0;
				break;
			case GameMode::Effect:
				if (this->effectCnt == 0) {
					//エフェクト開始
					pl->myhandTmp = pl->myHand;//一時保存
					pl->myHand = 0;//一度0クリア

					ge->CreateEffect(98, ML::Vec2());//フェード
					ge->CreateEffect(10, ML::Vec2());//ゆびすま～
					//発音
					se::Play(this->playerturn ? "player" : "cpu");
				}
				if (++this->effectCnt > 100) {
					//エフェクト開始から一定秒数たってから判定を開始する
					this->effectCnt = 0;
					this->mode = GameMode::EnemySet;

					pl->myHand = pl->myhandTmp;
					pl->myhandTmp = 0;
				}
				break;
			case GameMode::Judge://判定
				//発音
				{
					string seName;
					seName = (this->playerturn ? "player" : "cpu") + to_string(this->smashHand);
					se::Play(seName);
				}
				//全体であげられた手を取得
				this->hand = pl->myHand + ene->enemyHand;
				this->equalFlag = this->hand == this->smashHand;
				mode = GameMode::MiddleResult;

				break;
			case GameMode::MiddleResult:
				if (this->effectCnt < 30) { this->effectCnt += 2; }


				//Aキーが押されたら
				if (inp.SE.down) {
					se::Play("Select");
					//どちらかの指が先に降りたらループを抜ける
					//予想された値と全体であげられた値が同じなら（予想通りなら）予想した側のあげられる指の数を減らす
					if (this->equalFlag) {
						if (this->playerturn) {
							--pl->myHandMax;
						}
						else {
							--ene->enemyHandMax;
						}
					}
					//どちらかの指の数がなくなったら次へ
					if (pl->myHandMax == 0 || ene->enemyHandMax == 0) {
						if (pl->myHandMax == 0) {
							ge->GameClearFlag = true;
						}
						else if (ene->enemyHandMax == 0) {
							ge->GameOverFlag = true;
						}
						this->progressMode = Progress::Result;
					}
					else {
						//なくなっていなければターンを切り替える
						this->playerturn = !this->playerturn;

						//審判モードに戻す
						this->mode = GameMode::RefSet;

						//デイカウント
						++dayCnt;
					}
				}
				break;

			default:
				break;
			}
			
			break;
		case Progress::Result://勝敗の表示（タスク遷移）
			
			break;
		default:
			break;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		auto game = ge->GetTask<Game::Object>(Game::defGroupName, Game::defName);
		if (game->isPause) { return; }
		//モードによって色合いを変えるためのもの
		ML::Color white(1.0f, 1.0f, 1.0f, 1.0f);
		ML::Color red(1.0f, 1.0f, 0.7f, 0.0f);

		switch (this->progressMode) {
		case Progress::FirstSecond://先攻後攻の選択
			
			{
				//どうする
				ML::Box2D draw(150, 230, 956, 137);
				ML::Box2D src(0, 0, 637, 91);//この値を1.5倍で描画
				this->res->imgSelector->Draw(draw, src);
			}
			{
				//項目表示(先攻/後攻/コンピュータが決める)
				ML::Box2D drawSel(150, ge->screenHeight / 2, 956, 137);
				ML::Box2D srcSel(0, 91 * (this->attackSelect + 1), 637, 91);//この値を1.5倍
				this->res->imgSelector->Draw(drawSel, srcSel);
			}
			{	//操作方法表示
				
				//順番の選択
				ML::Box2D draw1(0, 0, 560, 92);
				draw1.Offset((int)ge->screenWidth - draw1.w, (int)ge->screenHeight - draw1.h * 2);
				ML::Box2D src1(0, 92 * 2, 560, 92);
				//決定
				ML::Box2D draw2(0, 0, 560, 92);
				draw2.Offset((int)ge->screenWidth - draw2.w, (int)ge->screenHeight - draw2.h);
				ML::Box2D src2(0, 0, 560, 92);

				this->res->imgController->Draw(draw1, src1);
				this->res->imgController->Draw(draw2, src2);
			}
			break;
		case Progress::Attack:
			{
				//先攻、後攻
				ML::Box2D draw = GameLib::SetBoxByCenter(ge->screenWidth / 2, ge->screenHeight / 2, 30 * this->effectCnt, 15 * this->effectCnt);
				//draw = GameLib::SetBoxByCenter(ge->screenWidth / 2, ge->screenHeight / 2, 64, 84);
				ML::Box2D src(0, this->playerturn ? 90 : 180, 180, 90);
				this->res->imgFirSecAttack->Draw(draw, src);
				
				//あなたは？
				ML::Box2D drawYour(0, 0, 9 * this->effectCnt, 3 * this->effectCnt);
				drawYour.Offset(draw.x - drawYour.w / 2, draw.y - drawYour.h / 2);
				//draw = GameLib::SetBoxByCenter(ge->screenWidth / 2, ge->screenHeight / 2, 64, 84);
				ML::Box2D srcYour(0, 0, 270, 90);
				this->res->imgFirSecAttack->Draw(drawYour, srcYour);

				//次へ
				ML::Box2D drawNext(0, 0, 320, 92);
				drawNext.Offset((int)ge->screenWidth - drawNext.w, (int)ge->screenHeight - drawNext.h);
				ML::Box2D srcNext(0, 0, 320, 92);
				this->res->imgController->Draw(drawNext, srcNext);
			}
			break;
		case Progress::Game://攻撃
			//共通
			{
				//敵表示
				ML::Box2D draw(0, 630, 220, 100);
				ML::Box2D src(0, 0, 221, 97);
				this->res->imgPlayer->Draw(draw, src, this->playerturn ? red : white);
			}
			{
				//プレイヤー表示
				ML::Box2D draw(1130, 630, 150, 90);
				ML::Box2D src(0, 0, 149, 90);
				this->res->imgCPU->Draw(draw, src, !this->playerturn ? red : white);
			}
			{
				//あなたorCPUの番です
				ML::Box2D draw(0, 0, 540,68);//360 * 1.5f, 45 * 1.5f
				ML::Box2D src(0, this->playerturn ? 0 : 90, 720, 90);
				this->res->imgTurn->Draw(draw, src);
			}
			//経過時間の表示
			Render_Number(800, ge->screenHeight - 90, 60, 90, this->dayCnt / 3 + 1, Dir::Left);
			{
				ML::Box2D draw(860, ge->screenHeight - 90, 150, 90);
				ML::Box2D src(0, 0, 147, 87);
				this->res->imgDays->Draw(draw, src);
			}

			//個々
			if (mode == GameMode::PlayerSet) {
				
				//操作方法表示
				{
					//『自分の出す指の数』
					ML::Box2D draw1(0, 0, 771 / 2, 92 / 2);
					draw1.Offset(ge->screenWidth - draw1.w, 0);
					ML::Box2D src1(0, 92 * 3, 771, 92);

					//『決定』
					ML::Box2D draw2(0, 0, 771 / 2, 92 / 2);
					draw2.Offset(ge->screenWidth - draw2.w, this->playerturn ? draw1.h * 2 : draw1.h);
					ML::Box2D src2(0, 0, 771, 92);

					this->res->imgController->Draw(draw1, src1);
					this->res->imgController->Draw(draw2, src2);
				}
				//プレイヤーの攻撃時のみ
				if (this->playerturn) {
					{
						//【予想された数】表示
						ML::Box2D draw(300, 300, 536, 90);
						ML::Box2D src(0, 0, 536, 90);
						this->res->imgFingerLabel->Draw(draw, src);
					}
					{
						//予想された【数】表示
						
						Render_Number(840, 305, 64, 84, this->smashHand, Dir::Left);
					}
					{
						//『予想する指の数』
						ML::Box2D draw3(0, 0, 771 / 2, 92 / 2);
						draw3.Offset(ge->screenWidth - draw3.w, draw3.h);
						ML::Box2D src3(0, 92, 771, 92);
						this->res->imgController->Draw(draw3, src3);
					}

				}

			}
			else if (mode == GameMode::Effect) {
				//処理なし
			}
			else if (mode == GameMode::Judge || mode == GameMode::MiddleResult) {
				
				//ゆびすま～〇！
				ML::Box2D draw = GameLib::SetBoxByCenter(ge->screenWidth / 2, ge->screenHeight / 2, 15 * this->effectCnt, 23 * this->effectCnt);
				Render_Number(draw.x, draw.y, draw.w, draw.h, this->smashHand, Dir::Left);


				//次へ
				ML::Box2D drawNext(0, 0, 270, 90);
				drawNext.Offset(ge->screenWidth - drawNext.w, 0);
				ML::Box2D srcNext(0, 0, 270, 90);
				this->res->imgTerminal->Draw(drawNext, srcNext);

				if (this->playerturn) {
					//あたりorはずれ
					ML::Box2D drawJudge(draw.x + draw.w * 4 / 5, draw.y + draw.h * 3 / 5, 8 * this->effectCnt, (int)(3.45f * this->effectCnt));
					ML::Box2D srcJudge(0, 0, 199, 86);
					if (!this->equalFlag) {
						srcJudge.Offset(0, 86);
					}
					this->res->imgMiddleResult->Draw(drawJudge, srcJudge);
				}

				
			}

			break;
		case Progress::Result://勝敗
			ML::Box2D draw = GameLib::SetBoxByCenter(ge->screenWidth / 2, ge->screenHeight / 3, 408 * 2, 91 * 2);
			ML::Box2D src;
			if (ge->GameClearFlag) {//プレイヤーの勝ち
				src = ML::Box2D(0, 0, 408, 91);
			}
			else if (ge->GameOverFlag) {//プレイヤーの負け
				src = ML::Box2D(0, 91, 408, 91);
			}
			this->res->imgJudgement->Draw(draw, src);
			
			//終了
			ML::Box2D drawNext = GameLib::SetBoxByCenter(ge->screenWidth / 2, ge->screenHeight * 2 / 3, 270, 90);
			ML::Box2D srcNext(0, 90, 270, 90);
			this->res->imgTerminal->Draw(drawNext, srcNext);
			
			break;
		}
		
		
		
	}

	void Object::Render_Number(int x_, int y_, int w_, int h_, int n_, Object::Dir dir_) {
		//桁数を把握
		int digit;
		if (n_ == 0) { 
			digit = 0; 
		}
		else if (n_ > 0) {
			digit = (int)log10f((float)n_);//n_の桁数
		}
		else { return; }
		//左桁の処理
		for (int i = 0; i < digit + 1; ++i) {
			int bit = n_ % 10;//一桁取得
			ML::Box2D draw(x_ - w_ * i, y_, w_, h_);
			ML::Box2D src(60 * bit, 0, 60, 90);
			this->res->imgNumber->Draw(draw, src);
			n_ /= 10;
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
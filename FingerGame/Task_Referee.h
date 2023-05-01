#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  Referee
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("審判");	//グループ名
	const  string  defName("仮");	//タスク名
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		//「変数宣言を書く」
		
		DG::Image::SP imgPlayer;
		DG::Image::SP imgCPU;
		DG::Image::SP imgSelector;		//先攻後攻の選択
		DG::Image::SP imgFingerLabel;	//あなたの予想する数は
		DG::Image::SP imgExpected;		//予想された数は
		DG::Image::SP imgJudgement;		//あなたの勝ちor負け
		DG::Image::SP imgController;	//操作方法表示
		DG::Image::SP imgMiddleResult;	//あたりorはずれ
		DG::Image::SP imgTerminal;		//次へor終了
		DG::Image::SP imgTurn;			//「あなたorCPU」の番です
		DG::Image::SP imgFirSecAttack;	//あなたは「先攻or後攻」
		DG::Image::SP imgNumber;		//数字
		DG::Image::SP imgDays;			//日目の表示


	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		//「変数宣言を書く」
		//「追加メソッドを書く」

		//------------
		// プレイヤーのターンかどうか（ゲームモード1,進行モード1で動作）
		// true:プレイヤーの番
		// false:敵の番
		// 
		//------------
		bool playerturn;	//プレイヤーの番ならtrue

		//------------
		// ゲームモード（進行モードが1の時に動作）
		// 0全体であげられる手の最大の取得
		// 
		// 1自分の出し目の選択
		// 2上処理の直後に相手の出し目の設定
		// 
		// （演出があれば追加）
		// 判定
		//------------
		enum class GameMode {//GameMode用
			Non,
			RefSet,		//審判設定
			PlayerSet,	//プレイヤー設定
			Judge,		//判定
			EnemySet,	//敵設定
			Effect,		//演出
			MiddleResult,	//途中結果
		};
		GameMode mode;			//敵タスクとプレイヤータスクのモードを管理

		//------------
		// 進行カウンタ
		// 0.先攻後攻の選択
		// 1.ゲームの進行
		// 2.勝敗
		//------------
		//int progressMode;
		enum class Progress {
			Non,
			FirstSecond,//先攻後攻の決定
			Attack,		//あなたは「先攻or後攻」
			Game,		//ゲーム
			Result		//リザルト
		};
		Progress progressMode;

		//------
		// プレイヤーの先攻後攻の選択
		// 0.先攻
		// 1.後攻
		// 2.コンピュータに任せる
		//------
		int attackSelect;

		int hand;		//全体で上げた手
		int smashHand;	//予想された手の数
		int handMax;	//全体で上げられる手

		bool equalFlag;//予想された数と実際の数が同じとき
		//演出
		int effectCnt;//エフェクトカウンタ

		int dayCnt;//デイカウンタ（ターンごとにインクリメント）

		enum class Dir { Right, Left };
		//
		// 
		// x_,y_	一文字目の座標[左方向なら右端,右方向なら左端]
		// w_,h_	一文字当たりの大きさ
		// n_		表示したい数
		// dir		向き
		//	left:000[1]←
		// Right:→[1]000
		//----
		void Render_Number(int x_, int y_, int w_, int h_, int n_, Dir dir_);

	};
}
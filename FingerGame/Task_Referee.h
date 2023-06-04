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

		enum class GameMode {	//ゲームモード
			Non,			//特になし
			RefSet,			//審判設定
			PlayerSet,		//プレイヤー設定
			Judge,			//判定
			EnemySet,		//敵設定
			Effect,			//演出
			MiddleResult,	//途中結果
		};
		GameMode mode;			//敵タスクとプレイヤータスクのモードを管理

		enum class Progress {	//進行モード
			Non,			//特になし
			FirstSecond,	//先攻後攻の決定
			Attack,			//あなたは「先攻or後攻」
			Game,			//ゲーム
			Result			//リザルト
		};
		Progress progressMode;

#if false
		typedef std::map<Referee::Object::GameMode, std::function<void()>> GameModeMap;
		typedef std::map<Progress, std::function<void()>> ProgressMap;
#else
		using GameModeMap = std::map<Referee::Object::GameMode, std::function<void()>>;
		using ProgressMap = std::map<Progress, std::function<void()>>;
#endif
		
		bool isPlayerTurn;	//プレイヤーの番か？
		bool isPredictably;	//予想された数と実際の数が同じか？

		//------
		// プレイヤーの先攻後攻の選択
		// 0.先攻
		// 1.後攻
		// 2.コンピュータに任せる
		//------
		int selectAttackType;

		int hand;			//全体で上げた手
		int smashHand;		//予想された手の数
		int handMax;		//全体で上げられる手

		//演出
		int effectCnt;		//エフェクトカウンタ

		int dayCnt;			//デイカウンタ（ターンごとにインクリメント）

		enum class Dir { Right, Left };
		// x_,y_	一文字目の座標[左方向なら右端,右方向なら左端]
		// w_,h_	一文字当たりの大きさ
		// n_		表示したい数
		// dir		向き
		//	left:000[1]←
		// Right:→[1]000
		//----
		void Number_Render(int x_, int y_, int w_, int h_, int n_, Dir dir_);

	private:
		//関数群配列
		ProgressMap progressUpDateMap, progressRenderMap;
		void ProgressMap_Initialize();
		void ProgressMap_Finalize();

		//進行モードごとの処理
		void Progress_UpDate(Progress progress_);
		void FirstSecond_UpDate();
		void Attack_UpDate();
		void Game_UpDate();

		void Progress_Render(Progress progress_);
		void FirstSecond_Render();
		void Attack_Render();
		void Game_Render();
		void Result_Render();

		//ゲーム動作モードの処理
		GameModeMap gamemodeUpDateMap, gamemodeRenderMap;
		void ModeMap_Initialize();
		void ModeMap_Finalize();

		void GameMode_UpDate(GameMode mode_);
		void Refset_UpDate();
		void Effect_UpDate();
		void Judge_UpDate();
		void MiddleResult_UpDate();

		void GameMode_Render(GameMode mode_);
		void PleyerSet_Render();
		void JudgeMiddleResult_Render();

	};
}
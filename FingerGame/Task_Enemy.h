#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//敵
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  Enemy
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("敵");	//グループ名
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
		DG::Image::SP img;
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

		int enemyHandMax;	//手の最大値
		int enemyHand;		//自分の出せる手の数
		//以下全体処理

		using EnemySetMap = std::map<int, std::function<void()>>;
		EnemySetMap enemySetMap;
	private:
		void EnemySetMap_Initialize();
		void EnemySetMap_Finalize();
		void EnemySetMap_UpDate(bool isEnemyTrun_);

		void EnemySetIfPlayerTurn();//プレイヤーのTurnの場合
		void EnemySetIfAllHand4();
		void EnemySetIfAllHand3();
		void EnemySetIfAllHand2();


	};
}
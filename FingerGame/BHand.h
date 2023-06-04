#pragma once
#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
// 汎用スーパークラス
//-----------------------------------------------------------------------------


#include "GameEngine_Ver3_83.h"

class BHand : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BHand>		SP;
	typedef weak_ptr<BHand>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	//共通メンバ変数
	enum class Dir
	{
		Left,	//左向き
		Right	//右向き
	};
	int handMax;//自分の手の最大値
	int hand;	//自分が出す手

	Dir dir;	//手の向く方向
	//メンバ変数の初期化
	BHand()
		: hand(0)
		, handMax(0)
	{}

	virtual ~BHand(){}

	void UpDate_Std();
	void Render_Std(DG::Image::SP img_, ML::Vec2 pos_);
	//共通メソッド
	

};


#pragma once
#include "myLib.h"

namespace GameLib {
	//オブジェクト設定系

	// オブジェクトの基準を中心にする関数
	// 引数
	// int w_ オブジェクトの幅
	// int h_ オブジェクトの高さ
	// 戻り値
	// ML::Box2D型 
	ML::Box2D SetCenteredBox(int w_, int h_);

	//基準が中心のオブジェクトを指定した位置に配置
	// 引数
	// int x_ 座標x
	// int y_ 座標y
	// int w_ オブジェクトの幅
	// int h_ オブジェクトの高さ
	// 戻り値
	// ML::Box2D型 
	ML::Box2D SetBoxByCenter(int x_, int y_, int w_, int h_);

}
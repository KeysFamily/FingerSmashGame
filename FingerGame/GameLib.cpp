#include "GameLib.h"
namespace MyGameLib {
	ML::Box2D SetCenteredBox(int w_, int h_) {
		return ML::Box2D(-w_ / 2, -h_ / 2, w_, h_);
	}

	ML::Box2D SetCenteredBox(int x_, int y_, int w_, int h_) {
		return ML::Box2D(x_ - w_ / 2, y_ - h_ / 2, w_, h_);
	}

}
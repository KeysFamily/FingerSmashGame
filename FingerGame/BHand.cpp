#include "MyPG.h"
#include "BHand.h"


void BHand::UpDate_Std() {

}
void BHand::Render_Std(DG::Image::SP img_, ML::Vec2 pos_) {

    ML::Box2D drawHandBack(pos_.x, pos_.y, 64 * 3, 85 * 3);   //奥の手
    ML::Box2D drawHandFront(pos_.x, pos_.y, 64 * 3, 85 * 3);  //手前の手

    //手の番号(-1~3で-1は描画しない)
    int handBackNum = -1;    //奥の手の番号
    int handFrontNum = -1;   //手前の手の番号

    //もし手の方向が右なら右を向く
    if (this->dir == Dir::Right) {
        drawHandBack.w *= -1;
        drawHandFront.w *= -1;
    }
    //右手
    if (this->handMax > 0) {
        if (this->dir == Dir::Left) {
            //handFrontNum = (1 3)
        }
        else {
            //handBackNum = (0 2)
        }
    }

    //左手
    if (this->handMax == 2) {
        if (this->dir == Dir::Left) {
            //handBackNum = (0 2);
            handBackNum = (this->hand / 2) * 2;
        }
        else {
            //handFrontNum = (1 3);
            handFrontNum = 1 + (this->hand / 2) * 2;

        }
    }

    //描画する
    //奥
    if (handBackNum >= 0) {
        float ang = ML::ToRadian(this->dir == Dir::Left ? 10.0f : -10.0f);
        img_->Rotation(ang, ML::Vec2(0.0f, 0.0f));
        img_->Draw(drawHandBack, ML::Box2D());
        img_->Rotation(0.0f, ML::Vec2(0.0f, 0.0f));
    }

    //手前
    if (handFrontNum >= 0) {
        float ang = ML::ToRadian(this->dir == Dir::Left ? 10.0f : -10.0f);
        img_->Rotation(ang, ML::Vec2(0.0f, 0.0f));
        img_->Draw(drawHandFront, ML::Box2D());
        img_->Rotation(0.0f, ML::Vec2(0.0f, 0.0f));
    }

}
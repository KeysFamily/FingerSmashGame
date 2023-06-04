#include "MyPG.h"
#include "BHand.h"


void BHand::UpDate_Std() {

}
void BHand::Render_Std(DG::Image::SP img_, ML::Vec2 pos_) {

    ML::Box2D drawHandBack(pos_.x, pos_.y, 64 * 3, 85 * 3);   //���̎�
    ML::Box2D drawHandFront(pos_.x, pos_.y, 64 * 3, 85 * 3);  //��O�̎�

    //��̔ԍ�(-1~3��-1�͕`�悵�Ȃ�)
    int handBackNum = -1;    //���̎�̔ԍ�
    int handFrontNum = -1;   //��O�̎�̔ԍ�

    //������̕������E�Ȃ�E������
    if (this->dir == Dir::Right) {
        drawHandBack.w *= -1;
        drawHandFront.w *= -1;
    }
    //�E��
    if (this->handMax > 0) {
        if (this->dir == Dir::Left) {
            //handFrontNum = (1 3)
        }
        else {
            //handBackNum = (0 2)
        }
    }

    //����
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

    //�`�悷��
    //��
    if (handBackNum >= 0) {
        float ang = ML::ToRadian(this->dir == Dir::Left ? 10.0f : -10.0f);
        img_->Rotation(ang, ML::Vec2(0.0f, 0.0f));
        img_->Draw(drawHandBack, ML::Box2D());
        img_->Rotation(0.0f, ML::Vec2(0.0f, 0.0f));
    }

    //��O
    if (handFrontNum >= 0) {
        float ang = ML::ToRadian(this->dir == Dir::Left ? 10.0f : -10.0f);
        img_->Rotation(ang, ML::Vec2(0.0f, 0.0f));
        img_->Draw(drawHandFront, ML::Box2D());
        img_->Rotation(0.0f, ML::Vec2(0.0f, 0.0f));
    }

}
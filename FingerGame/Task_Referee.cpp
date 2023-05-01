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
	//���\�[�X�̏�����
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
	//���\�[�X�̉��
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
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
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
		//���^�X�N�̐���
		

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//�R���g���[���̏�Ԃ��擾
		auto inp = ge->in1->GetState();

		//�v���C���ƓG�̏�Ԃ��擾
		auto pl = ge->GetTask<Player::Object>(Player::defGroupName, Player::defName);
		auto ene = ge->GetTask<Enemy::Object>(Enemy::defGroupName, Enemy::defName);
		if (pl == nullptr || ene == nullptr) { return; }
		
		switch (this->progressMode)
		{
		case Progress::FirstSecond://��U��U�̑I��
			//�ŏ��l�ő�l�𒴂��Ȃ����߂̏���
			if (inp.LStick.BU.down) {//��
				se::Play("decision");
				this->attackSelect = (this->attackSelect + 1) % 3;
			}
			else if(inp.LStick.BD.down) {//��
				se::Play("decision");
				--this->attackSelect;
				if (this->attackSelect < 0) {
					this->attackSelect = 2;
				}
			}
			//�����������m�肵���Ȃ�
			if (inp.SE.down) {
				se::Play("Select");
				if (this->attackSelect < 2) {
					this->playerturn = (this->attackSelect % 2 == 0);
				}
				else {
					//�����Ō��肷��
					this->playerturn = (GetRandom<int>(0, 9) % 2 == 0);
				}
				//�Q�[�����[�h�Ɉڍs
				this->progressMode = Progress::Attack;
			}
			break;
		case Progress::Attack://���Ȃ��́u�v�̕\��
			if (this->effectCnt < 30) { this->effectCnt += 2; }
			if (inp.SE.down) {
				se::Play("Select");
				this->progressMode = Progress::Game;
			}
			break;
		case Progress::Game://�Q�[����i�s����
			//�����̒���
			
			switch (this->mode)
			{
			case GameMode::RefSet:
				//�S�̂ł��������̎擾�i�\�z�ł��鐔�̍ő�l�����߂�j
				this->handMax = pl->myHandMax + ene->enemyHandMax;
				this->smashHand = 0;
				pl->myHand = 0;
				ene->enemyHand = 0;
				this->mode = GameMode::PlayerSet;
				this->effectCnt = 0;
				break;
			case GameMode::Effect:
				if (this->effectCnt == 0) {
					//�G�t�F�N�g�J�n
					pl->myhandTmp = pl->myHand;//�ꎞ�ۑ�
					pl->myHand = 0;//��x0�N���A

					ge->CreateEffect(98, ML::Vec2());//�t�F�[�h
					ge->CreateEffect(10, ML::Vec2());//��т��܁`
					//����
					se::Play(this->playerturn ? "player" : "cpu");
				}
				if (++this->effectCnt > 100) {
					//�G�t�F�N�g�J�n������b�������Ă��画����J�n����
					this->effectCnt = 0;
					this->mode = GameMode::EnemySet;

					pl->myHand = pl->myhandTmp;
					pl->myhandTmp = 0;
				}
				break;
			case GameMode::Judge://����
				//����
				{
					string seName;
					seName = (this->playerturn ? "player" : "cpu") + to_string(this->smashHand);
					se::Play(seName);
				}
				//�S�̂ł�����ꂽ����擾
				this->hand = pl->myHand + ene->enemyHand;
				this->equalFlag = this->hand == this->smashHand;
				mode = GameMode::MiddleResult;

				break;
			case GameMode::MiddleResult:
				if (this->effectCnt < 30) { this->effectCnt += 2; }


				//A�L�[�������ꂽ��
				if (inp.SE.down) {
					se::Play("Select");
					//�ǂ��炩�̎w����ɍ~�肽�烋�[�v�𔲂���
					//�\�z���ꂽ�l�ƑS�̂ł�����ꂽ�l�������Ȃ�i�\�z�ʂ�Ȃ�j�\�z�������̂�������w�̐������炷
					if (this->equalFlag) {
						if (this->playerturn) {
							--pl->myHandMax;
						}
						else {
							--ene->enemyHandMax;
						}
					}
					//�ǂ��炩�̎w�̐����Ȃ��Ȃ����玟��
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
						//�Ȃ��Ȃ��Ă��Ȃ���΃^�[����؂�ւ���
						this->playerturn = !this->playerturn;

						//�R�����[�h�ɖ߂�
						this->mode = GameMode::RefSet;

						//�f�C�J�E���g
						++dayCnt;
					}
				}
				break;

			default:
				break;
			}
			
			break;
		case Progress::Result://���s�̕\���i�^�X�N�J�ځj
			
			break;
		default:
			break;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		auto game = ge->GetTask<Game::Object>(Game::defGroupName, Game::defName);
		if (game->isPause) { return; }
		//���[�h�ɂ���ĐF������ς��邽�߂̂���
		ML::Color white(1.0f, 1.0f, 1.0f, 1.0f);
		ML::Color red(1.0f, 1.0f, 0.7f, 0.0f);

		switch (this->progressMode) {
		case Progress::FirstSecond://��U��U�̑I��
			
			{
				//�ǂ�����
				ML::Box2D draw(150, 230, 956, 137);
				ML::Box2D src(0, 0, 637, 91);//���̒l��1.5�{�ŕ`��
				this->res->imgSelector->Draw(draw, src);
			}
			{
				//���ڕ\��(��U/��U/�R���s���[�^�����߂�)
				ML::Box2D drawSel(150, ge->screenHeight / 2, 956, 137);
				ML::Box2D srcSel(0, 91 * (this->attackSelect + 1), 637, 91);//���̒l��1.5�{
				this->res->imgSelector->Draw(drawSel, srcSel);
			}
			{	//������@�\��
				
				//���Ԃ̑I��
				ML::Box2D draw1(0, 0, 560, 92);
				draw1.Offset((int)ge->screenWidth - draw1.w, (int)ge->screenHeight - draw1.h * 2);
				ML::Box2D src1(0, 92 * 2, 560, 92);
				//����
				ML::Box2D draw2(0, 0, 560, 92);
				draw2.Offset((int)ge->screenWidth - draw2.w, (int)ge->screenHeight - draw2.h);
				ML::Box2D src2(0, 0, 560, 92);

				this->res->imgController->Draw(draw1, src1);
				this->res->imgController->Draw(draw2, src2);
			}
			break;
		case Progress::Attack:
			{
				//��U�A��U
				ML::Box2D draw = GameLib::SetBoxByCenter(ge->screenWidth / 2, ge->screenHeight / 2, 30 * this->effectCnt, 15 * this->effectCnt);
				//draw = GameLib::SetBoxByCenter(ge->screenWidth / 2, ge->screenHeight / 2, 64, 84);
				ML::Box2D src(0, this->playerturn ? 90 : 180, 180, 90);
				this->res->imgFirSecAttack->Draw(draw, src);
				
				//���Ȃ��́H
				ML::Box2D drawYour(0, 0, 9 * this->effectCnt, 3 * this->effectCnt);
				drawYour.Offset(draw.x - drawYour.w / 2, draw.y - drawYour.h / 2);
				//draw = GameLib::SetBoxByCenter(ge->screenWidth / 2, ge->screenHeight / 2, 64, 84);
				ML::Box2D srcYour(0, 0, 270, 90);
				this->res->imgFirSecAttack->Draw(drawYour, srcYour);

				//����
				ML::Box2D drawNext(0, 0, 320, 92);
				drawNext.Offset((int)ge->screenWidth - drawNext.w, (int)ge->screenHeight - drawNext.h);
				ML::Box2D srcNext(0, 0, 320, 92);
				this->res->imgController->Draw(drawNext, srcNext);
			}
			break;
		case Progress::Game://�U��
			//����
			{
				//�G�\��
				ML::Box2D draw(0, 630, 220, 100);
				ML::Box2D src(0, 0, 221, 97);
				this->res->imgPlayer->Draw(draw, src, this->playerturn ? red : white);
			}
			{
				//�v���C���[�\��
				ML::Box2D draw(1130, 630, 150, 90);
				ML::Box2D src(0, 0, 149, 90);
				this->res->imgCPU->Draw(draw, src, !this->playerturn ? red : white);
			}
			{
				//���Ȃ�orCPU�̔Ԃł�
				ML::Box2D draw(0, 0, 540,68);//360 * 1.5f, 45 * 1.5f
				ML::Box2D src(0, this->playerturn ? 0 : 90, 720, 90);
				this->res->imgTurn->Draw(draw, src);
			}
			//�o�ߎ��Ԃ̕\��
			Render_Number(800, ge->screenHeight - 90, 60, 90, this->dayCnt / 3 + 1, Dir::Left);
			{
				ML::Box2D draw(860, ge->screenHeight - 90, 150, 90);
				ML::Box2D src(0, 0, 147, 87);
				this->res->imgDays->Draw(draw, src);
			}

			//�X
			if (mode == GameMode::PlayerSet) {
				
				//������@�\��
				{
					//�w�����̏o���w�̐��x
					ML::Box2D draw1(0, 0, 771 / 2, 92 / 2);
					draw1.Offset(ge->screenWidth - draw1.w, 0);
					ML::Box2D src1(0, 92 * 3, 771, 92);

					//�w����x
					ML::Box2D draw2(0, 0, 771 / 2, 92 / 2);
					draw2.Offset(ge->screenWidth - draw2.w, this->playerturn ? draw1.h * 2 : draw1.h);
					ML::Box2D src2(0, 0, 771, 92);

					this->res->imgController->Draw(draw1, src1);
					this->res->imgController->Draw(draw2, src2);
				}
				//�v���C���[�̍U�����̂�
				if (this->playerturn) {
					{
						//�y�\�z���ꂽ���z�\��
						ML::Box2D draw(300, 300, 536, 90);
						ML::Box2D src(0, 0, 536, 90);
						this->res->imgFingerLabel->Draw(draw, src);
					}
					{
						//�\�z���ꂽ�y���z�\��
						
						Render_Number(840, 305, 64, 84, this->smashHand, Dir::Left);
					}
					{
						//�w�\�z����w�̐��x
						ML::Box2D draw3(0, 0, 771 / 2, 92 / 2);
						draw3.Offset(ge->screenWidth - draw3.w, draw3.h);
						ML::Box2D src3(0, 92, 771, 92);
						this->res->imgController->Draw(draw3, src3);
					}

				}

			}
			else if (mode == GameMode::Effect) {
				//�����Ȃ�
			}
			else if (mode == GameMode::Judge || mode == GameMode::MiddleResult) {
				
				//��т��܁`�Z�I
				ML::Box2D draw = GameLib::SetBoxByCenter(ge->screenWidth / 2, ge->screenHeight / 2, 15 * this->effectCnt, 23 * this->effectCnt);
				Render_Number(draw.x, draw.y, draw.w, draw.h, this->smashHand, Dir::Left);


				//����
				ML::Box2D drawNext(0, 0, 270, 90);
				drawNext.Offset(ge->screenWidth - drawNext.w, 0);
				ML::Box2D srcNext(0, 0, 270, 90);
				this->res->imgTerminal->Draw(drawNext, srcNext);

				if (this->playerturn) {
					//������or�͂���
					ML::Box2D drawJudge(draw.x + draw.w * 4 / 5, draw.y + draw.h * 3 / 5, 8 * this->effectCnt, (int)(3.45f * this->effectCnt));
					ML::Box2D srcJudge(0, 0, 199, 86);
					if (!this->equalFlag) {
						srcJudge.Offset(0, 86);
					}
					this->res->imgMiddleResult->Draw(drawJudge, srcJudge);
				}

				
			}

			break;
		case Progress::Result://���s
			ML::Box2D draw = GameLib::SetBoxByCenter(ge->screenWidth / 2, ge->screenHeight / 3, 408 * 2, 91 * 2);
			ML::Box2D src;
			if (ge->GameClearFlag) {//�v���C���[�̏���
				src = ML::Box2D(0, 0, 408, 91);
			}
			else if (ge->GameOverFlag) {//�v���C���[�̕���
				src = ML::Box2D(0, 91, 408, 91);
			}
			this->res->imgJudgement->Draw(draw, src);
			
			//�I��
			ML::Box2D drawNext = GameLib::SetBoxByCenter(ge->screenWidth / 2, ge->screenHeight * 2 / 3, 270, 90);
			ML::Box2D srcNext(0, 90, 270, 90);
			this->res->imgTerminal->Draw(drawNext, srcNext);
			
			break;
		}
		
		
		
	}

	void Object::Render_Number(int x_, int y_, int w_, int h_, int n_, Object::Dir dir_) {
		//������c��
		int digit;
		if (n_ == 0) { 
			digit = 0; 
		}
		else if (n_ > 0) {
			digit = (int)log10f((float)n_);//n_�̌���
		}
		else { return; }
		//�����̏���
		for (int i = 0; i < digit + 1; ++i) {
			int bit = n_ % 10;//�ꌅ�擾
			ML::Box2D draw(x_ - w_ * i, y_, w_, h_);
			ML::Box2D src(60 * bit, 0, 60, 90);
			this->res->imgNumber->Draw(draw, src);
			n_ /= 10;
		}

	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
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
	//���\�[�X�N���X�̐���
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
//-------------------------------------------------------------------
// �R���^�X�N
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Referee.h"

#include  "sound.h"
#include  "randomLib.h"

#include  "Task_Game.h"
#include  "Task_Player.h"
#include  "Task_Enemy.h"

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

		this->isPlayerTurn = false;
		this->mode = GameMode::RefSet;
		this->progressMode = Progress::FirstSecond;
		this->selectAttackType = 0;

		this->hand = 0;
		this->smashHand = 0;
		this->handMax = 0;

		this->effectCnt = 0;
		this->isPredictably = false;

		this->dayCnt = 0;

		ge->GameClearFlag = ge->GameOverFlag = false;
		//���^�X�N�̐���

		this->ProgressMap_Initialize();
		this->ModeMap_Initialize();

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		this->ModeMap_Finalize();
		this->ProgressMap_Finalize();

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
		

		this->Progress_UpDate(this->progressMode);
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		auto game = ge->GetTask<Game::Object>(Game::defGroupName, Game::defName);
		if (game->isPause) { return; }

		this->Progress_Render(this->progressMode);

	}
	//-------------------------------------------------------------------
	// �����`��֐�
	//-------------------------------------------------------------------
	void Object::Number_Render(int x_, int y_, int w_, int h_, int n_, Object::Dir dir_) {
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
	//-------------------------------------------------------------------
	// �i�s���[�h�����s���̏���
	//-------------------------------------------------------------------
	void Object::ProgressMap_Initialize() {
		//�X�V
		progressUpDateMap[Progress::FirstSecond] = std::bind(&Referee::Object::FirstSecond_UpDate, this);
		progressUpDateMap[Progress::Attack] = std::bind(&Referee::Object::Attack_UpDate, this);
		progressUpDateMap[Progress::Game] = std::bind(&Referee::Object::Game_UpDate, this);

		//�`��
		progressRenderMap[Progress::FirstSecond] = std::bind(&Referee::Object::FirstSecond_Render, this);
		progressRenderMap[Progress::Attack] = std::bind(&Referee::Object::Attack_Render, this);
		progressRenderMap[Progress::Game] = std::bind(&Referee::Object::Game_Render, this);
		progressRenderMap[Progress::Result] = std::bind(&Referee::Object::Result_Render, this);
	}
	void Object::ProgressMap_Finalize() {
		this->progressUpDateMap.clear();
		this->progressRenderMap.clear();
	}

	// ���s���[�h�X�V����
	void Object::Progress_UpDate(Progress progress_) {
		//�I�����ꂽ�i�s���[�h�̊֐������݂��邩�H ���݂��Ȃ����0�C�����1��Ԃ�
		if (progressUpDateMap.count(progress_) == 1) {
			progressUpDateMap[progress_]();
		}
	}
	void Object::FirstSecond_UpDate() {
		auto inp = ge->in1->GetState();
		//�ŏ��l�ő�l�𒴂��Ȃ����߂̏���
		if (inp.LStick.BU.down) {//��
			se::Play("decision");
			this->selectAttackType = (this->selectAttackType + 1) % 3;
		}
		else if (inp.LStick.BD.down) {//��
			se::Play("decision");
			--this->selectAttackType;
			if (this->selectAttackType < 0) {
				this->selectAttackType = 2;
			}
		}
		//�����������m�肵���Ȃ�
		if (inp.SE.down) {
			se::Play("Select");
			if (this->selectAttackType < 2) {
				this->isPlayerTurn = (this->selectAttackType % 2 == 0);
			}
			else {
				//�����Ō��肷��
				this->isPlayerTurn = (GetRandom<int>(0, 9) % 2 == 0);
			}
			//�Q�[�����[�h�Ɉڍs
			this->progressMode = Progress::Attack;
		}
	}
	void Object::Attack_UpDate() {
		auto inp = ge->in1->GetState();

		if (this->effectCnt < 30) { this->effectCnt += 2; }
		if (inp.SE.down) {
			se::Play("Select");
			this->progressMode = Progress::Game;
		}
	}
	void Object::Game_UpDate() {
		GameMode_UpDate(this->mode);
	}
	
	//-------------------------------------------------------------------
	// �i�s���[�h�`��
	void Object::Progress_Render(Progress progress_) {
		if (progressRenderMap.count(progress_) == 1) {
			progressRenderMap[progress_]();
		}
	}
	void Object::FirstSecond_Render() {
		{
			//�ǂ�����
			ML::Box2D draw(150, 230, 956, 137);
			ML::Box2D src(0, 0, 637, 91);//���̒l��1.5�{�ŕ`��
			this->res->imgSelector->Draw(draw, src);
		}
		{
			//���ڕ\��(��U/��U/�R���s���[�^�����߂�)
			ML::Box2D drawSel(150, ge->screenHeight / 2, 956, 137);
			ML::Box2D srcSel(0, 91 * (this->selectAttackType + 1), 637, 91);//���̒l��1.5�{
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
	}
	void Object::Attack_Render() {
		//��U�A��U
		ML::Box2D draw = MyGameLib::SetCenteredBox(ge->screenWidth / 2, ge->screenHeight / 2, 30 * this->effectCnt, 15 * this->effectCnt);
		//draw = MyGameLib::SetCenteredBox(ge->screenWidth / 2, ge->screenHeight / 2, 64, 84);
		ML::Box2D src(0, this->isPlayerTurn ? 90 : 180, 180, 90);
		this->res->imgFirSecAttack->Draw(draw, src);

		//���Ȃ��́H
		ML::Box2D drawYour(0, 0, 9 * this->effectCnt, 3 * this->effectCnt);
		drawYour.Offset(draw.x - drawYour.w / 2, draw.y - drawYour.h / 2);
		//draw = MyGameLib::SetCenteredBox(ge->screenWidth / 2, ge->screenHeight / 2, 64, 84);
		ML::Box2D srcYour(0, 0, 270, 90);
		this->res->imgFirSecAttack->Draw(drawYour, srcYour);

		//����
		ML::Box2D drawNext(0, 0, 320, 92);
		drawNext.Offset((int)ge->screenWidth - drawNext.w, (int)ge->screenHeight - drawNext.h);
		ML::Box2D srcNext(0, 0, 320, 92);
		this->res->imgController->Draw(drawNext, srcNext);
	}
	void Object::Game_Render() {
		GameMode_Render(this->mode);
	}
	void Object::Result_Render() {
		ML::Box2D draw = MyGameLib::SetCenteredBox(ge->screenWidth / 2, ge->screenHeight / 3, 408 * 2, 91 * 2);
		ML::Box2D src;
		if (ge->GameClearFlag) {//�v���C���[�̏���
			src = ML::Box2D(0, 0, 408, 91);
		}
		else if (ge->GameOverFlag) {//�v���C���[�̕���
			src = ML::Box2D(0, 91, 408, 91);
		}
		this->res->imgJudgement->Draw(draw, src);

		//�I��
		ML::Box2D drawNext = MyGameLib::SetCenteredBox(ge->screenWidth / 2, ge->screenHeight * 2 / 3, 270, 90);
		ML::Box2D srcNext(0, 90, 270, 90);
		this->res->imgTerminal->Draw(drawNext, srcNext);
	}

	//-------------------------------------------------------------------
	// �Q�[�����[�h���̏���
	//-------------------------------------------------------------------
	void Object::ModeMap_Initialize() {
		//�X�V
		gamemodeUpDateMap[GameMode::RefSet] = std::bind(&Referee::Object::Refset_UpDate, this);
		gamemodeUpDateMap[GameMode::Effect] = std::bind(&Referee::Object::Effect_UpDate, this);
		gamemodeUpDateMap[GameMode::Judge] = std::bind(&Referee::Object::Judge_UpDate, this);
		gamemodeUpDateMap[GameMode::MiddleResult] = std::bind(&Referee::Object::MiddleResult_UpDate, this);

		//�`��
		gamemodeRenderMap[GameMode::PlayerSet] = std::bind(&Referee::Object::PleyerSet_Render, this);
		gamemodeRenderMap[GameMode::Judge] = std::bind(&Referee::Object::JudgeMiddleResult_Render, this);
		gamemodeRenderMap[GameMode::MiddleResult] = std::bind(&Referee::Object::JudgeMiddleResult_Render, this);
	}
	void Object::ModeMap_Finalize() {
		this->gamemodeUpDateMap.clear();
		this->gamemodeRenderMap.clear();
	}

	//-------------------------------------------------------------------
	// �X�V����
	void Object::GameMode_UpDate(GameMode mode_) {
		if (mode_ == GameMode::PlayerSet) {
			bool flag = true;
		}
		if (gamemodeUpDateMap.count(mode_) == 1) {
			gamemodeUpDateMap[mode_]();
		}
	}

	void Object::Refset_UpDate() {
		auto pl = ge->GetTask<Player::Object>(Player::defGroupName, Player::defName);
		auto ene = ge->GetTask<Enemy::Object>(Enemy::defGroupName, Enemy::defName);
		if (pl == nullptr || ene == nullptr) { return; }

		//�S�̂ł��������̎擾�i�\�z�ł��鐔�̍ő�l�����߂�j
		this->handMax = pl->myHandMax + ene->enemyHandMax;
		this->smashHand = 0;
		pl->myHand = 0;
		ene->enemyHand = 0;
		this->mode = GameMode::PlayerSet;
		this->effectCnt = 0;
	}
	void Object::Effect_UpDate() {
		auto pl = ge->GetTask<Player::Object>(Player::defGroupName, Player::defName);
		if (pl == nullptr) { return; }

		if (this->effectCnt == 0) {
			//�G�t�F�N�g�J�n
			pl->myhandTmp = pl->myHand;//�ꎞ�ۑ�
			pl->myHand = 0;//��x0�N���A

			ge->CreateEffect(98, ML::Vec2());//�t�F�[�h
			ge->CreateEffect(10, ML::Vec2());//��т��܁`
			//����
			se::Play(this->isPlayerTurn ? "player" : "cpu");
		}
		if (++this->effectCnt > 100) {
			//�G�t�F�N�g�J�n������b�������Ă��画����J�n����
			this->effectCnt = 0;
			this->mode = GameMode::EnemySet;

			pl->myHand = pl->myhandTmp;
			pl->myhandTmp = 0;
		}
	}
	void Object::Judge_UpDate() {
		auto pl = ge->GetTask<Player::Object>(Player::defGroupName, Player::defName);
		auto ene = ge->GetTask<Enemy::Object>(Enemy::defGroupName, Enemy::defName);
		if (pl == nullptr || ene == nullptr) { return; }

		//����
		{
			string seName;
			seName = (this->isPlayerTurn ? "player" : "cpu") + to_string(this->smashHand);
			se::Play(seName);
		}
		//�S�̂ł�����ꂽ����擾
		this->hand = pl->myHand + ene->enemyHand;
		this->isPredictably = this->hand == this->smashHand;
		mode = GameMode::MiddleResult;
	}
	void Object::MiddleResult_UpDate() {
		auto inp = ge->in1->GetState();
		auto pl = ge->GetTask<Player::Object>(Player::defGroupName, Player::defName);
		auto ene = ge->GetTask<Enemy::Object>(Enemy::defGroupName, Enemy::defName);
		if (pl == nullptr || ene == nullptr) { return; }

		if (this->effectCnt < 30) { this->effectCnt += 2; }
		//A�L�[�������ꂽ��
		if (inp.SE.down) {
			se::Play("Select");
			//�ǂ��炩�̎w����ɍ~�肽�烋�[�v�𔲂���
			//�\�z���ꂽ�l�ƑS�̂ł�����ꂽ�l�������Ȃ�i�\�z�ʂ�Ȃ�j�\�z�������̂�������w�̐������炷
			if (this->isPredictably) {
				if (this->isPlayerTurn) {
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
				this->isPlayerTurn = !this->isPlayerTurn;

				//�R�����[�h�ɖ߂�
				this->mode = GameMode::RefSet;

				//�f�C�J�E���g
				++dayCnt;
			}
		}
	}

	//-------------------------------------------------------------------
	// �`�揈��
	void Object::GameMode_Render(GameMode mode_) {
		ML::Color white(1.0f, 1.0f, 1.0f, 1.0f);
		ML::Color red(1.0f, 1.0f, 0.7f, 0.0f);

		//����
		{
			//�G�\��
			ML::Box2D draw(0, 630, 220, 100);
			ML::Box2D src(0, 0, 221, 97);
			this->res->imgPlayer->Draw(draw, src, this->isPlayerTurn ? red : white);
		}
		{
			//�v���C���[�\��
			ML::Box2D draw(1130, 630, 150, 90);
			ML::Box2D src(0, 0, 149, 90);
			this->res->imgCPU->Draw(draw, src, !this->isPlayerTurn ? red : white);
		}
		{
			//���Ȃ�orCPU�̔Ԃł�
			ML::Box2D draw(0, 0, 540, 68);//360 * 1.5f, 45 * 1.5f
			ML::Box2D src(0, this->isPlayerTurn ? 0 : 90, 720, 90);
			this->res->imgTurn->Draw(draw, src);
		}
		//�o�ߎ��Ԃ̕\��
		Number_Render(800, ge->screenHeight - 90, 60, 90, this->dayCnt / 3 + 1, Dir::Left);
		{
			ML::Box2D draw(860, ge->screenHeight - 90, 150, 90);
			ML::Box2D src(0, 0, 147, 87);
			this->res->imgDays->Draw(draw, src);
		}
		
		if (gamemodeRenderMap.count(mode_) == 1) {
			gamemodeRenderMap[mode_]();
		}
	}

	void Object::PleyerSet_Render() {
		//������@�\��
		{
			//�w�����̏o���w�̐��x
			ML::Box2D draw1(0, 0, 771 / 2, 92 / 2);
			draw1.Offset(ge->screenWidth - draw1.w, 0);
			ML::Box2D src1(0, 92 * 3, 771, 92);

			//�w����x
			ML::Box2D draw2(0, 0, 771 / 2, 92 / 2);
			draw2.Offset(ge->screenWidth - draw2.w, this->isPlayerTurn ? draw1.h * 2 : draw1.h);
			ML::Box2D src2(0, 0, 771, 92);

			this->res->imgController->Draw(draw1, src1);
			this->res->imgController->Draw(draw2, src2);
		}
		//�v���C���[�̍U�����̂�
		if (this->isPlayerTurn) {
			{
				//�y�\�z���ꂽ���z�\��
				ML::Box2D draw(300, 300, 536, 90);
				ML::Box2D src(0, 0, 536, 90);
				this->res->imgFingerLabel->Draw(draw, src);
			}
			{
				//�\�z���ꂽ�y���z�\��

				Number_Render(840, 305, 64, 84, this->smashHand, Dir::Left);
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
	void Object::JudgeMiddleResult_Render() {
		//��т��܁`�Z�I
		ML::Box2D draw = MyGameLib::SetCenteredBox(ge->screenWidth / 2, ge->screenHeight / 2, 15 * this->effectCnt, 23 * this->effectCnt);
		Number_Render(draw.x, draw.y, draw.w, draw.h, this->smashHand, Dir::Left);

		//����
		ML::Box2D drawNext(0, 0, 270, 90);
		drawNext.Offset(ge->screenWidth - drawNext.w, 0);
		ML::Box2D srcNext(0, 0, 270, 90);
		this->res->imgTerminal->Draw(drawNext, srcNext);

		if (this->isPlayerTurn) {
			//������or�͂���
			ML::Box2D drawJudge(draw.x + draw.w * 4 / 5, draw.y + draw.h * 3 / 5, 8 * this->effectCnt, (int)(3.45f * this->effectCnt));
			ML::Box2D srcJudge(0, 0, 199, 86);
			if (!this->isPredictably) {
				srcJudge.Offset(0, 86);
			}
			this->res->imgMiddleResult->Draw(drawJudge, srcJudge);
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
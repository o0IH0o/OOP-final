/*
 * mygame.h: ���ɮ��x�C��������class��interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID  				// �w�q�U�ح��Ī��s��
{
    AUDIO_DING,				// 0
    AUDIO_LAKE,				// 1
    AUDIO_NTUT,				// 2
	AUDIO_TOUCH				// 3
};

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// �o��class���ѥi�H����L�ηƹ�������l
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

class Sniper
{
    public:
        Sniper();
        int  GetSightX1();					// �Ǥ� x �y��
        int  GetSightY1();					// �Ǥ� y �y��
        void Initialize();				// �]�w�Ǥ߬���l��
        void LoadBitmap();				// ���J�ϧ�
        void OnMove();					// ���ʷǤ�
        void OnShow();					// �N�Ǥ߹ϧζK��e��
        void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
        void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
        void SetMovingRight(bool flag); // �]�w�O�_���b���k����
        void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
        void SetSightXY(int nx, int ny);		// �]�w�Ǥߥ��W���y��
    protected:
        CAnimation animation;		// ���l���ʵe
        int x, y;					// ���l���W���y��
        bool isMovingDown;			// �O�_���b���U����
        bool isMovingLeft;			// �O�_���b��������
        bool isMovingRight;			// �O�_���b���k����
        bool isMovingUp;			// �O�_���b���W����
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���ѷ|�@���u�����y
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

class Statue
{
public:
	Statue();
	void LoadBitmap();										// ���J�ϧ�
	void OnMove();											// ����
	void OnShow();											// �N�ϧζK��e��
	void SetXY(int nx, int ny);								// �]�w��ߪ��y��
	int GetX();
	int GetY();
	int GetWight();
	int GetHeight();
protected:
	CMovingBitmap bmp;			// �y����
	int x, y;					// ��ߪ��y��
private:
	CAnimation animation;
};

class Runner
{
    public:
        Runner();
		void LoadBitmap();		// ���J�ϧ�
        bool IsAlive();											// �O�_����
        void SetIsAlive(bool alive);							// �]�w�O�_����
        bool BeKill(Sniper* sight);// �O�_�Q����
		bool Touch(Statue* statue);
        void OnMove();			// ����
        void OnShow();			// �N�ϧζK��e��
        void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
        void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
        void SetMovingRight(bool flag); // �]�w�O�_���b���k����
        void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
        void Initialize();				// �]�w�H������l��
        void SetXY(int, int);
        void SetDirection(int flag);
		void SetStatue(int i);
		bool GetStatue(int i);
		bool Check();
        int GetX();
        int GetY();
        int GetWidth();
        int GetHeight();
    protected:
        CMovingBitmap bmp;			// �y����

    private:
        bool HitSight(int tx1, int ty1);	// �O�_�b�ǤߤW
		bool TouchStatue(int tx1, int ty1, int tx2, int ty2);
        int x, y, direction,statue[5];				// �ϧήy��
        bool isMovingDown;			// �O�_���b���U����
        bool isMovingLeft;			// �O�_���b��������
        bool isMovingRight;			// �O�_���b���k����
        bool isMovingUp;			// �O�_���b���W����
        bool is_alive;				// �O�_����
        CAnimation animation1, animation2, animation3, animation4;	// �Q�ΰʵe�@�ϧ�
};

/////////////////////////////////////////////////////////////////////////////
// �o��class����¶��骺�y
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState
{
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
        void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
    protected:
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        CMovingBitmap logo;								// csie��logo
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////
class CGameStateRun : public CGameState
{
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        const int		NUMBALLS;	// �y���`��
        int				player1, player2, player3;//�����H��
        CMovingBitmap	background;	// �I����
        CMovingBitmap	help;		// ������
        CMovingBitmap	corner;		// ������
        Sniper			sight;		// ������
        CInteger		hits_left;	// �ѤU��������
        Runner   runner[19];		// �]��
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState
{
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        int counter;	// �˼Ƥ��p�ƾ�
        CMovingBitmap SniperEnd;
        CMovingBitmap Player1End;
        CMovingBitmap Player2End;
        CMovingBitmap Player3End;
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C������檬�A(Choose Game)
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateChoose : public CGameState
{
    public:
        CGameStateChoose(CGame* g);
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnKeyUp(UINT, UINT, UINT);
        void MoveArrow(int z);
        void SetXY(int nx, int ny);
        int GetY();
    protected:
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
        int x = 330, y = 370, z, choose = 0;
    private:
        int gameplay, space = 0;
        CMovingBitmap background;								// csie��logo
        CMovingBitmap arrow;									//arrow
        CMovingBitmap deathrace;                                //how to play
        CMovingBitmap ninjaparty;
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C����Ninja party���A(ninja party)
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateNinja : public CGameState
{
    public:
        CGameStateNinja(CGame* g);
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        int				player1, player2, player3;//�����H��
        CMovingBitmap	background;	// �I����
        Runner   runner[19];		// �]��
        Statue   statue[5];
};

}
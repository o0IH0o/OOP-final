/*
 * mygame.cpp: ���ɮ��x�C��������class��implementation
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
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"

namespace game_framework
{
int x = 10;
/////////////////////////////////////////////////////////////////////////////
// CBall: Ball class
/////////////////////////////////////////////////////////////////////////////

Statue::Statue()
{
}

void Statue::LoadBitmap()
{
    animation.AddBitmap(".\\bitmaps\\statuee.bmp", RGB(50, 100, 150));
}

void Statue::OnMove()
{
}

void Statue::SetXY(int nx, int ny)
{
    x = nx;
    y = ny;
}

int Statue::GetX()
{
	return x;
}

int Statue::GetY()
{
	return y;
}

int Statue::GetWight()
{
	return animation.Width();
}

int Statue::GetHeight()
{
	return animation.Height();
}

void Statue::OnShow()
{
    animation.SetTopLeft(x, y);
    animation.OnShow();
}
/////////////////////////////////////////////////////////////////////////////
// CBouncingBall: BouncingBall class
/////////////////////////////////////////////////////////////////////////////

Runner::Runner()
{
    Initialize();
}

bool Runner::IsAlive()
{
    return is_alive;
}

void Runner::SetIsAlive(bool alive)
{
    is_alive = alive;
}

bool Runner::BeKill(Sniper* eraser)
{
    return HitSight(eraser->GetSightX1(), eraser->GetSightY1());
}

bool Runner::Touch(Statue * statue)
{
	return TouchStatue(statue->GetX(), statue->GetY(), statue->GetX()+statue->GetWight(), statue->GetY()+statue->GetHeight());
}

void Runner::LoadBitmap()
{
    char* filename[16] = { ".\\bitmaps\\walker1.bmp", ".\\bitmaps\\walker2.bmp", ".\\bitmaps\\walker3.bmp", ".\\bitmaps\\walker4.bmp", ".\\bitmaps\\walker5.bmp", ".\\bitmaps\\walker6.bmp", ".\\bitmaps\\walker7.bmp", ".\\bitmaps\\walker8.bmp", ".\\bitmaps\\walker9.bmp", ".\\bitmaps\\walker10.bmp", ".\\bitmaps\\walker11.bmp", ".\\bitmaps\\walker12.bmp", ".\\bitmaps\\walker13.bmp", ".\\bitmaps\\walker14.bmp", ".\\bitmaps\\walker15.bmp", ".\\bitmaps\\walker16.bmp" };

    for (int i = 0; i < 4; i++)	// ���J�ʵe(��4�i�ϧκc��)
        animation1.AddBitmap(filename[i], RGB(255, 255, 255));

    for (int i = 4; i < 8; i++)	// ���J�ʵe(��4�i�ϧκc��)
        animation2.AddBitmap(filename[i], RGB(255, 255, 255));

    for (int i = 8; i < 12; i++)	// ���J�ʵe(��4�i�ϧκc��)
        animation3.AddBitmap(filename[i], RGB(255, 255, 255));

    for (int i = 12; i < 16; i++)	// ���J�ʵe(��4�i�ϧκc��)
        animation4.AddBitmap(filename[i], RGB(255, 255, 255));
}

void Runner::OnMove()
{
    const int STEP_SIZE = 2;

    if (isMovingRight)
    {
        if (x + animation1.Width() >= 1280) {}
        else
        {
            this->x += STEP_SIZE; // ����@��animation.OnMove()�Aanimation�~�|����
            animation1.OnMove();
        }
    }

    if (isMovingDown)
    {
        if (y + animation3.Height() >= 960) {}
        else
        {
            this->y += STEP_SIZE;
            animation3.OnMove();
        }
    }

    if (isMovingLeft)
    {
        if (x <= 0) {}
        else
        {
            this->x -= STEP_SIZE;
            animation2.OnMove();
        }
    }

    if (isMovingUp)
    {
        if (y <= 50) {}
        else
        {
            this->y -= STEP_SIZE;
            animation4.OnMove();
        }
    }
}

void Runner::OnShow()
{
    animation1.SetTopLeft(x, y);
    animation2.SetTopLeft(x, y);
    animation3.SetTopLeft(x, y);
    animation4.SetTopLeft(x, y);

    if (direction == 1)
    {
        animation1.OnShow();
    }

    if (direction == 2)
    {
        animation2.OnShow();
    }

    if (direction == 3)
    {
        animation3.OnShow();
    }

    if (direction == 4)
    {
        animation4.OnShow();
    }
}

void Runner::SetMovingDown(bool flag)
{
    isMovingDown = flag;
}

void Runner::SetMovingLeft(bool flag)
{
    isMovingLeft = flag;
}

void Runner::SetMovingRight(bool flag)
{
    isMovingRight = flag;
}

void Runner::SetMovingUp(bool flag)
{
    isMovingUp = flag;
}

void Runner::Initialize()
{
    const int X_POS = 100;
    const int Y_POS = 400;
    x = X_POS;
    y = Y_POS;
    isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
    is_alive = true;
    direction = 0;
	for (int i = 0; i < 5; i++) {
		statue[i] = 0;
	}
}

void Runner::SetXY(int nx, int ny)
{
    x = nx;
    y = ny;
}

void Runner::SetDirection(int flag)
{
    direction = flag;
}

void Runner::SetStatue(int i)
{
	statue[i] = 1;
}

bool Runner::GetStatue(int i)
{
	return statue[i]==0;
}

bool Runner::Check()
{
	return (statue[0] == 1 && statue[1] == 1 && statue[2] == 1 && statue[3] == 1 && statue[4] == 1);
}

int Runner::GetX()
{
    return x;
}
int Runner::GetY()
{
    return y;
}
int Runner::GetWidth()
{
    return animation1.Width();
}
int Runner::GetHeight()
{
    return animation1.Height();
}
bool Runner::HitSight(int tx1, int ty1)
{
    int x1 = x;					// �H�������W��x�y��
    int y1 = y;					// �H�������W��y�y��
    int x2 = x1 + animation1.Width();	// �H�����k�U��x�y��
    int y2 = y1 + animation1.Height();	// �H�����k�U��y�y��
    //
    // �˴��H�����x�άO�_�b�Ǥߤ�
    //
    return (tx1 >= x1 && tx1 <= x2 && ty1 >= y1 && ty1 <= y2);
}

bool Runner::TouchStatue(int tx1, int ty1, int tx2, int ty2)
{
	int x1 = x;					// �H�������W��x�y��
	int y1 = y;					// �H�������W��y�y��
	int x2 = x1 + animation1.Width();	// �H�����k�U��x�y��
	int y2 = y1 + animation1.Height();	// �H�����k�U��y�y��
	return (tx1<=x && tx2>=x2 && ty1<=y && ty2 >=y2);
}

/////////////////////////////////////////////////////////////////////////////
// CEraser: Eraser class
/////////////////////////////////////////////////////////////////////////////

Sniper::Sniper()
{
    Initialize();
}

int Sniper::GetSightX1()
{
    return x + animation.Width() / 2;
}

int Sniper::GetSightY1()
{
    return y + animation.Height() / 2;
}

void Sniper::Initialize()
{
    const int X_POS = 280;
    const int Y_POS = 400;
    x = X_POS;
    y = Y_POS;
    isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
}

void Sniper::LoadBitmap()
{
    animation.AddBitmap(IDB_SNIPER, RGB(255, 255, 255));//�Ǥ�
}

void Sniper::OnMove()
{
    const int STEP_SIZE = 10;
    animation.OnMove();

    if (isMovingLeft)
        if (x <= -animation.Width() / 2) {}
        else
        {
            x -= STEP_SIZE;
        }

    if (isMovingRight)
        if (x + animation.Width() / 2 >= 1280) {}
        else
        {
            x += STEP_SIZE;
        }

    if (isMovingUp)
        if (y <= -animation.Height() / 2) {}
        else
        {
            y -= STEP_SIZE;
        }

    if (isMovingDown)
        if (y + animation.Height() / 2 >= 960) {}
        else
        {
            y += STEP_SIZE;
        }
}

void Sniper::SetMovingDown(bool flag)
{
    isMovingDown = flag;
}

void Sniper::SetMovingLeft(bool flag)
{
    isMovingLeft = flag;
}

void Sniper::SetMovingRight(bool flag)
{
    isMovingRight = flag;
}

void Sniper::SetMovingUp(bool flag)
{
    isMovingUp = flag;
}

void Sniper::SetSightXY(int nx, int ny)
{
    x = nx;
    y = ny;
}

void Sniper::OnShow()
{
    animation.SetTopLeft(x, y);
    animation.OnShow();
}

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame* g)
    : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
    //
    // ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
    //     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
    //
    ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
    //
    // �}�l���J���
    //
    logo.LoadBitmap(".\\bitmaps\\main.bmp");
    Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
    //
    // ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
    //
}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_ESC = 27;
    const char KEY_SPACE = ' ';

    if (nChar == KEY_SPACE)
        GotoGameState(GAME_STATE_CHOOSE);						// ������GAME_STATE_CHOOSE
    else if (nChar == KEY_ESC)								// Demo �����C������k
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// �����C��
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
   // GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
    //
    // �K�Wlogo
    //
    logo.ShowBitmap();
    //
    // Demo�ù��r�����ϥΡA���L�}�o�ɽкɶq�קK�����ϥΦr���A���CMovingBitmap����n
    //
    CDC* pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
    fp = pDC->SelectObject(&f);					// ��� font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(255, 255, 0));
    pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
    CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame* g)
    : CGameState(g)
{
}

void CGameStateOver::OnMove()
{
    counter--;

    if (counter < 0)
        GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
    counter = 30 * 5; // 5 seconds
}

void CGameStateOver::OnInit()
{
    //
    // ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
    //     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
    //
    ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
    //
    // �}�l���J���
    //
    Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
    //
    // �̲׶i�׬�100%
    //
    ShowInitProgress(100);
    SniperEnd.LoadBitmapA(".\\bitmaps\\sniperwin.bmp");
    Player1End.LoadBitmapA(".\\bitmaps\\player1win.bmp");
    Player2End.LoadBitmapA(".\\bitmaps\\player2win.bmp");
    Player3End.LoadBitmapA(".\\bitmaps\\player3win.bmp");
}

void CGameStateOver::OnShow()
{
    /* CDC* pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC
     CFont f, *fp;
     f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
     fp = pDC->SelectObject(&f);					// ��� font f
     pDC->SetBkColor(RGB(0, 0, 0));
     pDC->SetTextColor(RGB(255, 255, 0));
     char str[80];								// Demo �Ʀr��r�ꪺ�ഫ*/
    CAudio::Instance()->Stop(AUDIO_NTUT);	// ���J�n��gameBGM

    if (x == 0)
    {
        //sprintf(str, "Sniper Win! (%d)", counter / 30);
        SniperEnd.ShowBitmap();
    }

    if (x == 1)
    {
        //sprintf(str, "Player1 Win! (%d)", counter / 30);
        Player1End.ShowBitmap();
    }

    if (x == 2)
    {
        //sprintf(str, "Player2 Win! (%d)", counter / 30);
        Player2End.ShowBitmap();
    }

    if (x == 3)
    {
        //sprintf(str, "Player3 Win! (%d)", counter / 30);
        Player3End.ShowBitmap();
    }

    /*pDC->TextOut(550, 380, str);
    pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
    CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC*/
}

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g), NUMBALLS(28)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
    const int BALL_GAP = 90;
    const int BALL_XY_OFFSET = 45;
    const int BALL_PER_ROW = 7;
    const int HITS_LEFT = 5;
    const int HITS_LEFT_X = 590;
    const int HITS_LEFT_Y = 0;
    const int BACKGROUND_X = 0;
    const int ANIMATION_SPEED = 15;
    /**********************
    for (int i = 0; i < NUMBALLS; i++)  				// �]�w�y���_�l�y��
    {
    	int x_pos = i % BALL_PER_ROW;
    	int y_pos = i / BALL_PER_ROW;
    	ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
    	ball[i].SetDelay(x_pos);
    	ball[i].SetIsAlive(true);
    }*/
    sight.Initialize();
    background.SetTopLeft(BACKGROUND_X, 0);				// �]�w�I�����_�l�y��
    help.SetTopLeft(0, SIZE_Y - help.Height());			// �]�w�����Ϫ��_�l�y��
    hits_left.SetInteger(HITS_LEFT);					// ���w�ѤU��������
    hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// ���w�ѤU�����ƪ��y��
    //CAudio::Instance()->Play(AUDIO_LAKE, true);			// ���� WAVE
    //CAudio::Instance()->Play(AUDIO_DING, false);		// ���� WAVE
    CAudio::Instance()->Play(AUDIO_NTUT, true);			// ���� MIDI

    for (int i = 0, y = 50; i < 18; i++, y += 50)
    {
        runner[i].SetDirection(1);
        runner[i].SetIsAlive(1);
        runner[i].SetXY(30, y);
        runner[i].SetMovingRight(false);
    }

    player1 = rand() % 17;
    player2 = rand() % 17;

    while (player2 == player1)
    {
        player2 = rand() % 17;
    }

    player3 = rand() % 17;

    while (player3 == player2 || player3 == player1)
    {
        player3 = rand() % 17;
    }
}

void CGameStateRun::OnMove()							// ���ʹC������
{
    //
    // �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
    //
    // SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
    //
    // ���ʭI���Ϫ��y��
    //
    //if (background.Top() > SIZE_Y)
    //	background.SetTopLeft(60 ,-background.Height());
    //background.SetTopLeft(background.Left(),background.Top()+1);
    //
    // ���ʲy
    //
    //
    // �������l
    //
    sight.OnMove();

    //
    // �P�_���l�O�_�I��y
    //

    if (runner[player1].IsAlive() == false && runner[player2].IsAlive() == false && runner[player3].IsAlive() == false)
    {
        x = 0;
        GotoGameState(GAME_STATE_OVER);
    }
    else if (runner[player1].GetX() >= 1157)
    {
        x = 1;
        GotoGameState(GAME_STATE_OVER);
    }
    else if (runner[player2].GetX() >= 1157)
    {
        x = 2;
        GotoGameState(GAME_STATE_OVER);
    }
    else if (runner[player3].GetX() >= 1157)
    {
        x = 3;
        GotoGameState(GAME_STATE_OVER);
    }
    //
    // ���ʼu�����y
    //
    for (int i = 0; i < 19; i++)
    {
        if (i != player1 && i != player2 && i != player3)
        {
            int WalkOrNot = rand() % 300;

            if (runner[i].IsAlive() && runner[i].GetX() < 1157)
            {
                if (WalkOrNot <= 1)
                {
                    runner[i].SetMovingRight(true);
                }

                WalkOrNot = rand() % 100;

                if (WalkOrNot <= 1)
                {
                    runner[i].SetMovingRight(false);
                }
            }
        }

        runner[i].OnMove();
    }
}

void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
    //
    // ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H�������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
    //
    ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
    //
    // �}�l���J���
    //
    sight.LoadBitmap();
    background.LoadBitmap("Bitmaps/FloorWithFinishLine.bmp");					// ���J�I�����ϧ�(1157���I�u)
    //
    // ��������Loading�ʧ@�A�����i��
    //
    ShowInitProgress(50);
    ///////Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
    //
    // �~����J��L���
    //
    help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// ���J�������ϧ�
    corner.LoadBitmap(IDB_CORNER);							// ���J�����ϧ�
    corner.ShowBitmap(background);							// �Ncorner�K��background

    for (int i = 0, y = 0; i < 19; i++, y += 50)  							// ���J�ϧ�
    {
        runner[i].SetDirection(0);
        runner[i].LoadBitmap();
        runner[i].SetMovingRight(false);
        runner[i].SetIsAlive(true);
    }

    hits_left.LoadBitmap();
    CAudio::Instance()->Load(AUDIO_DING, "sounds\\gunshot.mp3");	// ���J�n��gunshot
    CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\gameBGM.mp3");	// ���J�n��gameBGM
    CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\scream_Karma_.wav");	//���J�n���d���y�s
	CAudio::Instance()->Load(AUDIO_TOUCH, "sounds\\Ring.mp3");
    //
    // ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
    //
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT = 0x25; // keyboard���b�Y
    const char KEY_UP = 0x26; // keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN = 0x28; // keyboard�U�b�Y
    const char KEY_D = 0x44; //D
    const char KEY_L = 0x4C; //L
    const char KEY_6 = 0x66; //6
    const char KEY_ENTER = 0x0D; //ENTER

    if (nChar == KEY_LEFT)
        sight.SetMovingLeft(true);

    if (nChar == KEY_RIGHT)
        sight.SetMovingRight(true);

    if (nChar == KEY_UP)
        sight.SetMovingUp(true);

    if (nChar == KEY_DOWN)
        sight.SetMovingDown(true);

    if (nChar == KEY_D)
    {
        if (runner[player1].IsAlive())
        {
            runner[player1].SetMovingRight(true);
        }
    }

    if (nChar == KEY_L)
    {
        if (runner[player2].IsAlive())
        {
            runner[player2].SetMovingRight(true);
        }
    }

    if (nChar == KEY_6)
    {
        if (runner[player3].IsAlive())
        {
            runner[player3].SetMovingRight(true);
        }
    }

    if (nChar == KEY_ENTER)
    {
        if (hits_left.GetInteger() != 0)
        {
            CAudio::Instance()->Play(AUDIO_DING);
            hits_left.Add(-1);

            for (int i = 0; i < 19; i++)
            {
                if (runner[i].IsAlive() && runner[i].BeKill(&sight))
                {
                    if (runner[player1].IsAlive() && runner[player1].BeKill(&sight))
                    {
                        CAudio::Instance()->Play(AUDIO_LAKE);
                        runner[i].SetIsAlive(false);
                    }

                    if (runner[player2].IsAlive() && runner[player2].BeKill(&sight))
                    {
                        CAudio::Instance()->Play(AUDIO_LAKE);
                        runner[i].SetIsAlive(false);
                    }

                    if (runner[player3].IsAlive() && runner[player3].BeKill(&sight))
                    {
                        CAudio::Instance()->Play(AUDIO_LAKE);
                        runner[i].SetIsAlive(false);
                    }

                    //CAudio::Instance()->Play(AUDIO_LAKE);
                    runner[i].SetIsAlive(false);
                }
            }
        }
    }
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT = 0x25; // keyboard���b�Y
    const char KEY_UP = 0x26; // keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN = 0x28; // keyboard�U�b�Y
    const char KEY_D = 0x44; //D
    const char KEY_L = 0x4C; //L
    const char KEY_6 = 0x66; //6
    const char KEY_ENTER = 0x0D; //ENTER

    if (nChar == KEY_LEFT)
        sight.SetMovingLeft(false);

    if (nChar == KEY_RIGHT)
        sight.SetMovingRight(false);

    if (nChar == KEY_UP)
        sight.SetMovingUp(false);

    if (nChar == KEY_DOWN)
        sight.SetMovingDown(false);

    if (nChar == KEY_D)
    {
        runner[player1].SetMovingRight(false);
    }

    if (nChar == KEY_L)
    {
        runner[player2].SetMovingRight(false);
    }

    if (nChar == KEY_6)
    {
        runner[player3].SetMovingRight(false);
    }

    if (nChar == KEY_ENTER)
    {
    }
}
void CGameStateRun::OnShow()
{
    //
    //  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��A
    //        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
    //        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
    //
    //
    //  �K�W�I���ϡB�����ơB�y�B���l�B�u�����y
    //
    background.ShowBitmap();			// �K�W�I����
    //help.ShowBitmap();					// �K�W������
    hits_left.ShowBitmap();

    for (int i = 0; i < NUMBALLS; i++)

        //ball[i].OnShow();				// �K�W��i���y
        for (int i = 0; i < 19; i++)
        {
            if (runner[i].IsAlive())
            {
                runner[i].OnShow();						// NPC�M���a
            }
        }

    sight.OnShow();					// �K�W���l
    //
    //  �K�W���W�Υk�U��������
    //
    corner.SetTopLeft(0, 0);
    corner.ShowBitmap();
    corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
    corner.ShowBitmap();
}

CGameStateChoose::CGameStateChoose(CGame* g) : CGameState(g)
{
}

void CGameStateChoose::OnInit()
{
    background.LoadBitmapA("Bitmaps/gamemode.bmp");
    deathrace.LoadBitmapA("Bitmaps/first.bmp");
    ninjaparty.LoadBitmapA("Bitmaps/ninjagamemode.bmp");
    arrow.LoadBitmapA("Bitmaps/arrow.bmp", RGB(255, 255, 255));
}

void CGameStateChoose::OnBeginState()
{
    space = 0;
    gameplay = 3;
}

void CGameStateChoose::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_UP = 0x26; // keyboard�W�b�Y
    const char KEY_DOWN = 0x28; // keyboard�U�b�Y
    const char KEY_SPACE = ' ';

    if (nChar == KEY_UP)
    {
        choose--;

        if (choose < 0)
        {
            choose = 0;
        }

        MoveArrow(choose);
    }

    if (nChar == KEY_DOWN)
    {
        choose++;

        if (choose > 1)
        {
            choose = 1;
        }

        MoveArrow(choose);
    }

    if (nChar == KEY_SPACE)
    {
        space++;

        if (GetY() == 470)
        {
            gameplay = 0;

            if (nChar == KEY_SPACE && space == 2)
            {
                GotoGameState(GAME_STATE_RUN);
            }
        }

        if (GetY() == 370)
        {
            gameplay = 1;

            if (nChar == KEY_SPACE && space == 2)
            {
                GotoGameState(GAME_STATE_NINJA);
            }
        }
    }
}

void CGameStateChoose::MoveArrow(int z)
{
    int y[2] = { 370, 470 };
    SetXY(330, y[z]);
}

void CGameStateChoose::SetXY(int nx, int ny)
{
    x = nx;
    y = ny;
}

int CGameStateChoose::GetY()
{
    return y;
}

void CGameStateChoose::OnShow()
{
    background.ShowBitmap();
    arrow.SetTopLeft(x, y);
    arrow.ShowBitmap();

    if (gameplay == 0)
    {
        deathrace.ShowBitmap();
    }
    else if (gameplay == 1)
    {
        ninjaparty.ShowBitmap();
    }
}

CGameStateNinja::CGameStateNinja(CGame* g) : CGameState(g)
{
}

void CGameStateNinja::OnBeginState()
{
    const int BALL_GAP = 90;
    const int BALL_XY_OFFSET = 45;
    const int BALL_PER_ROW = 7;
    const int HITS_LEFT = 5;
    const int HITS_LEFT_X = 590;
    const int HITS_LEFT_Y = 0;
    const int BACKGROUND_X = 0;
    const int ANIMATION_SPEED = 15;
    CAudio::Instance()->Play(AUDIO_NTUT, true);			// ���� MIDI

    for (int i = 0; i < 5; i++)
    {
        statue[i].LoadBitmapA();
    }

    statue[0].SetXY(200, 100);
    statue[1].SetXY(900, 100);
    statue[2].SetXY(550, 350);
    statue[3].SetXY(200, 600);
    statue[4].SetXY(900, 600);

    for (int i = 0, y = 0; i < 19; i++, y += 50)
    {
        runner[i].SetDirection(rand() % 4 + 1);
        runner[i].SetIsAlive(1);
        runner[i].SetXY(rand() % 1280, rand() % 960);
        runner[i].SetMovingRight(false);
        runner[i].SetMovingLeft(false);
        runner[i].SetMovingUp(false);
        runner[i].SetMovingDown(false);
    }

    player1 = rand() % 18;
    player2 = rand() % 18;

    while (player2 == player1)
    {
        player2 = rand() % 18;
    }

    player3 = rand() % 18;

    while (player3 == player2 || player3 == player1)
    {
        player3 = rand() % 18;
    }
}
void CGameStateNinja::OnInit()
{
    background.LoadBitmap("Bitmaps/background.bmp");					// ���J�I�����ϧ�(1157���I�u)

    for (int i = 0, y = 0; i < 19; i++, y += 50)  							// ���J�ϧ�
    {
        runner[i].SetDirection(rand() % 4 + 1);
        runner[i].SetXY(rand() % 1280, rand() % 900+60);
        runner[i].LoadBitmap();
        runner[i].SetMovingRight(false);
        runner[i].SetMovingLeft(false);
        runner[i].SetMovingUp(false);
        runner[i].SetMovingDown(false);
        runner[i].SetIsAlive(true);
    }
	for (int i = 0; i < 5; i++) {
		statue[i].LoadBitmap();
	}
    statue[0].SetXY(200, 100);
    statue[1].SetXY(900, 100);
    statue[2].SetXY(500, 500);
    statue[3].SetXY(200, 600);
    statue[4].SetXY(900, 600);
}
void CGameStateNinja::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT = 0x25; // keyboard���b�Y
    const char KEY_UP = 0x26; // keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN = 0x28; // keyboard�U�b�Y
    const char KEY_Q = 0x51;
    const char KEY_W = 0x57; //W
    const char KEY_A = 0x41; //A
    const char KEY_S = 0x53; //S
    const char KEY_D = 0x44; //D
    const char KEY_I = 0x49; //I
    const char KEY_J = 0x4a; //J
    const char KEY_K = 0x4b; //K
    const char KEY_L = 0x4C; //L
    const char KEY_8 = 0x68; //8
    const char KEY_4 = 0x64; //4
    const char KEY_5 = 0x65; //5
    const char KEY_6 = 0x66; //6
    const char KEY_ENTER = 0x0D; //ENTER

    if (nChar == KEY_W)
    {
        if (runner[player1].IsAlive())
        {
            runner[player1].SetDirection(4);
            runner[player1].SetMovingUp(true);
        }
    }

    if (nChar == KEY_Q)
    {
        x = 1;
        GotoGameState(GAME_STATE_OVER);
    }

    if (nChar == KEY_A)
    {
        if (runner[player1].IsAlive())
        {
            runner[player1].SetDirection(2);
            runner[player1].SetMovingLeft(true);
        }
    }

    if (nChar == KEY_S)
    {
        if (runner[player1].IsAlive())
        {
            runner[player1].SetDirection(3);
            runner[player1].SetMovingDown(true);
        }
    }

    if (nChar == KEY_D)
    {
        if (runner[player1].IsAlive())
        {
            runner[player1].SetDirection(1);
            runner[player1].SetMovingRight(true);
        }
    }

    if (nChar == KEY_I)
    {
        if (runner[player2].IsAlive())
        {
            runner[player2].SetDirection(4);
            runner[player2].SetMovingUp(true);
        }
    }

    if (nChar == KEY_J)
    {
        if (runner[player2].IsAlive())
        {
            runner[player2].SetDirection(2);
            runner[player2].SetMovingLeft(true);
        }
    }

    if (nChar == KEY_K)
    {
        if (runner[player2].IsAlive())
        {
            runner[player2].SetDirection(3);
            runner[player2].SetMovingDown(true);
        }
    }

    if (nChar == KEY_L)
    {
        if (runner[player2].IsAlive())
        {
            runner[player2].SetDirection(1);
            runner[player2].SetMovingRight(true);
        }
    }

    if (nChar == KEY_8)
    {
        if (runner[player3].IsAlive())
        {
            runner[player3].SetDirection(4);
            runner[player3].SetMovingUp(true);
        }
    }

    if (nChar == KEY_4)
    {
        if (runner[player3].IsAlive())
        {
            runner[player3].SetDirection(2);
            runner[player3].SetMovingLeft(true);
        }
    }

    if (nChar == KEY_5)
    {
        if (runner[player3].IsAlive())
        {
            runner[player3].SetDirection(3);
            runner[player3].SetMovingDown(true);
        }
    }

    if (nChar == KEY_6)
    {
        if (runner[player3].IsAlive())
        {
            runner[player3].SetDirection(1);
            runner[player3].SetMovingRight(true);
        }
    }
}
void CGameStateNinja::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT = 0x25; // keyboard���b�Y
    const char KEY_UP = 0x26; // keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN = 0x28; // keyboard�U�b�Y
    const char KEY_W = 0x57; //W
    const char KEY_A = 0x41; //A
    const char KEY_S = 0x53; //S
    const char KEY_D = 0x44; //D
    const char KEY_I = 0x49; //I
    const char KEY_J = 0x4a; //J
    const char KEY_K = 0x4b; //K
    const char KEY_L = 0x4C; //L
    const char KEY_8 = 0x68; //8
    const char KEY_4 = 0x64; //4
    const char KEY_5 = 0x65; //5
    const char KEY_6 = 0x66; //6
    const char KEY_ENTER = 0x0D; //ENTER

    if (nChar == KEY_W)
    {
        if (runner[player1].IsAlive())
        {
            runner[player1].SetMovingUp(false);
        }
    }

    if (nChar == KEY_A)
    {
        if (runner[player1].IsAlive())
        {
            runner[player1].SetMovingLeft(false);
        }
    }

    if (nChar == KEY_S)
    {
        if (runner[player1].IsAlive())
        {
            runner[player1].SetMovingDown(false);
        }
    }

    if (nChar == KEY_D)
    {
        if (runner[player1].IsAlive())
        {
            runner[player1].SetMovingRight(false);
        }
    }

    if (nChar == KEY_I)
    {
        if (runner[player2].IsAlive())
        {
            runner[player2].SetMovingUp(false);
        }
    }

    if (nChar == KEY_J)
    {
        if (runner[player2].IsAlive())
        {
            runner[player2].SetMovingLeft(false);
        }
    }

    if (nChar == KEY_K)
    {
        if (runner[player2].IsAlive())
        {
            runner[player2].SetMovingDown(false);
        }
    }

    if (nChar == KEY_L)
    {
        if (runner[player2].IsAlive())
        {
            runner[player2].SetMovingRight(false);
        }
    }

    if (nChar == KEY_8)
    {
        if (runner[player3].IsAlive())
        {
            runner[player3].SetMovingUp(false);
        }
    }

    if (nChar == KEY_4)
    {
        if (runner[player3].IsAlive())
        {
            runner[player3].SetMovingLeft(false);
        }
    }

    if (nChar == KEY_5)
    {
        if (runner[player3].IsAlive())
        {
            runner[player3].SetMovingDown(false);
        }
    }

    if (nChar == KEY_6)
    {
        if (runner[player3].IsAlive())
        {
            runner[player3].SetMovingRight(false);
        }
    }

    if (nChar == KEY_ENTER)
    {
        // 	CAudio::Instance()->Stop(AUDIO_DING);
    }
}
void CGameStateNinja::OnMove()
{
	if (runner[player1].Check()) {
		x = 1;
		GotoGameState(GAME_STATE_OVER);
	}
	else if (runner[player2].Check()) {
		x = 2;
		GotoGameState(GAME_STATE_OVER);
	}
	else if (runner[player3].Check()) {
		x = 3;
		GotoGameState(GAME_STATE_OVER);
	}
    //
    // ���ʼu�����y
    //
	for (int i = 0; i < 5; i++) {
		if (runner[player1].Touch(&statue[i])) {
			if (runner[player1].GetStatue(i)) {
				CAudio::Instance()->Play(AUDIO_TOUCH);
			}
			runner[player1].SetStatue(i);
		}
		if (runner[player2].Touch(&statue[i])) {
			if (runner[player2].GetStatue(i)) {
				CAudio::Instance()->Play(AUDIO_TOUCH);
			}
			runner[player2].SetStatue(i);
		}
		if (runner[player3].Touch(&statue[i])) {
			if (runner[player3].GetStatue(i)) {
				CAudio::Instance()->Play(AUDIO_TOUCH);
			}
			runner[player3].SetStatue(i);
		}
	}
    for (int i = 0; i < 19; i++)
    {
        if (i != player1 && i != player2 && i != player3)
        {
            int WalkOrNot = rand() % 300;

            if (runner[i].IsAlive())
            {
                if (WalkOrNot <= 10)
                {
                    int direction = rand() % 4 + 1;

                    if (direction == 1)
                    {
                        runner[i].SetDirection(1);
                        runner[i].SetMovingRight(true);
                    }

                    if (direction == 2)
                    {
                        runner[i].SetDirection(2);
                        runner[i].SetMovingLeft(true);
                    }

                    if (direction == 3)
                    {
                        runner[i].SetDirection(3);
                        runner[i].SetMovingDown(true);
                    }

                    if (direction == 4)
                    {
                        runner[i].SetDirection(4);
                        runner[i].SetMovingUp(true);
                    }
                }

                WalkOrNot = rand() % 100;

                if (WalkOrNot <= 1)
                {
                    runner[i].SetMovingRight(false);
                    runner[i].SetMovingLeft(false);
                    runner[i].SetMovingUp(false);
                    runner[i].SetMovingDown(false);
                }
            }
        }

        runner[i].OnMove();
    }
}
void CGameStateNinja::OnShow()
{
    background.ShowBitmap();			// �K�W�I����

    //help.ShowBitmap();					// �K�W������
    for (int i = 0; i < 19; i++)
    {
        if (runner[i].IsAlive())
        {
            runner[i].OnShow();						// NPC�M���a
        }
    }

    for (int i = 0; i < 5; i++)
    {
        statue[i].OnShow();
    }
}
}
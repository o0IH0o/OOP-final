/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
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

    for (int i = 0; i < 4; i++)	// 載入動畫(由4張圖形構成)
        animation1.AddBitmap(filename[i], RGB(255, 255, 255));

    for (int i = 4; i < 8; i++)	// 載入動畫(由4張圖形構成)
        animation2.AddBitmap(filename[i], RGB(255, 255, 255));

    for (int i = 8; i < 12; i++)	// 載入動畫(由4張圖形構成)
        animation3.AddBitmap(filename[i], RGB(255, 255, 255));

    for (int i = 12; i < 16; i++)	// 載入動畫(由4張圖形構成)
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
            this->x += STEP_SIZE; // 執行一次animation.OnMove()，animation才會換圖
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
    int x1 = x;					// 人物的左上角x座標
    int y1 = y;					// 人物的左上角y座標
    int x2 = x1 + animation1.Width();	// 人物的右下角x座標
    int y2 = y1 + animation1.Height();	// 人物的右下角y座標
    //
    // 檢測人物的矩形是否在準心中
    //
    return (tx1 >= x1 && tx1 <= x2 && ty1 >= y1 && ty1 <= y2);
}

bool Runner::TouchStatue(int tx1, int ty1, int tx2, int ty2)
{
	int x1 = x;					// 人物的左上角x座標
	int y1 = y;					// 人物的左上角y座標
	int x2 = x1 + animation1.Width();	// 人物的右下角x座標
	int y2 = y1 + animation1.Height();	// 人物的右下角y座標
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
    animation.AddBitmap(IDB_SNIPER, RGB(255, 255, 255));//準心
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
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame* g)
    : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
    //
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
    //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
    ShowInitProgress(0);	// 一開始的loading進度為0%
    //
    // 開始載入資料
    //
    logo.LoadBitmap(".\\bitmaps\\main.bmp");
    Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
    //
    // 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
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
        GotoGameState(GAME_STATE_CHOOSE);						// 切換至GAME_STATE_CHOOSE
    else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
   // GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
    //
    // 貼上logo
    //
    logo.ShowBitmap();
    //
    // Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
    //
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
    fp = pDC->SelectObject(&f);					// 選用 font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(255, 255, 0));
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
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
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
    //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
    ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
    //
    // 開始載入資料
    //
    Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
    //
    // 最終進度為100%
    //
    ShowInitProgress(100);
    SniperEnd.LoadBitmapA(".\\bitmaps\\sniperwin.bmp");
    Player1End.LoadBitmapA(".\\bitmaps\\player1win.bmp");
    Player2End.LoadBitmapA(".\\bitmaps\\player2win.bmp");
    Player3End.LoadBitmapA(".\\bitmaps\\player3win.bmp");
}

void CGameStateOver::OnShow()
{
    /* CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
     CFont f, *fp;
     f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
     fp = pDC->SelectObject(&f);					// 選用 font f
     pDC->SetBkColor(RGB(0, 0, 0));
     pDC->SetTextColor(RGB(255, 255, 0));
     char str[80];								// Demo 數字對字串的轉換*/
    CAudio::Instance()->Stop(AUDIO_NTUT);	// 載入聲音gameBGM

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
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC*/
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
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
    for (int i = 0; i < NUMBALLS; i++)  				// 設定球的起始座標
    {
    	int x_pos = i % BALL_PER_ROW;
    	int y_pos = i / BALL_PER_ROW;
    	ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
    	ball[i].SetDelay(x_pos);
    	ball[i].SetIsAlive(true);
    }*/
    sight.Initialize();
    background.SetTopLeft(BACKGROUND_X, 0);				// 設定背景的起始座標
    help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
    hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞擊數
    hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// 指定剩下撞擊數的座標
    //CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
    //CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
    CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI

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

void CGameStateRun::OnMove()							// 移動遊戲元素
{
    //
    // 如果希望修改cursor的樣式，則將下面程式的commment取消即可
    //
    // SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
    //
    // 移動背景圖的座標
    //
    //if (background.Top() > SIZE_Y)
    //	background.SetTopLeft(60 ,-background.Height());
    //background.SetTopLeft(background.Left(),background.Top()+1);
    //
    // 移動球
    //
    //
    // 移動擦子
    //
    sight.OnMove();

    //
    // 判斷擦子是否碰到球
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
    // 移動彈跳的球
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

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
    //
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
    ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
    //
    // 開始載入資料
    //
    sight.LoadBitmap();
    background.LoadBitmap("Bitmaps/FloorWithFinishLine.bmp");					// 載入背景的圖形(1157終點線)
    //
    // 完成部分Loading動作，提高進度
    //
    ShowInitProgress(50);
    ///////Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
    //
    // 繼續載入其他資料
    //
    help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// 載入說明的圖形
    corner.LoadBitmap(IDB_CORNER);							// 載入角落圖形
    corner.ShowBitmap(background);							// 將corner貼到background

    for (int i = 0, y = 0; i < 19; i++, y += 50)  							// 載入圖形
    {
        runner[i].SetDirection(0);
        runner[i].LoadBitmap();
        runner[i].SetMovingRight(false);
        runner[i].SetIsAlive(true);
    }

    hits_left.LoadBitmap();
    CAudio::Instance()->Load(AUDIO_DING, "sounds\\gunshot.mp3");	// 載入聲音gunshot
    CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\gameBGM.mp3");	// 載入聲音gameBGM
    CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\scream_Karma_.wav");	//載入聲音卡瑪尖叫
	CAudio::Instance()->Load(AUDIO_TOUCH, "sounds\\Ring.mp3");
    //
    // 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
    //
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT = 0x25; // keyboard左箭頭
    const char KEY_UP = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭
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
    const char KEY_LEFT = 0x25; // keyboard左箭頭
    const char KEY_UP = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭
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
    //  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
    //        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
    //        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
    //
    //
    //  貼上背景圖、撞擊數、球、擦子、彈跳的球
    //
    background.ShowBitmap();			// 貼上背景圖
    //help.ShowBitmap();					// 貼上說明圖
    hits_left.ShowBitmap();

    for (int i = 0; i < NUMBALLS; i++)

        //ball[i].OnShow();				// 貼上第i號球
        for (int i = 0; i < 19; i++)
        {
            if (runner[i].IsAlive())
            {
                runner[i].OnShow();						// NPC和玩家
            }
        }

    sight.OnShow();					// 貼上擦子
    //
    //  貼上左上及右下角落的圖
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
    const char KEY_UP = 0x26; // keyboard上箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭
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
    CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI

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
    background.LoadBitmap("Bitmaps/background.bmp");					// 載入背景的圖形(1157終點線)

    for (int i = 0, y = 0; i < 19; i++, y += 50)  							// 載入圖形
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
    const char KEY_LEFT = 0x25; // keyboard左箭頭
    const char KEY_UP = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭
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
    const char KEY_LEFT = 0x25; // keyboard左箭頭
    const char KEY_UP = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭
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
    // 移動彈跳的球
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
    background.ShowBitmap();			// 貼上背景圖

    //help.ShowBitmap();					// 貼上說明圖
    for (int i = 0; i < 19; i++)
    {
        if (runner[i].IsAlive())
        {
            runner[i].OnShow();						// NPC和玩家
        }
    }

    for (int i = 0; i < 5; i++)
    {
        statue[i].OnShow();
    }
}
}
/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
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

enum AUDIO_ID  				// 定義各種音效的編號
{
    AUDIO_DING,				// 0
    AUDIO_LAKE,				// 1
    AUDIO_NTUT,				// 2
	AUDIO_TOUCH				// 3
};

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// 這個class提供可以用鍵盤或滑鼠控制的擦子
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

class Sniper
{
    public:
        Sniper();
        int  GetSightX1();					// 準心 x 座標
        int  GetSightY1();					// 準心 y 座標
        void Initialize();				// 設定準心為初始值
        void LoadBitmap();				// 載入圖形
        void OnMove();					// 移動準心
        void OnShow();					// 將準心圖形貼到畫面
        void SetMovingDown(bool flag);	// 設定是否正在往下移動
        void SetMovingLeft(bool flag);	// 設定是否正在往左移動
        void SetMovingRight(bool flag); // 設定是否正在往右移動
        void SetMovingUp(bool flag);	// 設定是否正在往上移動
        void SetSightXY(int nx, int ny);		// 設定準心左上角座標
    protected:
        CAnimation animation;		// 擦子的動畫
        int x, y;					// 擦子左上角座標
        bool isMovingDown;			// 是否正在往下移動
        bool isMovingLeft;			// 是否正在往左移動
        bool isMovingRight;			// 是否正在往右移動
        bool isMovingUp;			// 是否正在往上移動
};

/////////////////////////////////////////////////////////////////////////////
// 這個class提供會一直彈跳的球
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

class Statue
{
public:
	Statue();
	void LoadBitmap();										// 載入圖形
	void OnMove();											// 移動
	void OnShow();											// 將圖形貼到畫面
	void SetXY(int nx, int ny);								// 設定圓心的座標
	int GetX();
	int GetY();
	int GetWight();
	int GetHeight();
protected:
	CMovingBitmap bmp;			// 球的圖
	int x, y;					// 圓心的座標
private:
	CAnimation animation;
};

class Runner
{
    public:
        Runner();
		void LoadBitmap();		// 載入圖形
        bool IsAlive();											// 是否活著
        void SetIsAlive(bool alive);							// 設定是否活著
        bool BeKill(Sniper* sight);// 是否被殺死
		bool Touch(Statue* statue);
        void OnMove();			// 移動
        void OnShow();			// 將圖形貼到畫面
        void SetMovingDown(bool flag);	// 設定是否正在往下移動
        void SetMovingLeft(bool flag);	// 設定是否正在往左移動
        void SetMovingRight(bool flag); // 設定是否正在往右移動
        void SetMovingUp(bool flag);	// 設定是否正在往上移動
        void Initialize();				// 設定人物為初始值
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
        CMovingBitmap bmp;			// 球的圖

    private:
        bool HitSight(int tx1, int ty1);	// 是否在準心上
		bool TouchStatue(int tx1, int ty1, int tx2, int ty2);
        int x, y, direction,statue[5];				// 圖形座標
        bool isMovingDown;			// 是否正在往下移動
        bool isMovingLeft;			// 是否正在往左移動
        bool isMovingRight;			// 是否正在往右移動
        bool isMovingUp;			// 是否正在往上移動
        bool is_alive;				// 是否活著
        CAnimation animation1, animation2, animation3, animation4;	// 利用動畫作圖形
};

/////////////////////////////////////////////////////////////////////////////
// 這個class提供繞圈圈的球
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState
{
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
        void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
    protected:
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        CMovingBitmap logo;								// csie的logo
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////
class CGameStateRun : public CGameState
{
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        const int		NUMBALLS;	// 球的總數
        int				player1, player2, player3;//角色隨機
        CMovingBitmap	background;	// 背景圖
        CMovingBitmap	help;		// 說明圖
        CMovingBitmap	corner;		// 角落圖
        Sniper			sight;		// 狙擊手
        CInteger		hits_left;	// 剩下的撞擊數
        Runner   runner[19];		// 跑者
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState
{
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        int counter;	// 倒數之計數器
        CMovingBitmap SniperEnd;
        CMovingBitmap Player1End;
        CMovingBitmap Player2End;
        CMovingBitmap Player3End;
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的選單狀態(Choose Game)
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateChoose : public CGameState
{
    public:
        CGameStateChoose(CGame* g);
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnKeyUp(UINT, UINT, UINT);
        void MoveArrow(int z);
        void SetXY(int nx, int ny);
        int GetY();
    protected:
        void OnShow();									// 顯示這個狀態的遊戲畫面
        int x = 330, y = 370, z, choose = 0;
    private:
        int gameplay, space = 0;
        CMovingBitmap background;								// csie的logo
        CMovingBitmap arrow;									//arrow
        CMovingBitmap deathrace;                                //how to play
        CMovingBitmap ninjaparty;
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的Ninja party狀態(ninja party)
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateNinja : public CGameState
{
    public:
        CGameStateNinja(CGame* g);
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        int				player1, player2, player3;//角色隨機
        CMovingBitmap	background;	// 背景圖
        Runner   runner[19];		// 跑者
        Statue   statue[5];
};

}
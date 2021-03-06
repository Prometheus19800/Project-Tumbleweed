/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>
#include "XinputManager.h"
#include "Table.h"
#include "Player.h"
#include "Potion.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Tutorial.h"
#include "SpawnManager.h"
#include "Powerup.h"
#include "Sign.h"
#include <fstream>
namespace Sedna {
	class SednaMenu;
}
class HelloWorld : public cocos2d::Scene,public Tutorial
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void update(float dt);

	void gameTutorial(float dt);
	void pause(float dt);
	void play(float dt);
	void boss(float dt);

	///<used to reduce duplicate code>
	void useBulletTime(float dt);
	void performBounce(Sedna::Player* p);
	void notDead(float dt);
	void checkUnderScreen(Sedna::Player* p);
	
	void initSprites();
	void checkInput(float dt);
	void getCollisions();
	void checkManyLists(float dt);
	void recursiveFunctionOutlaw();
	void recursiveFunctionTable();
	void recursiveFunctionKnocked();
	void checkPosAll();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);

	bool playedWinSound = false;
	float bossTimer = 0.0f;
	bool bossTime = false;
	std::vector<Sedna::Outlaw*> g;
	Sedna::CirclePrimitive* dummyTracker;


	cocos2d::Label* tutorialLabel;
	cocos2d::Label* tutorialLabel2;
	bool tutFunc1 = false;
	bool tutFunc2 = false;
	bool tutFunc3 = false;
	bool tutFunc4 = false;
	Sedna::Sign* healSign;
	Sedna::Sign* reviveSign;
	Sedna::Sign* invinceSign;
	Sedna::Sign* tablekickSign;
	Sedna::Sign* btSign;
	Sedna::Sign* shootSign;
	cocos2d::Sprite* tutorialGun;
	Sedna::Sign* movementSign;
	Sedna::Player* dummy;
	bool tutBool = true;
	bool tutCutscene = true;
	std::vector<Sedna::Table*>tutTables;
	std::vector<Sedna::Outlaw*>tutOutlaws;

	Sedna::Sign* testSign;

	cocos2d::Label* btLabel;
	Sedna::SquarePrimitive btMeter;

	bool decrimenting = false;
	float bulletTimeMax = 0.0f;

	float CAMERASPEED = 1.3f;

	Sedna::Powerup* bloodyMaryP_up;
	Sedna::Powerup* theBiggestIronP_up;

	bool playMusic = false;
	static bool end;
	cocos2d::Label* pausedLabel;
	cocos2d::Label* startLabel;
	Sedna::SednaMenu* pauseMenu;

	cocos2d::Director* director;

	void bounds();
	Sedna::SpawnManager sManager = Sedna::SpawnManager(this);
	Sedna::XinputManager managerR;
	Sedna::XinputController* p1Controller;
	Sedna::XinputController* p2Controller;
	Sedna::Stick p1Sticks[2];
	Sedna::Stick p2Sticks[2];
	Sedna::Triggers p1Triggers;
	Sedna::Triggers p2Triggers;

	cocos2d::Sprite* a;
	cocos2d::Sprite* b;
	cocos2d::Sprite* rsl;
	cocos2d::Sprite* rsr;
	cocos2d::Sprite* rt;

	Sedna::Player* playerOne;
	Sedna::Player* playerTwo;

	Sedna::Projectile *baseProjectile;
	std::vector<Sedna::Projectile*>pProjectiles;
	Sedna::Projectile* eBaseProjectile;
	std::vector<Sedna::Projectile*>eProjectiles;

	Sedna::GameObject* DDOS;
	std::vector <Sedna::Potion> potionList;

	cocos2d::Sprite* bg;
	cocos2d::Sprite* bg2;
	cocos2d::Sprite* bg3;

	int barRightMax = 430;
	int barLeftMax = 90;

	float gameStart = 0.0f;
	bool moveScreen = true;

	bool TRUEPAUSE = false;
	bool TEMPPAUSE = false;
	bool paused = false;
	void togglePause();
	bool bulletTime = false;

	float loseTimer = 0.0f;

	std::ifstream highFileIn;
	std::ofstream highFileOut;
	bool hasWritten = false;
	void writeScore();
	cocos2d::Label *highScoreLabel;

	private:
		
};

#endif // __HELLOWORLD_SCENE_H__

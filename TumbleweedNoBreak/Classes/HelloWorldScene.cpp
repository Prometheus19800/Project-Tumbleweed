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

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <iostream>
#include "AudioEngine.h"
#include "menuScene.h"
#include "MusicList.h"
#include "Options.h"

USING_NS_CC;

bool HelloWorld::end = false;

Scene* HelloWorld::createScene()
{
	return HelloWorld::create();
}



// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//bottom label

	auto localL1 = cocos2d::Label::create("Exit to Main Menu", "fonts/Montague.ttf", 25);
	auto localL2 = cocos2d::Label::create("Resume", "fonts/Montague.ttf", 25);
	pauseMenu = new Sedna::SednaMenu(2, localL1, localL2);


	p1Controller = managerR.getController(0);
	p2Controller = managerR.getController(1);

	managerR.update();

	p1Controller->updateSticks(p1Sticks);
	p1Controller->getTriggers(p1Triggers);
	p2Controller->updateSticks(p2Sticks);
	p2Controller->getTriggers(p2Triggers);
	initSprites();

	director = cocos2d::Director::getInstance();
	end = false;
	this->scheduleUpdate();

	return true;
}
void HelloWorld::initSprites()
{
	cocos2d::experimental::AudioEngine::preload("bgm.mp3");
	///<cocos2d::experimental::AudioEngine::preload("oRsound.mp3");>




	DDOS = new Sedna::GameObject("a.png", cocos2d::Vec2(100, 300), 1, 1, 1);
	this->addChild(DDOS->getBox()->getDrawNode());
	this->addChild(DDOS->getSprite());
	DDOS->getSprite()->setVisible(true);


	btLabel = cocos2d::Label::create("Bullet Time", "fonts/Montague.ttf", 8);
	btLabel->setPosition(cocos2d::Vec2(230, DDOS->getSprite()->getPosition().y - 15));
	this->addChild(btLabel, 101);
	btMeter = Sedna::SquarePrimitive(cocos2d::Vec2(190, DDOS->getSprite()->getPosition().y - 20), cocos2d::Vec2(280, DDOS->getSprite()->getPosition().y - 10));
	this->addChild(btMeter.getDrawNode(), 100);
	/////////////////////////////////////


	goldman = new Sedna::Goldman(250, 250);
	this->addChild(goldman->getBox()->getDrawNode());
	this->addChild(goldman->getSprite());
	goldmans.push_back(goldman);


	bloodyMaryP_up = new Sedna::Powerup("gun2.png", Sedna::Guns::bloodyMary, -1000, 0);
	theBiggestIronP_up = new Sedna::Powerup("gun3.png", Sedna::Guns::theBiggestIron, -1000, 0);

	this->addChild(bloodyMaryP_up->getBox()->getDrawNode());
	this->addChild(bloodyMaryP_up->getSprite());
	this->addChild(theBiggestIronP_up->getBox()->getDrawNode());
	this->addChild(theBiggestIronP_up->getSprite());


	playerOne = new Sedna::Player(1, 300, 150, managerR, Sedna::Guns::olReliable);
	this->addChild(playerOne->getBox()->getDrawNode());
	this->addChild(playerOne->getSprite(), 10);
	this->addChild(playerOne->getUI()->getUIGunSprite(), 20);


	for (unsigned int i = 0; i < playerOne->getUI()->getLabelList().size(); i++)
		this->addChild(playerOne->getUI()->getLabelList()[i], 20);

	for (unsigned int i = 0; i < playerOne->getUI()->getHPSprites().size(); i++)
		this->addChild(playerOne->getUI()->getHPSprites()[i]);


	playerTwo = new Sedna::Player(2, 300, 100, managerR, Sedna::Guns::olReliable2);
	this->addChild(playerTwo->getBox()->getDrawNode());
	this->addChild(playerTwo->getSprite(), 10);
	this->addChild(playerTwo->getUI()->getUIGunSprite(), 20);
	for (unsigned int i = 0; i < playerTwo->getUI()->getLabelList().size(); i++)
		this->addChild(playerTwo->getUI()->getLabelList()[i], 20);
	for (unsigned int i = 0; i < playerTwo->getUI()->getHPSprites().size(); i++)
		this->addChild(playerTwo->getUI()->getHPSprites()[i]);



	bg = cocos2d::Sprite::create("bg1.png");
	this->addChild(bg, -1000);
	bg->setScale(0.85f, 0.92f);
	bg->setAnchorPoint(cocos2d::Vec2(0, 0));
	bg->setPosition(0, 0);



	bg2 = cocos2d::Sprite::create("bgPlain.png");
	this->addChild(bg2, -1000);
	bg2->setScale(0.85f, 0.92f);
	bg2->setAnchorPoint(cocos2d::Vec2(0, 0));
	bg2->setPosition(cocos2d::Vec2(0, bg->getContentSize().height * 0.92f));

	bg3 = cocos2d::Sprite::create("bgPlain.png");
	this->addChild(bg3, -1000);
	bg3->setScale(0.85f, 0.92f);
	bg3->setAnchorPoint(cocos2d::Vec2(0, 0));
	bg3->setPosition(cocos2d::Vec2(0, (bg2->getContentSize().height * 0.92f) * 2));


	///menu 

	pausedLabel = Label::create("Paused", "fonts/Montague.ttf", 25);
	pausedLabel->setAnchorPoint(Vec2(0.0f, 0.0f));
	pausedLabel->setPosition(Vec2(140, 250));
	pausedLabel->enableShadow();
	this->addChild(pausedLabel, 100);
	pausedLabel->setVisible(false);

	startLabel = Label::create("3", "fonts/Montague.ttf", 100);
	startLabel->setAnchorPoint(Vec2(0.0f, 0.0f));
	startLabel->setPosition(Vec2(200, 150));
	startLabel->enableShadow();
	this->addChild(startLabel, 100);
	startLabel->setVisible(false);



	for (unsigned int i = 0; i < pauseMenu->getLabelList().size(); i++) {
		this->addChild(pauseMenu->getLabelList()[i], 100);
		pauseMenu->getLabelList()[i]->enableShadow();
		pauseMenu->getLabelList()[i]->setAnchorPoint(cocos2d::Vec2(0, 0));
		if (i == 0)
			pauseMenu->getLabelList()[i]->setPosition(140, 50);
		else {
			pauseMenu->getLabelList()[i]->setPosition(140, pauseMenu->getLabelList()[i - 1]->getPosition().y + 30);

		}
	}



	for (unsigned int i = 0; i < pauseMenu->getLabelList().size(); i++) {
		pauseMenu->getLabelList()[i]->setVisible(false);
	}
	pauseMenu->select(1);


	tutOutlaws.push_back(new Sedna::Outlaw(200, DDOS->getSprite()->getPosition().y));
	this->addChild(tutOutlaws.back()->getBox()->getDrawNode());
	this->addChild(tutOutlaws.back()->getSprite());

	for (int i = 0; i < 3; i++) {
		tutTables.push_back(new Sedna::Table(200 + (i * 70), 100));
		this->addChild(tutTables.back()->getBox()->getDrawNode());
		this->addChild(tutTables.back()->getSprite());
	}

	dummy = new Sedna::Player(1, 320, 40, managerR, Sedna::Guns::olReliable);
	this->addChild(dummy->getBox()->getDrawNode());
	this->addChild(dummy->getSprite());

	movementSign = new Sedna::Sign("Use the Left Thumbstick to Move", this, cocos2d::Vec2(-1000, 0));
	shootSign = new Sedna::Sign("Use the Right Thumbstick to aim\n Hold the Right Trigger to shoot", this, cocos2d::Vec2(-1000, 0));
	btSign = new Sedna::Sign("Hold the Left Trigger for Bullet Time", this, cocos2d::Vec2(-1000, 0));
	tablekickSign = new Sedna::Sign("Hold A to kick tables\nKick tables with drinks on \nthem for special effects!", this, cocos2d::Vec2(-1000, 0));

	auto temp = cocos2d::Label::create("Tutorial", "fonts/Montague.ttf", 20);
	temp->setPosition(cocos2d::Vec2(380, 280));
	this->addChild(temp, 1000);
}

void HelloWorld::update(float dt)
{
	if (!end) {

		managerR.update();
		p1Controller->updateSticks(p1Sticks);
		p1Controller->getTriggers(p1Triggers);
		p2Controller->updateSticks(p2Sticks);
		p2Controller->getTriggers(p2Triggers);


		if (playerOne->getCurrentGun()->getAmmo() <= 0)
			playerOne->setCurrentGun(Sedna::Guns::olReliable);
		if (playerTwo->getCurrentGun()->getAmmo() <= 0)
			playerTwo->setCurrentGun(Sedna::Guns::olReliable2);


		if (tutBool)
			this->gameTutorial(dt);
		else {
			this->pause(dt);
			this->play(dt);
		}



	}

}
void HelloWorld::boss(float dt)
{

	goldman->shoot(dt, this);
	goldman->checkList();
	goldman->updateGameObject();
	//goldman->checkBCollision(playerOne);
	//goldman->checkBCollision(playerTwo);


}
void HelloWorld::useBulletTime(float dt)
{
	if ((p1Triggers.LT > 0.0f || p2Triggers.LT > 0.0f) && bulletTimeMax < 3.0f)//triggers can be replaced by a power up boolean for a drink instead of a toggle thing
		bulletTime = true;

	if (bulletTime)
	{
		togglePause();

		playerOne->getBox()->setRadius(15);	///
		playerTwo->getBox()->setRadius(15);	///
		bulletTimeMax += dt;
	}
	if (p1Triggers.LT == 0 && p2Triggers.LT == 0 || bulletTimeMax >= 3.0f)
	{

		bulletTime = false;
		paused = false;
		bulletTimeMax -= dt;

		playerOne->getBox()->setRadius(20);	 ///
		playerTwo->getBox()->setRadius(20);	 ///
	}
	if (bulletTimeMax < 0.0f) {
		bulletTimeMax = 0;

	}
}
void HelloWorld::gameTutorial(float dt)
{

	btMeter.setP2x(280 - (bulletTimeMax * 30));

	btMeter.update();

	useBulletTime(dt);
	if (!paused) {

		//playerOne->update(dt);
		//playerTwo->update(dt);




		movementSign->signUpdate(playerOne, playerTwo);
		shootSign->signUpdate(playerOne, playerTwo);
		btSign->signUpdate(playerOne, playerTwo);
		tablekickSign->signUpdate(playerOne, playerTwo);

		if (tutCutscene) {


			if (dummy->getBox()->getLocation().x == 320)
				dummy->getBox()->setForce(cocos2d::Vec2(1, 0));
			else if (dummy->getBox()->getLocation().x == 380)
				dummy->getBox()->setForce(cocos2d::Vec2(-1, 0));
			bounds();

			//	playerOne->updateGameObject();
				//playerTwo->updateGameObject();
			dummy->updateGameObject();

			//temp static bool
			static bool yes = false;
			static bool no = true;
			if (tutTables.size() == 2 && !yes) {
				tutOutlaws.push_back(new Sedna::ShotgunOutlaw(250, DDOS->getSprite()->getPosition().y));
				this->addChild(tutOutlaws.back()->getBox()->getDrawNode());
				this->addChild(tutOutlaws.back()->getSprite());
				yes = true;
			}
			else if (tutTables.size() == 1 && no) {
				tutOutlaws.push_back(new Sedna::RifleOutlaw(350, DDOS->getSprite()->getPosition().y));
				this->addChild(tutOutlaws.back()->getBox()->getDrawNode());
				this->addChild(tutOutlaws.back()->getSprite());
				no = false;
			}

			if (tutOutlaws.back()->getBox()->getLocation().y > 200)
				tutOutlaws.back()->getBox()->setForce(cocos2d::Vec2(0, -2));
			else
				tutOutlaws.back()->getBox()->setForce(cocos2d::Vec2(0, 0));
			if (tutTables.empty())
				tutCutscene = false;
		}
		if (!tutCutscene) {
			static bool cleared = false;
			if (!cleared) {
				dummy->getBox()->getDrawNode()->removeFromParent();
				dummy->getSprite()->removeFromParent();

				for (auto x : tutOutlaws) {
					x->removeProjectiles();
					x->getBox()->getDrawNode()->removeFromParent();
					x->getSprite()->removeFromParent();
				}
				for (auto x : tutTables) {

					x->getBox()->getDrawNode()->removeFromParent();
					x->getSprite()->removeFromParent();
				}

				tutOutlaws.clear();
				tutTables.clear();
				cleared = true;

				for (int i = 0; i < 4; i++) {
					tutTables.push_back(new Sedna::Table(150, 40 + (70 * i), Sedna::health + i));
					this->addChild(tutTables.back()->getBox()->getDrawNode());
					this->addChild(tutTables.back()->getSprite());
				}
				movementSign->getBox()->setLocation(cocos2d::Vec2(300, 150));
				shootSign->getBox()->setLocation(cocos2d::Vec2(340, 190));
				btSign->getBox()->setLocation(cocos2d::Vec2(360, 80));
				//260 370
				tablekickSign->getBox()->setLocation(cocos2d::Vec2(340, 270));

				movementSign->getLabel()->setVisible(true);
				shootSign->getLabel()->setVisible(true);
				btSign->getLabel()->setVisible(true);
				tablekickSign->getLabel()->setVisible(true);

			}
			bounds();

			//signs
			checkInput(dt);
			playerOne->update(dt);
			playerOne->updateGameObject();
			playerTwo->update(dt);
			playerTwo->updateGameObject();
			checkPosAll();

			std::cout << playerOne->getBox()->getLocation().x << " " << playerOne->getBox()->getLocation().y << "\n";

		}
		srand(time(0));

		getCollisions();
		checkManyLists(dt);
	}

}
void HelloWorld::play(float dt)
{
	if (!paused)
	{
		btMeter.setP2x(280 - (bulletTimeMax * 30));

		btMeter.update();
		if (playerOne->isDead() && playerTwo->isDead())//is this loss
		{
			CAMERASPEED = 0;
			startLabel->setString("You Lose");
			startLabel->setVisible(true);
			loseTimer += dt;
			startLabel->setPosition(50, startLabel->getPosition().y);
			if (loseTimer >= 4.0f)
			{
				auto mMenu = MenuScene::create();
				cocos2d::experimental::AudioEngine::stopAll();
				end = true;

				director->replaceScene(TransitionFade::create(2.0f, mMenu));
			}

		}
		else
		{
			playerOne->update(dt);
			playerTwo->update(dt);
			bloodyMaryP_up->updateGameObject();
			theBiggestIronP_up->updateGameObject();
			if (tutBool/*&&tutorialEnd*/)
			{
				//make stuff invisible
				tutBool = false;
			}
			if (!tutBool)
			{
				CAMERASPEED += 0.005 * dt;
				sManager.update(dt, DDOS->getSprite()->getPosition().y);

				if (moveScreen)
				{
					for (unsigned int i = 0; i < pauseMenu->getLabelList().size(); i++) {
						pauseMenu->getLabelList()[i]->setPosition(cocos2d::Vec2(pauseMenu->getLabelList()[i]->getPosition().x,
							pauseMenu->getLabelList()[i]->getPosition().y + CAMERASPEED));
					}
					pausedLabel->setPosition(pausedLabel->getPosition() + cocos2d::Vec2(0, CAMERASPEED));
					startLabel->setPosition(startLabel->getPosition() + cocos2d::Vec2(0, CAMERASPEED));

					playerOne->getUI()->updatePosition(cocos2d::Vec2(0, CAMERASPEED));
					playerTwo->getUI()->updatePosition(cocos2d::Vec2(0, CAMERASPEED));

					this->getDefaultCamera()->setPosition(cocos2d::Vec2(this->getDefaultCamera()->getPosition().x,
						this->getDefaultCamera()->getPosition().y + CAMERASPEED));
					DDOS->getSprite()->setPosition(cocos2d::Vec2(100, (DDOS->getSprite()->getPosition().y + CAMERASPEED)));

					if (DDOS->getSprite()->getPosition().y - bg2->getPosition().y >= 588.8f) {
						bg2->setPosition(cocos2d::Vec2(bg2->getPosition().x, bg2->getPosition().y + 588.8f));
					}
					if (DDOS->getSprite()->getPosition().y - bg3->getPosition().y >= 588.8f) {
						bg3->setPosition(cocos2d::Vec2(bg3->getPosition().x, bg3->getPosition().y + 588.8f));
					}
				}
			}

			else//if tutorial//TODO
			{//make stuff for player to interact with


				//if (tutorialSlow&&tutorialShoot&&tutorialKick)//more to be added
					//tutorialEnd = true;
			}
		}

		srand(time(0));
		checkInput(dt);
		getCollisions();


#ifdef _DEBUG
		if (p1Controller->isButtonPressed(Sedna::Y) || p2Controller->isButtonPressed(Sedna::Y))
			moveScreen ^= 1;

		if (p1Controller->isButtonPressed(Sedna::X) || p2Controller->isButtonPressed(Sedna::X)) {
			for (unsigned int i = 0; i < sManager.outlawList.size(); i++)
				sManager.outlawList[i]->getBox()->getDrawNode()->setVisible(true);
			for (unsigned int i = 0; i < sManager.tableList.size(); i++)
				sManager.tableList[i]->getBox()->getDrawNode()->setVisible(true);
			playerOne->getBox()->getDrawNode()->setVisible(true);
			playerTwo->getBox()->getDrawNode()->setVisible(true);

		}
		else {
			for (unsigned int i = 0; i < sManager.outlawList.size(); i++)
				sManager.outlawList[i]->getBox()->getDrawNode()->setVisible(false);
			for (unsigned int i = 0; i < sManager.tableList.size(); i++)
				sManager.tableList[i]->getBox()->getDrawNode()->setVisible(false);

			playerOne->getBox()->getDrawNode()->setVisible(false);
			playerTwo->getBox()->getDrawNode()->setVisible(false);
		}//show hitboxes
#endif

		if (!bossTime)
			checkManyLists(dt);
		else {
			boss(dt);
		}

		playerOne->updateGameObject();
		playerTwo->updateGameObject();
		bounds();

		bloodyMaryP_up->pickUp(playerOne);
		bloodyMaryP_up->pickUp(playerTwo);
		theBiggestIronP_up->pickUp(playerOne);
		theBiggestIronP_up->pickUp(playerTwo);

	}
}

void HelloWorld::pause(float dt)
{
	if (p1Controller->isButtonPressed(Sedna::SELECT) || p2Controller->isButtonPressed(Sedna::SELECT))
	{
		p1Controller->setVibration(0, 0);
		p2Controller->setVibration(0, 0);
		if (!TEMPPAUSE)
		{
			for (unsigned int i = 0; i < pauseMenu->getLabelList().size(); i++) {
				pausedLabel->setVisible(true);
				pauseMenu->getLabelList()[i]->setVisible(true);
			}
			TEMPPAUSE = true;
			TRUEPAUSE ^= 1;
		}
	}


	if (p1Controller->isButtonReleased(Sedna::SELECT) && p2Controller->isButtonReleased(Sedna::SELECT))
		TEMPPAUSE = false;


	if (TRUEPAUSE)
	{

		paused = true;
		for (unsigned int i = 0; i < pauseMenu->getLabelList().size(); i++) {
			pausedLabel->setVisible(true);
			pauseMenu->getLabelList()[i]->setVisible(true);
		}



		if ((p1Sticks[0].y < -0.3f || p2Sticks[0].y < -0.3f) && pauseMenu->getIndexOfSelected() != 0) {

			pauseMenu->select(pauseMenu->getIndexOfSelected() - 1);
		}


		if (p1Sticks[0].y > 0.3f || p2Sticks[0].y > 0.3f) {
			if (pauseMenu->getIndexOfSelected() + 1 > pauseMenu->getLabelList().size() - 1) {
			}
			else
				pauseMenu->select(pauseMenu->getIndexOfSelected() + 1);
		}



		if (pauseMenu->getIndexOfSelected() == 1 && (p1Controller->isButtonPressed(Sedna::A) || p2Controller->isButtonPressed(Sedna::A))) {
			TRUEPAUSE = false;
		}


		if (pauseMenu->getIndexOfSelected() == 0 && (p1Controller->isButtonPressed(Sedna::A) || p2Controller->isButtonPressed(Sedna::A)) && !end) {
			auto mMenu = MenuScene::create();
			cocos2d::experimental::AudioEngine::stopAll();
			cocos2d::experimental::AudioEngine::play2d("cha ching.mp3", false);
			end = true;
			director->replaceScene(TransitionFade::create(2.0f, mMenu));
		}
	}


	else {
		for (unsigned int i = 0; i < pauseMenu->getLabelList().size(); i++) {
			pausedLabel->setVisible(false);
			pauseMenu->getLabelList()[i]->setVisible(false);
		}
	}



	if (!TRUEPAUSE)
	{

		useBulletTime(dt);

		if (gameStart < 5)
		{
			gameStart += dt;
			paused = true;
			startLabel->setVisible(true);
			if (gameStart > 1 && gameStart < 2)
			{
				startLabel->setString("3");
				if (!playMusic) {
					//					if (Sedna::optionStuff::music)
					cocos2d::experimental::AudioEngine::play2d("bgm.mp3", true);
					playMusic = true;
				}
			}
			else if (gameStart > 2 && gameStart < 3)
			{
				startLabel->setString("2");
			}
			else if (gameStart > 3 && gameStart < 4)
			{
				startLabel->setString("1");
			}
			else if (gameStart > 4 && gameStart < 5) {
				startLabel->setString("0");

			}
			else if (!playerOne->isDead() && !playerTwo->isDead())
			{
				startLabel->setVisible(false);
			}
		}

	}

}




void HelloWorld::checkInput(float dt)
{
	playerOne->checkInput(dt);
	playerTwo->checkInput(dt);
	if (!paused) {
		playerOne->shoot(dt, this);
		playerTwo->shoot(dt, this);
	}
}

void HelloWorld::getCollisions()
{
	if (tutBool) {
		playerOne->checkTableStuff(tutTables, playerTwo);
		playerTwo->checkTableStuff(tutTables, playerOne);
	}
	playerOne->checkTableStuff(sManager.tableList, playerTwo);
	playerTwo->checkTableStuff(sManager.tableList, playerOne);
}



void HelloWorld::checkManyLists(float dt)
{
	if (tutBool) {

		for (unsigned int i = 0; i < tutOutlaws.size(); i++) {

			tutOutlaws[i]->animate(dt);
			tutOutlaws[i]->checkBCollision(tutTables);
			if (tutOutlaws[i]->points == 300)
				((Sedna::RifleOutlaw*)tutOutlaws[i])->setTrack(dummy);

			tutOutlaws[i]->shoot(dt, this);
			tutOutlaws[i]->checkList();
			tutOutlaws[i]->updateGameObject();

		}
		for (unsigned int i = 0; i < tutTables.size(); i++)
			tutTables[i]->updateGameObject();

		if (!tutCutscene) {
			playerOne->checkList();
			playerTwo->checkList();
		}

	}
	else {

		for (unsigned int i = 0; i < sManager.outlawList.size(); i++) {
			auto first = playerOne->getBox()->getLocation() - sManager.outlawList[i]->getBox()->getLocation();
			auto second = playerTwo->getBox()->getLocation() - sManager.outlawList[i]->getBox()->getLocation();

			if (sManager.outlawList[i]->points == 200)
				((Sedna::ShotgunOutlaw*)sManager.outlawList[i])->onLeftSideOf
				((first.getLengthSq() < second.getLengthSq()) ? playerOne : playerTwo);

			if (sManager.outlawList[i]->points == 300)
				((Sedna::RifleOutlaw*)sManager.outlawList[i])->setTrack
				((first.getLengthSq() < second.getLengthSq()) ? playerOne : playerTwo);

			if (sManager.outlawList[i]->points == 1000)
				((Sedna::CrazyPete*)sManager.outlawList[i])->updateDyn(dt, this);
			else
				sManager.outlawList[i]->shoot(dt, this);
		}


		checkPosAll();
		if (sManager.outlawList.size() > 6) {
			sManager.outlawList.front()->removeProjectiles();
			sManager.outlawList.front()->getBox()->getDrawNode()->removeFromParent();
			sManager.outlawList.front()->getSprite()->removeFromParent();
			sManager.outlawList.erase(sManager.outlawList.begin());
		}

		recursiveFunctionKnocked();
		recursiveFunctionOutlaw();
		recursiveFunctionTable();


		playerOne->checkBCollision(sManager.outlawList, bloodyMaryP_up, theBiggestIronP_up);
		playerTwo->checkBCollision(sManager.outlawList, bloodyMaryP_up, theBiggestIronP_up);
		//playerOne->checkBCollision(sManager.tableList);
		//playerTwo->checkBCollision(sManager.tableList);
		playerOne->checkList();
		playerTwo->checkList();
		for (unsigned int i = 0; i < sManager.tableList.size(); i++)
			sManager.tableList[i]->updateGameObject();

		for (unsigned int i = 0; i < sManager.outlawList.size(); i++) {

			sManager.outlawList[i]->animate(dt);
			sManager.outlawList[i]->checkBCollision(sManager.tableList);
			sManager.outlawList[i]->checkBCollision(playerOne);
			sManager.outlawList[i]->checkBCollision(playerTwo);
			sManager.outlawList[i]->checkList();
			sManager.outlawList[i]->updateGameObject();
		}

		for (unsigned int i = 0; i < sManager.tableList.size(); i++) {
			for (unsigned int j = 0; j < sManager.tableList.size(); j++) {
				if (i == j)
					continue;
				sManager.tableList[i]->collideTable(sManager.tableList[j]);
			}
		}

		btMeter.getDrawNode()->setPosition(btMeter.getDrawNode()->getPosition().x, btMeter.getDrawNode()->getPosition().y + CAMERASPEED);
		btLabel->setPosition(btLabel->getPosition().x, btLabel->getPosition().y + CAMERASPEED);
	}
}


void HelloWorld::recursiveFunctionOutlaw()
{
	for (unsigned int i = 0; i < sManager.outlawList.size(); i++) {
		for (unsigned int j = 0; j < sManager.outlawList.size(); j++) {
			if (i == j)
				continue;
			if (sManager.outlawList[i]->getBox()->checkCollision(*sManager.outlawList[j]->getBox())) {
				sManager.outlawList[i]->getBox()->setLocation(cocos2d::Vec2(100 + (rand() % 300),
					sManager.outlawList[i]->getBox()->getLocation().y + 50));
				recursiveFunctionOutlaw();
			}

		}
	}
}
void HelloWorld::recursiveFunctionTable()
{
	for (unsigned int i = 0; i < sManager.tableList.size(); i++) {
		if (sManager.tableList[i]->knocked != true)
		{
			for (unsigned int j = 0; j < sManager.tableList.size(); j++) {
				if (i == j)
					continue;
				if (sManager.tableList[i]->getBox()->checkCollision(*sManager.tableList[j]->getBox())) {
					sManager.tableList[i]->getBox()->setLocation(cocos2d::Vec2(100 + (rand() % 300),
						sManager.tableList[i]->getBox()->getLocation().y + 50));
					recursiveFunctionTable();
				}

			}
		}

	}

}
void HelloWorld::recursiveFunctionKnocked()
{
	for (unsigned int i = 0; i < sManager.tableList.size(); i++) {
		if (sManager.tableList[i]->knocked != true)
		{
			for (unsigned int j = 0; j < sManager.outlawList.size(); j++) {
				if (sManager.tableList[i]->getBox()->checkCollision(*sManager.outlawList[j]->getBox())) {
					sManager.tableList[i]->getBox()->setLocation(cocos2d::Vec2(100 + (rand() % 300),
						sManager.tableList[i]->getBox()->getLocation().y + 50));
					recursiveFunctionKnocked();
				}

			}
		}

	}

}

void HelloWorld::checkPosAll()//this function will remove and objects that go to far below the screen
{
	if (!bossTime) {

		for (unsigned int i = 0; i < sManager.outlawList.size(); i++)
		{
			if (sManager.outlawList[i]->getBox()->getLocation().y < DDOS->getSprite()->getPosition().y - 400)
			{
				sManager.outlawList[i]->removeProjectiles();
				sManager.outlawList[i]->getBox()->getDrawNode()->removeFromParent();
				sManager.outlawList[i]->getSprite()->removeFromParent();
				sManager.outlawList.erase(sManager.outlawList.begin() + i);
				i--;
			}
		}
	}

	for (unsigned int i = 0; i < sManager.tableList.size(); i++)
	{
		if (sManager.tableList[i]->getBox()->getLocation().y < DDOS->getSprite()->getPosition().y - 400)
		{
			sManager.tableList[i]->getBox()->getDrawNode()->removeFromParent();
			sManager.tableList[i]->getSprite()->removeFromParent();
			sManager.tableList.erase(sManager.tableList.begin() + i);
			i--;
		}
	}
	if (playerOne->getBox()->getLocation().y <= DDOS->getSprite()->getPosition().y - 400)//TODO change health sprites as well
	{
		playerOne->setHP(0);
		playerOne->die();
	}
	if (playerTwo->getBox()->getLocation().y <= DDOS->getSprite()->getPosition().y - 400)
	{
		playerTwo->setHP(0);
		playerTwo->die();
	}
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}
void HelloWorld::performBounce(Sedna::Player* p) {
	if ((int)p->getBox()->getLocation().x >= barRightMax)
	{
		p->getBox()->setLocation(cocos2d::Vec2(430, p->getBox()->getLocation().y));
		p->getBox()->addForce(-25, 0);
	}

	if ((int)p->getBox()->getLocation().x <= barLeftMax)
	{
		p->getBox()->setLocation(cocos2d::Vec2(90, p->getBox()->getLocation().y));
		p->getBox()->addForce(25, 0);
	}
	if (p->getBox()->getLocation().y >= DDOS->getSprite()->getPosition().y) {
		p->getBox()->setLocation(cocos2d::Vec2(p->getBox()->getLocation().x, DDOS->getSprite()->getPosition().y));
		p->getBox()->addForce(0, -25);
	}
}
void HelloWorld::bounds()//this function stops the player from leaving the screen on the left and right
{

	performBounce(playerOne);
	performBounce(playerTwo);



	if (tutBool) {
		for (unsigned int i = 0; i < tutTables.size(); i++)
		{
			if ((int)tutTables[i]->getBox()->getLocation().x >= barRightMax)
			{
				tutTables[i]->getBox()->setLocation(cocos2d::Vec2(430, tutTables[i]->getBox()->getLocation().y));
				tutTables[i]->getBox()->addForce(-25, 0);
			}

			if ((int)tutTables[i]->getBox()->getLocation().x <= barLeftMax)
			{
				tutTables[i]->getBox()->setLocation(cocos2d::Vec2(90, tutTables[i]->getBox()->getLocation().y));
				tutTables[i]->getBox()->addForce(25, 0);
			}
		}
	}
	else {
		for (unsigned int i = 0; i < sManager.tableList.size(); i++)
		{
			if ((int)sManager.tableList[i]->getBox()->getLocation().x >= barRightMax)
			{
				sManager.tableList[i]->getBox()->setLocation(cocos2d::Vec2(430, sManager.tableList[i]->getBox()->getLocation().y));
				sManager.tableList[i]->getBox()->addForce(-25, 0);
			}

			if ((int)sManager.tableList[i]->getBox()->getLocation().x <= barLeftMax)
			{
				sManager.tableList[i]->getBox()->setLocation(cocos2d::Vec2(90, sManager.tableList[i]->getBox()->getLocation().y));
				sManager.tableList[i]->getBox()->addForce(25, 0);
			}
		}

	}

}

void HelloWorld::togglePause() {//this actually has many applications
	paused ^= 1;
}
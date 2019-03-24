#include "menuScene.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"
#include "HelloWorldScene.h"
#include "MusicList.h"
#include "Options.h"

USING_NS_CC;

bool MenuScene::end = false;

Scene* MenuScene::createScene() {
	return MenuScene::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


namespace Sedna {

	SednaMenu::SednaMenu(int args, ...)
	{
		va_list LIST;
		va_start(LIST, args);
		for (int i = 0; i < args; i++) {
			labelList.push_back(va_arg(LIST, cocos2d::Label*));
		}
		va_end(LIST);

	}



	void SednaMenu::select(unsigned int index)
	{
#ifdef _DEBUG
		if (index < 0 || index > labelList.size() - 1)
			exit(std::stoi("exception"));
#endif

		labelList[lastIndex]->enableWrap(false);
		labelList[index]->enableWrap(false);
		labelList[lastIndex]->disableEffect(LabelEffect::UNDERLINE);
		labelList[index]->enableUnderline();

		labelList[index]->enableWrap(true);

		lastIndex = index;
	}

	unsigned int SednaMenu::getIndexOfSelected() const
	{
		for (int i = 0; i < labelList.size(); i++) {
			if (labelList[i]->isWrapEnabled())
				return i;
		}
		return 0;
	}


	std::vector<cocos2d::Label*> SednaMenu::getLabelList() const
	{
		return labelList;
	}

	cocos2d::Label * SednaMenu::operator[](const unsigned int index)
	{
		if (index > labelList.size())
			exit(std::stoi("We're out of index chief"));
		return labelList[index];
	}

}







bool MenuScene::init() {
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
		CC_CALLBACK_1(MenuScene::menuCloseCallback, this));

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

	director = Director::getInstance();
	//p2Controller = manager.getController(1);

	p1Controller = manager.getController(0);
	p1Controller->updateSticks(p1Sticks);

	manager.update();

	label = Label::create("Exit", "fonts/Montague.ttf", 25);
	label2 = Label::create("Options", "fonts/Montague.ttf", 25);
	label3 = Label::create("Start Game", "fonts/Montague.ttf", 25);
	menuE = new Sedna::SednaMenu(3, label, label2, label3);

	oLabel = Label::create(oLabelStrings[Sedna::optionStuff::music], "fonts/Montague.ttf", 25);
	oLabel2 = Label::create(oLabel2Strings[Sedna::optionStuff::tutorial], "fonts/Montague.ttf", 25);
	menuO = new Sedna::SednaMenu(2,oLabel,oLabel2);

	


	
	initMenu();

	menuE->select(2);

	if(Sedna::optionStuff::music)
		cocos2d::experimental::AudioEngine::play2d("bgm2.mp3", true);
	end = false;
	this->scheduleUpdate();

	return true;
}

void MenuScene::update(float dt)
{
	if (director->getInstance()->getRunningScene()==this) {
	

		manager.update();
		p1Controller->updateSticks(p1Sticks);
		if (!optionMenuBool)
		{
			if (!menuWait)
			{
				if (p1Sticks[0].y < -0.3f && menuE->getIndexOfSelected() != 0)
				{

					menuE->select(menuE->getIndexOfSelected() - 1);
					menuWait += dt;
				}
				if (p1Sticks[0].y > 0.3f)
				{
					if (menuE->getIndexOfSelected() + 1 > menuE->getLabelList().size() - 1) {
					}
					else
					{
						menuE->select(menuE->getIndexOfSelected() + 1);
						menuWait += dt;
					}
				}
			}
			else
			{
				menuWait += dt;
				if (menuWait >= 0.3)
					menuWait = 0.0f;
			}

			if (menuE->getIndexOfSelected() == 2 && p1Controller->isButtonPressed(Sedna::A)) {
				auto game = HelloWorld::createScene();
				cocos2d::experimental::AudioEngine::play2d("cha ching.mp3", false);

				cocos2d::experimental::AudioEngine::stop(0);
				end = true;
				//this->onExit();
				director->replaceScene(TransitionFade::create(1.0f, game));
			}
			if (menuE->getIndexOfSelected() == 1 && p1Controller->isButtonPressed(Sedna::A)) {
				optionMenuBool = true;
				selectWait += dt;
				for (unsigned int i=0; i<menuE->getLabelList().size();i++)
				{
					menuE->getLabelList()[i]->setVisible(false);
				}
				for (unsigned int i=0; i <menuO->getLabelList().size();i++)
				{
					menuO->getLabelList()[i]->setVisible(true);
				}
				menuO->select(1);
			}
			if (menuE->getIndexOfSelected() == 0 && p1Controller->isButtonPressed(Sedna::A)) {
				cocos2d::experimental::AudioEngine::play2d("cha ching.mp3", false);

				exit(0);

			}
		}
		else
		{
			if (p1Sticks[0].y < -0.3f && menuO->getIndexOfSelected() != 0)
			{

				menuO->select(menuE->getIndexOfSelected() - 1);
				menuWait += dt;
			}
			if (p1Sticks[0].y > 0.3f)
			{
				if (menuO->getIndexOfSelected() + 1 > menuO->getLabelList().size() - 1)
				{
				}
				else
				{
					menuO->select(menuO->getIndexOfSelected() + 1);
					menuWait += dt;
				}
			}

			if (menuO->getIndexOfSelected() == 0 && p1Controller->isButtonPressed(Sedna::A) && !selectWait)
			{
				Sedna::optionStuff::music ^= 1;
				menuO->getLabelList()[0]->setString(oLabelStrings[Sedna::optionStuff::music]);
				selectWait += dt;
			}
			else if (menuO->getIndexOfSelected() == 1 && p1Controller->isButtonPressed(Sedna::A) && !selectWait)
			{
				Sedna::optionStuff::tutorial ^= 1;
				menuO->getLabelList()[1]->setString(oLabel2Strings[Sedna::optionStuff::tutorial]);
				selectWait += dt;
			}
			else
			{
				if(selectWait)
					selectWait += dt;
				if (selectWait >= 0.3f)
					selectWait = 0.0f;
			}
			if (p1Controller->isButtonPressed(Sedna::B))
			{
				optionMenuBool = false;
				for (unsigned int i = 0; i < menuE->getLabelList().size(); i++)
				{
					menuE->getLabelList()[i]->setVisible(true);
				}
				for (unsigned int i = 0; i < menuO->getLabelList().size(); i++)
				{
					menuO->getLabelList()[i]->setVisible(false);
				}
				menuE->select(1);

			}
		}
		
	}
}

void MenuScene::initMenu()
{
	cocos2d::experimental::AudioEngine::preload("cha ching.mp3");
	cocos2d::experimental::AudioEngine::preload("bgm2.mp3");
	Vec2 windowSize = director->getWinSizeInPixels();

	background = Sprite::create("Menu.png");
	background->setAnchorPoint(Vec2(0.0f, 0.0f));
	background->setScale(0.85f, 0.92f);
	background->setPosition(0, 0);

	title = Label::create("Goldman's Saloon", "fonts/Montague.ttf", 25);
	title->setAnchorPoint(Vec2(0.0f, 0.0f));
	title->setPosition(Vec2(140, 250));
	title->enableShadow();


	for (int i = 0; i < menuE->getLabelList().size(); i++) {
		this->addChild(menuE->getLabelList()[i]);
		menuE->getLabelList()[i]->enableShadow();
		menuE->getLabelList()[i]->setAnchorPoint(cocos2d::Vec2(0, 0));
		if (i == 0)
			menuE->getLabelList()[i]->setPosition(140, 50);
		else {
			menuE->getLabelList()[i]->setPosition(140, menuE->getLabelList()[i - 1]->getPosition().y + 30);

		}
	}

	for (int i = 0; i < menuO->getLabelList().size(); i++) {
		this->addChild(menuO->getLabelList()[i]);
		menuO->getLabelList()[i]->enableShadow();
		menuO->getLabelList()[i]->setAnchorPoint(cocos2d::Vec2(0, 0));
		if (i == 0)
			menuO->getLabelList()[i]->setPosition(140, 50);
		else {
			menuO->getLabelList()[i]->setPosition(140, menuO->getLabelList()[i - 1]->getPosition().y + 30);
		}
		menuO->getLabelList()[i]->setVisible(false);
	}	

	this->addChild(background, -1000);
	this->addChild(title);
}





void MenuScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}


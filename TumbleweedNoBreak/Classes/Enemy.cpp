#include "Enemy.h"
#include "baseObjectManager.h"
#include "Player.h"
#include "Table.h"
namespace Sedna {

Sedna::Outlaw::Outlaw(float x, float y)
{
	
	sprite = cocos2d::Sprite::create("enemy1.png");
	sprite->setScale(0.85f);

	hitBox = new CirclePrimitive(cocos2d::Vec2(x, y), 28, 5, 30);
	hitBox->getDrawNode()->setVisible(false);

	this->objectHp = 3;
}
void Outlaw::shoot(float dt,cocos2d::Scene* s)
{
	if (eShootTimer > 0.3f) {
		eShootTimer = 0.0f;
		eHasShot = false;
	}
	if (!eShootTimer) {
		eHasShot = true;
		Projectile* eBaseProjectile = new Sedna::Projectile(-1000, 10, Enemy);
		s->addChild(eBaseProjectile->getBox()->getDrawNode());
		s->addChild(eBaseProjectile->getSprite());

		eProjectiles.push_back(new Sedna::Projectile(*eBaseProjectile));
		BaseObjectManager::eProjectileBObjects.push_back(eBaseProjectile);

		eProjectiles.back()->getBox()->setLocation(this->getBox()->getLocation());
		eProjectiles.back()->getBox()->setForce(cocos2d::Vec2(0, -5));
	}
	if (eHasShot)
		eShootTimer += dt;
}
void Outlaw::checkList()
{
	if (eProjectiles.size() > 4) {
		eProjectiles.front()->getBox()->getDrawNode()->removeFromParent();
		eProjectiles.front()->getSprite()->removeFromParent();
		eProjectiles.erase(eProjectiles.begin());
	}
	for (int i = 0; i < eProjectiles.size(); i++)
		eProjectiles[i]->updateGameObject();
}
void Outlaw::removeProjectiles()
{
	for (int i = 0; i < eProjectiles.size(); i++) {
		eProjectiles[i]->getBox()->getDrawNode()->removeFromParent();
		eProjectiles[i]->getSprite()->removeFromParent();
		eProjectiles.erase(eProjectiles.begin() + i);
		BaseObjectManager::eProjectileBObjects.erase(BaseObjectManager::eProjectileBObjects.begin() + i);
		i--;
	}
}
void Outlaw::checkBCollision(Player * p)
{
	for (int i = 0; i < eProjectiles.size(); i++) {
		if (eProjectiles.empty())
			break;
		if(eProjectiles[i]->getBox()->checkCollision(*p->getBox())) {
			eProjectiles[i]->getBox()->getDrawNode()->removeFromParent();
			eProjectiles[i]->getSprite()->removeFromParent();
			eProjectiles.erase(eProjectiles.begin() + i);
			BaseObjectManager::eProjectileBObjects.erase(BaseObjectManager::eProjectileBObjects.begin() + i);
		}
	}
}
void Outlaw::checkBCollision(std::vector<Table*>& tableList)
{
	bool check = false;
	for (int i = 0; i < eProjectiles.size(); i++) {
		for (int j = 0; j < tableList.size(); j++) {
			if (eProjectiles.empty())
				break;
			if (eProjectiles[i]->getBox()->checkCollision(*tableList[j]->getBox())) {

				tableList[j]->setHP(tableList[j]->getHP() - 1);
				eProjectiles[i]->getBox()->getDrawNode()->removeFromParent();
				eProjectiles[i]->getSprite()->removeFromParent();
				eProjectiles.erase(eProjectiles.begin() + i);
				BaseObjectManager::eProjectileBObjects.erase(BaseObjectManager::eProjectileBObjects.begin() + i);
				check = true;
				break;


			}
			else
				check = false;
			if (!tableList[j]->getHP()) {
				tableList[j]->getBox()->getDrawNode()->removeFromParent();
				tableList[j]->getSprite()->removeFromParent();
				tableList.erase(tableList.begin() + j);
				j--;
			}

			if (check) {
				i--;
				check = false;
			}
		}
	}
}
}		

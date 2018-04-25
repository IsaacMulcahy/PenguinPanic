#include "Cannon.h"

Cannon::Cannon(ASGE::Renderer* renderer)
{
	base = std::make_unique<GameObject>();
	base->loadObject(renderer, "..\\..\\Resources\\Textures\\Cannon\\Cannon.png");

	loaded = std::make_unique<GameObject>();
	loaded->loadObject(renderer, "..\\..\\Resources\\Textures\\Cannon\\CannonLoaded.png");

	loaded_light = std::make_unique<GameObject>();
	loaded_light->loadObject(renderer, "..\\..\\Resources\\Textures\\Cannon\\CannonLoadedLight.png");
	
	loaded_heavy = std::make_unique<GameObject>();
	loaded_heavy->loadObject(renderer, "..\\..\\Resources\\Textures\\Cannon\\CannonLoadedHeavy.png");
}

vector2 Cannon::getObjectPosition()
{
	return base->getObjectPosition();
}

void Cannon::positionObject(int x, int y)
{
	base->positionObject(x, y);
	loaded->positionObject(x, y);
	loaded_light->positionObject(x, y);
	loaded_heavy->positionObject(x, y);
}

void Cannon::rotateObject(float value)
{
	base->rotateObject(value);
	loaded->rotateObject(value);
	loaded_light->rotateObject(value);
	loaded_heavy->rotateObject(value);
}

void Cannon::render(ASGE::Renderer* renderer)
{
	switch (state)
	{
		case CANNON_STATE::UNLOADED:
			renderer->renderSprite(*base->getObject());
		break;
		case CANNON_STATE::LOADED:
			renderer->renderSprite(*loaded->getObject());
		break;
		case CANNON_STATE::LOADED_LIGHT:
			renderer->renderSprite(*loaded_light->getObject());
		break;
		case CANNON_STATE::LOADED_HEAVY:
			renderer->renderSprite(*loaded_heavy->getObject());
		break;
	}
}
#include "Cannon.h"

Cannon::Cannon(ASGE::Renderer* renderer)
{
	base = std::make_unique<GameObject>();
	base->loadObject(renderer, "..\\..\\Resources\\Textures\\Cannon.png");

	loaded = std::make_unique<GameObject>();
	loaded->loadObject(renderer, "..\\..\\Resources\\Textures\\CannonLoaded.png");
}

vector2 Cannon::getObjectPosition()
{
	return base->getObjectPosition();
}

void Cannon::positionObject(int x, int y)
{
	base->positionObject(x, y);
	loaded->positionObject(x, y);
}

void Cannon::rotateObject(float value)
{
	base->rotateObject(value);
	loaded->rotateObject(value);
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
	}
}
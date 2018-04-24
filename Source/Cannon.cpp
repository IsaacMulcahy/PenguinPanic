#include "Cannon.h"

void Cannon::setupObject(ASGE::Renderer* renderer)
{
	base->loadObject(renderer, "..\\..\\Resources\\Textures\\Cannon.png");
	base->loadObject(renderer, "..\\..\\Resources\\Textures\\CannonLoaded.png");
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
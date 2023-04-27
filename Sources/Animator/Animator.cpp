#include "pch.h"
#include "Animator.h"

Animator::Animator()
	: totalTime(0.0f)
	, elapsTime(0.0f)
{}

Animator::~Animator()

{
}

void Animator::SetAnim(Sprite sprite, float lifeTime, float totalTime)
{
	this->_sprites.push_back(sprite);
	this->_lifeTimes.push_back(lifeTime);
	this->totalTime = totalTime;
}

Sprite & Animator::getSprite(unsigned int index)
{
	return _sprites[index];
}

unsigned int Animator::getSize()
{
	return this->_sprites.size();
}

void Animator::Update(float dt)
{
	for (int i = 0; i < _sprites.size(); i++)
	{
		if ((_sprites.size() > 0) && (this->_lifeTimes[i] >= totalTime))
		{
			this->_sprites.erase(_sprites.begin() + i);
			this->_lifeTimes.erase(_lifeTimes.begin() + i);
		}
		else if (_sprites.size() > 0)
		{
			this->_lifeTimes[i] += dt;

			this->_sprites.at(i).setPosition({
				this->_sprites.at(i).getPosition().x + 1,
				this->_sprites.at(i).getPosition().y + 2
				});

			this->_sprites.at(i).setRotation(
				this->_sprites.at(i).getRotation() + 3
			);

			//this->_sprites.at(i).setTint({Base::RandF(),Base::RandF(),Base::RandF(), 1.f});

			this->_sprites.at(i).MakeObjectTM();
		}
	}

}
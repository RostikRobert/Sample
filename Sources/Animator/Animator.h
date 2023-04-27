#include "Sprite/Sprite.h"
#include "Node/Node.h"

class Animator : public Node
{
public:
	// default {ctor} and {dtor}
	//
	Animator();
	~Animator();

	// lock copy and assigment
	//
	Animator(const Animator& other) = delete;
	Animator& operator=(const Animator& other) = delete;

	void SetAnim(Sprite sprite, float lifeTime, float totalTime);

	Sprite & Animator::getSprite(unsigned int index);
	unsigned int Animator::getSize();

	void Update(float dt);
	
	//void DropSprite(Sprite & spr);

private:
	Sprite * spr;
	float elapsTime;
	float totalTime;

	std::vector<Sprite> _sprites;
	std::vector<float> _lifeTimes;
	
};
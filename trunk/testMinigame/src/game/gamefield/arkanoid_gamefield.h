#ifndef ARKANOID_GAMEFIELD_H
#define ARKANOID_GAMEFIELD_H

#include <vector>

class RenderSystem;
class InputMessage;

class ArkanoidPad;
class ArkanoidBall;
class ArkanoidBrick;
class ArkanoidPadBonus;

typedef std::vector<ArkanoidBall> ArkanoidBallsVec;
typedef std::vector<ArkanoidBrick> ArkanoidBricksVec;
typedef std::vector<ArkanoidPadBonus> ArkanoidBonusesVec;

class ArkanoidGamefield
{
public:
	ArkanoidGamefield(RenderSystem* renderSystem);
	~ArkanoidGamefield();

	void draw();
	void update(float dt);
};

#endif //ARKANOID_GAMEFIELD_H
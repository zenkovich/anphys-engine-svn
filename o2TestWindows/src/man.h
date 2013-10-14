#ifndef MAN_H
#define MAN_H

#include "public.h"

#include <vector>
#include <map>

#include "util/xml_tools.h"
#include "util/math/rect.h"

OPEN_O2_NAMESPACE

class grSprite;
class cManField;

class cMan
{
	struct WalkAnimation
	{
		typedef std::vector<fRect> RectsList;
		typedef std::map<std::string, vec2i> AnimRangesVec;
		
		grSprite*     mAnimatingSprite;
		RectsList     mFrames;
		float         mFramesDelay;
		AnimRangesVec mAnimations;

		float         mCurrTime;
		int           mCurrAnim[2];


		WalkAnimation(grSprite* animatingSprite, pugi::xml_node& xmlNode);

		void update(float dt);
		void setPlayingAnim(const std::string& id);
	};

	vec2f          mPosition;
	vec2f          mSpriteOffset;
	grSprite*      mSprite;
	cManField*     mManField;
	WalkAnimation* mWalkAnim;

public:
	cMan(cManField* manField, pugi::xml_node& xmlNode);
	~cMan();

	void draw();
	void update(float dt);

	void setWayPoint(const vec2f& point);
};

CLOSE_O2_NAMESPACE

#endif //MAN_H
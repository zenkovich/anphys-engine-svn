#include "man.h"

#include "render_system/sprite.h"
#include "app/application.h"

#include "man_field.h"

OPEN_O2_NAMESPACE

cMan::WalkAnimation::WalkAnimation( grSprite* animatingSprite, pugi::xml_node& xmlNode )
{
	mAnimatingSprite = animatingSprite;

	mCurrTime = 0;

	mFramesDelay = xmlNode.child("framesDelay").attribute("v").as_float();

	if (pugi::xml_node framesNode = xmlNode.child("frames"))
	{
		for (pugi::xml_node obj = framesNode.child("frame"); obj; obj = obj.next_sibling("frame"))
		{
			mFrames.push_back(cXmlTools::node2rectf(obj));
		}
	}

	if (pugi::xml_node animationsNode = xmlNode.child("animations"))
	{
		for (pugi::xml_node obj = animationsNode.child("anim"); obj; obj = obj.next_sibling("anim"))
		{
			std::string id = obj.attribute("id").value();
			int beg = obj.attribute("begin").as_int();
			int end = obj.attribute("end").as_int();
			
			mAnimations[id].x = beg;
			mAnimations[id].y = end;
		}
	}

	mCurrAnim[0] = 0; mCurrAnim[1] = 1;
}

void cMan::WalkAnimation::update( float dt )
{
	mCurrTime += dt;
	int frameId = (int)(mCurrTime/mFramesDelay)%(mCurrAnim[1] - mCurrAnim[0]) + mCurrAnim[0];

	mAnimatingSprite->setTextureSrcRect(mFrames[frameId]);
}

void cMan::WalkAnimation::setPlayingAnim( const std::string& id )
{
	mCurrAnim[0] = mAnimations[id].x;
	mCurrAnim[1] = mAnimations[id].y;
}


cMan::cMan( cManField* manField, pugi::xml_node& xmlNode )
{
	mManField = manField;
	
	mSpriteOffset = cXmlTools::node2vecf(xmlNode.child("spriteOffset"));
	mSprite = mnew grSprite(mManField->mApplication->getRenderSystem(), xmlNode.child("sprite"));

	mWalkAnim = mnew  WalkAnimation(mSprite, xmlNode);
}

cMan::~cMan()
{
	safe_release(mWalkAnim);
	safe_release(mSprite);
}

void cMan::draw()
{
	mSprite->draw();
}

void cMan::update( float dt )
{
	mWalkAnim->update(dt);
	mSprite->setPosition( mPosition.scale(mManField->mClusterSize) + mSpriteOffset );
}

void cMan::setWayPoint( const vec2f& point )
{
	vec2f diff = point - mPosition;

	if (abs(diff.x) > abs(diff.y))
	{
		if (diff.x < 0)
			mWalkAnim->setPlayingAnim("walk_left");
		else
			mWalkAnim->setPlayingAnim("walk_right");
	}
	else
	{		
		if (diff.y < 0)
			mWalkAnim->setPlayingAnim("walk_up");
		else
			mWalkAnim->setPlayingAnim("walk_down");
	}

	mPosition = point;
}

CLOSE_O2_NAMESPACE
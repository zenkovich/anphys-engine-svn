#include "puzzle_pic_minigame.h"

#include <float.h>
#include <algorithm>

#include "render_system/render_system.h"
#include "render_system/sprite.h"

#include "../dependencies/pugixml/pugixml.hpp"

PuzzlePictureMinigame::Cell::Cell( PuzzlePictureMinigame* puzzleMinigame, Texture* texture, const fRect& srcRect, 
	                               const fRect& spritePos, const vec2i& correctPos )
{
	mPuzzleMinigame = puzzleMinigame;

	mSprite = new Sprite(puzzleMinigame->mRenderSystem, texture, srcRect);
	mSprite->setPosition(spritePos.leftTop).setSize(spritePos.getSize());

	mCurrentPos = correctPos;
	mCorrectPos = correctPos;

	mClickable  = true;

	mSelected     = false;
	mSelectedCoef = 0;

	mUnderCursor     = false;
	mUnderCursorCoef = 0;

	update(0);
}

PuzzlePictureMinigame::Cell::~Cell()
{
	delete mSprite;
}

void PuzzlePictureMinigame::Cell::draw()
{
	if (mPerspectiveOffset > FLT_EPSILON) //shadow
	{
		mPuzzleMinigame->drawShadow(fRect(mSprite->getPosition(), 
		                                  mSprite->getPosition() + mSprite->getSize().scale(mSprite->getScale())), 
								    mUnderCursorCoef);
	}
	
	mSprite->draw();

	if (mSelectedCoef > FLT_EPSILON) //selection
	{
		mPuzzleMinigame->mSelectionSprite->setPosition(mCurrentScreenPos).
		                                   setColor(mSelectionColor).draw();
	}
}

void PuzzlePictureMinigame::Cell::update( float dt )
{
//selection coef
	if (mSelected || mSelectedCoef > FLT_EPSILON) 
	{
		mSelectedCoef += dt;

		if (mSelectedCoef > 1.0f)
			mSelectedCoef = 0;
	}

//under cursor coef
	if (mUnderCursor)
	{
		mUnderCursorCoef += dt/0.1f; // 0.1 - moving up time, sec
	}
	else
	{
		if (mClickable)
			mUnderCursorCoef -= dt/0.3f; // 0.3 - moving down time, sec
	}

	mUnderCursorCoef = max(min(mUnderCursorCoef, 1.0f), 0.0f); //clamp

//cell position with perspective effect
	vec2f cellSize = mPuzzleMinigame->mScreenRect.getSize().scale(vec2f(1.0f/(float)mPuzzleMinigame->mColums, 
		                                                                1.0f/(float)mPuzzleMinigame->mRows));

	vec2f initScreenPos = mPuzzleMinigame->mScreenRect.leftTop + 
		mCurrentPos.castTo<float>().scale(cellSize);

	vec2f screenCenterPos = initScreenPos + mSprite->getSize()*0.5f;
	vec2f screenRectCenter = (mPuzzleMinigame->mScreenRect.leftTop + mPuzzleMinigame->mScreenRect.rightDown)*0.5f;

	vec2f perspectiveVec = (screenCenterPos - screenRectCenter).normalize();

	mPerspectiveOffset = mMaxPerspOffset*mUnderCursorCoef;

	if (mClickable) 
		mCurrentScreenPos = initScreenPos + perspectiveVec*mPerspectiveOffset;

	mCurrentScreenRect = fRect(initScreenPos, initScreenPos + cellSize);

	mSprite->setPosition(mCurrentScreenPos);
	
	mSelectionColor = mPuzzleMinigame->mSelectionColor;
	mSelectionColor.a = (int)((float)mPuzzleMinigame->mSelectionColor.a*sinf(mSelectedCoef*3.1415926f));
}

bool PuzzlePictureMinigame::Cell::isIntersectPoint( const vec2f& point )
{
	return mClickable && mCurrentScreenRect.inRect(point);
}

float PuzzlePictureMinigame::Cell::mMaxPerspOffset = 7.0f;
float PuzzlePictureMinigame::SwappingCellsPair::mAnimationDelay = 0.7f;

PuzzlePictureMinigame::SwappingCellsPair::SwappingCellsPair( Cell* a, Cell* b )
{
	mSwappingCells[0] = a;
	mSwappingCells[1] = b;

	mAnimatingTime = 0;
	mCellsInitialPos[0] = a->mCurrentScreenPos;
	mCellsInitialPos[1] = b->mCurrentScreenPos;
	mFinished = false;
	
	a->mClickable = false;
	b->mClickable = false;
	
	a->mSelected = false;
	b->mSelected = false;

	vec2i tmp = mSwappingCells[0]->mCurrentPos;
	mSwappingCells[0]->mCurrentPos = mSwappingCells[1]->mCurrentPos;
	mSwappingCells[1]->mCurrentPos = tmp;
}

void PuzzlePictureMinigame::SwappingCellsPair::update( float dt )
{
	if (mFinished)
		return;

	mAnimatingTime += dt;

	float animCoef = min(mAnimatingTime/mAnimationDelay, 1.0f);

	animCoef = std::min<float>( sinf((animCoef + 0.5f)*-3.1415926f)*0.5f + 0.5f, 1.0f ); // magic smooth formula

	mFinished = mAnimatingTime > mAnimationDelay;
	if (mFinished)
	{
		mSwappingCells[0]->mClickable = true;
		mSwappingCells[1]->mClickable = true;
	}
	
	mSwappingCells[0]->mCurrentScreenPos = mCellsInitialPos[0] + (mCellsInitialPos[1] - mCellsInitialPos[0])*animCoef;
	mSwappingCells[1]->mCurrentScreenPos = mCellsInitialPos[1] + (mCellsInitialPos[0] - mCellsInitialPos[1])*animCoef;
}

bool PuzzlePictureMinigame::SwappingCellsPair::isFinished()
{
	return mFinished;
}

PuzzlePictureMinigame::PuzzlePictureMinigame(RenderSystem* renderSystem)
{
	mRenderSystem = renderSystem;
}

PuzzlePictureMinigame::~PuzzlePictureMinigame()
{
	delete mSelectionSprite;
	delete mShadowSprite;
	delete mBkSprite;
}

void PuzzlePictureMinigame::initialize()
{
	mSelectionColor = color4(6.0f, 1.0f, 0.6f, 0.5f);
	mSelectionSprite = new Sprite(mRenderSystem);
	mShadowSprite = new Sprite(mRenderSystem);

	loadConfig("../data/config.xml");
}

void PuzzlePictureMinigame::loadConfig( const char* configFile )
{
	pugi::xml_document xmlDoc;
	pugi::xml_parse_result res = xmlDoc.load_file(configFile);

	if (res.status != pugi::status_ok)
	{
		printf("ERROR: Failed to load xml file '%s'\nError desc: %s", configFile, res.description());
		return;
	}

//grid size
	pugi::xml_node gridSizeNode = xmlDoc.child("gridSize");

	if (!gridSizeNode.empty())
	{
		mColums = gridSizeNode.attribute("colums").as_int();
		mColums = min(max(mColums, 2), 100);

		mRows = gridSizeNode.attribute("rows").as_int();
		mRows = min(max(mRows, 2), 100);
	}
	else
	{
		mColums = mRows = 5;

		printf("WARNING: Failed to load grid size, Setting 5x5 as default\n");
	}

//picture filename
	pugi::xml_node picFilenameNode = xmlDoc.child("picture");
	if (picFilenameNode.empty())
	{
		printf("ERROR: Failed to load picture filename\n");
		return;
	}
		
	char pictureFilename[512];
	strcpy(pictureFilename, picFilenameNode.attribute("filename").value());

//texture
	Texture* texture = mRenderSystem->createTexture(pictureFilename);	

//back filename
	Texture* backTexture = NULL;

	pugi::xml_node backFilenameNode = xmlDoc.child("back");
	if (backFilenameNode.empty())
	{
		backTexture = NULL;
		printf("WARNING: Failed to load back filename\n");
		return;
	}
	else
	{
		backTexture = mRenderSystem->createTexture(backFilenameNode.attribute("filename").value());
	}

//back
	mBkSprite = new Sprite(mRenderSystem, backTexture);
	mBkSprite->setSize(mRenderSystem->getResolution().castTo<float>());

//screenrect
	pugi::xml_node screenRectNode = xmlDoc.child("screenRect");

	if (!screenRectNode.empty())
	{
		mScreenRect.leftTop.x = screenRectNode.attribute("left").as_float();
		mScreenRect.leftTop.y = screenRectNode.attribute("top").as_float();

		if (screenRectNode.attribute("right").empty())
		{
			mScreenRect.rightDown = texture->getSize() + mScreenRect.leftTop;
		}
		else
		{
			mScreenRect.rightDown.x = screenRectNode.attribute("right").as_float();
			mScreenRect.rightDown.y = screenRectNode.attribute("down").as_float();
		}
	}
	else
	{
		mScreenRect = fRect(vec2f(0, 0), mRenderSystem->getResolution().castTo<float>());

		printf("WARNING: Failed to screen size, Setting as resolution\n");
	}

//hint
	pugi::xml_node hintNode = xmlDoc.child("hint");
	if (hintNode.empty())
	{
		printf("WARNING: Failed to load hint parametres\n");

		mHintSprite = new Sprite(mRenderSystem, texture);
		mHintSprite->setSize(mHintSprite->getSize()*0.1f);
		mHintSprite->setPosition(vec2f((float)mRenderSystem->getResolution().x - mHintSprite->getSize().x, 0));
	}
	else
	{
		vec2f hintPos( hintNode.attribute("posx").as_float(), hintNode.attribute("posy").as_float() );
		float scale = hintNode.attribute("scale").as_float();

		mHintSprite = new Sprite(mRenderSystem, texture);
		mHintSprite->setPosition(hintPos).setScale(vec2f(scale, scale));
	}

//complete sprite
	mCompleteSprite = new Sprite(mRenderSystem, mRenderSystem->createTexture("../data/complete.tga"));

//picture pad
	mPicPadSprite = new Sprite(mRenderSystem);
	mPicPadSprite->setPosition(mScreenRect.leftTop - vec2f(4, 4)).setSize(mScreenRect.getSize() + vec2f(8, 8)).
		           setColor(color4(0.0f, 0.0f, 0.0f, 0.5f));

//initializing cells
	vec2f cellSize = mScreenRect.getSize().scale(vec2f(1.0f/(float)mColums, 1.0f/(float)mRows));
	vec2f cellTexSize = texture->getSize().scale(vec2f(1.0f/(float)mColums, 1.0f/(float)mRows));
	for (int x = 0; x < mColums; x++)
	{
		for (int y = 0; y < mRows; y++)
		{
			mCells.push_back(new Cell(this, texture, 
				                      fRect((float)x*cellTexSize.x, (float)y*cellTexSize.y,
				                            (float)(x + 1)*cellTexSize.x, (float)(y + 1)*cellTexSize.y), 

				                      fRect((float)x*cellSize.x, (float)y*cellSize.y,
				                            (float)(x + 1)*cellSize.x, (float)(y + 1)*cellSize.y), 
									  vec2i(x, y)));
		}
	}

	mSelectionSprite->setSize(cellSize);

	mGameState = GS_WAIT_BEGINNING;
	mSelectedCell = NULL;
	mBeginTime = 0;
}

void PuzzlePictureMinigame::click( float x, float y )
{
	if (mGameState == GS_WAIT_BEGINNING)
	{
		mGameState = GS_MIXING;
		beginMixing();
	}
	else if (mGameState == GS_MIXING)
	{
		completeMixingAnim();
		mGameState = GS_PLAYING;
	}
	else if (mGameState == GS_PLAYING)
	{
		processPlayingClick(x, y);
	}
	else if (mGameState == GS_COMPLETE_ANIM || mGameState == GS_FULLY_COMPLETE)
	{
		restart();
	}
}

void PuzzlePictureMinigame::cursorMove( float x, float y )
{	
	vec2f cursorPt(x, y);

	for (CellsArr::iterator it = mCells.begin(); it != mCells.end(); ++it)
	{
		(*it)->mUnderCursor = (*it)->isIntersectPoint(cursorPt);
	}
}

bool PuzzlePictureMinigame::isComplete()
{
	return mGameState == GS_FULLY_COMPLETE;
}

void PuzzlePictureMinigame::render()
{
	mBkSprite->draw();

	mPicPadSprite->draw();

//sort by perspective offset value
	std::sort(mCells.begin(), mCells.end(), Cell::compareByOffset);

	for (CellsArr::iterator it = mCells.begin(); it != mCells.end(); ++it)
	{
		if ((*it)->mClickable)
			(*it)->draw();
	}

	for (SwapCellsArr::iterator it = mSwappingCells.begin(); it != mSwappingCells.end(); ++it)
	{
		it->mSwappingCells[0]->draw();
		it->mSwappingCells[1]->draw();
	}

	mHintSprite->draw();

	if (mGameState == GS_COMPLETE_ANIM || mGameState == GS_FULLY_COMPLETE)
		mCompleteSprite->draw();
}

void PuzzlePictureMinigame::update( float dt )
{	
	if (mGameState == GS_WAIT_BEGINNING)
	{
		mBeginTime += dt;

		if (mBeginTime > mBeginDelay)
		{
			mGameState = GS_MIXING;
			beginMixing();
		}
	}
	else if (mGameState == GS_MIXING)
	{
		updateMixingAnim(dt);
	}
	else if (mGameState == GS_PLAYING)
	{
		updatePlayingState(dt);
	}
	else if (mGameState == GS_COMPLETE_ANIM)
	{
		updatePlayingState(dt);
		updateCompleteAnim(dt);
	}
}

void PuzzlePictureMinigame::drawShadow( const fRect& rect, float alpha )
{
	float shadowSize = 14;

	float resAlpha = 0.5f*alpha;
	
	drawShadowPart(rect.leftTop + vec2f(-shadowSize, -shadowSize), rect.leftTop, 0, 0, 1, 0, resAlpha);

	drawShadowPart(rect.leftTop + vec2f(0, -shadowSize), vec2f(rect.rightDown.x, rect.leftTop.y), 0, 0, 1, 1, resAlpha);

	drawShadowPart(vec2f(rect.rightDown.x, rect.leftTop.y) + vec2f(0, -shadowSize), 
		           vec2f(rect.rightDown.x, rect.leftTop.y) + vec2f(shadowSize, 0), 0, 0, 0, 1, resAlpha);

	drawShadowPart(vec2f(rect.rightDown.x, rect.leftTop.y), 
		           vec2f(rect.rightDown.x, rect.rightDown.y) + vec2f(shadowSize, 0), 1, 0, 0, 1, resAlpha);

	drawShadowPart(vec2f(rect.rightDown.x, rect.rightDown.y), 
		           vec2f(rect.rightDown.x, rect.rightDown.y) + vec2f(shadowSize, shadowSize), 1, 0, 0, 0, resAlpha);

	drawShadowPart(vec2f(rect.leftTop.x, rect.rightDown.y), 
		           vec2f(rect.rightDown.x, rect.rightDown.y) + vec2f(0, shadowSize), 1, 1, 0, 0, resAlpha);

	drawShadowPart(vec2f(rect.leftTop.x, rect.rightDown.y) + vec2f(-shadowSize, 0), 
		           vec2f(rect.leftTop.x, rect.rightDown.y) + vec2f(0, shadowSize), 0, 1, 0, 0, resAlpha);

	drawShadowPart(vec2f(rect.leftTop.x, rect.leftTop.y) + vec2f(-shadowSize, 0), 
		           vec2f(rect.leftTop.x, rect.rightDown.y), 0, 1, 1, 0, resAlpha);
}

void PuzzlePictureMinigame::drawShadowPart( const vec2f& leftTop, const vec2f& rightDown, 
	                                        float c0, float c1, float c2, float c3, float alpha )
{
	mShadowSprite->setPosition(leftTop).setSize(rightDown - leftTop). 
		           setColor(color4(0.0f, 0.0f, 0.0f, c0*alpha), 0).         
				   setColor(color4(0.0f, 0.0f, 0.0f, c1*alpha), 1).         
				   setColor(color4(0.0f, 0.0f, 0.0f, c2*alpha), 2).         
				   setColor(color4(0.0f, 0.0f, 0.0f, c3*alpha), 3).draw();
}

void PuzzlePictureMinigame::updateMixingAnim( float dt )
{
	mMixingTime += dt;

	if (mMixingTime > mMixingDelay)
	{
		mGameState = GS_PLAYING;
		completeMixingAnim();
		return;
	}

	const float oneCellAnimDelay = 0.5f;
	const float maxCoordsSumm = (float)(mColums + mRows - 2);

	vec2f cellSize = mScreenRect.getSize().scale(vec2f(1.0f/(float)mColums, 1.0f/(float)mRows));

	for (CellsArr::iterator it = mCells.begin(); it != mCells.end(); ++it)
	{
		Cell* animCell = (*it);
		float coordsSumm = (float)(animCell->mCurrentPos.x + animCell->mCurrentPos.y);

		float beginTime = (mMixingDelay - oneCellAnimDelay)*coordsSumm/maxCoordsSumm;

		float timeCoef = max(min((mMixingTime - beginTime)/oneCellAnimDelay, 1.0f), 0.0f);

		vec2f texSize = animCell->mSprite->getTexture()->getSize();
		vec2f texPos = animCell->mCurrentPos.castTo<float>();

		if (timeCoef > 0.5f)
			texPos = animCell->mCorrectPos.castTo<float>();

		//updating tex coords
		animCell->mSprite->setTextureSrcRect(
			fRect( vec2f(texSize.x/(float)mColums*texPos.x,          texSize.y/(float)mRows*texPos.y), 
			       vec2f(texSize.x/(float)mColums*(1.0f + texPos.x), texSize.y/(float)mRows*(1.0f + texPos.y)) ) );

		//calculate sprite pos and size
		float ySize = abs(cosf(timeCoef*3.1415926f)*cellSize.y);

		vec2f screenPos = mScreenRect.leftTop + cellSize.scale(animCell->mCurrentPos.castTo<float>());

		vec2f spritePos = screenPos + vec2f(0, cellSize.y - ySize)*0.5f;
		vec2f spriteSize = vec2f(cellSize.x, ySize);

		animCell->mSprite->setPosition(spritePos).setSize(spriteSize);
	}
}

void PuzzlePictureMinigame::updatePlayingState( float dt )
{
	for (CellsArr::iterator it = mCells.begin(); it != mCells.end(); ++it)
	{
		(*it)->update(dt);
	}

	for (SwapCellsArr::iterator it = mSwappingCells.begin(); it != mSwappingCells.end();)
	{
		it->update(dt);

		if (it->isFinished())
		{
			checkCompletion();
			it = mSwappingCells.erase(it); //remove completed pair
		}
		else
		{
			it++;
		}
	}
}

void PuzzlePictureMinigame::beginMixing()
{
	mMixingTime = 0;

	for (CellsArr::iterator it = mCells.begin(); it != mCells.end(); ++it)
	{
		//search correct cell
		Cell* swapCell = NULL;
		do 
		{
			swapCell = mCells[rand()%mCells.size()];
		}
		while(!swapCell && swapCell != *it && swapCell->mCurrentPos == (*it)->mCorrectPos);

		//swap current positions
		vec2i tmp = swapCell->mCurrentPos;
		swapCell->mCurrentPos = (*it)->mCurrentPos;
		(*it)->mCurrentPos = tmp;
	}
}

void PuzzlePictureMinigame::checkCompletion()
{
	for (CellsArr::iterator it = mCells.begin(); it != mCells.end(); ++it)
	{
		if ((*it)->mCurrentPos != (*it)->mCorrectPos)
			return;
	}

	mGameState = GS_COMPLETE_ANIM;
	beginCompleteAnim();
}

void PuzzlePictureMinigame::completeMixingAnim()
{	
	//forcible setting position and tex coords
	vec2f cellSize = mScreenRect.getSize().scale(vec2f(1.0f/(float)mColums, 1.0f/(float)mRows));
	for (CellsArr::iterator it = mCells.begin(); it != mCells.end(); ++it)
	{
		Cell* animCell = (*it);

		vec2f texSize = animCell->mSprite->getTexture()->getSize();
		vec2f texPos = animCell->mCorrectPos.castTo<float>();

		animCell->mSprite->setTextureSrcRect(
			fRect( vec2f(texSize.x/(float)mColums*texPos.x,          texSize.y/(float)mRows*texPos.y), 
				   vec2f(texSize.x/(float)mColums*(1.0f + texPos.x), texSize.y/(float)mRows*(1.0f + texPos.y)) ) );

		vec2f screenPos = mScreenRect.leftTop + cellSize.scale(animCell->mCurrentPos.castTo<float>());

		animCell->mSprite->setPosition(screenPos).setSize(cellSize);
	}
}

void PuzzlePictureMinigame::beginCompleteAnim()
{
	mCompleteTime = 0;

	for (CellsArr::iterator it = mCells.begin(); it != mCells.end(); ++it)
		(*it)->mSelected = false;

	updateCompleteAnim(0);
}

void PuzzlePictureMinigame::updateCompleteAnim( float dt )
{
	mCompleteTime += dt;

	if (mCompleteTime > mCompleteAnimDelay)
		mGameState = GS_FULLY_COMPLETE;

	//sprite alpha and positions
	mCompleteSprite->setPosition(mRenderSystem->getResolution().castTo<float>()*0.5f - 
		                         mCompleteSprite->getSize()*0.5f).
					 setColor(color4(1.0f, 1.0f, 1.0f, min(mCompleteTime/mCompleteAnimDelay, 1.0f)));
}

void PuzzlePictureMinigame::restart()
{
	mGameState = GS_WAIT_BEGINNING;
	mBeginTime = 0;

	//reset positions and tex coords
	vec2f cellSize = mScreenRect.getSize().scale(vec2f(1.0f/(float)mColums, 1.0f/(float)mRows));
	for (CellsArr::iterator it = mCells.begin(); it != mCells.end(); ++it)
	{
		Cell* animCell = (*it);

		animCell->mCurrentPos = animCell->mCorrectPos;
		vec2f texSize = animCell->mSprite->getTexture()->getSize();
		vec2f texPos = animCell->mCorrectPos.castTo<float>();

		animCell->mSprite->setTextureSrcRect(
			fRect( vec2f(texSize.x/(float)mColums*texPos.x,          texSize.y/(float)mRows*texPos.y), 
					vec2f(texSize.x/(float)mColums*(1.0f + texPos.x), texSize.y/(float)mRows*(1.0f + texPos.y)) ) );

		vec2f screenPos = mScreenRect.leftTop + cellSize.scale(animCell->mCurrentPos.castTo<float>());

		animCell->mSprite->setPosition(screenPos).setSize(cellSize);
	}
}

void PuzzlePictureMinigame::processPlayingClick( float x, float y )
{
	vec2f cursorPt(x, y);

	//getting cell under cursor
	Cell* cellUnderCursor = NULL;
	for (CellsArr::iterator it = mCells.begin(); it != mCells.end(); ++it)
	{
		if ((*it)->isIntersectPoint(cursorPt))
		{
			cellUnderCursor = *it;
			break;
		}
	}

	if (!cellUnderCursor) //if no exist cell under cursor
	{
		if (mSelectedCell) //clear selected
		{
			mSelectedCell->mSelected = false;
			mSelectedCell = NULL;
		}

		return;
	}

	if (mSelectedCell) //second cell - begin swapping
	{
		mSwappingCells.push_back(SwappingCellsPair(mSelectedCell, cellUnderCursor));
		cellUnderCursor->mUnderCursor = false;
		cellUnderCursor->mUnderCursorCoef = 0;
		mSelectedCell->mUnderCursor = true;
		mSelectedCell->mUnderCursorCoef = 1;
		mSelectedCell = NULL;
	}
	else //selecting cell
	{
		mSelectedCell = cellUnderCursor;
		mSelectedCell->mSelected = true;
	}
}

float PuzzlePictureMinigame::mCompleteAnimDelay = 1.0f;
float PuzzlePictureMinigame::mBeginDelay = 2.0f;
float PuzzlePictureMinigame::mMixingDelay = 2.0f;

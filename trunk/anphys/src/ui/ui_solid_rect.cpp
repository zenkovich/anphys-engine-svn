#include "ui_solid_rect.h"

#include "render/render_objects/2d/sprite.h"
#include "render/render.h"
#include "render/render_state/2d_render_state.h"

#include "ui_manager.h"

REGIST_TYPE(uiSolidRect)

uiSolidRect::uiSolidRect( uiWidgetsManager* widgetsManager, const std::string& id, const color4& bordersColor, 
                          const color4& inColor ):
	uiWidget(widgetsManager, id), mBordersColor(bordersColor), mInColor(inColor)
{
	mSolidSprite = new grSprite(widgetsManager->mRender);
	mSolidSprite->setColor(inColor);
}

uiSolidRect::~uiSolidRect()
{
	safe_release(mSolidSprite);
}

void uiSolidRect::derivedDraw()
{
	mSolidSprite->draw();

	gr2DRenderState* renderState = static_cast<gr2DRenderState*>(mWidgetsManager->mRender->mCurrentRenderState);
	
	renderState->pushLine(mGlobalPosition, mGlobalPosition + vec2(mResSize.x, 0), mResBordersColor);
	renderState->pushLine(mGlobalPosition + vec2(mResSize.x, 0), mGlobalPosition + vec2(mResSize.x, mResSize.y), mResBordersColor);
	renderState->pushLine(mGlobalPosition + vec2(mResSize.x, mResSize.y), mGlobalPosition + vec2(0, mResSize.y), mResBordersColor);
	renderState->pushLine(mGlobalPosition, mGlobalPosition + vec2(0, mResSize.y), mResBordersColor);

	renderState->renderLinesData();
}

void uiSolidRect::derivedUpdate( float dt )
{
	mSolidSprite->setPosition(mGlobalPosition);
	mSolidSprite->setSize(mResSize);
	
	mResInColor.a = (int)((float)mResInColor.a*mResTransparency);
	mResBordersColor.a = (int)((float)mResBordersColor.a*mResTransparency);

	mSolidSprite->setColor(mResInColor);
}

void uiSolidRect::setupInitialProperties()
{
	mResBordersColor = mBordersColor;
	mResInColor = mInColor;

	uiWidget::setupInitialProperties();
}

#include "ui_border.h"

#include "render\render_state\2d_render_state.h"
#include "render\render.h"
#include "ui_manager.h"
#include "ui_font.h"

REGIST_TYPE(uiBorder)

uiBorder::uiBorder( uiWidgetsManager* widgetsManager, const std::string& id, uiFont* captionFont,
                    const fRect& borderSizes /*= fRect(5, 5, 5, 5)*/, LineType linesType /*= LT_NO_LINE*/,
					const color4& linesColor /*= color4(150, 150, 150, 255)*/):
	uiWidget(widgetsManager, id), mBorderSizes(borderSizes), mLinesType(linesType), mCaption(captionFont),
	mLinesColor(linesColor)
{
	mCaption->setHorAlign(uiFont::AL_LEFT);
	mCaption->setVerAlign(uiFont::AL_MIDDLE);
}

uiBorder::~uiBorder()
{
	safe_release(mCaption);
}

void uiBorder::derivedUpdate( float dt )
{
	mCaption->setTextArea(fRect(mGlobalPosition.x + 5.0f, mGlobalPosition.y - mBorderSizes.leftTop.y,
		                        mGlobalPosition.x + mSize.x - 5.0f, mGlobalPosition.y));
}

void uiBorder::derivedDraw()
{
	mCaption->draw();

	if (mLinesType == LT_SINGLE || mLinesType == LT_DOUBLE)
	{
		fRect inBorderRect(vec2(mGlobalPosition + mBorderSizes.leftTop*0.5f), 
			               vec2(mGlobalPosition + mSize - mBorderSizes.rightDown*0.5f));

		gr2DRenderState* renderState = static_cast<gr2DRenderState*>(mWidgetsManager->mRender->mCurrentRenderState);

		bool solidUpLine = false;
		vec2 upLineLeftSide, upLineRightSide;
		if (mCaption->getWText(false).length() == 0)
		{
			solidUpLine = true;
		}
		else
		{
			upLineLeftSide = vec2(mCaption->mRealTextRect.leftTop.x - 2.0f, inBorderRect.leftTop.y);
			upLineRightSide = vec2(mCaption->mRealTextRect.rightDown.x + 2.0f, inBorderRect.leftTop.y);
		}

		if (mLinesType == LT_SINGLE)
		{
			renderState->pushLine(inBorderRect.getltCorner(), inBorderRect.getldCorner(), mLinesColor);
			renderState->pushLine(inBorderRect.getldCorner(), inBorderRect.getrdCorner(), mLinesColor);
			renderState->pushLine(inBorderRect.getrdCorner(), inBorderRect.getrtCorner(), mLinesColor);

			if (solidUpLine)
			{
				renderState->pushLine(inBorderRect.getltCorner(), inBorderRect.getrtCorner(), mLinesColor);
			}
			else
			{
				renderState->pushLine(inBorderRect.getltCorner(), upLineLeftSide, mLinesColor);
				renderState->pushLine(inBorderRect.getrtCorner(), upLineRightSide, mLinesColor);
			}
		}
		else
		{
			float linesDist = 1;
			
			renderState->pushLine(inBorderRect.getltCorner() + vec2(-linesDist, -linesDist), 
				                  inBorderRect.getldCorner() + vec2(-linesDist, linesDist), mLinesColor);
			renderState->pushLine(inBorderRect.getltCorner() + vec2(linesDist, linesDist), 
				                  inBorderRect.getldCorner() + vec2(linesDist, -linesDist), mLinesColor);
			
			renderState->pushLine(inBorderRect.getldCorner() + vec2(linesDist, -linesDist), 
				                  inBorderRect.getrdCorner() + vec2(-linesDist, -linesDist), mLinesColor);
			renderState->pushLine(inBorderRect.getldCorner() + vec2(-linesDist, linesDist), 
				                  inBorderRect.getrdCorner() + vec2(linesDist, linesDist), mLinesColor);
			
			renderState->pushLine(inBorderRect.getrtCorner() + vec2(-linesDist, linesDist), 
				                  inBorderRect.getrdCorner() + vec2(-linesDist, -linesDist), mLinesColor);
			renderState->pushLine(inBorderRect.getrtCorner() + vec2(linesDist, -linesDist), 
				                  inBorderRect.getrdCorner() + vec2(linesDist, linesDist), mLinesColor);

			if (solidUpLine)
			{	
				renderState->pushLine(inBorderRect.getltCorner() + vec2(-linesDist, -linesDist), 
									  inBorderRect.getrtCorner() + vec2(linesDist, -linesDist), mLinesColor);
				renderState->pushLine(inBorderRect.getltCorner() + vec2(linesDist, linesDist), 
									  inBorderRect.getrtCorner() + vec2(-linesDist, linesDist), mLinesColor);
			}
			else
			{				
				renderState->pushLine(inBorderRect.getltCorner() + vec2(-linesDist, -linesDist), 
									  upLineLeftSide + vec2(0, -linesDist), mLinesColor);
				renderState->pushLine(inBorderRect.getltCorner() + vec2(linesDist, linesDist), 
									  upLineLeftSide + vec2(0, linesDist), mLinesColor);

				renderState->pushLine(inBorderRect.getrtCorner() + vec2(linesDist, -linesDist), 
									  upLineRightSide + vec2(0, -linesDist), mLinesColor);
				renderState->pushLine(inBorderRect.getrtCorner() + vec2(-linesDist, linesDist), 
									  upLineRightSide + vec2(0, linesDist), mLinesColor);
			}
		}

		renderState->renderLinesData();
	}
}

void uiBorder::addChild( uiWidget* widget )
{
	widget->setPosition(widget->getPosition() + mBorderSizes.leftTop);
	uiWidget::addChild(widget);

	adjustSizeByChilds();

	mSize += mBorderSizes.rightDown;
}

void uiBorder::adjustSizeByChilds()
{
	uiWidget::adjustSizeByChilds();
}

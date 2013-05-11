#include "ui_lines_geometry.h"

#include "render\render_state\2d_render_state.h"
#include "render\render.h"
#include "ui_manager.h"

uiLinesGeometry::uiLinesGeometry( uiWidgetsManager* widgetsManager, const std::string& id ):
	uiWidget(widgetsManager, id)
{
}

uiLinesGeometry::~uiLinesGeometry()
{
}

void uiLinesGeometry::derivedDraw()
{
	gr2DRenderState* renderState = static_cast<gr2DRenderState*>(mWidgetsManager->mRender->mCurrentRenderState);

	for (LineList::iterator it = mLines.begin(); it != mLines.end(); ++it)
	{
		renderState->pushLine(it->a + mGlobalPosition, it->b + mGlobalPosition, it->color);
	}

	renderState->renderLinesData();
}

void uiLinesGeometry::addLine( const vec2& a, const vec2& b, const color4& color /*= color(0, 0, 0, 255)*/ )
{
	mLines.push_back(Line(a, b, color));
}

void uiLinesGeometry::clear()
{
	mLines.clear();
}

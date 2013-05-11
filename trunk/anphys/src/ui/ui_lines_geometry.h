#ifndef UI_LINES_GEOMETRY_H
#define UI_LINES_GEOMETRY_H

#include "ui_widget.h"

#include <vector>
#include "util/math/mmath.h"

struct uiLinesGeometry:public uiWidget
{
	struct Line
	{
		vec2 a, b;
		color4 color;

		Line() {}
		Line(const vec2& va, const vec2& vb, const color4& vcolr):
			a(va), b(vb), color(vcolr) {}
	};
	typedef std::vector<Line> LineList;

	LineList mLines;

//functions
	uiLinesGeometry(uiWidgetsManager* widgetsManager, const std::string& id);
	~uiLinesGeometry();

	void derivedDraw();

	void addLine(const vec2& a, const vec2& b, const color4& color = color4(0, 0, 0, 255));
	void clear();

	bool isAdjustingSizeByChilds() { return false; }
};

#endif //UI_LINES_GEOMETRY_H
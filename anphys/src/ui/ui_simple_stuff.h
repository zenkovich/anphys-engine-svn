#ifndef UI_SIMPLE_STUFF_H
#define UI_SIMPLE_STUFF_H

#include "util/math/mmath.h"

#include "ui_state.h"
#include "ui_property.h"

struct uiSpriteWidget;
struct uiWidgetsManager;
struct uiLabel;
struct uiButton;
struct cCallbackInterface;
struct uiWidget;
struct uiScrollbar;
struct uiScrollArea;
struct uiTextEdit;

struct uiSimpleStuff
{
	static color4 mColor1, mColor2, mColor3, mColor4;

	static uiSpriteWidget* createSpriteWidget(uiWidgetsManager* widgetsManager, const color4& color, 
		                                      const vec2& pos, const vec2& size, const std::string& id);

	static uiLabel* createLabel(uiWidgetsManager* widgetsManager, const vec2& pos, const vec2& size,
		                        const std::string& id, const std::string& text);

	static uiButton* createButton(uiWidgetsManager* widgetManager, const vec2& pos, const vec2& size,
		                          const std::string& id, const std::string& caption, 
								  cCallbackInterface* callback);

	static void createSizeEffect(uiWidget* widget, float duration = 0.5f);

	static uiScrollbar* createScrollbar(uiWidgetsManager* widgetManager, const vec2& pos, const vec2& size,
		                                const std::string& id, int type, float minv = 0.0f, float maxv = 1.0f);

	static uiScrollArea* createScrollarea(uiWidgetsManager* widgetsManager, const vec2& pos, const vec2& size,
		                                  const std::string& id);

	static uiTextEdit* createTextEdit(uiWidgetsManager* widgetsManager, const vec2& pos, const vec2& size,
		                              const std::string& id);
};

#endif //UI_SIMPLE_STUFF_H
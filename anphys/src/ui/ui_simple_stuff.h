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

	static void addWidgetState(uiWidget* widget, const std::string& id, float transparency, const vec2& offs,
		                       float duration = 0.15f);

	static uiState* addWidgetState(uiWidget* widget, const std::string& id);

	template<typename T>
	static uiState* addStateProperty(uiState* state, T* valuePtr, T targetValue, float duration = 0.15f)
	{
		state->addProperty(new uiParameterProperty<T>(valuePtr, targetValue, uiProperty::IT_LINEAR, duration));
		return state;
	}
};

#endif //UI_SIMPLE_STUFF_H
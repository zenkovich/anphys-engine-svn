#include "game_over_game_state.h"

#include "render_system/text.h"
#include "render_system/render_system.h"
#include "render_system/font.h"
#include "bubble_app.h"
#include "render_system/sprite.h"
#include "render_system/texture.h"
#include "util/animation/animation.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(GameOverGameState);

GameOverGameState::GameOverGameState()
{
	grFont* font = fontsManager()->loadBMFont("bubble_game/font/bubble_font");
	mGameText = mnew grText(font);
	mOverText = mnew grText(font);
	mScoreText = mnew grText(font);
	mPressText = mnew grText(font);

	mGameText->ctext = "game";
	mOverText->ctext = "over";
	mScoreText->ctext = "Score:33";
	mPressText->ctext = "Press to restart";

	mBack = mnew grSprite(grTexture::createFromFile("bubble_game/back"));
	
	mGameTextAnimation = mnew cAnimation<vec2f>();
	mGameTextAnimation->addFrame(vec2f(-300.0f, 100.0f), -1, 0.0f, false, IT_EASY_OUT);
	mGameTextAnimation->addFrame(vec2f(-300.0f, 100.0f), -1, 0.5f, false, IT_EASY_OUT);
	mGameTextAnimation->addFrame(vec2f(250.0f, 100.0f), -1, 0.5f, false, IT_EASY_OUT);
	mAllAnimation.addAnimation(mGameTextAnimation);	

	mOverTextAnimation = mnew cAnimation<vec2f>();
	mOverTextAnimation->addFrame(vec2f(1000.0f, 150.0f), -1, 0.0f, false, IT_EASY_OUT);
	mOverTextAnimation->addFrame(vec2f(1000.0f, 150.0f), -1, 1.2f, false, IT_EASY_OUT);
	mOverTextAnimation->addFrame(vec2f(350.0f, 150.0f), -1, 0.5f, false, IT_EASY_OUT);
	mAllAnimation.addAnimation(mOverTextAnimation);

	mScoreTextAnimation = mnew cAnimation<vec2f>();
	mScoreTextAnimation->addFrame(vec2f(300.0f, 1000.0f), -1, 0.0f, false, IT_EASY_OUT);
	mScoreTextAnimation->addFrame(vec2f(300.0f, 1000.0f), -1, 1.8f, false, IT_EASY_OUT);
	mScoreTextAnimation->addFrame(vec2f(300.0f, 220.0f), -1, 0.5f, false, IT_EASY_OUT);
	mAllAnimation.addAnimation(mScoreTextAnimation);

	mPressTextAnimation = mnew cAnimation<vec2f>();
	mPressTextAnimation->addFrame(vec2f(150.0f, 1000.0f), -1, 0.0f, false, IT_EASY_OUT);
	mPressTextAnimation->addFrame(vec2f(150.0f, 1000.0f), -1, 2.6f, false, IT_EASY_OUT);
	mPressTextAnimation->addFrame(vec2f(150.0f, 370.0f), -1, 0.5f, false, IT_EASY_OUT);
	mAllAnimation.addAnimation(mPressTextAnimation);
}

GameOverGameState::~GameOverGameState()
{
	safe_release(mBack);
	safe_release(mGameText);
	safe_release(mOverText);
	safe_release(mScoreText);
	safe_release(mPressText);
}

void GameOverGameState::onActivate()
{
	mAllAnimation.setTime(0);
	mAllAnimation.play();
	update(0);
}

void GameOverGameState::onDeactivate()
{
}


void GameOverGameState::update( float dt )
{
	mAllAnimation.update(dt);
	
	mGameText->position = **mGameTextAnimation;
	mOverText->position = **mOverTextAnimation;
	mScoreText->position = **mScoreTextAnimation;
	mPressText->position = **mPressTextAnimation;
}

void GameOverGameState::draw()
{
	mBack->draw();
	mGameText->draw();
	mOverText->draw();
	mScoreText->draw();
	mPressText->draw();

	if (appInput()->isCursorPressed())
		mBubbeGameApp->goGamePlayState();
}

void GameOverGameState::setScore( int score )
{
	mScoreText->ctext = format("Score: %i", score);
}

CLOSE_O2_NAMESPACE

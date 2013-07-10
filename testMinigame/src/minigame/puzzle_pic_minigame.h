#ifndef PUZZLE_PIC_MINIGAME_H
#define PUZZLE_PIC_MINIGAME_H

#include <vector>

#include "minigame_interface.h"

#include "math/vector2.h"
#include "math/rect.h"
#include "math/color.h"

class Sprite;
class Texture;
class RenderSystem;

/** Puzzle picture minigame. Cycled. */
class PuzzlePictureMinigame:public MiniGame
{
	/** Cell of image part. cantaining current, correct positions, sprite, some other parametres. */
	struct Cell
	{
		PuzzlePictureMinigame* mPuzzleMinigame; /**< Pointer to owner minigame. */

		Sprite* mSprite;              /**< Sprite with part of picture. */
								      
		vec2i   mCurrentPos;          /**< Correct position. */
		vec2i   mCorrectPos;          /**< Current position. */
								      
		vec2f   mCurrentScreenPos;    /** Currect screen position of sprite. */
		fRect   mCurrentScreenRect;   /**< Rect of currect screen position of cell. */
		float   mPerspectiveOffset;   /**< Perspective offset, for perspective effect. */
		static float mMaxPerspOffset; /**< max perspective offset. */

		bool    mUnderCursor;         /**< true, if cell under cursor. */
		float   mUnderCursorCoef;     /**< Perspective effect coeficient. value in 0...1 */
									  
		bool    mSelected;            /**< Is cell selected. */
		float   mSelectedCoef;        /**< Selected effect coeficient. value in 0...1 */
		color4  mSelectionColor;      /**< Color of selector sprite. */
									  
		bool    mClickable;           /**< True if could be selected. */
	
		/** ctor.
		 *  @param puzzleMinigame pointer to owner puzzle minigame.
		 *  @param texture        target texture.
		 *  @param srcrect        texture source rect.
		 *  @param spritePos      initial sprite position.
		 *  @param correctPos     correct position of cell. */
		Cell(PuzzlePictureMinigame* puzzleMinigame, Texture* texture, const fRect& srcRect, const fRect& spritePos, 
			 const vec2i& correctPos);

		/** dtor. */
		~Cell();

		/** Drawing sprite? selection and shadow. */
		void draw();

		/** Updating effects and sprite pos. */
		void update(float dt);

		/** Returns true, if poin inside cell rect. */
		bool isIntersectPoint(const vec2f& point);

		/** Sort pred. */
		static bool compareByOffset(Cell* a, Cell* b) { return a->mPerspectiveOffset < b->mPerspectiveOffset; }
	};
	typedef std::vector<Cell*> CellsArr;

	/** Pair of swapping cells. */
	struct SwappingCellsPair
	{
		Cell*        mSwappingCells[2];   /**< Poinetrs to swapping cells. */
		float        mAnimatingTime;      /**< Elapsed time from beginning anim. */
		static float mAnimationDelay;     /**< Delay of anim. */

		vec2f        mCellsInitialPos[2]; /**< Initial positions of cell sprites. */

		bool         mFinished;           /**< true, if animation finished. */
	
		/** ctor. */
		SwappingCellsPair(Cell* a, Cell* b);

		/** Updating cell sprites positions. */
		void update(float dt);

		/** Return true, when anim completed. */
		bool isFinished();
	};
	typedef std::vector<SwappingCellsPair> SwapCellsArr;

	enum GameState { GS_WAIT_BEGINNING = 0, GS_MIXING, GS_PLAYING, GS_COMPLETE_ANIM, GS_FULLY_COMPLETE };

	RenderSystem* mRenderSystem;  

	int           mColums;          /**< Colums count. */
	int           mRows;            /**< Rows count. */
	fRect         mScreenRect;      /**< Target screen rect, where the picture is located. */
			      
	CellsArr      mCells;           /**< Vector of cells. */

	Sprite*       mSelectionSprite; /**< Cell selection solid sprite. */
	color4        mSelectionColor;  /**< Cell selection sprite color. */

	Sprite*       mShadowSprite;    /**< Sprite using for shadow rendering. */
	Sprite*       mBkSprite;        /**< Back fullscreen sprite. */
	Sprite*       mPicPadSprite;    /**< Pad under the picture. */
	Sprite*       mHintSprite;      /**< Small hint picture. */
	Sprite*       mCompleteSprite;  /**< Complete caption sprite. */
				  
	Cell*         mSelectedCell;    /**< Selected cell ptr, NULL if no selected cell. */
	SwapCellsArr  mSwappingCells;   /**< vector of swapping cells pairs. */
				  
	GameState     mGameState;       /**< Current game state. */

	float         mBeginTime;       /**< Elapsed time from beginning. */
	static float  mBeginDelay;      /**< Delay begore beginning. */

	float         mMixingTime;      /**< Elapsed time of mixing animation. */
	static float  mMixingDelay;     /**< Mixing animation delay. */
	
	float         mCompleteTime;      /**< Elapsed time from complete game. */
	static float  mCompleteAnimDelay; /**< Delay of complete animation. */

public:
	/** ctor. */
	PuzzlePictureMinigame(RenderSystem* renderSystem);

	/** dtor. */
	~PuzzlePictureMinigame();

	/** called before start. */
	void initialize();

	/** called on mouse click ((x, y) - screen coordinates). */
	void click(float x, float y);

	/** called when cursor moved ((x, y) - screen coordinates). */
	void cursorMove(float x, float y);

	/** returns true then minigame successfully completed. */
	bool isComplete();

	/** called on rendering. */
	void render();

	/** called on updating. */
	void update(float dt);

private:

	/** Loading config from xml file. */
	void loadConfig(const char* configFile);

	/** Beginning mixing anim. */
	void beginMixing();

	/** Updating mixing anim. */
	void updateMixingAnim(float dt);

	/** Calling on mixing anim complete. */
	void completeMixingAnim();

	/** Updating paying state. Updates cells and swapping pairs. */
	void updatePlayingState( float dt );

	/** processing player clicking in playing state. */
	void processPlayingClick( float x, float y );

	/** Start complete anim. */
	void beginCompleteAnim();

	/** Update complete anim. */
	void updateCompleteAnim(float dt);

	/** Checking cells position for correctness. */
	void checkCompletion();

	/** restarting game. */
	void restart();

	/** Drawing part of shadow. */
	void drawShadowPart(const vec2f& leftTop, const vec2f& rightDown, float c0, float c1, float c2, float c3, float alpha);

	/** Drawing shadow of rect. */
	void drawShadow(const fRect& rect, float alpha);
};

#endif //PUZZLE_PIC_MINIGAME_H
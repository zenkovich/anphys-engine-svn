#ifndef LANDSCAPE_CREATOR_WND_H
#define LANDSCAPE_CREATOR_WND_H

struct uiWindow;
struct uiWidgetsManager;

struct LandscapeCreatorWnd
{
	uiWindow*         mWindow;
	uiWidgetsManager* mWidgetsManager;

	float             mMinRangeX;
	float             mMinRangeY;
	float             mMinRangeZ;

	float             mMaxRangeX;
	float             mMaxRangeY;
	float             mMaxRangeZ;
	
	int               mSegmentsXCount;
	int               mSegmentsZCount;

//functions
	LandscapeCreatorWnd(uiWidgetsManager* widgetsManager);
	~LandscapeCreatorWnd();

	void show();

protected:
	void onRecreateLandcapeBtnPressed();
	void resetParametres();
};

#endif //LANDSCAPE_CREATOR_WND_H
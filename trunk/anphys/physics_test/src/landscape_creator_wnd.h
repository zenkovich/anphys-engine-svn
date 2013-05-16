#ifndef LANDSCAPE_CREATOR_WND_H
#define LANDSCAPE_CREATOR_WND_H

struct uiWindow;
struct uiWidgetsManager;
struct cObject;
struct grRenderSceneBaseInterface;

struct LandscapeCreatorWnd
{
	uiWindow*                   mWindow;
	uiWidgetsManager*           mWidgetsManager;
					            
	float                       mMinRangeX;
	float                       mMinRangeY;
	float                       mMinRangeZ;
					            
	float                       mMaxRangeX;
	float                       mMaxRangeY;
	float                       mMaxRangeZ;
					            
	int                         mSegmentsXCount;
	int                         mSegmentsZCount;
					            
	cObject*                    mLandscapeObject;
	grRenderSceneBaseInterface* mRenderScene;

//functions
	LandscapeCreatorWnd(uiWidgetsManager* widgetsManager);
	~LandscapeCreatorWnd();

	void show();

	void recreateLandscape();

protected:
	void onRecreateLandcapeBtnPressed();
	void resetParametres();
};

#endif //LANDSCAPE_CREATOR_WND_H
#include "landscape_creator_wnd.h"

#include "engine/engine_incl.h"


LandscapeCreatorWnd::LandscapeCreatorWnd( uiWidgetsManager* widgetsManager ):mWidgetsManager(widgetsManager)
{
	mLandscapeObject = NULL;

	resetParametres();

	mWindow = uiSimpleStuff::createWindow(mWidgetsManager, "LanscapeCreatorWnd", vec2(200, 0), vec2(300, 300), 
		"Landscape creator");

	uiSimpleStuff::createSizeEffect(mWindow);

	uiVertLayoutWidget* verLayoutWidget = new uiVertLayoutWidget(mWidgetsManager, "verLayout");
	verLayoutWidget->setPosition(vec2(5, 5));
	verLayoutWidget->mWidgetsDistance = 5.0f;

	uiButton* recreateBtn = uiSimpleStuff::createButton(mWidgetsManager, vec2(0, 0), vec2(100, 25), "landscapeBtn",
		"Recreate", new cCallback<LandscapeCreatorWnd>(this, &LandscapeCreatorWnd::onRecreateLandcapeBtnPressed));
		
	uiSimpleStuff::addProperty(verLayoutWidget, "Min X Range", &mMinRangeX);
	uiSimpleStuff::addProperty(verLayoutWidget, "Min Y Range", &mMinRangeY);
	uiSimpleStuff::addProperty(verLayoutWidget, "Min Z Range", &mMinRangeZ);

	uiSimpleStuff::addProperty(verLayoutWidget, "Max X Range", &mMaxRangeX);
	uiSimpleStuff::addProperty(verLayoutWidget, "Max Y Range", &mMaxRangeY);
	uiSimpleStuff::addProperty(verLayoutWidget, "Max Z Range", &mMaxRangeZ);
	
	uiBindingValues::BindValue<int>* xSegmentsBindValue =
		uiSimpleStuff::addProperty(verLayoutWidget, "Segments X", &mSegmentsXCount);
	xSegmentsBindValue->setLimitations(1, 100);

	uiBindingValues::BindValue<int>* zSegmentsBindValue = 
		uiSimpleStuff::addProperty(verLayoutWidget, "Segments Z", &mSegmentsZCount);
	zSegmentsBindValue->setLimitations(1, 100);


	
	verLayoutWidget->addChild((uiWidget*)recreateBtn);

	mWindow->addChild(verLayoutWidget);

	mWidgetsManager->addWidget(mWindow);
}

LandscapeCreatorWnd::~LandscapeCreatorWnd()
{
	mWidgetsManager->removeWidget(mWindow);
}

void LandscapeCreatorWnd::show()
{
	mWindow->show();
}

void LandscapeCreatorWnd::onRecreateLandcapeBtnPressed()
{
	recreateLandscape();
}

void LandscapeCreatorWnd::resetParametres()
{
	mMinRangeX = -100;
	mMinRangeY = 0;
	mMinRangeZ = -100;

	mMaxRangeX = 100;
	mMaxRangeY = 0;
	mMaxRangeZ = 100;

	mSegmentsXCount = 100;
	mSegmentsZCount = 100;
}

void LandscapeCreatorWnd::recreateLandscape()
{
	phLandscapeCollisionGeometry* collisionGeometry = static_cast<phLandscapeCollisionGeometry*>(
		mLandscapeObject->getComponent<cPhysicsStaticBodyObjectComponent>()->mStaticPhysicsBody
		->getCollisionGeometry()->mParts[0]);	

	int vertexCount = (mSegmentsXCount + 1)*(mSegmentsZCount + 1);
	int polyCount = mSegmentsXCount*mSegmentsZCount*2;

	/*grRender3DObjectMesh* renderMesh = static_cast<grRender3DObjectMesh*>(
		mLandscapeObject->getComponent<cRender3DObjectComponent>()->mRender3DObject);*/
	mLandscapeObject->removeComponent(mLandscapeObject->getComponent<cRender3DObjectComponent>());

	grRender3DObjectMesh* renderMesh = new grRender3DObjectMesh(NULL, vertexCount, polyCount);
	mRenderScene->mObjects->createObject(renderMesh);

	cRender3DObjectComponent* renderObjectComponent = new cRender3DObjectComponent(mLandscapeObject, renderMesh);
	mLandscapeObject->addComponent(renderObjectComponent);

//create physics mesh
	safe_release_arr(collisionGeometry->mVerticies);
	safe_release_arr(collisionGeometry->mPolygons);

	collisionGeometry->mVerticies = new phLandscapeCollisionGeometry::lVertex[vertexCount];
	collisionGeometry->mVerticiesCount = vertexCount;

	collisionGeometry->mPolygons = new phLandscapeCollisionGeometry::lPolygon[polyCount];
	collisionGeometry->mPolygonsCount = polyCount;
	
	vertexTexNorm* meshVerticies = new vertexTexNorm[vertexCount];
	int*           meshIndexes = new int[polyCount*3];

	int polygonIdx = 0;
	float invXSegmentsCount = 1.0f/(float)mSegmentsXCount;
	float invZSegmentsCount = 1.0f/(float)mSegmentsZCount;
	for (int i = 0; i < mSegmentsXCount + 1; i++)
	{
		float xCoef = (float)i*invXSegmentsCount;
		float xCoord = mMinRangeX + (mMaxRangeX - mMinRangeX)*xCoef;

		for (int j = 0; j < mSegmentsZCount + 1; j++)
		{
			float zCoef = (float)j*invZSegmentsCount;
			float zCoord = mMinRangeZ + (mMaxRangeZ - mMinRangeZ)*zCoef;
			float yCoord = random(mMinRangeY, mMaxRangeY);

			int vertexId = i*(mSegmentsZCount + 1) + j;

			collisionGeometry->mVerticies[vertexId].mPosition = vec3(xCoord, yCoord, zCoord);
			collisionGeometry->mVerticies[vertexId].mFrictionCoef = 1.0f;
			collisionGeometry->mVerticies[vertexId].mBounceCoef = 0.1f;

			meshVerticies[vertexId] = vertexTexNorm(vec3(xCoord, yCoord, zCoord), vec3(0, 1, 0), 0, 0);

			if (i > 0 && j > 0)
			{
				int a = vertexId - 1 - (mSegmentsZCount + 1);
				int b = vertexId - (mSegmentsZCount + 1);
				int c = vertexId;
				int d = vertexId - 1;
				
				meshVerticies[a].tu = xCoef; meshVerticies[a].tv = zCoef;
				meshVerticies[b].tu = xCoef + invXSegmentsCount; meshVerticies[b].tv = zCoef;
				meshVerticies[c].tu = xCoef + invXSegmentsCount; meshVerticies[c].tv = zCoef + invZSegmentsCount;
				meshVerticies[d].tu = xCoef; meshVerticies[d].tv = zCoef + invZSegmentsCount;
				
				collisionGeometry->mPolygons[polygonIdx] = 
					phLandscapeCollisionGeometry::lPolygon(a, b, c, collisionGeometry->mVerticies);
				
				meshIndexes[polygonIdx*3] = c;
				meshIndexes[polygonIdx*3 + 1] = b;
				meshIndexes[polygonIdx*3 + 2] = a;

				polygonIdx++;

				collisionGeometry->mPolygons[polygonIdx] = 
					phLandscapeCollisionGeometry::lPolygon(a, c, d, collisionGeometry->mVerticies);				
				
				meshIndexes[polygonIdx*3] = d;
				meshIndexes[polygonIdx*3 + 1] = c;
				meshIndexes[polygonIdx*3 + 2] = a;

				polygonIdx++;
			}
		}
	}

//graphics
	/*for (int i = 0; i < vertexCount; i++)
	{
		gLog->fout(1, "v[%i] (%.2f %.2f %.2f) (%.2f %.2f %.2f) (%.1f %.1f)\n", i,
			meshVerticies[i].x, meshVerticies[i].y, meshVerticies[i].z,
			meshVerticies[i].nx, meshVerticies[i].ny, meshVerticies[i].nz,
			meshVerticies[i].tu, meshVerticies[i].tv);
	}

	for (int i = 0; i < polyCount; i++)
	{
		gLog->fout(1, "idx[%i] = %i\n", i, meshIndexes[i]);
	}*/
	
	renderMesh->addPart(meshVerticies, vertexCount, meshIndexes, polyCount, 
		renderMesh->mRenderObjectsManager->mRender->mSurfaceMaterials->getSurfaceMaterial("grass"));
	
	safe_release_arr(meshVerticies);
	safe_release_arr(meshIndexes);
}

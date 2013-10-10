#ifndef MAN_FIELD_H
#define MAN_FIELD_H

#include <vector>

#include "util/math/vector2.h"
#include "render_system/camera.h"

OPEN_O2_NAMESPACE

class grSprite;
class cInputMessage;

class cMan;

struct cFieldCluster
{
	vec2i     mPosition;
	grSprite* mSprite;
	vec2f     mSpriteOffset;

	cFieldCluster();
	cFieldCluster(const cFieldCluster& cluster);
	~cFieldCluster();
};

class cManField
{
	typedef std::vector<cFieldCluster> FieldClustersVec;

	vec2i            mFieldSize;
	vec2f            mClusterSize;
	FieldClustersVec mFieldClusters;
	FieldClustersVec mFieldClusterPrototypes;

	grCamera         mCamera;

	cInputMessage*   mInputMessage;
	cMan*            mMan;

public:
	cManField(cInputMessage* inputMsg);
	~cManField();

	void draw();
	void update(float dt);

protected:
	void loadConfig();
};

CLOSE_O2_NAMESPACE

#endif //MAN_FIELD_H
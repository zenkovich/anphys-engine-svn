#ifndef MAN_FIELD_H
#define MAN_FIELD_H

#include <vector>

#include "util/math/vector2.h"
#include "render_system/camera.h"

OPEN_O2_NAMESPACE

class grMesh;
class cInputMessage;
class cApplication;
class grTexture;

class cMan;
struct cWaypointWeb;

class cManField
{
	friend class cMan;

	struct cFieldCluster
	{
		vec2i     mPosition;
		float     mWayTime;
	};
	typedef std::vector<cFieldCluster> FieldClustersVec;
	typedef std::vector<grMesh*> MeshVec;

	cApplication*    mApplication;
	cInputMessage*   mInputMessage;

	vec2i            mFieldSize;
	vec2f            mClusterSize;
	FieldClustersVec mFieldClusters;

	grCamera         mCamera;
	MeshVec          mFieldMeshes;

	cWaypointWeb*    mWaypointWeb;
	cMan*            mMan;

public:
	cManField(cApplication* app);
	~cManField();

	void draw();
	void update(float dt);

protected:
	void loadConfig();
	void initializeWaypointWeb();

	grMesh* createMesh(int clustersCount, grTexture* texture);
};

CLOSE_O2_NAMESPACE

#endif //MAN_FIELD_H
#include "man_field.h"

//o2
#include "pugixml/pugixml.hpp"
#include "render_system/sprite.h"
#include "util/log.h"
#include "util/xml_tools.h"

OPEN_O2_NAMESPACE

//cFieldCluster class

cFieldCluster::cFieldCluster():mSprite(NULL)
{
}

cFieldCluster::cFieldCluster( const cFieldCluster& cluster )
{
	mPosition = cluster.mPosition;
	mSprite = mnew grSprite(*cluster.mSprite);
	mSpriteOffset = cluster.mSpriteOffset;
}

cFieldCluster::~cFieldCluster()
{
	safe_release(mSprite);
}

//cManField class

cManField::cManField(cInputMessage* inputMsg):mInputMessage(inputMsg), mMan(NULL)
{
	loadConfig();
}

cManField::~cManField()
{
//	safe_release(mMan);
}

void cManField::loadConfig()
{
	const std::string configFileName = "data/config";

	pugi::xml_document xmlDoc;
	if (!cXmlTools::loadFromFile(configFileName, xmlDoc))
	{
		llog("ERROR: Failed to load man field config file: %s", configFileName.c_str());
		return;
	}

	//mFieldSize
}

void cManField::draw()
{

}

void cManField::update( float dt )
{

}

CLOSE_O2_NAMESPACE

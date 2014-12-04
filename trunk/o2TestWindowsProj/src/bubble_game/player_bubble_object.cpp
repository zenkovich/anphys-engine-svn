#include "player_bubble_object.h"

#include "app/application.h"

OPEN_O2_NAMESPACE

PlayerBubble::PlayerBubble(const vec2f& position /*= vec2f()*/):
	IGameObject(position), mRadius(1.0f), mInputSense(0.2f, 0.6f), mSegmentsCount(30), mMesh(NULL), mRootHardness(0.01f),
	mPressureCoef(0.3f), mShellHardness(0.8f)
{
	initPhysicsModel();
	initGraphics();
}

PlayerBubble::~PlayerBubble()
{
	safe_release(mMesh);

	verletPhysics()->removeParticle(mPhysicsRootParticle);

	foreach(VeretPhysics::ParticlesArr, mPhysicsShellParticles, particleIt)
		verletPhysics()->removeParticle(*particleIt);
	
	foreach(VeretPhysics::LinksArr, mPhysicsLinks, linkIt)
		verletPhysics()->removeLink(*linkIt);
}

void PlayerBubble::initPhysicsModel()
{
	const float segRadiusDecrCoef = 0.6f;

	float segRadius = 2.0f*PI*mRadius/(float)mSegmentsCount/2.0f*segRadiusDecrCoef;

	mPhysicsRootParticle = verletPhysics()->createParticle(mPosition);

	for (int i = 0; i < mSegmentsCount; i++)
	{
		float cf = (float)i/(float)mSegmentsCount;
		float angle = 2.0f*PI*cf;
		vec2f pos = mPosition + vec2f::rotated(angle)*(mRadius - segRadius);
		
		VeretPhysics::Particle* newParticle = verletPhysics()->createParticle(pos, segRadius);
		mPhysicsShellParticles.add(newParticle);

		mPhysicsLinks.add(verletPhysics()->createLink(mPhysicsRootParticle, newParticle, mRadius, mRootHardness));

		if (i > 0)
			mPhysicsLinks.add(verletPhysics()->createLink(mPhysicsShellParticles[i - 1], newParticle, -1, mShellHardness));
	}
	
	mPhysicsLinks.add(verletPhysics()->createLink(mPhysicsShellParticles.last(), mPhysicsShellParticles[0], -1, mShellHardness));
	
	mInitialSquare = getSquare();
}

void PlayerBubble::draw()
{
	updateMesh();
	mMesh->draw();
}

void PlayerBubble::update(float dt)
{
	updateControl(dt);
	updateVolumePressure(dt);
	updateStabilityEuristic(dt);
	mPosition = mPhysicsRootParticle->mPosition;
}

void PlayerBubble::updateControl(float dt)
{
	vec2f moveVec;
	if (appInput()->isKeyDown(VK_UP))
		moveVec += vec2f::up();	
	if (appInput()->isKeyDown(VK_DOWN))
		moveVec += vec2f::down();	
	if (appInput()->isKeyDown(VK_RIGHT))
		moveVec += vec2f::right();	
	if (appInput()->isKeyDown(VK_LEFT))
		moveVec += vec2f::left();

	moveVec = moveVec.scale(mInputSense)*dt;

	foreach(VeretPhysics::ParticlesArr, mPhysicsShellParticles, particleIt)
		(*particleIt)->mPosition += moveVec;
}

void PlayerBubble::updateVolumePressure(float dt)
{
	//get scaling pressure vector
	float curSquare = getSquare();
	float targetSquare = mInitialSquare;
	float scaling = (1.0f - targetSquare/curSquare)*mPressureCoef + 1.0f;
	vec2f scaleVec = vec2f::one()/scaling;
	
	//scale particles shell center
	vec2f particlesCenter;
	foreach(VeretPhysics::ParticlesArr, mPhysicsShellParticles, particleIt)
		particlesCenter += (*particleIt)->mPosition;

	particlesCenter /= (float)mPhysicsShellParticles.count();
	
	foreach(VeretPhysics::ParticlesArr, mPhysicsShellParticles, particleIt)
		(*particleIt)->mPosition = (*particleIt)->mPosition.scaleOrigin(scaleVec, particlesCenter);

	//move root particle to shell center
	mPhysicsRootParticle->mPosition = interpolate(mPhysicsRootParticle->mPosition, particlesCenter, 0.3f);
}

void PlayerBubble::updateStabilityEuristic(float dt)
{	
	//just keep root particle inside shell
	for (int i = 0; i < mPhysicsShellParticles.count() - 1; i++)
	{
		vec2f a = mPhysicsShellParticles[i]->mPosition;
		vec2f b = mPhysicsShellParticles[i + 1]->mPosition;
		vec2f dir = b - a;
		float dirLength = dir.length();
		vec2f normalizedDirLength = dir/dirLength;
		vec2f nDir = vec2f(-normalizedDirLength.y, normalizedDirLength.x);
		vec2f rootPos = mPhysicsRootParticle->mPosition;

		float projx = (rootPos - a)*normalizedDirLength;
		if (projx < 0.0f || projx > dirLength)
			continue;

		float proj = (rootPos - a)*nDir;
		if (proj < 0)
			mPhysicsRootParticle->mPosition -= nDir*proj;
	}
}

float PlayerBubble::getSquare()
{
	float res = 0.0f;
	int n = mPhysicsShellParticles.count();
	for (int i = 0; i < n; i++)
	{
		if (i == 0)
		{
			res += mPhysicsShellParticles[i]->mPosition.x*(mPhysicsShellParticles[n - 1]->mPosition.y - 
				                                           mPhysicsShellParticles[i + 1]->mPosition.y);
		}
		else if (i == n - 1)
		{
			res += mPhysicsShellParticles[i]->mPosition.x*(mPhysicsShellParticles[i - 1]->mPosition.y - 
				                                           mPhysicsShellParticles[0]->mPosition.y);
		}
		else 
		{
			res += mPhysicsShellParticles[i]->mPosition.x*(mPhysicsShellParticles[i - 1]->mPosition.y - 
				                                           mPhysicsShellParticles[i + 1]->mPosition.y);
		}
	}

	res = abs(res*0.5f);

	return res;
}

void PlayerBubble::initGraphics()
{
	const string texturePath = "bubble_game/bubble";
	grTexture texture = grTexture::createFromFile(texturePath);

	mMesh = mnew grMesh(texture, mPhysicsShellParticles.count()*3, mPhysicsShellParticles.count());
	mMesh->mVertexCount = mPhysicsShellParticles.count()*3;
	mMesh->mPolyCount = mPhysicsShellParticles.count();

	for (int i = 0; i < mMesh->mVertexCount; i++)
	{
		mMesh->mVerticies[i].tu = 0.5f;
		mMesh->mVerticies[i].tv = 0.5f;		
		mMesh->mVerticies[i].color = color4::white().dword();
		mMesh->mVerticies[i].z = 1.0f;
	}
}

void PlayerBubble::updateMesh()
{
	vec2f ctr = mPhysicsRootParticle->mPosition;
	int n = mPhysicsShellParticles.count();
	for (int i = 0; i < n; i++)
	{
		vec2f p1 = mPhysicsShellParticles[i]->mPosition;
		vec2f p2 = mPhysicsShellParticles[(i + 1)%n]->mPosition;

		mMesh->mVerticies[i*3]     = ctr;
		mMesh->mVerticies[i*3 + 1] = p1;
		mMesh->mVerticies[i*3 + 2] = p2;
		
		mMesh->mIndexes[i*3]     = i*3;
		mMesh->mIndexes[i*3 + 1] = i*3 + 1;
		mMesh->mIndexes[i*3 + 2] = i*3 + 2;

		vec2f n1 = (p1 - ctr).normalize();
		vec2f tex1 = (vec2f::one() + n1)*0.5f;

		vec2f n2 = (p2 - ctr).normalize();
		vec2f tex2 = (vec2f::one() + n2)*0.5f;

		mMesh->mVerticies[i*3    ].setUV(0.5f, 0.5f);
		mMesh->mVerticies[i*3 + 1].setUV(tex1);
		mMesh->mVerticies[i*3 + 2].setUV(tex2);
	}
}

CLOSE_O2_NAMESPACE
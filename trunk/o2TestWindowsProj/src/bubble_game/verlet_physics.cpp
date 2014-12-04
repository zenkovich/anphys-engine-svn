#include "verlet_physics.h"

OPEN_O2_NAMESPACE

VeretPhysics::Particle::Particle( const vec2f& position /*= vec2f()*/ )
{

}

void VeretPhysics::Particle::integrate()
{

}


VeretPhysics::Link::Link( Particle* particleA, Particle* particleB, float distance, float hardness /*= 1.0f*/ )
{

}

void VeretPhysics::Link::resolve()
{

}


VeretPhysics::VeretPhysics()
{

}

VeretPhysics::~VeretPhysics()
{

}

void VeretPhysics::update( float dt )
{

}

Particle* VeretPhysics::createParticle( const vec2f& position /*= vec2f()*/ )
{

}

void VeretPhysics::removeParticle( Particle* particle )
{

}

Link* VeretPhysics::createLink( Particle* particleA, Particle* particleB, float distance, float hardness /*= 1.0f*/ )
{

}

void VeretPhysics::removeLink( Link* link )
{

}

void VeretPhysics::clear()
{

}

CLOSE_O2_NAMESPACE
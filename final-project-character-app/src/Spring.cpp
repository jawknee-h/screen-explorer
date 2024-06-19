#include "Spring.h"
#include <iostream>

using namespace std;

Spring::Spring(int rest_len, float spring_const, float dmping, ofVec2f pssive_force)
{
	// setting consts passed in (make them actually const****)
	rest_length = rest_len;
	k = spring_const;
	damping = dmping;
	
	// initializing variables
	end_pos = { 0, 0 };
	anchor_pos = { 0, 0 };
	velocity = { 0, 0 };
	passive_force = pssive_force;
	impulse_force = { 0, 0 };
	external_force = { 0, 0 };

	//damping = 0.93f;
	//k = 0.01;
}

Spring::~Spring()
{
	cout << "~Spring(): Destructed" << endl;
}

void Spring::update(const float& dt)
{
	spring_force = end_pos - anchor_pos;
	const float x = spring_force.length() - rest_length;
	
	spring_force.normalize();
	spring_force *= (-1 * k * x);

	// F = A
	velocity += spring_force + passive_force + external_force + impulse_force;
	end_pos += velocity;
	velocity *= damping;

	impulse_force *= 0.6f;
	external_force = { 0, 0 }; // resetting external force every frame, since it's intended more for temporary impulses
}

void Spring::setAnchorPos(ofVec2f pos)
{
	anchor_pos = pos;
}

void Spring::setSpringConst(float sk)
{
	k = sk;
}

void Spring::multiplySpringConst(float factor)
{
	k *= factor;
}

void Spring::setDamping(float d)
{
	damping = d;
}

const ofVec2f& Spring::getAnchorPos()
{
	return anchor_pos;
}

const ofVec2f& Spring::getEndPos()
{
	return end_pos;
}

// this is the force which will be applied passively at all times
// For example gravity, or push a wing to the side it should be on.
void Spring::setPassiveForce(ofVec2f pf)
{
	passive_force = pf;
}

void Spring::addImpulse(ofVec2f imp_f)
{
	impulse_force = imp_f;
}

void Spring::applyExternalForce(ofVec2f ef)
{
	external_force = ef;
}
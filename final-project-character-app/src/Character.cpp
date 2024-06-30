#include "Character.h"
#include <iostream>


using namespace std;

Character::Character(const float fy)
	: FLOOR_Y{fy}
{
	cout << "Character(): Constructed!" << endl;

	legs.foot_radius = 10; // the radius that the feet orbit feet_orbit_origin

	// set initial position..
	phys_position = { 500, 500 };


	// printing instructions
	cout << "\n==================================================================================" << endl;
	cout << "==================================== INSTRUCTIONS ================================" << endl;
	cout << "==================================================================================\n" << endl;
	cout << "- MAKE SURE YOU'RE TABBED INTO 'My Window!!!' with ALT+TAB" << endl;
	cout << "- ARROW KEYS to move around" << endl;
	cout << "- UP ARROW key to JUMP" << endl;
	cout << "- 'E' and 'Q' to REACH TOWARDS CURSOR" << endl;
	cout << "- '1' and '2' to CHANGE WALK SPEED" << endl;
	cout << "- it looks at the cursor" << endl;
	cout << "\n==================================================================================" << endl;
}

Character::~Character()
{
	cout << "Character(): Destructed!" << endl;
}

void Character::updatePhysics(const float& dt)
{
	// Automatic walking to target
	if (walkingTo)
	{
		if (abs(walkToTargetXPos - getPosition().x) < 10)
		{
			walkingTo = false;
		}
		// If the target is to the right of the character
		else if (walkToTargetXPos > getPosition().x)
		{
			walkRight();
			
			// If we have passed the target now, no longer should be walkingTo
			if (walkToTargetXPos <= getPosition().x)
			{
				walkingTo = false;
			}
		}
		// Otherwise, if the target is to the left of the character
		else if (walkToTargetXPos < getPosition().x)
		{
			walkLeft();

			// If we have passed the target now, no longer should be walkingTo
			if (walkToTargetXPos >= getPosition().x)
			{
				walkingTo = false;
			}
		}
	}


	/////////////////////////////////////////////////

	velocity.x = normalized_walk_speed * walk_speed;
	phys_position += velocity * dt; // apply velocity to position (averaging velocity this and last frame for reasons https://gamedev.stackexchange.com/a/126069)
	velocity.y += GRAVITY * dt; // apply gravity to velocity

	update_legs(legs, dt);
	update_arms(dt);
	antenna_spring1.update(dt);
	antenna_spring2.update(dt);

	if (onGround)
	{
		normalized_walk_speed *= DRAG; // applying drag
		legs.foot_speed = walk_speed * 0.0005f; /*0.2 or /2000*/
	}
	else // if airborne, or just landed this frame
	{
		legs.foot_speed *= 0.98; // make this framerate independent somehow****

		// reducing the spring const gradually while airborne
		// so the legs are more floaty
		legs.foot1spring.multiplySpringConst(0.996);
		legs.foot2spring.multiplySpringConst(0.996);


		// WINGS FLAPPING
		// flap when coming down
		if (fmod(velocity.y, 4) > 3.5f)
		{
			auto f = min(0.0f, -abs(velocity.y)) * 0.01f + 0.5f;
			wing1spring.addImpulse({ 0, f });
			wing2spring.addImpulse({ 0, f });
		}

		//// flap when going up **** make framerate independent
		//if (fmod(-velocity.y, 4) > 3.5f)
		//{
		//	auto f = min(0.0f, velocity.y) * 0.006f - 0.5f;
		//	//cout << f << endl;
		//	wing1spring.addExternalForce({ 0, f });
		//	wing2spring.addExternalForce({ 0, f });
		//}
	}

	if (phys_position.y > FLOOR_Y)
	{
		// if onGround is false, but we actually should be grounded,
		// in other words, we just landed this frame..
		if (!onGround)
		{
			justLanded();
		}

		// preventing us from falling through the floor
		phys_position.y = FLOOR_Y;
		velocity.y = 0;
	}

	antenna_spring1.setAnchorPos({ sprite.getPosition().x - 15, sprite.getPosition().y - (sprite.getHeight() * 0.67f)});
	antenna_spring2.setAnchorPos({ sprite.getPosition().x + 15, sprite.getPosition().y - (sprite.getHeight() * 0.67f)});
}

void Character::draw(const float& dt)
{
	switch (drawMode)
	{
		case dm_animated:
			draw_animated(dt);
			break;
		case dm_static:
			draw_static();
			break;
	}
}

void Character::draw_static()
{
	standing_sprite.setPosition({ phys_position.x, phys_position.y });
	standing_sprite.draw();
}

void Character::draw_animated(const float& dt)
{
	auto draw_verts = [](auto& vertarray, ofColor& c) {
		ofPushStyle();
		ofSetColor(c);
		ofBeginShape();
		for (auto& v : vertarray)
		{
			ofVertex(v);
		}
		ofEndShape(true);
		ofPopStyle();
	};

	auto draw_verts_gl = [](auto& vertarray, ofFloatColor& c) {
		glColor4f(c.r, c.g, c.b, c.a);
		glBegin(GL_TRIANGLE_FAN);
		for (auto& v : vertarray)
		{
			glVertex2f(v.x, v.y);
		}
		glEnd();
	};

	draw_verts_gl(legs.leg1, maincol);
	draw_verts_gl(legs.leg2, maincol);
	draw_verts_gl(wing1, wingcol);
	draw_verts_gl(wing2, wingcol);
	draw_antenna(antenna_spring1, maincol);
	draw_antenna(antenna_spring2, maincol);

	// bobbing the sprite up and down while walking
	float v_bob = 0.0f;
	if (velocity.x != 0)
	{
		v_bob = ((sin(phys_position.x * 0.1) + 1) * 2) * onGround; // only bobbing when on the ground (onGround == 1 on ground, == 0 not on ground. So multiplying by it makes zero when not on ground)
	}
	sprite.setPosition({ phys_position.x, phys_position.y - v_bob });
	sprite.draw();

	// drawing arms after sprite so they are drawn in front
	draw_verts_gl(arm1, maincol);
	draw_verts_gl(arm2, maincol);

	ofPushStyle();
	ofSetColor(maincol);
		ofDrawCircle(arm1spring.getEndPos(), 4);
		ofDrawCircle(arm2spring.getEndPos(), 4);
	ofPopStyle();

	// eyes
	update_eyes(dt);

	// Blinking intermittently
	if (frames_until_blink < 0)
	{
		frames_until_blink = rand() % 250 + 100;
		blink();
	}
	frames_until_blink -= 1;

	// Looking at new target intermittently
	if (frames_until_new_look_target < 0)
	{
		frames_until_new_look_target = rand() % 250 + 100;
		lookAt(ofVec2f(ofRandomWidth(), ofRandomHeight()));
	}
	frames_until_new_look_target -= 1;

	// updating and drawing the eyelid sprite
	if (blinking)
	{
		if (eyelid_alpha < 255) eyelid_alpha = 255;
		else blinking = false;
	}
	else
	{
		if (eyelid_alpha > 0) eyelid_alpha -= 15; // **dt
		eyelid_alpha = max(eyelid_alpha, 0);
	}

	eyelidsprite.setPosition(sprite.getPosition());
	eyelidsprite.setAlpha(eyelid_alpha);
	eyelidsprite.draw();
}

void Character::draw_antenna(Spring& ant_spr, ofColor col)
{

	// getting positions from the antenna spring
	ofVec2f anchor_pos = ant_spr.getAnchorPos();
	ofVec2f end_pos = ant_spr.getEndPos();

	// setting up drawing shapes
	//sf::Vertex line[4]; // make this only happen once in future
	ofPath line;
	line.setFillColor(col);

	// setting antenna line vertices..
	vector<ofVec2f> line_verts;
	line_verts.push_back({ anchor_pos.x - 1, anchor_pos.y });
	line_verts.push_back({ end_pos.x - 2, end_pos.y });
	line_verts.push_back({ line_verts[1].x + 4, line_verts[1].y });
	line_verts.push_back({ anchor_pos.x + 1, anchor_pos.y });

	// and then drawing them..
	ofPushStyle();
		ofSetColor(col);
		ofBeginShape();
			for (auto& v : line_verts) ofVertex(v.x, v.y);
		ofEndShape(true);
		ofDrawCircle(end_pos, 5); // drawing the ball on the end
	ofPopStyle();
}

void Character::update_eyes(const float& dt)
{
	ofPushStyle();
	ofSetColor(34, 32, 52);

	// left eye
	ofVec2f rest_pos(sprite.getPosition().x - 20, sprite.getPosition().y - 100);
	ofVec2f look_offset = set_length(look_target - rest_pos, 5);
	
	eye_spring_L.setAnchorPos(rest_pos + look_offset);
	eye_spring_L.update(dt);
	ofDrawCircle(eye_spring_L.getEndPos() , 10);

	// right eye
	rest_pos = { sprite.getPosition().x + 33, sprite.getPosition().y - 91 };

	eye_spring_R.setAnchorPos(rest_pos + look_offset);
	eye_spring_R.update(dt);
	ofDrawCircle(eye_spring_R.getEndPos(), 10);

	ofPopStyle();
}

void Character::setPosition(const ofVec2f& position)
{
	sprite.setPosition(position);
}

const ofVec2f& Character::getPosition()
{
	return phys_position;
}

void Character::jump()
{
	if (onGround)
	{
		velocity.y = -JUMP_FORCE;
		onGround = false;

		// Updating the spring const of feet when jumping.
		// (makes the feet lag behind for a squash and stretch style effect)
		legs.foot1spring.setDamping(legs.AIRBORNE_DAMPING);
		legs.foot2spring.setDamping(legs.AIRBORNE_DAMPING);

	}
}

void Character::justLanded()
{
	onGround = true;

	legs.just_landed();
}

void Character::walk(float norm_speed)
{
	if (!onGround) return;
	normalized_walk_speed = norm_speed;
}

void Character::walkLeft()
{
	walk(-1 * walkToUrgency);
	legs.sin_angle = fmod(legs.sin_angle + (legs.foot_speed * walkToUrgency), 2*MYPI); // incrementing sin_angle but keeping it in 0-360 range
}

void Character::walkRight()
{
	walk(1 * walkToUrgency);
	legs.sin_angle = fmod(legs.sin_angle - (legs.foot_speed * walkToUrgency), 2*MYPI); // decrementing sin_angle but keeping it in 0-360 range
}

void Character::walkTo(int target_xpos)
{
	// Set the target position and toggle walking to
	walkToTargetXPos = target_xpos;
	walkToUrgency = ofRandom(0.1, 1);
	walkingTo = true;
}

void Character::jumpTowards(const ofVec2f& target)
{
	if (!onGround) return; // only jumping if grounded

	auto normalize_vec = [](ofVec2f& v)
	{
		float length = sqrt((v.x * v.x) + (v.y * v.y));
		if (length != 0) // making sure not to divide by zero
		{
			v.x = v.x / length;
			v.y = v.y / length;
		}
	};

	ofVec2f trajectory = target - phys_position;
	normalize_vec(trajectory);
	
	trajectory.x *= JUMP_FORCE;
	trajectory.y *= JUMP_FORCE;
	//cout << "traj.y: " << trajectory.y << ", min_y_jump_force: " << MIN_Y_JUMP_FORCE << endl;
	trajectory.y = min(trajectory.y, MIN_Y_JUMP_FORCE);
	//cout << "traj: " << trajectory.y << endl;

	velocity = trajectory;

	onGround = false;
}

void Character::lookAt(const ofVec2f target)
{
	auto distance_to_new_target = ofVec2f(target - look_target).length();

	// chance to blink if the new target is far away from the previous target
	if (distance_to_new_target > 40)
	{
		// blinking some of the time
		if (rand() % 10 > 5)
		{
			blink();
		}
	}
	look_target = target;
}

void Character::blink()
{
	blinking = true;

	// occasionally twitch antennas when blinking.
	if (rand() % 10 > 7)
	{
		antenna_spring1.addImpulse({ -0.4f, 0.4f });
		antenna_spring2.addImpulse({ 0.4f, 0.4f });
	}
}

void Character::update_arms(const float& dt) // hange to update wings****
{
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		ofVec2f arm1reachforce = of_get_mouse_pos() - arm1spring.getAnchorPos();
		arm1reachforce = normalize_vec(arm1reachforce) * 4.0;
		arm1spring.applyExternalForce(arm1reachforce);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		ofVec2f arm2reachforce = of_get_mouse_pos() - arm2spring.getAnchorPos();
		arm2reachforce = normalize_vec(arm2reachforce) * 4.0;
		arm2spring.applyExternalForce(arm2reachforce);
	}*/


	// updating positions of hands
	arms.wing_origin = { phys_position.x, sprite.getPosition().y - 80 };
	arms.arm_origin = { arms.wing_origin.x, arms.wing_origin.y - 4 };
	arms.arm1_origin = { arms.arm_origin.x - 20, arms.arm_origin.y + 18 };
	arms.arm2_origin = { arms.arm_origin.x + 20, arms.arm_origin.y + 18 };

	arm1spring.setAnchorPos(arms.arm1_origin);
	arm2spring.setAnchorPos(arms.arm2_origin);

	arm1spring.update(dt);
	arm2spring.update(dt);

	arm1[0] = arms.arm1_origin;
	auto aoffset = rotated_90_counterclockwise(arm1spring.getEndPos() - arms.arm1_origin, 4); // finding vector perpendicular to arm reach direction to give the arm some thickness
	arm1[1] = arm1spring.getEndPos() - aoffset;
	arm1[2] = arm1spring.getEndPos() + aoffset;

	arm2[0] = arms.arm2_origin;
	aoffset = rotated_90_clockwise(arm2spring.getEndPos() - arms.arm2_origin, 4);
	arm2[1] = arm2spring.getEndPos() - aoffset;
	arm2[2] = arm2spring.getEndPos() + aoffset;

	// updating the wings
	wing1spring.setAnchorPos(arms.wing_origin);
	wing2spring.setAnchorPos(arms.wing_origin);
	wing1spring.update(dt);
	wing2spring.update(dt);

	// updating the visuals

	wing1[0] = arms.wing_origin;
	wing1[1] = { wing1[0].x - 20, wing1[0].y + 6 };
	//wing1[1] = wing1spring.getEndPos() + ofVec2f{ 23, -20};
	//wing1[2].position = { wing1[1].position.x - 18, wing1[1].position.y + 20 };
	wing1[2] = wing1spring.getEndPos();
	wing1[3] = { wing1[2].x - 8, wing1[2].y + 23 };
	wing1[4] = { wing1[3].x + 13, wing1[3].y - 5 };
	wing1[5] = { wing1[4].x + 5, wing1[4].y - 1 };
	wing1[6] = { wing1[5].x + 14, wing1[5].y - 20 };
	//wing1[6] = { wing1[0].x - 10, wing1[0].y + 20 };


	wing2[0] = arms.wing_origin;
	wing2[1] = { wing2[0].x + 20, wing2[0].y + 6 };
	//wing2[2].position = { wing2[1].position.x + 18, wing2[1].position.y + 20 }; // uncomment for static wings
	wing2[2] = { wing2spring.getEndPos().x, wing2spring.getEndPos().y };
	wing2[3] = { wing2[2].x + 8, wing2[2].y + 23 };
	wing2[4] = { wing2[3].x - 13, wing2[3].y - 5 };
	wing2[5] = { wing2[4].x - 5, wing2[4].y - 1 };
	wing2[6] = { wing2[5].x - 14, wing2[5].y - 20 };
}

void Character::update_legs(Legs& lgs, const float& dt)
{
	lgs.feet_orbit_origin = { phys_position.x, phys_position.y - 10 }; // the base position that the feet revolve around
	// Calculating the feet's position using sine and cosine to go in a circular pattern,
	// with one foot's phase offset by PI (180deg) so they take turns on the ground.
	// Scaling the x and y of that circle by foot_radius, and scaling the y value by (normalized_walk_speed*walk_speed)/MAX_WALK_SPEED.
	// This makes the height of the steps decrease at lower speeds.
	// The result of abs(normalized_walk_speed) * walk_speed)/MAX__WALK_SPEED will always be 1 if walk_speed == MAX_WALK_SPEED
	// Decreasing normalized_walk_speed constantly trends towards zero so the feet return to the ground when not walking (when norm_walk_speed == 0).
	// FIND A BETTER WAY OF DOING THIS WHERE I ONLY NEED TO USE SINE AND COSINE ONCE!!****
	ofVec2f normalized_foot_offset = {
		sin(lgs.sin_angle),
		(cos(lgs.sin_angle) - 1) * (abs(normalized_walk_speed) * walk_speed)/MAX_WALK_SPEED
	};
	ofVec2f normalized_foot_offset1 = {
		sin(lgs.sin_angle + MYPI),
		(cos(lgs.sin_angle + MYPI) - 1) * (abs(normalized_walk_speed) * walk_speed)/MAX_WALK_SPEED
	};
	// updating the feet's positions
	legs.foot1spring.setAnchorPos(lgs.feet_orbit_origin + normalized_foot_offset * lgs.foot_radius);
	legs.foot2spring.setAnchorPos(lgs.feet_orbit_origin + normalized_foot_offset1 * lgs.foot_radius);

	legs.update_springs(dt);
	/*legs.foot1spring.update();
	legs.foot2spring.update();*/

	// updating the visuals
	ofVec2f new_hip_pos = { phys_position.x, phys_position.y - 80 };
legs.update_visuals(new_hip_pos);
}

void Character::ReachTowards(const ofVec2f& target, float reach_strength)
{

	ofVec2f arm1reachforce = target - arm1spring.getAnchorPos();
	ofVec2f arm2reachforce = target - arm2spring.getAnchorPos();
	float a1mag = arm1reachforce.length();
	float a2mag = arm2reachforce.length();

	// always reach with both arms

	arm1reachforce = normalize_vec(arm1reachforce) * reach_strength;
	arm1spring.applyExternalForce(arm1reachforce);

	arm2reachforce = normalize_vec(arm2reachforce) * reach_strength;
	arm2spring.applyExternalForce(arm2reachforce);
}

void Character::increase_walk_speed()
{
	walk_speed = min(walk_speed + 10.0f, MAX_WALK_SPEED);

	cout << "walk_speed increased to: " << walk_speed << endl;
}

void Character::decrease_walk_speed()
{
	walk_speed = max(walk_speed - 10.0f, MIN_WALK_SPEED);

	cout << "walk_speed decreased to: " << walk_speed << endl;
}

void Character::chase(ofVec2f& target)
{
	ofVec2f distance_to_target = target - phys_position;
	const auto MIN_CHASE_DISTANCE = 20; // how far away the target has to be for him to try to chase it

	// mapping walk speed to distance but only while on the ground
	if (onGround) walk_speed = ofMap(abs(distance_to_target.x), 0, 400, 20, MAX_WALK_SPEED, true);
	if (walk_speed == 20) walk_speed = 0;

	// walking towards target
	if (abs(distance_to_target.x) > 20)
	{
		if (distance_to_target.x < 0)
		{
			walkLeft();
		}
		else if (distance_to_target.x > 0)
		{
			walkRight();
		}
	}

	// grabbing
	if (abs(distance_to_target.x) < 100 && distance_to_target.y < -100 && distance_to_target.y > -300)
	{
		ReachTowards(target, 4.0);
	}
	// reach/wave around arms
	if (distance_to_target.y < -800)
	{
		ReachTowards(target, 4.0);
	}
}

void Character::switch_draw_mode()
{
	if (drawMode == dm_animated) drawMode = dm_static;
	else if (drawMode == dm_static) drawMode = dm_animated;
}
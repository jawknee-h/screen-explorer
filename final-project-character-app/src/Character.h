#include "Spring.h"
#include "Utilities.h"
#include <algorithm>
#include <cstdint>
#include <iomanip>

//#pragma once

class Character
{
	public:
		Character(const float fy);
		~Character();

		void updatePhysics(const float& dt);
		void draw(const float& dt);
		void draw_animated(const float& dt);
		void draw_static();
		void switch_draw_mode();
		enum DrawMode { dm_animated, dm_static };
		DrawMode drawMode = dm_animated;


		// basic AI
		void chase(ofVec2f& target);

		// position
		void setPosition(const ofVec2f& position);
		const ofVec2f& getPosition();
		
		// movement
		void walkLeft();
		void walkRight();
		void walkTo(int _target_xpos);
		bool walkingTo = false;
		float walkToUrgency = 1;
		int walkToTargetXPos;
		void jump();
		void jumpTowards(const ofVec2f& target);

		// eyes
		void lookAt(const ofVec2f target);
		ofVec2f look_target;
		void blink();
		bool blinking = false;
		Spring eye_spring_L{ 0, 0.3f, 0.5f, {0, 0} };
		Spring eye_spring_R{ 0, 0.3f, 0.5f, {0, 0} };

		void ReachTowards(const ofVec2f& target, float reach_strength);

		void decrease_walk_speed();
		void increase_walk_speed();

	private:

		struct Legs {
			// set these..
			float foot_speed;
			float foot_radius;
			ofVec2f hip_pos; // the position the top of the legs are drawn from
			ofVec2f feet_orbit_origin; // the position the feet revolve around

			// constants
			const float AIRBORNE_SPRING_CONST = 0.05f;
			const float GROUNDED_SPRING_CONST = 0.09f;
			const float GROUNDED_DAMPING = 0.76f;
			const float AIRBORNE_DAMPING = 0.82f;

			// springs
			Spring foot1spring{ 0, GROUNDED_SPRING_CONST, GROUNDED_DAMPING, {0.0, 0.0} };
			Spring foot2spring{ 0, GROUNDED_SPRING_CONST, GROUNDED_DAMPING, {0.0, 0.0} };

			// visuals (vertex arrays for the polygons)
			ofVec2f leg1[3];
			ofVec2f leg2[3];

			void update_springs(const float& dt)
			{
				foot1spring.update(dt);
				foot2spring.update(dt);
			}

			void just_landed()
			{
				foot1spring.setSpringConst(GROUNDED_SPRING_CONST);
				foot2spring.setSpringConst(GROUNDED_SPRING_CONST);
				foot1spring.setDamping(GROUNDED_DAMPING);
				foot2spring.setDamping(GROUNDED_DAMPING);
			}

			void update_visuals(ofVec2f new_hip_pos)
			{
				hip_pos = new_hip_pos;
				leg1[0] = hip_pos;
				leg1[1] = { foot1spring.getEndPos().x + 3, foot1spring.getEndPos().y };
				leg1[2] = { foot1spring.getEndPos().x - 3, foot1spring.getEndPos().y };
				leg2[0] = hip_pos;
				leg2[1] = { foot2spring.getEndPos().x + 3, foot2spring.getEndPos().y };
				leg2[2] = { foot2spring.getEndPos().x - 3, foot2spring.getEndPos().y };
			}

			// these are set/updated in update_legs();
			float sin_angle = 0.8f;
		} legs;

		// for physics..
		ofVec2f phys_position{0, 0};
		ofVec2f velocity{0, 0};
		const float GRAVITY = 980.0f;
		const float DRAG = 0.83;
		const float JUMP_FORCE = 500.0f;
		const float MIN_Y_JUMP_FORCE = -JUMP_FORCE * 0.5f;

		bool onGround = false;
		float normalized_walk_speed = 0.0f;
		const float MAX_WALK_SPEED = 400.0f;
		const float MIN_WALK_SPEED = 50.0f;
		float walk_speed = MAX_WALK_SPEED;
		const float FLOOR_Y;

		// methods
		void walk(float norm_speed);
		void justLanded();

		// legs
		void update_legs(Legs& lgs, const float& dt);

		// arms/wings
		struct Arms {
			const ofVec2f ARM1_OFFSET{ -60, -40 };
			const ofVec2f ARM2_OFFSET{ 60, -40 };
			ofVec2f wing_origin;
			ofVec2f arm_origin;
			ofVec2f arm1_origin;
			ofVec2f arm2_origin;
		} arms;

		// wings
		const float WING_SPRING_CONST = 0.09f;
		const float WING_DAMPING = 0.76f;
		Spring wing1spring{ 20, WING_SPRING_CONST, WING_DAMPING, { -2.0, 1.5 }};
		Spring wing2spring{ 20, WING_SPRING_CONST, WING_DAMPING, { 2.0, 1.5 } };
		void update_arms(const float& dt);
		ofFloatColor wingcol{ 0.67f, 0.67f, 0.78f, 0.59f };

		// antenna
		/*Spring antenna_spring1{ 10, 0.01f, 0.93f, {-0.15, -0.3} };
		Spring antenna_spring2{ 10, 0.008, 0.97, {0.19, -0.3} };*/
		Spring antenna_spring1{ 30, 0.01f*3, 0.91f, {-0.15, -0.3} };
		Spring antenna_spring2{ 30, 0.008*3, 0.95f, {0.19, -0.3} };
		void draw_antenna(Spring& ant_spr, ofColor col);

		// eyes
		void update_eyes(const float& dt);

		// arms
		const float ARM_SPRING_CONST = 0.13f;
		const float ARM_DAMPING = 0.6f;
		Spring arm1spring{ 25, ARM_SPRING_CONST, ARM_DAMPING, {-1.8, 1.8} };
		Spring arm2spring{ 25, ARM_SPRING_CONST, ARM_DAMPING, {1.8, 1.8} };
		ofVec2f arm1[3];
		ofVec2f arm2[3];

		// drawing objects
		//ofColor maincol{ 172, 50, 50 };
		ofFloatColor maincol{ 0.67, 0.20, 0.20 };
		
		ofVec2f wing1[7];
		ofVec2f wing2[7];
		Sprite sprite{ "red_weird_guy_nopupils.png" };

		// static sprites - no animated mode
		Sprite standing_sprite{ "standing.png" };
};
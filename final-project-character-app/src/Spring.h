#include "Utilities.h"
#include "ofMain.h"

class Spring
{
	public:
		Spring(int rest_len = 0, float spring_const = 0.01f, float dmping = 0.93f, ofVec2f pssive_force={0,-0.3});
		~Spring();

		void update(const float& dt);
		void setAnchorPos(ofVec2f pos);
		const ofVec2f& getAnchorPos();
		const ofVec2f& getEndPos();
		void setSpringConst(float sk);
		void multiplySpringConst(float factor);
		void setDamping(float d);
		void setPassiveForce(ofVec2f pf);
		void addImpulse(ofVec2f imp_f);
		void applyExternalForce(ofVec2f ef);
		void Spring::applyExternalForceOverride(ofVec2f ef);
	private:		
		ofVec2f anchor_pos;
		ofVec2f end_pos;
		ofVec2f velocity;
		ofVec2f spring_force;
		ofVec2f passive_force;
		ofVec2f impulse_force;
		ofVec2f external_force;
		int rest_length;
		float k;
		float damping;
};
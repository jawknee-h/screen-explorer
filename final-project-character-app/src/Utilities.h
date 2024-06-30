#ifndef UTILS
#define UTILS

//#include <tgmath.h> // for sqrt
#include "ofMain.h"

/// NEED TO USE INLINE FUNCTIONS TO AVOID LINKER ERRORS
//namespace ut
//{
//
//}

struct Sprite
{
	Sprite(std::string path, float xpos=0, float ypos=0)
	{
		cout << "Sprite(): Constructed" << endl;

		// loading image
		if (!img.load(path)) cout << "Sprite(): image failed to load." << endl;

		// intializing position
		pos = { xpos, ypos };
	};
	~Sprite()
	{
		cout << "Sprite(): Destructed" << endl;
	}
	void setPosition(ofVec2f p)
	{
		pos = p;
	}
	const ofVec2f& getPosition()
	{
		return pos;
	}
	void setAlpha(const int new_alpha)
	{
		alpha = new_alpha;
	}
	void draw()
	{
		ofPushStyle();
		ofSetColor(255, 255, 255, alpha);
		img.draw({pos.x - getWidth()/2, pos.y - getHeight()}); // drawing centered
		ofPopStyle();
	}
	const float& getWidth()
	{
		return img.getWidth();
	}
	const float& getHeight()
	{
		return img.getHeight();
	}
	ofVec2f pos;
	ofImage img;
	int alpha = 255;
};

inline auto normalize_vec(const ofVec2f& v)
{
	ofVec2f v1 = v;
	float length = sqrt((v.x * v.x) + (v.y * v.y));
	if (length != 0) // making sure not to divide by zero
	{
		v1.x = v1.x / length;
		v1.y = v1.y / length;
	}
	return v1;
}

inline auto get_mag(const ofVec2f& v)
{
	return sqrt((v.x * v.x) + (v.y * v.y));
}

inline auto set_length(const ofVec2f& v, float length)
{
	return normalize_vec(v) * length;
}

inline auto rotated_90_clockwise(const ofVec2f& v)
{
	// multiply v.x by -1, and then swap x and y
	return ofVec2f(v.y, -v.x);
}

inline auto rotated_90_clockwise(const ofVec2f& v, float length)
{
	return normalize_vec(rotated_90_clockwise(v))*length;
}

inline auto rotated_90_counterclockwise(const ofVec2f& v)
{
	// rotating 90 degrees clockwise and then inverting.
	ofVec2f v1 = rotated_90_clockwise(v);
	return -v1;
}

inline auto rotated_90_counterclockwise(const ofVec2f& v, float length)
{
	return normalize_vec(rotated_90_counterclockwise(v)) * length;
}

//inline auto of_get_mouse_pos()
//{
//	return ofVec2f(ofGetMouseX(), ofGetMouseY());
//}

inline auto of_get_mouse_pos()
{
	POINT p;
	GetCursorPos(&p);;
	return ofVec2f(p.x, p.y);
	//return ofVec2f(ofGetMouseX(), ofGetMouseY());
}

template <typename T>
T clamp(const T& n, const T& lower, const T& upper)
{
	return std::max(lower, std::min(n, upper));
};

const auto MYPI = 355.0f / 113.0f;

#endif
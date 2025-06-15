#pragma once
#include "SFML/Graphics.hpp"
#include <math.h>
#ifndef PI
#define PI 3.14159265
#define Rad(a) (a * (PI * 0.00555555555))
#define Deg(a) (a * (180.0f / PI))
#endif // !PI

struct OKLab {
	float L, a, b;
	OKLab(float L = 0, float a = 0, float b = 0) : L(L), a(a), b(b) {}
};

class VectorHelper
{
public:
	//Returns a normalised vector
	static sf::Vector2f normalise(const sf::Vector2f& source);
	static sf::Vector3f normalise(const sf::Vector3f& source);
	//Returns the Vector magnitude
	static float magnitude(sf::Vector2f vec);
	static float magnitude(sf::Vector3f vec);
	//Returns the magnitude squared
	static float magnitudeSqrd(sf::Vector2f v);
	static float magnitudeSqrd(sf::Vector3f v);
	//Returns the dot product of 2 vectors
	static float dot(sf::Vector2f v, sf::Vector2f u);
	static float dot(sf::Vector2i v, sf::Vector2i u);
	static float dot(sf::Vector3f v, sf::Vector3f u);
	//Returns the angle between 2 vectors
	static float angle(sf::Vector2f v, sf::Vector2f u);
	static float angle(sf::Vector2f v);
	//Linearly interpolates a point between 2 vectors by percentage P
	static sf::Vector2f lerp(sf::Vector2f v, sf::Vector2f u, float P);
	//Linearly interpolates a float between 2 floats by percentage P
	static float lerp(float v, float u, float P);
	//Returns a vector of the multiplicataion of the components of 2 vectors
	static sf::Vector2f componentMult(sf::Vector2f v, sf::Vector2f u);
	//Returns the cross product of 2 vectors
	static sf::Vector3f cross(sf::Vector3f v, sf::Vector3f u);
	static float cross(sf::Vector2f v, sf::Vector2f u);
	//Rotates a vector by an angle (in radians)
	static sf::Vector2f rotate(sf::Vector2f v, const float& angle);
	static sf::Vector2f rotateDegrees(sf::Vector2f v, const float& angle);

	//Converts Colour to HSV and HSV to Colour
	static sf::Vector3f RGBtoHSV(const sf::Color& colour);
	static sf::Color HSVtoRGB(const sf::Vector3f& hsv);
	//Converts RGB to OKLAB colourspace and vica versa
	static OKLab RGBtoOKLAB(const sf::Color& rgb);
	static sf::Color OKLABtoRGB(const OKLab& oklab);

	static float fastInvSqrt(float n);
};



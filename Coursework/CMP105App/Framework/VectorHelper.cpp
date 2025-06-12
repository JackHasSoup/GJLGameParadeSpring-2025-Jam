#include "VectorHelper.h"

// Returns a normalised vector
sf::Vector2f VectorHelper::normalise(const sf::Vector2f& source)
{
	float invLength = sqrt((double(source.x) * double(source.x)) + (double(source.y) * double(source.y)));
	if (invLength != 0)
		return sf::Vector2f(source.x, source.y) / invLength;
	else
		return source;
}

sf::Vector3f VectorHelper::normalise(const sf::Vector3f& source)
{
	float length = magnitude(source);
	if (length != 0)
		return source / length;
	else
		return source;
}

// Returns the Vector magnitude
float VectorHelper::magnitude(sf::Vector2f vec)
{
	return sqrt((double(vec.x) * double(vec.x)) + (double(vec.y) * double(vec.y)));
}

float VectorHelper::magnitude(sf::Vector3f vec)
{
	return (float)sqrt((double(vec.x) * double(vec.x)) + (double(vec.y) * double(vec.y)) + (double(vec.z) * double(vec.z)));
}

float VectorHelper::magnitudeSqrd(sf::Vector2f v)
{
	return (double(v.x) * double(v.x)) + (double(v.y) * double(v.y));
}
float VectorHelper::magnitudeSqrd(sf::Vector3f vec)
{
	return (double(vec.x) * double(vec.x)) + (double(vec.y) * double(vec.y)) + (double(vec.z) * double(vec.z));
}

//Returns the dot product of 2 vectors
float VectorHelper::dot(sf::Vector2f v, sf::Vector2f u)
{
	return v.x * u.x + v.y * u.y;
}
float VectorHelper::dot(sf::Vector2i v, sf::Vector2i u)
{
	return v.x * u.x + v.y * u.y;
}

float VectorHelper::dot(sf::Vector3f v, sf::Vector3f u)
{
	return v.x*u.x + v.y*u.y + v.z * v.z;
}

//Returns the angle between 2 vectors
float VectorHelper::angle(sf::Vector2f v, sf::Vector2f u)
{
	//A.B = |A||B|cosθ
	const float dot = VectorHelper::dot(v, u);
	const float a = acos(dot / (VectorHelper::magnitude(v) * VectorHelper::magnitude(u)));
	return a;
}

float VectorHelper::angle(sf::Vector2f v)
{
	return atan2(v.y, v.x);
}

sf::Vector2f VectorHelper::lerp(sf::Vector2f v, sf::Vector2f u, float P)
{
	return v + (u - v) * P;
}

sf::Vector2f VectorHelper::componentMult(sf::Vector2f v, sf::Vector2f u)
{
	return sf::Vector2f(v.x * u.x, v.y * u.y);
}

sf::Vector3f VectorHelper::cross(sf::Vector3f v, sf::Vector3f u)
{
	sf::Vector3f t;
	t.x = (v.y * u.z) - (v.z * u.y);
	t.y = (v.z * u.z) - (v.x * u.z);
	t.z = (v.x * u.y) - (v.y * u.x);
	return t;
}

float VectorHelper::cross(sf::Vector2f v, sf::Vector2f u)
{
	return (v.x * u.y) - (u.x * v.y);
}

float VectorHelper::fastInvSqrt(float n)
{
	if (n == 0) return n;
	//from the famous quake algorithm
	long i;
	float x2, y;

	x2 = n * 0.5f;
	y = n;
	i = *(long*)&y;
	i = 0x5f379df - (i >> 1);
	y = *(float*)&i;
	y *= 1.5f - (x2 * y * y);
	return y;
}

sf::Vector3f VectorHelper::RGBtoHSV(const sf::Color& colour)
{
	float r = colour.r / 255.f;
	float g = colour.g / 255.f;
	float b = colour.b / 255.f;

	float cmax = std::max({ r, g, b });
	float cmin = std::min({ r, g, b });
	float delta = cmax - cmin;

	float h = 0.f;
	if (delta != 0.f) {
		if (cmax == r)
			h = 60.f * std::fmod(((g - b) / delta), 6.f);
		else if (cmax == g)
			h = 60.f * (((b - r) / delta) + 2.f);
		else
			h = 60.f * (((r - g) / delta) + 4.f);
	}
	if (h < 0) h += 360.f;

	float s = (cmax == 0) ? 0.f : delta / cmax;
	float v = cmax;

	return { h, s, v };
}

sf::Color VectorHelper::HSVtoRGB(const sf::Vector3f& hsv)
{
	float h = hsv.x;
	float s = hsv.y;
	float v = hsv.z;

	float c = v * s;
	float x = c * (1 - std::fabs(std::fmod(h / 60.f, 2) - 1));
	float m = v - c;

	float r, g, b;
	if (h < 60) { r = c; g = x; b = 0; }
	else if (h < 120) { r = x; g = c; b = 0; }
	else if (h < 180) { r = 0; g = c; b = x; }
	else if (h < 240) { r = 0; g = x; b = c; }
	else if (h < 300) { r = x; g = 0; b = c; }
	else { r = c; g = 0; b = x; }

	return sf::Color(
		static_cast<sf::Uint8>((r + m) * 255),
		static_cast<sf::Uint8>((g + m) * 255),
		static_cast<sf::Uint8>((b + m) * 255)
	);
}

OKLab VectorHelper::RGBtoOKLAB(const sf::Color& rgb)
{
	//bormalize RGB from 0-255 to 0-1
	float r = rgb.r / 255.0f;
	float g = rgb.g / 255.0f;
	float b = rgb.b / 255.0f;

	//convert to linear RGB
	auto toLinear = [](float c) {
		return c <= 0.04045f ? c / 12.92f : pow((c + 0.055f) / 1.055f, 2.4f);
		};
	r = toLinear(r);
	g = toLinear(g);
	b = toLinear(b);

	//linear RGB to LMS
	float l = 0.4122214708f * r + 0.5363325363f * g + 0.0514459929f * b;
	float m = 0.2119034982f * r + 0.6806995451f * g + 0.1073969566f * b;
	float s = 0.0883024619f * r + 0.2817188376f * g + 0.6299787005f * b;

	//LMS to OKLAB
	float l_ = cbrt(l);
	float m_ = cbrt(m);
	float s_ = cbrt(s);

	return OKLab(
		0.2104542553f * l_ + 0.7936177850f * m_ - 0.0040720468f * s_,
		1.9779984951f * l_ - 2.4285922050f * m_ + 0.4505937099f * s_,
		0.0259040371f * l_ + 0.7827717662f * m_ - 0.8086757660f * s_
	);
}

sf::Color VectorHelper::OKLABtoRGB(const OKLab& oklab)
{
	float l_ = oklab.L + 0.3963377774f * oklab.a + 0.2158037573f * oklab.b;
	float m_ = oklab.L - 0.1055613458f * oklab.a - 0.0638541728f * oklab.b;
	float s_ = oklab.L - 0.0894841775f * oklab.a - 1.2914855480f * oklab.b;

	float l = l_ * l_ * l_;
	float m = m_ * m_ * m_;
	float s = s_ * s_ * s_;

	//LMS to linear RGB
	float r = +4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s;
	float g = -1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s;
	float b = -0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s;

	//linear RGB to sRGB
	auto toSRGB = [](float c) {
		return c <= 0.0031308f ? 12.92f * c : 1.055f * pow(c, 1.0f / 2.4f) - 0.055f;
		};
	r = toSRGB(r);
	g = toSRGB(g);
	b = toSRGB(b);

	//clamp and convert to 0-255 range
	r = std::clamp(r * 255.0f, 0.0f, 255.0f);
	g = std::clamp(g * 255.0f, 0.0f, 255.0f);
	b = std::clamp(b * 255.0f, 0.0f, 255.0f);

	return sf::Color(r, g, b);
}

sf::Vector2f VectorHelper::rotate(sf::Vector2f v, const float& angle)
{
	const auto c = cos(angle); const auto s = sin(angle);
	return sf::Vector2f((c * v.x) - (s * v.y), (s * v.x) + (c * v.y));
}

sf::Vector2f VectorHelper::rotateDegrees(sf::Vector2f v, const float& angle)
{
	return rotate(v, Rad(angle));
}

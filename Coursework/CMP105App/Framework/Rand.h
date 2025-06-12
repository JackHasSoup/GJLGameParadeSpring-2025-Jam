#pragma once
#include "VectorHelper.h"
#include <vector>
#include <cmath>
#include <random>
#include <iostream>

class Rand
{
public:
	Rand(float a, float seed);
	Rand() {};
	~Rand() {};

	static float quickSample(int x, int y);
	static int quickIntRangeRand(int min, int max);
	static float quickRangeRand(float min, float max);

	float perlinSample(int x, int y, float scale);
private:
	unsigned short int PERLIN_OCTAVES = 4;
	static constexpr int PERLIN_SIZE = 511;
	int blurBoxCount = 8;
	float amplitude = 1.f;
	float seed = 0.f;

	std::vector<unsigned short int> perlinPermutations;

	static double fadeVal(double v);
	static double lerpVal(double a, double b, double step);
	static double gradient(int h, double x, double y, double z);
	double perlinNoise(double xp, double yp);
	double fractalBrownianMotion(double x, double y, float scale);
	sf::Vector2f getCornerVector(int v);

	void shufflePermutations();
	void permutate();
};
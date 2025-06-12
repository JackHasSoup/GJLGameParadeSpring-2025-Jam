#include "Rand.h"

Rand::Rand(float a, float seed)
{
	this->seed = seed;
	amplitude = a;
	permutate();
}

float Rand::quickSample(int x, int y)
{
	float pseudoRandom = VectorHelper::dot(sf::Vector2f(x, y), { 543.34523, 1267.456723 });
	pseudoRandom = modf(pseudoRandom * (static_cast<double>(time(0)) / 10000.f), new double);
	return pseudoRandom;
}

float Rand::quickRangeRand(float min, float max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	if (min > max || max < min) std::swap(min, max);
	std::uniform_real_distribution<float> dist(min, max);

	return dist(gen);
}

int Rand::quickIntRangeRand(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	if (min > max || max < min) std::swap(min, max);
	std::uniform_int_distribution<int> dist(min, max);

	return dist(gen);
}

float Rand::perlinSample(int x, int y, float scale)
{
	//This perlin noise implementation is from these blog posts:
	//https://rtouti.github.io/graphics/perlin-noise-algorithm
	//https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/
	//https://github.com/Reputeless/PerlinNoise
	//This is a reference, I am referencing this
	double result = fractalBrownianMotion(x, y , scale);

	if (result <= -1) return 0;
	else if (result >= 1) return 1;
	return (result * 0.5) + 0.5;
	//return perlinNoise(sf::Vector2f(x, y));
}


double Rand::fadeVal(double v)
{
	return v * v * v * (v * (v * 6 - 15) + 10);
}

sf::Vector2f Rand::getCornerVector(int v)
{
	const int h = v & 3;
	switch (h)
	{
	case 0:
		return sf::Vector2f(1, 1);
		break;
	case 1:
		return sf::Vector2f(-1, 1);
		break;
	case 2:
		return sf::Vector2f(-1, -1);
		break;
	default:
		return sf::Vector2f(1, -1);
		break;
	}
}

double Rand::lerpVal(double a, double b, double step)
{
	return a + (step * (b - a));
}

double Rand::gradient(int h, double x, double y, double z)
{
	h = h & 15;
	double u = h < 8 ? x : y;
	double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double Rand::perlinNoise(double xp, double yp)
{
	//A fake Z is added since all the perlin documents I could find implement perlin in 3D then squash it, so I made up a number
	const double Z = 0.31415926;
	const double x = std::floor(xp);
	const double y = std::floor(yp);
	const double z = std::floor(Z);

	const double fx = xp - x;
	const double fy = yp - y;
	const double fz = Z - z;

	const int ix = static_cast<int>(x) & PERLIN_SIZE;
	const int iy = static_cast<int>(y) & PERLIN_SIZE;
	const int iz = static_cast<int>(z) & PERLIN_SIZE;

	const double u = fadeVal(fx);
	const double v = fadeVal(fy);
	const double w = fadeVal(fz);

	const int A = (perlinPermutations[ix & PERLIN_SIZE] + iy) & PERLIN_SIZE;
	const int B = (perlinPermutations[(ix + 1) & PERLIN_SIZE] + iy) & PERLIN_SIZE;

	const int TL = (perlinPermutations[A] + iz) & PERLIN_SIZE;
	const int TR = (perlinPermutations[(A + 1) & PERLIN_SIZE] + iz) & PERLIN_SIZE;

	const int BL = (perlinPermutations[B] + iz) & PERLIN_SIZE;
	const int BR = (perlinPermutations[(B + 1) & PERLIN_SIZE] + iz) & PERLIN_SIZE;

	const double corner1 = lerpVal(gradient(perlinPermutations[TL], fx, fy, fz), gradient(perlinPermutations[BL], fx - 1, fy, fz), u);
	const double corner2 = lerpVal(gradient(perlinPermutations[TR], fx, fy - 1, fz), gradient(perlinPermutations[BR], fx - 1, fy - 1, fz), u);
	const double corner3 = lerpVal(gradient(perlinPermutations[(TL + 1) & PERLIN_SIZE], fx, fy, fz - 1), gradient(perlinPermutations[(BL + 1) & PERLIN_SIZE], fx - 1, fy, fz - 1), u);
	const double corner4 = lerpVal(gradient(perlinPermutations[(TR + 1) & PERLIN_SIZE], fx, fy - 1, fz - 1), gradient(perlinPermutations[(BR + 1) & PERLIN_SIZE], fx - 1, fy - 1, fz - 1), u);

	const double r1 = lerpVal(corner1, corner2, v);
	const double r2 = lerpVal(corner3, corner4, v);

	return lerpVal(r1, r2, w);
}

double Rand::fractalBrownianMotion(double x, double y, float scale)
{
	double result = 0.0;
	double amp = amplitude;//make a copy of amplitude since its modified

	double currentX = x;
	double currentY = y;

	//precompute constants
	const double inv9 = 1.0 / 9.0;
	const double invBlurBoxCount = 1.0 / blurBoxCount;

	for (int octave = 0; octave < PERLIN_OCTAVES; octave++) {
		double n = 0.0;

		//calculate scaled coordinates once per iteration
		double scaledX = currentX * scale;
		double scaledY = currentY * scale;

		for (int i = 0; i < blurBoxCount; i++) {
			//calculate offset coordinates once
			double xi = scaledX + i * scale;
			double yi = scaledY + i * scale;
			double xip = scaledX + (i + 1) * scale;
			double yip = scaledY + (i + 1) * scale;
			double xin = scaledX - (i + 1) * scale;
			double yin = scaledY - (i + 1) * scale;

			//sum all nine perlin noise samples
			n += (perlinNoise(xi, yi) +//center
				perlinNoise(xip, yip) +//topright
				perlinNoise(xin, yin) +//bottomleft
				perlinNoise(xin, yi) +//left
				perlinNoise(xi, yin) +//bottom
				perlinNoise(xi, yip) +//top
				perlinNoise(xip, yi) +//right
				perlinNoise(xip, yin) +//bottomright
				perlinNoise(xin, yip)) * inv9;//topleft
		}

		//combine the multiple optimizations
		result += n * invBlurBoxCount * amp;

		//prepare for next octave
		amp *= 0.5;
		currentX *= 2.0;
		currentY *= 2.0;
	}

	return result;
}

void Rand::shufflePermutations()
{
	//rng that updates fast enough for this purpose
	//std::random_device rd;  
	//std::mt19937 gen(rd()); 
	std::default_random_engine re(seed);
	std::shuffle(perlinPermutations.begin(), perlinPermutations.end(), re);

	/*for (int i = perlinPermutations.size() - 1; i > 0; i--)
	{
		std::uniform_int_distribution<int> dist(0, i - 1);
		const int index = dist(gen);
		const int temp = perlinPermutations[i];

		perlinPermutations[i] = perlinPermutations[index];
		perlinPermutations[index] = temp;
	}*/
}

void Rand::permutate()
{
	for (int i = 0; i < 512 + 1; i++)
	{
		perlinPermutations.push_back(i);
	}

	shufflePermutations();

	for (int i = 0; i < 512 + 1; i++)
	{
		perlinPermutations.push_back(perlinPermutations[i]);
	}
}

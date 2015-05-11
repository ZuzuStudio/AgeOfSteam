#include "../../include/algorithm/perlinnoise.h"

PerlinNoise::PerlinNoise(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed)
{
    persistence = _persistence;
    frequency = _frequency;
    amplitude  = _amplitude;
    octaves = _octaves;
    randomseed = 2 + _randomseed * _randomseed;
}

void PerlinNoise::init(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed)
{
    persistence = _persistence;
    frequency = _frequency;
    amplitude  = _amplitude;
    octaves = _octaves;
    randomseed = 2 + _randomseed * _randomseed;
}

double PerlinNoise::getHeight(double x, double y) const
{
    return amplitude * Total(x, y);
}

double PerlinNoise::Total(double i, double j) const
{
    double t = 0.0f;
    double _amplitude = 1;
    double freq = frequency;

    for(int k = 0; k < octaves; k++)
    {
        t += GetValue(j * freq + randomseed, i * freq + randomseed) * _amplitude;
        _amplitude *= persistence;
        freq *= 2;
    }

    return t;
}

double PerlinNoise::GetValue(double x, double y) const
{
    int X = (int)x;
    int Y = (int)y;
    double _X = x - X;
    double _Y = y - Y;

    double n01 = Noise(X - 1, Y - 1);
    double n02 = Noise(X + 1, Y - 1);
    double n03 = Noise(X - 1, Y + 1);
    double n04 = Noise(X + 1, Y + 1);
    double n05 = Noise(X - 1, Y);
    double n06 = Noise(X + 1, Y);
    double n07 = Noise(X, Y - 1);
    double n08 = Noise(X, Y + 1);
    double n09 = Noise(X, Y);

    double n12 = Noise(X + 2, Y - 1);
    double n14 = Noise(X + 2, Y + 1);
    double n16 = Noise(X + 2, Y);

    double n23 = Noise(X - 1, Y + 2);
    double n24 = Noise(X + 1, Y + 2);
    double n28 = Noise(X, Y + 2);

    double n34 = Noise(X + 2, Y + 2);

    double x0y0 = 0.0625 * (n01 + n02 + n03 + n04) + 0.125 * (n05 + n06 + n07 + n08)
                                                                        + 0.25 * (n09);
    double x1y0 = 0.0625 * (n07 + n12 + n08 + n14) + 0.125 * (n09 + n16 + n02 + n04)
                                                                        + 0.25 * (n06);
    double x0y1 = 0.0625 * (n05 + n06 + n23 + n24) + 0.125 * (n03 + n04 + n09 + n28)
                                                                        + 0.25 * (n08);
    double x1y1 = 0.0625 * (n09 + n16 + n28 + n34) + 0.125 * (n08 + n14 + n06 + n24)
                                                                        + 0.25 * (n04);

    double v1 = Interpolate(x0y0, x1y0, _X);
    double v2 = Interpolate(x0y1, x1y1, _X);
    double fin = Interpolate(v1, v2, _Y);

    return fin;
}

double PerlinNoise::Interpolate(double x, double y, double a) const
{
    double negA = 1.0 - a;
    double negASqr = negA * negA;
    double fac1 = 3.0 * (negASqr) - 2.0 * (negASqr * negA);
    double aSqr = a * a;
    double fac2 = 3.0 * aSqr - 2.0 * (aSqr * a);

    return x * fac1 + y * fac2;
}

double PerlinNoise::Noise(int x, int y) const
{
    int n = x + y * 57;
    n = (n << 13) ^ n;
    int t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;

    return 1.0 - double(t) * 0.931322574615478515625e-9;/// 1073741824.0);
}


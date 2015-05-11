#ifndef PERLINNOISE_H
#define PERLINNOISE_H


class PerlinNoise
{

public:

    PerlinNoise(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed);

    double getHeight(double x, double y) const;
    void init(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed);

private:
    PerlinNoise();

    double Total(double i, double j) const;
    double GetValue(double x, double y) const;
    double Interpolate(double x, double y, double a) const;
    double Noise(int x, int y) const;

    double persistence, frequency, amplitude;
    int octaves, randomseed;


};
#endif // PERLINNOISE_H

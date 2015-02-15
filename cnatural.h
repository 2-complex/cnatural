
#ifndef _CNATURAL_
#define _CNATURAL_

static const double SAMPLE_RATE = 44100.0;

typedef double (*wave_function)(double);
typedef double (*param_function)(void*, double, double);

double lerp(double x, double a, double b);
double smoothstep(double x, double a, double b);
double identity(double x);
double factor(double half_steps);
double note(double freq);


typedef struct point
{
    double x;
    double y;
} point;

typedef struct piecewise_desc
{
    int n;
    point* p;
    double (*interpolation)(double, double, double);
    double (*filter)(double);
} piecewise_desc;

double piecewise(void* ptr, double multiplier, double t);

void waveform(
    double* data,
    double start, double duration,
    param_function freq_function, void* freq_params, double freq,
    param_function amp_function, void* amp_params, double amp,
    wave_function wave);

void echo(double* data, double start, double duration, double period, double factor);

double constant(void* ptr, double param, double x);
double sinwave(double x);
double sawtoothwave(double x);
double rampwave(double x);
double trianglewave(double x);
double squarewave(double x);
double noise(double x);

void print(
    short* destination,
    double** tracks,
    int numChannels,
    int numSamples);

#endif




#include "cnatural.h"

#include <math.h>
#include <stdlib.h>

double lerp(double x, double a, double b)
{
	if(x<=a)
		return 0.0;
	if(x>=b)
		return 1.0;

	return (x-a)/(b-a);
}

double smoothstep(double x, double a, double b)
{
	x = lerp(x, a, b);
	return 3*x*x - 2*x*x*x;
}

double identity(double x)
{
	return x;
}

double factor(double half_steps)
{
	return pow(1.0594630943592953, half_steps);
}

double note(double freq)
{
	return log(freq) / log(1.0594630943592953);
}

double piecewise(void* ptr, double multiplier, double t)
{
	piecewise_desc* desc = (piecewise_desc*)(ptr);

	if( t < desc->p[0].x )
		return multiplier * desc->filter(desc->p[0].y);

	int i=0;
	for(; i < desc->n-1; i++)
	{
		double a = desc->p[i].x;
		double b = desc->p[i+1].x;
		
		if( !(t < a) && t < b )
		{
			double y0 = desc->p[i].y;
			double y1 = desc->p[i+1].y;
			double v = desc->interpolation(t, a, b);
			return multiplier * desc->filter(((1.0-v)*y0 + v*y1));
		}
	}

	return multiplier * desc->filter(desc->p[i].y);
}

int min(int a, int b)
{
	if( a < b )
		return a;
	return b;
}

void waveform(
	double* data,
	double start, double duration,
	param_function freq_function, void* freq_params, double freq,
	param_function amp_function, void* amp_params, double amp,
	wave_function wave)
{
	int startIndex = start * SAMPLE_RATE;
	int durationIndex = duration * SAMPLE_RATE;

	double t = 0.0;
	double theta = 0.0;

	int i = 0;

	for( i = startIndex; i < startIndex + durationIndex; i++ )
	{
		double frequency = freq_function(freq_params, freq, t);
		theta += freq_function(freq_params, freq, t) / SAMPLE_RATE;
		theta = fmod(theta, 1.0);
		t += 1.0 / SAMPLE_RATE;

		double v = amp_function(amp_params, amp, t) * wave(theta);

		int d = min(i - startIndex, startIndex + durationIndex - 1 - i);
		if( d < 10 )
			v *= (1.0 * d / 10.0);

		data[i] += v;
	}
}

void echo(double* data, double start, double duration, double period, double factor)
{
	int startIndex = start * SAMPLE_RATE;
	int durationIndex = duration * SAMPLE_RATE;
	int gapIndex = period * SAMPLE_RATE;

	int i = 0;

	for( i = startIndex; i < startIndex + durationIndex; i++ )
	{
		data[i + gapIndex] += factor * data[i];
	}
}

double constant(void* ptr, double param, double x)
{
	return param;
}

double sinwave(double x)
{
	x = fmod(x, 1.0);
	return sin(2.0 * M_PI * x);
}

double rampwave(double x)
{
	x = fmod(x, 1.0);
	point myp[] = {{0.0, 1.0}, {0.25, 1.0}, {0.50, -1.0}, {0.75, -1.0}, {1.0,1.0}};
    piecewise_desc myform = {sizeof(myp)/sizeof(point), myp, lerp, identity};
	return piecewise(&myform, 1.0, x);
}

double sawtoothwave(double x)
{
	x = fmod(x, 1.0);
	return 2.0 * x - 1.0;
}

double trianglewave(double x)
{
	x = fmod(x, 1.0);
	return 2.0 * fabs(2.0 * x - 1.0) - 1.0;
}

double squarewave(double x)
{
	x = fmod(x, 1.0);
	if( x < 0.5 )
		return -1.0;
	return 1.0;
}

double noise(double x)
{
	return ((rand() & 0xffff) - (double)(0xefff)) / (double)(0xefff);
}

void print(
	short* destination,
	double** channels,
	int numChannels,
	int numSamples)
{
	int i = 0;
	int channel = 0;
	int sample = 0;

	for( sample = 0; sample < numSamples; sample++ )
	{
		for( channel = 0; channel < numChannels; channel++ )
		{
			destination[i] += 32767.0 * channels[channel][sample];
			i++;		
		}
	}
}




#include "cnatural.h"
#include "wav.h"

#include "stdio.h"

#include <stdlib.h>
#include <math.h>


typedef double (*wave_function)(double);
typedef double (*param_function)(void*, double, double);

void tone(
	double* data,
	double start,
	double duration,
	double freq,
	double amp)
{
	waveform(
		data, start, duration,
		constant, NULL, freq,
		constant, NULL, amp,
		sinwave);
}

void kick(
	double* data,
	double start,
	double duration,
	double freq,
	double amp)
{
	point uppity[] = {
		{0.0, 0.0},
		{0.01, 1.0},
		{1.0, 0.0}};

	point ramp_pts[] = {
		{0.0, 0.0},
		{1.0, -1.0}};

	piecewise_desc amp_desc = {sizeof(uppity)/sizeof(point), uppity, lerp, identity};
	piecewise_desc my_ramp = {sizeof(ramp_pts)/sizeof(point), ramp_pts, lerp, factor};  

	waveform(
		data, start, duration,
		piecewise, &my_ramp, freq,
		piecewise, &amp_desc, amp,
		sinwave);
}

void snare(
	double* data,
	double start,
	double duration,
	double freq,
	double amp)
{
	point myp[] = {
		{0.0, 0.0},
		{0.25, 1.0},
		{1.0, 0.0}};
	piecewise_desc mypieces = {sizeof(myp)/sizeof(point), myp, smoothstep, identity};

	wavefor	m(
		data, start, duration,
		constant, NULL, freq,
		piecewise, &mypieces, amp,
		noise);
}


int main()
{
	point myp[] = {{0.0, 2.0}, {0.25, 4.0}, {1.0, 3.0}};
	piecewise_desc myramp = {sizeof(myp)/sizeof(point), myp, lerp, identity};

	int n = 44100 * 60;
	int numSamples = n;
	double* data = (double*)malloc(n * sizeof(double));
	short* pritedData  = (short*)malloc(n * sizeof(short));
	int c = 1;

	int e[] = {0, 2, 4, 7, 9, 12, 14, 16, 19, 21};
	int index = 0;
	
	int p = 0;
	for( int j = 0; j < 20; j++ )
	{
		for( int i = 0; i < 4; i++ )
		{
			int newindex = index;
			while(newindex == index)
				newindex = ((rand() & 0xffff) % (sizeof(e)/sizeof(int)));
			index = newindex;
			double m = factor(e[index]);
			tone(data, 0.25*(p++), 0.25, 220.0 * m, 0.5);
		}
		p += 4;
	}
        
	
	echo(data, 0, 100, 1.0, 0.2);

	p = 0;
	for( int j = 0; j < 80; j++ )
	{
		snare(data, p*0.25, 0.25, 220.0, 0.25);
		p++;
		
		if (j%4 == 0) 
			kick(data, p*0.25, 0.25, 220.0, 0.25);
		p++;
	}

	
	FILE* f = fopen("out.wav", "w");	
	
	WavHeader wh = makeWavHeader();
	fwrite(&wh, sizeof(WavHeader), 1, f);
	
	double* tracks[] = {data};

	print(pritedData, tracks, 1, numSamples);
	
	fwrite(pritedData, n*2, 1, f);
	
	fclose(f);	

	return 0;
}



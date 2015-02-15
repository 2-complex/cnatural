
#include "cnatural.h"
#include "wav.h"

#include "stdio.h"

#include <stdlib.h>
#include <math.h>


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
        rampwave);
}


void kick(
    double* data,
    double start,
    double duration,
    double freq,
    double amp)
{
    point uppity[] =
    {
        {0.0, 0.0},
        {0.01, 1.0},
        {1.0, 0.0}
    };

    point ramp_pts[] =
    {
        {0.0, 0.0},
        {1.0, -80.0}
    };

    piecewise_desc amp_desc = {sizeof(uppity) / sizeof(point), uppity, lerp, identity};
    piecewise_desc my_ramp = {sizeof(ramp_pts) / sizeof(point), ramp_pts, lerp, factor};

    waveform(
        data, start, duration,
        piecewise, &my_ramp, freq,
        piecewise, &amp_desc, amp,
        squarewave);
}


void snare(
    double* data,
    double start,
    double duration,
    double freq,
    double amp)
{
    point myp[] = {
        {0.0, 1.0},
        {0.25, 0.2},
        {1.0, 0.0}};
    piecewise_desc mypieces = {sizeof(myp)/sizeof(point), myp, smoothstep, identity};

    waveform(
        data, start, duration,
        constant, NULL, freq,
        piecewise, &mypieces, amp,
        noise);
}


int main()
{
    point myp[] = {{0.0, 2.0}, {0.25, 4.0}, {1.0, 3.0}};
    piecewise_desc myramp = {sizeof(myp)/sizeof(point), myp, lerp, identity};

    int n = 44100 * 200;

    int numSamples = n;
    double* data = (double*)malloc(n * sizeof(double));
    short* pritedData  = (short*)malloc(n * sizeof(short));
    int c = 1;

    int e[] = {0, 2, 4, 7, 9, 12, 14, 16, 19, 21};
    int index = 0;

    int i = 0;
    int j = 0;
    int p = 0;


    for( j = 0; j < 20; j++ )
    {
        for( i = 0; i < 4; i++ )
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

    p = 0;
    for( j = 0; j < 80; j++ )
    {
        if (j%2 == 1)
            snare(data, p*0.25, 0.25, 220.0, 0.1);
        p++;

        if (j%2 == 0)
            kick(data, p*0.25, 0.25, 220.0, 0.25);
        p++;
    }

    echo(data, 0, 100, 1.0, 0.2);


    FILE* f = fopen("out.wav", "w");

    WavHeader wh = makeWavHeader(n, 1, 44100, 2);
    fwrite(&wh, sizeof(WavHeader), 1, f);

    double* tracks[] = {data};

    print(pritedData, tracks, 1, numSamples);
    fwrite(pritedData, n*2, 1, f);

    fclose(f);

    return 0;
}



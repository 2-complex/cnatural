
#include "wav.h"

WavHeader makeWavHeader(int numSamples, int numChannels, int sampleRate, int bytesPerChannel)
{
	WavHeader result;

	int n = numSamples;
	int c = numChannels;
	int s = sampleRate;
	int b = bytesPerChannel;

	result.fh = {{'R','I','F','F'}, 36 + n*c*b};
	result.wh = {{'W','A','V','E'}, {'f','m','t',' '}, 8*b, 1, 1, b, c*2*b, 2, 8*b, {'d','a','t','a'}, n*c*2 };
	return result;
}



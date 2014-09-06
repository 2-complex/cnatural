
#include "wav.h"

WavHeader makeWavHeader(
    int numSamples,
    int numChannels,
    int sampleRate,
    int bytesPerChannel)
{
    int n = numSamples;
    int c = numChannels;
    int s = sampleRate;
    int b = bytesPerChannel;

    WavHeader result =
    {
        {
            {'R','I','F','F'},
            36 + n * c * b // riff box size
        },
        {
            {'W','A','V','E'},
            {'f','m','t',' '},
            16, // Size of PCM header
            1,  // Indicates PCM
            c,  // Number of channels
            s,  // sample-rate
            s * c * b,  // byte-rate
            c * b, // This is called block-align
            8 * b, // bits-per-sample
            {'d','a','t','a'},
            n * c * b // data-size
        }
    };

    return result;
}



#ifndef _WAV_
#define _WAV_

typedef struct RiffHeader
{
    char riffTag[4]; /* "RIFF" */
    int chunkSize;
} RiffHeader;

typedef struct WavBoxHeader {
    char waveTag[4];   /* "WAVE" */
    char fmtTag[4];    /* "fmt " */
    int Subchunk1Size; /* 16 for PCM. */
    short audioFormat; /* PCM = 1*/
    short numChannels; /* Mono = 1, Stereo = 2, etc. */
    int sampleRate;    /*8000, 44100, etc.*/
    int byteRate;      /* == sampleRate * numChannels * bitsPerSample/8*/
    short blockAlign;  /* == numChannels * bitsPerSample / 8 */
    short bitsPerSample;  /* 8 bits = 8, 16 bits = 16, etc. */
    char dataTag[4];  /* "data" */
    int dataSize;
} WavBoxHeader;

typedef struct WavHeader {
    RiffHeader rh;
    WavBoxHeader wh;
} 

#endif



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#include "ReverbEffect.h"

int main()
{
    int sampleCount = 0;
    int frequency = 44100;


    printf("Loading raw samples (only 16-bit, %d mono supported in this program)\n" ,frequency);
    FILE *fp = fopen("test.raw","rb");
    if( fp == NULL )
    {
        perror("Error while opening the file...\n");
        exit(EXIT_FAILURE);
    }

    // get file size
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);

    // seek back to beginning
    fseek(fp, 0L, SEEK_SET);

    // calculate number of samples (assune 16-bit, mono, so, 2 bytes per sample)

    sampleCount = size / 2;
    int16_t *samples = new int16_t[sampleCount];
    fread(samples, 1, size, fp);
    fclose(fp);

    printf("Converting now samples...\n");
    float *floatSamplesIn =  new float[sampleCount];

    // now convert samples into floats
    int i;
    for (i=0; i<sampleCount; i++)
    {
        floatSamplesIn[i] = (float)samples[i] / 32767.0f;
    }
    delete[] samples;
    printf("Found %d samples\n", sampleCount);


    printf("Initializing effect...\n");
    ReverbEffect effect(frequency);


    printf("Applying enviroment reverb effect...\n");

    fp = fopen("output.raw","wb");
    if( fp == NULL )
    {
        perror("Error while creating the output file...\n");
        exit(EXIT_FAILURE);
    }

//castle
    effect.LoadPreset(26.0f, 8.3f, 0.890f, -1000.0f, -800.0f, -2000.0f, 1.22f, 0.83f, 0.31f, -100.0f, 0.022f, 0.0f, 0.0f, 0.0f, 600.0f, 0.011f, 0.0f, 0.0f, 0.0f, 0.138f, 0.080f, 0.250f, 0.0f, -5.0f, 5168.6f, 139.5f, 0.0f, 0x20);

//stadium
    //effect.LoadPreset(26, 7.2f, 1.0f, -1000, -700, -200, 6.26f, 0.51f, 1.10f, -2400, 0.183f, 0.0f, 0.0f, 0.0f, -800, 0.038f, 0.0f, 0.0f, 0.0f, 0.250f, 0.0f, 0.250f, 0.0f, -5.0f, 5000.0f, 250.0f, 0.0f, 0x20);

// heaven
   //effect.LoadPreset(26, 19.6f, 0.940f, -1000, -200, -700, 5.04f, 1.12f, 0.56f, -1230, 0.020f, 0.0f, 0.0f, 0.0f, 200, 0.029f, 0.0f, 0.0f, 0.0f, 0.250f, 0.080f, 2.742f, 0.050f, -2.0f, 5000.0f, 250.0f, 0.0f, 0x3f);

//sewer
    //effect.LoadPreset(21, 1.7f, 0.800f, -1000, -1000, 0, 2.81f, 0.14f, 1.0f, 429, 0.014f, 0.0f, 0.0f, 0.0f, 1023, 0.021f, 0.0f, 0.0f, 0.0f, 0.250f, 0.0f, 0.250f, 0.0f, -5.0f, 5000.0f, 250.0f, 0.0f, 0x3f);

//psychotic
    //effect.LoadPreset(25, 1.0f, 0.500f, -1000, -151, 0, 7.56f, 0.91f, 1.0f, -626, 0.020f, 0.0f, 0.0f, 0.0f, 774, 0.030f, 0.0f, 0.0f, 0.0f, 0.250f, 0.0f, 4.0f, 1.0f, -5.0f, 5000.0f, 250.0f, 0.0f, 0x1f);

    effect.Update(frequency);

    int offset = 0;
    float floatSamplesOut[REVERB_BUFFERSIZE * OUTPUT_CHANNELS];

    do {

        int workSamples = REVERB_BUFFERSIZE / 4;
        if (workSamples>sampleCount)
        {
            workSamples = sampleCount;
        }

        effect.Process(workSamples, &floatSamplesIn[offset],  floatSamplesOut);

        for (i=0; i<workSamples; i++)
        {
            int16_t outSample = (int16_t) (floatSamplesOut[i*2 + 0] * 32767.0f);
            fwrite(&outSample, 1, 2, fp);

            outSample = (int16_t) (floatSamplesOut[i*2 + 1] * 32767.0f);
            fwrite(&outSample, 1, 2, fp);
        }

        sampleCount -= workSamples;
        offset += workSamples;
    } while (sampleCount>0);

    delete[] floatSamplesIn;


    fclose(fp);

    printf("Done!\n");

    return 0;
}

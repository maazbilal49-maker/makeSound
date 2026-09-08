#include <stdio.h>
#include <stdlib.h>  
#include <math.h>
#include <SDL2/SDL.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define PI 3.14159265358979323846

#include <stdint.h>

void saveWav(const char *filename, float *samples, int sampleCount)
{
    FILE *file = fopen(filename, "wb");

    if (!file) {
        printf("Could not open file\n");
        return;
    }

    uint16_t audioFormat = 3; // IEEE float
    uint16_t channels = 1;
    uint32_t sampleRate = 44100;
    uint16_t bitsPerSample = 32;

    uint16_t blockAlign = channels * (bitsPerSample / 8);
    uint32_t byteRate = sampleRate * blockAlign;

    uint32_t dataSize = sampleCount * sizeof(float);
    uint32_t chunkSize = 36 + dataSize;

    // RIFF header
    fwrite("RIFF", 1, 4, file);
    fwrite(&chunkSize, 4, 1, file);
    fwrite("WAVE", 1, 4, file);

    // fmt chunk
    uint32_t fmtSize = 16;

    fwrite("fmt ", 1, 4, file);
    fwrite(&fmtSize, 4, 1, file);
    fwrite(&audioFormat, 2, 1, file);
    fwrite(&channels, 2, 1, file);
    fwrite(&sampleRate, 4, 1, file);
    fwrite(&byteRate, 4, 1, file);
    fwrite(&blockAlign, 2, 1, file);
    fwrite(&bitsPerSample, 2, 1, file);

    // data chunk
    fwrite("data", 1, 4, file);
    fwrite(&dataSize, 4, 1, file);
    fwrite(samples, sizeof(float), sampleCount, file);

    fclose(file);
}

unsigned char* getImageData(const char* filename){
    int width, height, channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    if(!data){
        printf("Failed to load image: %s\n", filename);
        return NULL;
    }
    return data;
}

int* getDimensions(const char* filename){
    int width, height, channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    if(!data){
        printf("Failed to load image: %s\n", filename);
        return NULL;
    }
    int *dimensions = (int*)malloc(2 * sizeof(int));
    dimensions[0] = width;
    dimensions[1] = height;
    stbi_image_free(data);
    return dimensions;
}

void turnDataToSound(unsigned char* imageData, int* dimensions, int playSound){

    int width = dimensions[0];
    int height = dimensions[1];

    int sampleCount = width * height;
    float* samples = (float*)malloc(sampleCount * sizeof(float));

    float sampleRate = 44100.0f;
    float frequency = 440.0f;

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            unsigned char pixelValue = imageData[(y * width + x) * 3];
            
            float t = (float)(y * width + x) / sampleRate;
            float sample = sinf(2.0f * PI * frequency * t * (pixelValue / 255.0f));
            samples[y * width + x] = sample;
        }
    }

    saveWav("output.wav", samples, sampleCount);

    if(playSound){
        SDL_AudioSpec spec;
        SDL_zero(spec);
        spec.freq = 44100;
        spec.format = AUDIO_F32SYS;
        spec.channels = 1;
        spec.samples = 4096;

        SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
        if(deviceId == 0){
            printf("Failed to open audio device: %s\n", SDL_GetError());
            free(samples);
            return;
        }

        SDL_QueueAudio(deviceId, samples, sampleCount *sizeof(float));
        SDL_PauseAudioDevice(deviceId, 0);

        while(SDL_GetQueuedAudioSize(deviceId) > 0){
            SDL_Delay(100);
        }

        SDL_CloseAudioDevice(deviceId);
    }

    free(samples);
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    int playSound;

    if(argc >= 3 && strcmp(argv[2], "--noplay") == 0){
        playSound = 0;
    }else{
        playSound = 1;
    }
    unsigned char *imageData = getImageData(argv[1]);
    int *dimensions = getDimensions(argv[1]);
    if(!imageData){
        printf("Error loading image data.\n");
        return 1;
    }

    SDL_Init(SDL_INIT_AUDIO);
    turnDataToSound(imageData, dimensions, playSound);

    stbi_image_free(imageData);
    free(dimensions);
    return 0;
}
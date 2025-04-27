#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "reciter.h"
#include "sam.h"
#include "debug.h"

#ifdef USESDL
#include <SDL.h>
#include <SDL_audio.h>
#endif

void WriteWav(char* filename, char* buffer, int bufferlength)
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL) return;
    //RIFF header
    fwrite("RIFF", 4, 1,file);
    unsigned int filesize=bufferlength + 12 + 16 + 8 - 8;
    fwrite(&filesize, 4, 1, file);
    fwrite("WAVE", 4, 1, file);

    //format chunk
    fwrite("fmt ", 4, 1, file);
    unsigned int fmtlength = 16;
    fwrite(&fmtlength, 4, 1, file);
    unsigned short int format=1; //PCM
    fwrite(&format, 2, 1, file);
    unsigned short int channels=1;
    fwrite(&channels, 2, 1, file);
    unsigned int samplerate = 22050;
    fwrite(&samplerate, 4, 1, file);
    fwrite(&samplerate, 4, 1, file); // bytes/second
    unsigned short int blockalign = 1;
    fwrite(&blockalign, 2, 1, file);
    unsigned short int bitspersample=8;
    fwrite(&bitspersample, 2, 1, file);

    //data chunk
    fwrite("data", 4, 1, file);
    fwrite(&bufferlength, 4, 1, file);
    fwrite(buffer, bufferlength, 1, file);

    fclose(file);
}

// usage: sam [options] Word1 Word2 ....
// options
//     -phonetic         enters phonetic mode. (see below)
//     -pitch number        set pitch value (default=64)
//     -speed number        set speed value (default=72)
//     -throat number        set throat value (default=128)
//     -mouth number        set mouth value (default=128)
//     -wav filename        output to wav instead of libsdl
//     -sing            special treatment of pitch
//     -debug            print additional debug messages
//      VOWELS                            VOICED CONSONANTS    
// IY           f(ee)t                    R        red        
// IH           p(i)n                     L        allow        
// EH           beg                       W        away        
// AE           Sam                       W        whale        
// AA           pot                       Y        you        
// AH           b(u)dget                  M        Sam        
// AO           t(al)k                    N        man        
// OH           cone                      NX       so(ng)        
// UH           book                      B        bad        
// UX           l(oo)t                    D        dog        
// ER           bird                      G        again        
// AX           gall(o)n                  J        judge        
// IX           dig(i)t                   Z        zoo        
//                        ZH       plea(s)ure    
//    DIPHTHONGS                          V        seven        
// EY           m(a)de                    DH       (th)en        
// AY           h(igh)                        
// OY           boy                        
// AW           h(ow)                     UNVOICED CONSONANTS    
// OW           slow                      S         Sam        
// UW           crew                      Sh        fish        
//                                        F         fish        
//                                        TH        thin        
//  SPECIAL PHONEMES                      P         poke        
// UL           sett(le) (=AXL)           T         talk        
// UM           astron(omy) (=AXM)        K         cake        
// UN           functi(on) (=AXN)         CH        speech        
// Q            kitt-en (glottal stop)    /H        a(h)ead    

int debug = 0;

int main(int argc, char **argv)
{
    int i;
    int phonetic = 0;

    char* wavfilename = NULL;
    char input[256];

    for(i=0; i<256; i++) input[i] = 0;

    if (argc <= 1)
    {
        PrintUsage();
        return 1;
    }

    i = 1;
    while(i < argc)
    {
        if (argv[i][0] != '-')
        {
            strncat(input, argv[i], 255);
            strncat(input, " ", 255);
        } else
        {
            if (strcmp(&argv[i][1], "wav")==0)
            {
                wavfilename = argv[i+1];
                i++;
            } else
            if (strcmp(&argv[i][1], "sing")==0)
            {
                EnableSingmode();
            } else
            if (strcmp(&argv[i][1], "phonetic")==0)
            {
                phonetic = 1;
            } else
            if (strcmp(&argv[i][1], "debug")==0)
            {
                debug = 1;
            } else
            if (strcmp(&argv[i][1], "pitch")==0)
            {
                SetPitch(atoi(argv[i+1]));
                i++;
            } else
            if (strcmp(&argv[i][1], "speed")==0)
            {
                SetSpeed(atoi(argv[i+1]));
                i++;
            } else
            if (strcmp(&argv[i][1], "mouth")==0)
            {
                SetMouth(atoi(argv[i+1]));
                i++;
            } else
            if (strcmp(&argv[i][1], "throat")==0)
            {
                SetThroat(atoi(argv[i+1]));
                i++;
            } else
            {
                PrintUsage();
                return 1;
            }
        }

        i++;
    } //while

    for(i=0; input[i] != 0; i++)
        input[i] = toupper((int)input[i]);

    if (debug)
    {
        if (phonetic) printf("phonetic input: %s\n", input);
        else printf("text input: %s\n", input);
    }

    if (!phonetic)
    {
        strncat(input, "[", 256);
        if (!TextToPhonemes((unsigned char *)input)) return 1;
        if (debug)
            printf("phonetic input: %s\n", input);
    } else strncat(input, "\x9b", 256);

#ifdef USESDL
    if ( SDL_Init(SDL_INIT_AUDIO) < 0 )
    {
        printf("Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);
#endif

    SetInput(input);
    if (!SAMMain())
    {
        PrintUsage();
        return 1;
    }

    if (wavfilename != NULL)
        WriteWav(wavfilename, GetBuffer(), GetBufferLength()/50);
    else
        OutputSound();


    return 0;

}

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include "sam.h"

void WriteWav(unsigned char* buffer, int bufferlength)
{
    FILE* file = stdout;
    if (file == NULL) return;
    _setmode(_fileno(stdout), _O_BINARY);

    // RIFF header
    fwrite("RIFF", 4, 1, file);
    unsigned int filesize = bufferlength + 12 + 16 + 8 - 8;
    fwrite(&filesize, 4, 1, file);
    fwrite("WAVE", 4, 1, file);

    // format chunk
    fwrite("fmt ", 4, 1, file);
    unsigned int fmtlength = 16;
    fwrite(&fmtlength, 4, 1, file);
    unsigned short int format = 1; // PCM
    fwrite(&format, 2, 1, file);
    unsigned short int channels = 1;
    fwrite(&channels, 2, 1, file);
    unsigned int samplerate = 22050;
    fwrite(&samplerate, 4, 1, file);
    fwrite(&samplerate, 4, 1, file); // bytes/second
    unsigned short int blockalign = 1;
    fwrite(&blockalign, 2, 1, file);
    unsigned short int bitspersample = 8;
    fwrite(&bitspersample, 2, 1, file);

    // data chunk
    fwrite("data", 4, 1, file);
    fwrite(&bufferlength, 4, 1, file);
    fwrite(buffer, bufferlength, 1, file);
}

void PrintUsage(void)
{
    fprintf(stderr,
        "usage: sam [options] Word1 Word2 .... >output.wav\n"
        "options\n"
        "    -phonetic         enters phonetic mode. (see below)\n"
        "    -pitch number     set pitch value (default=64)\n"
        "    -speed number     set speed value (default=72)\n"
        "    -throat number    set throat value (default=128)\n"
        "    -mouth number     set mouth value (default=128)\n"
        "    -test             ignore input and emit test sequence\n"
        "\n"
        "     VOWELS                            VOICED CONSONANTS\n"
        "IY           f(ee)t                    R        red\n"
        "IH           p(i)n                     L        allow\n"
        "EH           beg                       W        away\n"
        "AE           Sam                       W        whale\n"
        "AA           pot                       Y        you\n"
        "AH           b(u)dget                  M        Sam\n"
        "AO           t(al)k                    N        man\n"
        "OH           cone                      NX       so(ng)\n"
        "UH           book                      B        bad\n"
        "UX           l(oo)t                    D        dog\n"
        "ER           bird                      G        again\n"
        "AX           gall(o)n                  J        judge\n"
        "IX           dig(i)t                   Z        zoo\n"
        "                       ZH       plea(s)ure\n"
        "   DIPHTHONGS                          V        seven\n"
        "EY           m(a)de                    DH       (th)en\n"
        "AY           h(igh)\n"
        "OY           boy\n"
        "AW           h(ow)                     UNVOICED CONSONANTS\n"
        "OW           slow                      S         Sam\n"
        "UW           crew                      Sh        fish\n"
        "                                       F         fish\n"
        "                                       TH        thin\n"
        " SPECIAL PHONEMES                      P         poke\n"
        "UL           sett(le) (=AXL)           T         talk\n"
        "UM           astron(omy) (=AXM)        K         cake\n"
        "UN           functi(on) (=AXN)         CH        speech\n"
        "Q            kitt-en (glottal stop)    /H        a(h)ead\n");
}

static unsigned char buffer[22 * 11050];
static unsigned char* buffer_ptr = buffer;

void outputSample(unsigned char sample)
{
    *buffer_ptr++ = sample;
}

int main(int argc, char** argv)
{
    int i;
    int phonetic = 0;
    int speed = 72;
    int pitch = 64;
    int mouth = 128;
    int throat = 128;

    char input[256];

    for (i = 0; i < 256; i++)
        input[i] = 0;

    if (argc <= 1) {
        PrintUsage();
        return 1;
    }

    i = 1;
    while (i < argc) {
        if (argv[i][0] != '-') {
            strncat(input, argv[i], 255);
            strncat(input, " ", 255);
        } else {
            if (strcmp(&argv[i][1], "phonetic") == 0) {
                phonetic = 1;
            } else if (strcmp(&argv[i][1], "pitch") == 0) {
                pitch = atoi(argv[i + 1]);
                i++;
            } else if (strcmp(&argv[i][1], "speed") == 0) {
                speed = atoi(argv[i + 1]);
                i++;
            } else if (strcmp(&argv[i][1], "mouth") == 0) {
                mouth = atoi(argv[i + 1]);
                i++;
            } else if (strcmp(&argv[i][1], "throat") == 0) {
                throat = atoi(argv[i + 1]);
                i++;
            } else {
                PrintUsage();
                return 1;
            }
        }

        i++;
    } // while

    for (i = 0; input[i] != 0; i++)
        input[i] = toupper((int)input[i]);

    if (phonetic) {
        fprintf(stderr, "phonetic input: %s\n", input);
        strncat(input, "\x9b", sizeof(input) - 1);
    } else {
        fprintf(stderr, "text input: %s\n", input);
        strncat(input, "[", sizeof(input) - 1);
        if (!TextToPhonemes((unsigned char*)input)) { // overwrites input!
            fprintf(stderr, "TextToPhonemes failed\n");
            return 1;
        }
        char* eos = memchr(input, 0x9b, sizeof(input));
        fprintf(stderr, "phonetic output: %*s\n", (int)(eos - input), input);
    }

    SetSAMInputFull((unsigned char*)input, speed, pitch, mouth, throat);

    if (!SAMMain(outputSample)) {
        fprintf(stderr, "SAMMain returned fail\n");
        PrintUsage();
        return 1;
    }

    WriteWav(buffer, buffer_ptr - buffer);
    return 0;
}

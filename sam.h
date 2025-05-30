#pragma once

typedef void (*output_sample_fn)(unsigned char ch);

int TextToPhonemes(unsigned char* input);
void SetSAMInputFull(unsigned char* _input, unsigned char _speed, unsigned char _pitch, unsigned char _mouth, unsigned char _throat);
#define SetSAMInput(_input) SetSAMInputFull(_input, 72, 64, 128, 128)
int SAMMain(output_sample_fn outputSample);

// char input[]={"/HAALAOAO MAYN NAAMAEAE IHSTT SAEBAASTTIHAAN \x9b\x9b\0"};
// unsigned char input[]={"/HAALAOAO \x9b\0"};
// unsigned char input[]={"AA \x9b\0"};
// unsigned char input[] = {"GUH5DEHN TAEG\x9b\0"};

// unsigned char input[]={"AY5 AEM EY TAO4LXKIHNX KAX4MPYUX4TAH. GOW4 AH/HEH3D PAHNK.MEYK MAY8 DEY.\x9b\0"};
// unsigned char input[]={"/HEH3LOW2, /HAW AH YUX2 TUXDEY. AY /HOH3P YUX AH FIYLIHNX OW4 KEY.\x9b\0"};
// unsigned char input[]={"/HEY2, DHIHS IH3Z GREY2T. /HAH /HAH /HAH.AYL BIY5 BAEK.\x9b\0"};
// unsigned char input[]={"/HAH /HAH /HAH \x9b\0"};
// unsigned char input[]={"/HAH /HAH /HAH.\x9b\0"};
// unsigned char input[]={".TUW BIY5Y3,, OHR NAA3T - TUW BIY5IYIY., DHAE4T IHZ DHAH KWEH4SCHAHN.\x9b\0"};
// unsigned char input[]={"/HEY2, DHIHS \x9b\0"};

// unsigned char input[]={" IYIHEHAEAAAHAOOHUHUXERAXIX  \x9b\0"};
// unsigned char input[]={" RLWWYMNNXBDGJZZHVDH \x9b\0"};
// unsigned char input[]={" SSHFTHPTKCH/H \x9b\0"};

// unsigned char input[]={" EYAYOYAWOWUW ULUMUNQ YXWXRXLX/XDX\x9b\0"};

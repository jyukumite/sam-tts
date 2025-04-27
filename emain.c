#include <stdio.h>
#include "sam.h"
#include <stdint.h>

extern uint32_t _sidata; /* Start address for the initialization values of the .data section (in flash) */
extern uint32_t _sdata; /* Start address for the .data section (in RAM) */
extern uint32_t _edata; /* End address for the .data section (in RAM) */
extern uint32_t _sbss; /* Start address for the .bss section (in RAM) */
extern uint32_t _ebss; /* End address for the .bss section (in RAM) */

extern int main(void);

void Default_Handler(void);
void Reset_Handler(void);
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

extern uint32_t _estack;

__attribute__((section(".isr_vector")))
const void* vector_table[]
    = {
          &_estack, /* Initial Stack Pointer */
          Reset_Handler, /* Reset Handler */
          NMI_Handler, /* NMI Handler */
          HardFault_Handler, /* Hard Fault Handler */
          0, 0, 0, 0, 0, 0, 0, /* Reserved */
          SVC_Handler, /* SVCall Handler */
          0, 0, /* Reserved */
          PendSV_Handler, /* PendSV Handler */
          SysTick_Handler, /* SysTick Handler */
      };

void Reset_Handler(void)
{
    uint32_t *src, *dst;

    src = &_sidata;
    dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    main();
    while (1)
        ;
}

void Default_Handler(void)
{
    while (1)
        ;
}

static void outputSample(unsigned char sample)
{
    (*(unsigned char*)0x150000) = sample;
}

int main(void)
{
    char input[256] = "Hello world, I'm talking to you.";
    // char input[256] = "/HEHLOW WERLD, AY5M TAO4KIHNX TUX YUW.";

    if (TextToPhonemes((unsigned char*)input)) { // overwrites input
        SetSAMInput((unsigned char*)input);
        if (!SAMMain(outputSample)) {
            // printf("SAMMain returned fail\n");
        }
    }

    return 0;
}

// ~12981 bytes of thumb with TextToPhonemes
// ~7074 bytes without TextToPhonemes (pre-computed phoneme strings)

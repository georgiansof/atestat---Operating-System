#include "types.h"

void printf(char*);

extern "C" void kernelMain(void* multiboot_structure, unsigned int magicnumber)
{
    printf("Hello World!"); // un mesaj de proba
    while(true);
}

void printf(char* str) /// functioneaza pe principiul unei masini de scris clasice (typewriter)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000; // pointer catre zona de memorie video.
                                                      // acesta se afla in zona de memorie 0xb8000,
                                                     // o constanta impusa, standard pentru procesoarele i386.
                                                    // Majoritatea sunt orice procesoare de PC de pe piata care se vand 
                                                   // si separat, producatorii fiind AMD si intel
    static uint8_t x=0, y=0;
    
    for(int i=0;str[i] != '\0'; ++i) // parcurgem sirul ce trebuie afisat
    {
        
        switch(str[i]) // verificam daca este sfarsit de linie
        {
            case '\n':
                ++y,x=0;
                break;
            default: // altfel afisam
            VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) | str[i]; // avem grija sa modificam doar ultimii 8 biti 
                                                                          // ( de text ), pentru ca primii 8 sunt pentru 
                                                                         // culoarea textului si a fontului
            ++x;
        }
        
        if(x >= 80) // daca am ajuns la capatul ecranului trecem pe linia urmatoare
        {
            ++y;
            x=0;
        }
        
        if(y >= 25) // daca s-a terminat ecranul il stergem si o luam de la capat
        {
            for(y=0;y<25;++y)
                for(x=0;x<80;++x)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
                    
            x=0;
            y=0;
        }
    }
    
}

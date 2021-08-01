
#include "keyboard.h"


KeyboardDriver::KeyboardDriver(InterruptManager* manager) 
:InterruptHandler(0x21, manager),
dataport(0x60),
commandport(0x64)
{
    while(commandport.Read() & 0x1)
        dataport.Read();
    commandport.Write(0xAE); 
    commandport.Write(0x20); 
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60); 
    dataport.Write(status);
    
    dataport.Write(0xF4);
}

KeyboardDriver::~KeyboardDriver()
{
}

void printf(char*);

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t key = dataport.Read();
    static bool SHIFT=0,RSHIFT=0,LSHIFT=0,CAPSLOCK=0;
    static bool CTRL=0;
    static bool LALT=0,RALT=0;
    static bool escape=0;
    static bool functionkey=0;
    switch(key)
    {
        /// escape character
        case 0xE0: escape=1; return esp;
        
        /// Keyboard Special Codes
        case 0xFA:
        case 0x45:
        case 0xC5:
            break;

        case 0x2A: if(!escape) {LSHIFT=1; SHIFT=1; printf(" LSHIFT ");} else functionkey=1; break;
        case 0xAA: if(!escape) {LSHIFT=0; if(RSHIFT==0) SHIFT=0;} else functionkey=0; break;
        case 0x1D: CTRL=1; printf(" CTRL ");break;
        case 0x9D: CTRL=0; break;
        case 0x38: if(escape) {printf(" RALT ");RALT=1;} else {LALT=1; printf(" LALT ");} break;
        case 0xB8: if(escape) RALT=0; else LALT=0; break;
        case 0x36: printf(" RSHIFT ");RSHIFT=1; SHIFT=1; break;
        case 0xB6: RSHIFT=0; if(LSHIFT==0) SHIFT=0; break;
        case 0x3A: CAPSLOCK=!CAPSLOCK; if(CAPSLOCK) printf(" CAPSLOCK "); else printf(" capslock "); break;
        case 0xBA: break; // CAPS RELEASE
        
        /// release
        
        case 0xC6: break; // scroll lock
        case 0x81: break; // esc
        case 0xBB: break; // f1
        case 0xBC: break; // f2
        case 0xBD: break; // f3
        case 0xBE: break; // f4
        case 0xBF: break; // f5
        case 0xC0: break; // f6
        case 0xC1: break; // f7
        case 0xC2: break; // f8
        case 0xC3: break; // f9
        case 0xC4: break; // f10
        case 0xD7: break; // f11
        case 0xD8: break; // f12
        case 0x8F: break; // TAB
        case 0xA9: break; // ~
        case 0x82: break; // 1
        case 0x83: break; // 2
        case 0x84: break; // ...
        case 0x85: break;
        case 0x86: break;
        case 0x87: break;
        case 0x88: break;
        case 0x89: break;
        case 0x8A: break; // 9
        case 0x8B: break; // 0
        case 0x8C: break; // +
        case 0x8D: break; // =
        case 0x8E: break; // BKSPC
        case 0xB7: break; // Keypad *
        case 0xCA: break; // keypad -
        case 0xCE: break; // keypad +
        case 0x9C: break; // enter
        case 0xD3: break; // keypad del
        case 0xD2: if(functionkey) ; else ; break; // Keypad 0
        case 0xCF: if(functionkey) ; else ; break; // keypad 1
        case 0xD0: break; // keypad 2
        case 0xD1: if(functionkey) ; else ; break; // keypad 3
        case 0xCB: break; // keypad 4
        case 0xCC: break; // keypad 5
        case 0xCD: break; // keypad 6
        case 0xC7: if(functionkey) ; else ; break; // keypad 7
        case 0xC8: break; // keypad 8
        case 0xC9: if(functionkey) ; else ; break; // keypad 9
        case 0xB9: break; // SPACE
        case 0xDB: escape=0; break; // winkey
        case 0x90: break; // q
        case 0x91: break; // w
        case 0x92: break; // e
        case 0x93: break; // r
        case 0x94: break; // t
        case 0x95: break; // y
        case 0x96: break; // u
        case 0x97: break; // i
        case 0x98: break; // o
        case 0x99: break; // p
        case 0x9A: break; // [
        case 0x9B: break; // ]
        case 0x9E: break; // A
        case 0x9F: break; // S
        case 0xA0: break; // D
        case 0xA1: break; // F
        case 0xA2: break; // G
        case 0xA3: break; // H
        case 0xA4: break; // J
        case 0xA5: break; // K
        case 0xA6: break; // L
        case 0xA7: break; // ;
        case 0xA8: break; // '
        case 0xAB: break; // |
        case 0xAC: break; // Z
        case 0xAD: break; // X
        case 0xAE: break; // C
        case 0xAF: break; // V
        case 0xB0: break; // B
        case 0xB1: break; // N
        case 0xB2: break; // M
        case 0xB3: break; // ,
        case 0xB4: break; // .
        case 0xB5: break; // ?
        case 0xD6: break; // <>
        /// press
        case 0x46: printf(" SCROLL LOCK "); break;
        case 0x10: if(CAPSLOCK ^ SHIFT) printf("Q"); else printf("q"); break;
        case 0x11: if(CAPSLOCK ^ SHIFT) printf("W"); else printf("w"); break;
        case 0x12: if(CAPSLOCK ^ SHIFT) printf("E"); else printf("e"); break;
        case 0x13: if(CAPSLOCK ^ SHIFT) printf("R"); else printf("r"); break;
        case 0x14: if(CAPSLOCK ^ SHIFT) printf("T"); else printf("t"); break;
        case 0x15: if(CAPSLOCK ^ SHIFT) printf("Y"); else printf("y"); break;
        case 0x16: if(CAPSLOCK ^ SHIFT) printf("U"); else printf("u"); break;
        case 0x17: if(CAPSLOCK ^ SHIFT) printf("I"); else printf("i"); break;
        case 0x18: if(CAPSLOCK ^ SHIFT) printf("O"); else printf("o"); break;
        case 0x19: if(CAPSLOCK ^ SHIFT) printf("P"); else printf("p"); break;
        case 0x1A: if(CAPSLOCK ^ SHIFT) printf("{"); else printf("["); break;
        case 0x1B: if(CAPSLOCK ^ SHIFT) printf("}"); else printf("]"); break;
        case 0x1C: printf("\n"); break;
        case 0x1E: if(CAPSLOCK ^ SHIFT) printf("A"); else printf("a"); break;
        case 0x1F: if(CAPSLOCK ^ SHIFT) printf("S"); else printf("s"); break;
        case 0x20: if(CAPSLOCK ^ SHIFT) printf("D"); else printf("d"); break;
        case 0x21: if(CAPSLOCK ^ SHIFT) printf("F"); else printf("f"); break;
        case 0x22: if(CAPSLOCK ^ SHIFT) printf("G"); else printf("g"); break;
        case 0x23: if(CAPSLOCK ^ SHIFT) printf("H"); else printf("h"); break;
        case 0x24: if(CAPSLOCK ^ SHIFT) printf("J"); else printf("j"); break;
        case 0x25: if(CAPSLOCK ^ SHIFT) printf("K"); else printf("k"); break;
        case 0x26: if(CAPSLOCK ^ SHIFT) printf("L"); else printf("l"); break;
        case 0x27: if(CAPSLOCK ^ SHIFT) printf(":"); else printf(";"); break;
        case 0x28: if(CAPSLOCK ^ SHIFT) printf("\""); else printf("'"); break;
        case 0x2B: if(CAPSLOCK ^ SHIFT) printf("|"); else printf("\\"); break;
        case 0x2C: if(CAPSLOCK ^ SHIFT) printf("Z"); else printf("z"); break;
        case 0x2D: if(CAPSLOCK ^ SHIFT) printf("X"); else printf("x"); break;
        case 0x2E: if(CAPSLOCK ^ SHIFT) printf("C"); else printf("c"); break;
        case 0x2F: if(CAPSLOCK ^ SHIFT) printf("V"); else printf("v"); break;
        case 0x30: if(CAPSLOCK ^ SHIFT) printf("B"); else printf("b"); break;
        case 0x31: if(CAPSLOCK ^ SHIFT) printf("N"); else printf("n"); break;
        case 0x32: if(CAPSLOCK ^ SHIFT) printf("M"); else printf("m"); break;
        case 0x33: if(CAPSLOCK ^ SHIFT) printf("<"); else printf(","); break;
        case 0x34: if(CAPSLOCK ^ SHIFT) printf(">"); else printf("."); break;
        case 0x35: if(escape) printf(" KEYPAD / "); else {if(CAPSLOCK ^ SHIFT) printf("?"); else printf("/");} break;
        case 0x39: printf(" "); break;
        case 0x29: if(CAPSLOCK ^ SHIFT) printf("~"); else printf("`"); break;
        case 0x02: if(CAPSLOCK ^ SHIFT) printf("!"); else printf("1"); break;
        case 0x03: if(CAPSLOCK ^ SHIFT) printf("@"); else printf("2"); break;
        case 0x04: if(CAPSLOCK ^ SHIFT) printf("#"); else printf("3"); break;
        case 0x05: if(CAPSLOCK ^ SHIFT) printf("$"); else printf("4"); break;
        case 0x06: if(CAPSLOCK ^ SHIFT) printf("%"); else printf("5"); break;
        case 0x07: if(CAPSLOCK ^ SHIFT) printf("^"); else printf("6"); break;
        case 0x08: if(CAPSLOCK ^ SHIFT) printf("&"); else printf("7"); break;
        case 0x09: if(CAPSLOCK ^ SHIFT) printf("*"); else printf("8"); break;
        case 0x0A: if(CAPSLOCK ^ SHIFT) printf("("); else printf("9"); break;
        case 0x0B: if(CAPSLOCK ^ SHIFT) printf(")"); else printf("0"); break;
        case 0x0C: if(CAPSLOCK ^ SHIFT) printf("_"); else printf("-"); break;
        case 0x0D: if(CAPSLOCK ^ SHIFT) printf("+"); else printf("="); break;
        case 0x5B: printf(" WINKEY "); escape=0; break;
        case 0x0F: printf(" TAB "); break;
        case 0x01: printf(" ESC "); break;
        case 0x3B: printf(" F1 "); break;
        case 0x3C: printf(" F2 "); break;
        case 0x3D: printf(" F3 "); break;
        case 0x3E: printf(" F4 "); break;
        case 0x3F: printf(" F5 "); break;
        case 0x40: printf(" F6 "); break;
        case 0x41: printf(" F7 "); break;
        case 0x42: printf(" F8 "); break;
        case 0x43: printf(" F9 "); break;
        case 0x44: printf(" F10 "); break;
        case 0x57: printf(" F11 "); break;
        case 0x58: printf(" F12 "); break;
        case 0x0E: printf(" BACKSPACE "); break;
        case 0x52: if(!functionkey) printf(" KEYPAD 0 "); else printf(" INSERT "); break;
        case 0x53: if(functionkey) printf(" DELETE "); else printf(" . "); break;
        case 0x37: printf(" KEYPAD * "); break;
        case 0x4A: printf(" KEYPAD - "); break;
        case 0x4E: printf(" KEYPAD + "); break;
        case 0x4F: if(functionkey) printf(" END "); else printf(" KEYPAD 1 "); break;
        case 0x50: if(functionkey) printf(" ARROW DOWN "); else printf(" KEYPAD 2 "); break;
        case 0x51: if(functionkey) printf(" Page Down "); else printf(" KEYPAD 3 "); break;
        case 0x4B: if(functionkey) printf(" ARROW LEFT "); else printf(" KEYPAD 4 "); break;
        case 0x4C: printf(" KEYPAD 5 "); break;
        case 0x4D: if(functionkey) printf(" ARROW RIGHT"); else printf(" KEYPAD 6 "); break;
        case 0x47: if(functionkey) printf(" HOME "); else printf(" KEYPAD 7 "); break;
        case 0x48: if(functionkey) printf(" ARROW UP "); else printf(" KEYPAD 8 "); break;
        case 0x49: if(functionkey) printf(" Page Up "); else printf(" KEYPAD 9 "); break;
        case 0x56: if(CAPSLOCK ^ SHIFT) printf(">"); else printf("<"); break;
        
        default:
            char* msg = "KEYBOARD 0x00 ";
            char* hex = "0123456789ABCDEF";
            msg[11] = hex[(key >> 4) & 0x0F];
            msg[12] = hex[key & 0x0F];
            printf(msg); 
    }
    escape = 0;
    return esp;
}

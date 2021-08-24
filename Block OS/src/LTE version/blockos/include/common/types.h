#ifndef __BLOCKOS__COMMON__TYPES_H
#define __BLOCKOS__COMMON__TYPES_H

#define NULL 0
#define null 0

namespace drivers
{
class VideoGraphicsArray;
}

namespace blockos
{
    namespace common 
    {
        typedef char int8_t;
        typedef unsigned char uint8_t;
        
        typedef short int16_t;
        typedef unsigned short uint16_t;
        
        typedef int int32_t;
        typedef unsigned int uint32_t;
        
        typedef long long int64_t;
        typedef unsigned long long uint64_t;
		
        typedef uint32_t size_t;
    }
    
    template <typename type> void Swap(type &x,type &y)
    {
        type aux=x;
        x=y;
        y=aux;
    }
}  


#endif

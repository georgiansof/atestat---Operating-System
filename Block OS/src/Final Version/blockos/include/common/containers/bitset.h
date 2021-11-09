#ifndef __BLOCKOS__COMMON__CONTAINERS_H
#define __BLOCKOS__COMMON__CONTAINERS_H

#include <common/types.h>

using namespace blockos::common;

namespace blockos
{
    namespace common
    {
        namespace containers
        {
            class Bit
            {
            private:
                uint32_t _Byte;
                uint8_t _Bit;
                uint8_t *data;
            public:
                Bit() {}
                void Set(uint8_t *data,uint32_t _Byte,uint8_t _Bit)
                {
                    this->data=data;
                    this->_Byte=_Byte;
                    this->_Bit=_Bit;
                }
                void operator= (bool x)
                {
                    if(x==1) (*(data+_Byte)) = (*(data+_Byte)) | (1<<_Bit);
                    else (*(data+_Byte)) = ~(~(*(data+_Byte)) | (1<<_Bit));
                }
                operator bool ()
                {
                    bool x;
                    x=((*(data+_Byte))>>_Bit) & 0x01;
                    return x;
                }
            };

            template<int size> class bitset
            {
                uint32_t bytes=size/8+(size%8!=0);
                uint8_t lastByteBits=size%8;
                uint8_t *data=new uint8_t[bytes];
            public:
                Bit b;
                bitset() 
                {
                    for(int i=0;i<size;++i) 
                        *(data+i)=0;
                }
                Bit& operator[] (int32_t index)
                {
                    b.Set(data,index/8,(uint8_t)(index%8));
                    return b;
                }
                void set(bool val)
                {
                    if(val==0) bitset();
                    else 
                        for(int i=0;i<size;++i) 
                            *(data+i)=255;
                }
                void flip()
                {
                    for(int i=0;i<size;++i) 
                        *(data+i)=~(*(data+i));
                }
            };
        }
    }
}

#endif
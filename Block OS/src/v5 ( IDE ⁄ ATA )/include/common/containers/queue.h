#ifndef __BLOCKOS__COMMON__CONTAINERS__QUEUE_H
#define __BLOCKOS__COMMON__CONTAINERS__QUEUE_H

#include <common/containers/list.h>

namespace blockos
{
    namespace common
    {
        namespace containers
        {
            template <typename generalised> class queue : public list<generalised>
            {
            public:
                void push(generalised x) {list<generalised>::push_back(x);}
                void pop() {list<generalised>::pop_front();}
                generalised& front() {return list<generalised>::first();}
            };
        }
    }
}

#endif
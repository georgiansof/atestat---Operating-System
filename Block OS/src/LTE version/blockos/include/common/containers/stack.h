#ifndef __BLOCKOS__COMMON__CONTAINERS__STACK_H
#define __BLOCKOS__COMMON__CONTAINERS__STACK_H

#include <common/containers/list.h>

namespace blockos
{
    namespace common
    {
        namespace containers
        {
            template<typename generalised> class stack : public list<generalised>
            {
            public:
                void push(generalised x) {list<generalised>::push_front(x);}
                void pop() {list<generalised>::pop_front();}
                generalised& top() {return list<generalised>::first();}
            };
        }
    }
}

#endif
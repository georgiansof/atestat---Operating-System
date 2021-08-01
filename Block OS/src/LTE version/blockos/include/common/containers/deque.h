#ifndef __BLOCKOS__COMMON__CONTAINERS__DEQUE_H
#define __BLOCKOS__COMMON__CONTAINERS__DEQUE_H

#include <common/types.h>

void printf(char);
void printf(const char*);
void printf(int);

using namespace blockos::common;

namespace blockos
{
    namespace common
    {
        namespace containers
        {
            template <typename generalised> class deque_iterator;

            template <typename generalised> struct deque_node
            {
                deque_node *next;
                deque_node *prev;
                generalised info;

                deque_node() {next=prev=nullptr;}
                operator deque_iterator<generalised>() {deque_iterator<generalised> it; it->ptr=this; return it;}
            };
            template <typename generalised> class deque;

            template <typename generalised> class deque_iterator
            {
            public:
                deque_node<generalised> *ptr;
                deque_iterator operator + (int x)
                {
                    deque_iterator it=*this;
                    for(int i=1;i<=x;++i) ++it;
                    return it;
                }
                deque_iterator operator - (int x)
                {
                    deque_iterator it=*this;
                    for(int i=1;i<=x;++i) --it;
                    return it;
                }

                void operator ++() {ptr=ptr->next;}
                void operator ++(int) {ptr=ptr->next;}
                
                void operator --() {ptr=ptr->prev;}
                void operator --(int) {ptr=ptr->prev;}

                void operator =(deque_node<generalised> *x) {this->ptr=x;}
                void operator =(deque_iterator x) {this->ptr=x->ptr;}
                bool operator ==(deque_node<generalised> *x) {return x==ptr;}
                bool operator !=(deque_node<generalised> *x) {return !(x==ptr);}
                generalised operator *() {return ptr->info;}
                deque_node<generalised>* operator ->() {return ptr;}
                deque_iterator<generalised>() {ptr=nullptr;}
                deque_iterator<generalised>(deque_node<generalised>* ptr) {this->ptr=ptr;}
                operator deque_node<generalised>*() {return ptr;}
            };

            
            template<typename generalised> class deque
            {   
            protected:
                deque_node<generalised> *First, *Last;
                deque_iterator<generalised> it;
                int Size;
            public:
                deque() {First=Last=nullptr;}

                deque_iterator<generalised> begin() {it.ptr=First; return it;}
                deque_iterator<generalised> end() {it.ptr=nullptr; return it;}
                deque_iterator<generalised> rbegin() {it.ptr=Last; return it;}
                deque_iterator<generalised> rend() {it.ptr=nullptr; return it;}

                generalised& front() {return First->info;}
                generalised& back() {return Last->info;}
                bool isEmpty() {return First==nullptr;}
                void empty() {while(!this->isEmpty()) pop_front();}
                int size() {return Size;}
                void push_back(generalised x)
                {
                    ++Size;
                    deque_node<generalised> *tmp = new deque_node<generalised>;
                    tmp->next=tmp->prev=nullptr;
                    tmp->info=x;
                    if(Last!=nullptr) 
                    {
                        tmp->prev=Last;
                        Last->next=tmp;
                        Last=tmp;
                    }
                    else Last=First=tmp;
                }
                void push_front(generalised x)
                {
                    ++Size;
                    deque_node<generalised> *tmp = new deque_node<generalised>;
                    tmp->info=x;
                    tmp->prev=nullptr;
                    if(First!=nullptr) tmp->next=First;
                    else tmp->next=nullptr;
                    First->prev=tmp;
                    First=tmp;
                    if(Last==nullptr) First=Last=tmp;
                }
                void pop_back()
                {
                    --Size;
                    deque_iterator<generalised> it=Last;
                    if(Last->prev) 
                    {
                        it=Last->prev;
                        delete it->next;
                        it->next=nullptr;
                    }
                    else
                    {
                        First=Last=nullptr;
                        delete it.ptr;
                    }
                }
                void pop_front()
                {
                    --Size;
                    deque_iterator<generalised> tmp=First;
                    if(First->next) First=First->next,First->prev=nullptr;
                    else First=Last=nullptr;
                    delete tmp.ptr;
                }
            };
        }
    }
}

#endif
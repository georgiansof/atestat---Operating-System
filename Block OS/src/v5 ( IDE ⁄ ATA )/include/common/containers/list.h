#ifndef __BLOCKOS__COMMON__CONTAINERS__LIST_H
#define __BLOCKOS__COMMON__CONTAINERS__LIST_H

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
            template <typename generalised> class list_iterator;

            template <typename generalised> struct list_node
            {
                list_node *next;
                generalised info;

                list_node() {next=nullptr;}
                operator list_iterator<generalised>() {list_iterator<generalised> it; it->ptr=this; return it;}
            };
            template <typename generalised> class list;

            template <typename generalised> class list_iterator
            {
            public:
                list_node<generalised> *ptr;
                list_iterator operator + (int x)
                {
                    list_iterator it=*this;
                    for(int i=1;i<=x;++i) ++it;
                    return it;
                }
                void operator ++() {ptr=ptr->next;}
                void operator ++(int) {ptr=ptr->next;}
                void operator +=(int x)
                {
                    for(int i=1;i<=x;++i) 
                        if(this->ptr!=nullptr) ++(*this);
                }
                void operator =(list_node<generalised> *x) {this->ptr=x;}
                void operator =(list_iterator x) {this->ptr=x->ptr;}
                bool operator ==(list_node<generalised> *x) {return x==ptr;}
                bool operator !=(list_node<generalised> *x) {return !(x==ptr);}
                generalised& operator *() {return ptr->info;}
                generalised& operator ->() {return ptr->info;}
                list_iterator<generalised>() {ptr=nullptr;}
                list_iterator<generalised>(list_node<generalised>* ptr) {this->ptr=ptr;}
                operator list_node<generalised>*() {return ptr;}
            };

            
            template<typename generalised> class list
            {   
            protected:
                list_node<generalised> *First, *Last;
                list_iterator<generalised> it;
                int Size;
            public:
                list() {First=Last=nullptr;}

                list_iterator<generalised> begin() {it.ptr=First; return it;}
                list_iterator<generalised> end() {it.ptr=nullptr; return it;}
                generalised& first() {return First->info;}
                generalised& last() {return Last->info;}
                bool isEmpty() {return First==nullptr;}
                void empty() {while(!this->isEmpty()) pop_front();}
                void erase(generalised val)
                {
                    if(this->begin().ptr!=nullptr)
                        for(list_iterator<generalised> it=this->begin();it+1!=this->end();++it)
                        {
                            if(*(it+1)==val)
                            {
                                list_iterator<generalised> tmp=it,del=(it+1);
                                it+=2;
                                tmp.ptr->next=it.ptr;
                                delete del.ptr;
                            }
                        }
                }
                int size() {return Size;}
                void push_back(generalised x)
                {
                    ++Size;
                    list_node<generalised> *tmp = new list_node<generalised>;
                    tmp->next=nullptr;
                    tmp->info=x;
                    if(Last!=nullptr) Last->next=tmp,Last=tmp;
                    else Last=First=tmp;
                }
                void push_front(generalised x)
                {
                    ++Size;
                    list_node<generalised> *tmp = new list_node<generalised>;
                    tmp->info=x;
                    if(First!=nullptr) tmp->next=First;
                    First=tmp;
                    if(Last==nullptr) Last=tmp;
                }
                void pop_back()
                {
                    --Size;
                    list_iterator<generalised> it=First;
                    if(First->next==nullptr) {First=Last=nullptr; delete it.ptr; return;}
                    
                    for(;it+1!=Last;++it);
                    delete it->next;
                    it->next=0;
                    Last=it;
                }
                void pop_front()
                {
                    --Size;
                    list_iterator<generalised> tmp=First;
                    if(First->next) First=First->next;
                    else First=Last=nullptr;
                    delete tmp.ptr;
                }
            
            };
        }

    }
}

#endif
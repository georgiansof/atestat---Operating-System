#include <gui/widget.h>

using namespace blockos::common;
using namespace blockos::gui;

Widget::Widget(Widget* parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b): KeyboardEventHandler()
{
    this->parent = parent;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->r = r;
    this->g = g;
    this->b = b;
    this->isFocusable = true;
}
Widget::~Widget()
{
}


void Widget::GetFocus(Widget* widget)
{
    if(parent)
        parent->GetFocus(widget);
}
void Widget::ModelToScreen(common::int32_t &x,common::int32_t &y)
{
    if(parent)
        parent->ModelToScreen(x,y);
    x += this->x;
    y += this->y;
}

void Widget::Draw(GraphicsContext* gc)
{
    int X = 0;
    int Y = 0;
    ModelToScreen(X,Y);
    gc->FillRectangle(X,Y,w,h,  r,g,b);
}
void Widget::OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button)
{
    if(isFocusable)
        GetFocus(this);
}

bool Widget::ContainsCoordinate(common::int32_t x, common::int32_t y)
{
    return this->x <= x && x < this->x + this->w
        && this->y <= y && y < this->y + this->h;
}

void Widget::OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button)
{

}
void Widget::OnMouseMove(common::int32_t oldx, common::int32_t oldy, common::int32_t newx, common::int32_t newy)
{

}




CompositeWidget::CompositeWidget(Widget* parent, 
                common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                common::uint8_t r, common::uint8_t g, common::uint8_t b)
: Widget(parent, x,y,w,h, r,g,b)
{
    focusedChild = 0;
    lastchild=firstchild=nullptr;
}
CompositeWidget::~CompositeWidget()
{

}

void CompositeWidget::GetFocus(Widget* widget)
{
    this->focusedChild = widget;
    if(parent)
        parent->GetFocus(this);
}

void CompositeWidget::AddChild(Widget* child)
{
    childrenList_node *tmp = new childrenList_node;
    tmp->next = nullptr;
    tmp->prev = lastchild;
    tmp->child = child;

    if(lastchild) lastchild->next=tmp,lastchild=tmp;
    else lastchild=firstchild=tmp;
}

void CompositeWidget::Draw(GraphicsContext* gc)
{
    Widget::Draw(gc);
    for(childrenList_node *node=lastchild;node!=nullptr;node=node->prev)
        node->child->Draw(gc);
}

void CompositeWidget::OnMouseDown(int32_t x, int32_t y, uint8_t button)
{
    for(childrenList_node *node=firstchild;node!=nullptr;node=node->next)
        if(node->child->ContainsCoordinate(x - this->x,y - this->y))
        {
            //FIXME HERE
            Widget *tmp;
            tmp=node->child;
            node->child=firstchild->child;
            firstchild->child=tmp;
            //
            tmp->OnMouseDown(x - this->x,y - this->y, button);
            break;
        }
}
void CompositeWidget::OnMouseUp(int32_t x, int32_t y, uint8_t button)
{
    for(childrenList_node *node=firstchild;node!=nullptr;node=node->next)
        if(node->child->ContainsCoordinate(x - this->x,y - this->y))
        {
            node->child->OnMouseUp(x - this->x,y - this->y, button);
            break;
        }
}
void CompositeWidget::OnMouseMove(int32_t oldx, int32_t oldy, int32_t newx, int32_t newy)
{
    Widget *firstchild;
    for(childrenList_node *node=this->firstchild;node!=nullptr;node=node->next)
        if(node->child->ContainsCoordinate(oldx - this->x,oldy - this->y))
        {
            node->child->OnMouseMove(oldx - this->x,oldy - this->y, newx - this->x, newy - this->y);
            firstchild=node->child;
            break;
        }
    
    for(childrenList_node *node=this->firstchild;node!=nullptr;node=node->next)
        if(node->child->ContainsCoordinate(newx - this->x,newy - this->y))
        {
            if(firstchild!=node->child)
                node->child->OnMouseMove(oldx - this->x,oldy - this->y, newx - this->x, newy - this->y);
            break;
        }
}

void CompositeWidget::OnKeyDown(char str)
{
    if(focusedChild)
        focusedChild->OnKeyDown(str);
}
void CompositeWidget::OnKeyUp(char str)
{
    if(focusedChild)
        focusedChild->OnKeyUp(str);
}

void Widget::operator = (Widget *w2)
{
    this->parent=w2->parent;
    this->x=w2->x;
    this->y=w2->y;
    this->w=w2->w;
    this->h=w2->h;
    this->r=w2->r;
    this->g=w2->g;
    this->b=w2->b;
    this->isFocusable=w2->isFocusable;
}
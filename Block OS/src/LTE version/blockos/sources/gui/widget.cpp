#include <gui/widget.h>

using namespace blockos::common;
using namespace blockos::gui;

Widget::Widget(CompositeWidget* parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b): KeyboardEventHandler()
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
void Widget::init(CompositeWidget* parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b)
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

void Widget::SetWidget(Widget wgt)
{
    this->parent = wgt.parent;
    this->x = wgt.x;
    this->y = wgt.y;
    this->w = wgt.w;
    this->h = wgt.h;
    this->r = wgt.r;
    this->g = wgt.g;
    this->b = wgt.b;
    this->isFocusable = true;
}

Widget::Widget(CompositeWidget* parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, common::uint8_t colorIndex): KeyboardEventHandler()
{
    this->parent = parent;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    int8_t r,g,b;
    GetRGB(colorIndex,r,g,b);
    this->r = r;
    this->g = g;
    this->b = b;
    this->isFocusable = true;
}

void Widget::init(CompositeWidget* parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, common::uint8_t colorIndex)
{
    this->parent = parent;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    int8_t r,g,b;
    GetRGB(colorIndex,r,g,b);
    this->r = r;
    this->g = g;
    this->b = b;
    this->isFocusable = true;
}

void Widget::GetRGB(int8_t colorIndex, int8_t &r, int8_t &g, int8_t &b)
{
    switch(colorIndex)
    {
        case 0x00: r = 0x00 , g = 0x00 , b = 0x00;
            break;
        case 0x01: r = 0x00 , g = 0x00 , b = 0xA8;
            break;
        case 0x02: r = 0x00 , g = 0xA8 , b = 0x00;
            break;
        case 0x03: r = 0x00 , g = 0xA8 , b = 0xA8;
            break;
        case 0x04: r = 0xA8 , g = 0x00 , b = 0x00;
            break;
        case 0x05: r = 0xA8 , g = 0x00 , b = 0xA8;
            break;
        case 0x06: r = 0xA8 , g = 0xA8 , b = 0x00; 
            break;
        case 0x07: r = 0xA8 , g = 0xA8 , b = 0xA8;
            break;
        case 0x08: r = 0x00 , g = 0x00 , b = 0x57;
            break;
        case 0x09: r = 0x00 , g = 0x00, b = 0xFF;
            break;
        case 0x0A: r = 0x00 , g = 0xA8 , b = 0x57;
            break;
        case 0x0B: r = 0x00 , g = 0xA8 , b = 0xFF;
            break;
        case 0x0C: r = 0xA8 , g = 0x00 , b = 0x57;
            break;
        case 0x0D: r = 0xA8 , g = 0x00 , b = 0xFF;
            break;
        case 0x0E: r = 0xA8 , g = 0xA8 , b = 0x57;
            break;
        case 0x0F: r = 0xA8 , g = 0xA8 , b = 0xFF;
            break;
        case 0x10: r = 0x00 , g = 0x57 , b = 0x00;
            break;
        case 0x11: r = 0x00 , g = 0x57, b = 0xA8;
            break;
        case 0x12: r = 0x00 , g = 0xFF,  b = 0x00;
            break;
        case 0x13: r = 0x00 ,g = 0xFF , b = 0xA8;
            break;
        case 0x14: r = 0xA8 , g = 0x57 , b = 0x00;
            break;
        case 0x15: r = 0xA8 , g = 0x57 , b = 0xA8;
            break;
        case 0x16: r = 0xA8 , g = 0xFF , b = 0x00;
            break;
        case 0x17: r = 0xA8 , g = 0xFF , b = 0xA8;
            break;
        case 0x18: r = 0x00 , g = 0x57 , b = 0x57;
            break;
        case 0x19: r = 0x00 , g = 0x57 , b = 0xFF;
            break;
        case 0x1A: r = 0x00 , g = 0xFF , b = 0x57;
            break;
        case 0x1B: r = 0x00 , g = 0xFF , b = 0xFF;
            break;
        case 0x1C: r = 0xA8 , g = 0x57 , b = 0x57;
            break;
        case 0x1D: r = 0xA8 , g = 0x57 , b = 0xFF;
            break;
        case 0x1E: r = 0xA8 , g = 0xFF , b = 0x57;
            break;
        case 0x1F: r = 0xA8 , g = 0xFF , b = 0xFF;
            break;
        case 0x20: r = 0x57 , g = 0x00 , b = 0x00;
            break;
        case 0x21: r = 0x57 , g = 0x00 , b = 0xA8;
            break;
        case 0x22: r = 0x57 , g = 0xA8 , b = 0x00;
            break;
        case 0x23: r = 0x57 , g = 0xA8 , b = 0xA8;
            break;
        case 0x24: r = 0xFF , g = 0x00 , b = 0x00;
            break;
        case 0x25: r = 0xFF , g = 0x00 , b = 0xA8;
            break;
        case 0x26: r = 0xFF , g = 0xA8 , b = 0x00;
            break;
        case 0x27: r = 0xFF , g = 0xA8 , b = 0xA8;
            break;
        case 0x28: r = 0x57 , g = 0x00 , b = 0x57;
            break;
        case 0x29: r = 0x57 , g = 0x00 , b = 0xFF;
            break;
        case 0x2A: r = 0x57 , g = 0xA8 , b = 0x57;
            break;
        case 0x2B: r = 0x57 , g = 0xA8 , b = 0xFF;
            break;
        case 0x2C: r = 0xFF , g = 0x00 , b = 0x57;
            break;
        case 0x2D: r = 0xFF , g = 0x00 , b = 0xFF;
            break;
        case 0x2E: r = 0xFF , g = 0xA8 , b = 0x57;
            break;
        case 0x2F: r = 0xFF , g = 0xA8 , b = 0xFF;
            break;
        case 0x30: r = 0x57 , g = 0x57 , b = 0x00;
            break;
        case 0x31: r = 0x57 , g = 0x57 , b = 0xA8;
            break;
        case 0x32: r = 0x57 , g = 0xFF , b = 0x00;
            break;
        case 0x33: r = 0x57 , g = 0xFF , b = 0xA8;
            break;
        case 0x34: r = 0xFF , g = 0x57 , b = 0x00;
            break;
        case 0x35: r = 0xFF , g = 0x57 , b = 0xA8;
            break;
        case 0x36: r = 0xFF , g = 0xFF , b = 0x00;
            break;
        case 0x37: r = 0xFF , g = 0xFF , b = 0xA8;
            break;
        case 0x38: r = 0x57 , g = 0x57 , b = 0x57;
            break;
        case 0x39: r = 0x57 , g = 0x57 , b = 0xFF;
            break;
        case 0x3A: r = 0x57 , g = 0xFF , b = 0x57;
            break;
        case 0x3B: r = 0x57 , g = 0xFF , b = 0xFF;
            break;
        case 0x3C: r = 0xFF , g = 0x57 , b = 0x57;
            break;
        case 0x3D: r = 0xFF , g = 0x57 , b = 0xFF;
            break;
        case 0x3E: r = 0xFF , g = 0xFF , b = 0x57;
            break;
        case 0x3F: r = 0xFF , g = 0xFF , b = 0xFF;
            break;
    }
}


Widget::~Widget()
{
}

Widget::Widget()
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




CompositeWidget::CompositeWidget(CompositeWidget* parent, 
                common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                common::uint8_t r, common::uint8_t g, common::uint8_t b)
: Widget(parent, x,y,w,h, r,g,b)
{
    focusedChild = 0;
}

CompositeWidget::CompositeWidget(CompositeWidget* parent, 
                common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                common::uint8_t colorIndex)
: Widget(parent, x,y,w,h, colorIndex)
{
    focusedChild = 0;
}
void CompositeWidget::init(CompositeWidget* parent, 
                common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                common::uint8_t r, common::uint8_t g, common::uint8_t b)
{
    focusedChild = 0;
    Widget::init(parent,x,y,w,h,r,g,b);
}

void CompositeWidget::init(CompositeWidget* parent, 
                common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                common::uint8_t colorIndex)
{
    focusedChild = 0;
    Widget::init(parent,x,y,w,h,colorIndex);
}

CompositeWidget::CompositeWidget() : Widget()
{
    focusedChild = 0;
}


CompositeWidget::~CompositeWidget()
{
    children.empty();
}

void CompositeWidget::GetFocus(Widget* widget)
{
    this->focusedChild = widget;
    if(parent)
        parent->GetFocus(this);
}

void printf(const char*);

void CompositeWidget::AddChild(CompositeWidget* child)
{
    children.push_back(child);
}

void CompositeWidget::RemoveChild(CompositeWidget* child)
{
    children.erase(child);
}

void CompositeWidget::Draw(GraphicsContext* gc)
{
    Widget::Draw(gc);
    for(deque_iterator<CompositeWidget*> it=children.rbegin();it!=children.rend();--it)
       it->CompositeWidget::Draw(gc);
}

void CompositeWidget::OnMouseDown(int32_t x, int32_t y, uint8_t button)
{
    for(deque_iterator<CompositeWidget*> it=children.begin();it!=children.end();++it)
        if(it->ContainsCoordinate(x - this->x,y - this->y))
        {
            deque_node<CompositeWidget*> *tmp;
            tmp=it.ptr;
            it.ptr=children.begin().ptr;
            children.begin().ptr=tmp;
            //
            tmp->info->OnMouseDown(x - this->x,y - this->y, button);
            break;
        }
}
void CompositeWidget::OnMouseUp(int32_t x, int32_t y, uint8_t button)
{
    for(deque_iterator<CompositeWidget*> it=children.begin();it!=children.end();++it)
        if(it->ContainsCoordinate(x - this->x,y - this->y))
        {
            it->OnMouseUp(x - this->x,y - this->y, button);
            break;
        }
}
void CompositeWidget::OnMouseMove(int32_t oldx, int32_t oldy, int32_t newx, int32_t newy)
{
    CompositeWidget *firstchild;
    for(deque_iterator<CompositeWidget*> it=children.begin();it!=children.end();++it)
        if(it->ContainsCoordinate(oldx - this->x,oldy - this->y))
        {
            it->OnMouseMove(oldx - this->x,oldy - this->y, newx - this->x, newy - this->y);
            firstchild=*it;
            break;
        }
    
    for(deque_iterator<CompositeWidget*> it=children.begin();it!=children.end();++it)
        if(it->ContainsCoordinate(newx - this->x,newy - this->y))
        {
            if(firstchild!=*it)
                it->OnMouseMove(oldx - this->x,oldy - this->y, newx - this->x, newy - this->y);
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
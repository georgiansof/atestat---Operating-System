#include <gui/window.h>

using namespace blockos::common;
using namespace blockos::gui;


/*Button::Button(Widget* parent, common::int32_t WindowX, common::int32_t WindowY) 
: Widget(parent,WindowX-8,WindowY-9,7,7,RED)
{

}

void Button::OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button)
{
    if(button==1)
    {
        Button::~Button();
        parent->Widget::~Widget();
    }
}*/

Window::Window(Widget* parent,
               common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
               common::uint8_t r, common::uint8_t g, common:: uint8_t b) 
: CompositeWidget(parent, x,y,w,h, r,g,b)
{
    Dragging = false;
}

Window::Window(Widget* parent,
               common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
               common::uint8_t colorIndex) 
: CompositeWidget(parent, x,y,w,h, colorIndex)
{
    
    Dragging = false;
}

Window::~Window()
{
    this->~CompositeWidget();
}

void Window::OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button)
{
    if(button==1) Dragging = true;
    CompositeWidget::OnMouseDown(x,y,button);
}
void Window::OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button)
{
    if(button==1) Dragging = false;
    CompositeWidget::OnMouseUp(x,y,button);
}
void Window::OnMouseMove(common::int32_t oldx, common::int32_t oldy, common::int32_t newx, common::int32_t newy)
{
    if(Dragging)
    {
        this->x += newx-oldx;
        this->y += newy-oldy;
    }
    CompositeWidget::OnMouseMove(oldx, oldy, newx, newy);
}

#include <gui/window.h>

using namespace blockos::common;
using namespace blockos::gui;


Button::Button()
: CompositeWidget()
{
}
void Button::initialise(Window* parent, bool type)
{
    this->type=type;
    this->parent=parent;
    if(type==false) CompositeWidget::init(parent,parent->w-8,1, 7,7, RED);
    if(type==true) CompositeWidget::init(parent,parent->w-8-8,+1, 7,7, YELLOW);
}

Button::~Button()
{
    Widget::~Widget();
}

void Button::OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button)
{
    /// probably unsafe
    if(this->type==false)
    {
        if(button==1)
        {
            Button::~Button();
            parent->~Window();
            delete this->parent;
        }
    }
    else
    {   
        this->parent->parent->RemoveChild(this->parent);
    }
}

Window::Window(CompositeWidget* parent,
               common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
               common::uint8_t r, common::uint8_t g, common:: uint8_t b) 
: CompositeWidget(parent, x,y,w,h, r,g,b)
{
    Dragging = false;
    exit_button.initialise(this,false);
    minimize_button.initialise(this,true);
    AddChild(&exit_button);
    AddChild(&minimize_button);
}

Window::Window(CompositeWidget* parent,
               common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
               common::uint8_t colorIndex) 
: CompositeWidget(parent, x,y,w,h, colorIndex)
{
    Dragging = false;
    exit_button.initialise(this,false);
    minimize_button.initialise(this,true);
    AddChild(&exit_button);
    AddChild(&minimize_button);
}

Window::Window() : CompositeWidget()
{
}

Window::~Window()
{
    CompositeWidget::~CompositeWidget();
    parent->RemoveChild(this);
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

#include <gui/window.h>

using namespace blockos::common;
using namespace blockos::gui;


WindowBar::~WindowBar()
{
    exit_button.~Button();
    minimize_button.~Button();
}

WindowBar::WindowBar()
{

}

void WindowBar::initialise(Window *parent, Button *&exit_buttonptr, Button *&minimize_buttonptr)
{
    CompositeWidget::init(parent,0,0,parent->w,9,GREY);
    exit_button.initialise(parent,this,0);
    minimize_button.initialise(parent,this,1);
    exit_buttonptr=&exit_button;
    minimize_buttonptr=&minimize_button;
}

Button::Button()
: CompositeWidget()
{
}
void Button::initialise(Window* winparent, CompositeWidget *parent, bool type)
{
    this->type=type;
    this->winparent=winparent;
    if(type==false) CompositeWidget::init(parent,parent->w-8,1, 7,7, RED);
    if(type==true) CompositeWidget::init(parent,parent->w-8-8,+1, 7,7, YELLOW);
    this->isFocusable=false;
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
            winparent->~Window();
            delete this->parent;
        }
    }
    else
    {   
        CompositeWidget *dsk=this->winparent->parent;
        CompositeWidget *win=this->winparent;
        dsk->RemoveChild(win);
    }
}

Window::Window(CompositeWidget* parent,
               common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
               common::uint8_t r, common::uint8_t g, common:: uint8_t b) 
{
    if(w>=25)
    {
        CompositeWidget::init(parent, x,y,w,h+9, r,g,b);
        Dragging = false;
        window_bar.initialise(this, exit_button, minimize_button);
        AddChild(&window_bar);
        AddChild(exit_button);
        AddChild(minimize_button);
    }
}

Window::Window(CompositeWidget* parent,
               common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
               common::uint8_t colorIndex)
{
    if(w>=25)
    {
        CompositeWidget::init(parent, x,y,w,h+9, colorIndex);
        this->position.x=x;
        this->position.y=y;
        this->position.h=h+9;
        this->position.w=w;
        Dragging = false;
        window_bar.initialise(this, exit_button, minimize_button);
        AddChild(&window_bar);
        AddChild(exit_button);
        AddChild(minimize_button);
    }
}

Window::Window() : CompositeWidget()
{
}

Window::~Window()
{
    window_bar.~WindowBar();
    CompositeWidget::~CompositeWidget();
    parent->RemoveChild(this);
}



void Window::OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button)
{
    if(button==1) 
    {
        if(y < this->y + window_bar.h )
            Dragging = true;
    }
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
        x += newx-oldx;
        y += newy-oldy;
        this->position.x += newx-oldx;
        this->position.y += newy-oldy;
    }
    CompositeWidget::OnMouseMove(oldx, oldy, newx, newy);
}

void WindowBar::OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button)
{
}
void WindowBar::OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button)
{
}

void WindowBar::OnMouseMove(common::int32_t oldx, common::int32_t oldy, common::int32_t newx, common::int32_t newy)
{
}
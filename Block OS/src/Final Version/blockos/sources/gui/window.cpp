#include <gui/window.h>
#include <common/containers/deque.h>

using namespace blockos::common;
using namespace blockos::gui;


Icon::Icon()
{
}

void Icon::initialise(CompositeWidget *handler, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t colorIndex)
{
    CompositeWidget::init(handler,x,y,w,h,colorIndex);
}

void Icon::initialise(CompositeWidget *handler, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint8_t g, uint8_t b)
{
    CompositeWidget::init(handler,x,y,w,h,r,g,b);
}

void Icon::OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button)
{
    winparent->desktop_parent->AddChild(winparent);
}

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
    this->~Widget();
}

void Button::OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button)
{
    /// probably unsafe
    if(this->type==false)
    {
        if(button==1)
        {
            winparent->Dragging=false;
            winparent->~Window();
            delete this->parent;
            winparent->desktop_parent->task_bar.RemoveWindow(winparent);
        }
    }
    else
    {   
        winparent->Dragging=false;
        CompositeWidget *dsk=this->winparent->parent;
        CompositeWidget *win=this->winparent;
        dsk->RemoveChild(win);
    }
}

Window::Window(Desktop* parent,
               common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
               common::uint8_t r, common::uint8_t g, common:: uint8_t b) 
{
    if(w>=25)
    {
        desktop_parent=parent;
        taskbar_icon.winparent=this;
        windowbar_icon.winparent=this;
        CompositeWidget::init(parent, x,y,w,h+9, r,g,b);
        this->position.x=x;
        this->position.y=y+9;
        this->position.h=h;
        this->position.w=w;
        Dragging = false;
        window_bar.initialise(this, exit_button, minimize_button);
        AddChild(exit_button);
        AddChild(minimize_button);
        AddChild(&window_bar);
        parent->task_bar.window_list.push_back(this);
        taskbar_icon.initialise(&(parent->task_bar),(parent->task_bar.window_list.size())*14,1,13,13,r,g,b);
        parent->task_bar.AddChild(&(this->taskbar_icon));
    }
}

void TaskBar::RemoveWindow(Window *rmv)
{
    bool start=0;
    for(deque_iterator<Window*> it=window_list.begin();it!=window_list.end();++it)
    {
        if(*it==rmv)
            start=1;

        if(start) 
            it->taskbar_icon.x-=14;
    }
    this->window_list.erase(rmv);
    this->RemoveChild(&(rmv->taskbar_icon));
}

Window::Window(Desktop* parent,
               common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
               common::uint8_t colorIndex)
{
    if(w>=25)
    {
        desktop_parent=parent;
        taskbar_icon.winparent=this;
        windowbar_icon.winparent=this;
        CompositeWidget::init(parent, x,y,w,h+9, colorIndex);
        this->position.x=x;
        this->position.y=y+9;
        this->position.h=h;
        this->position.w=w;
        Dragging = false;
        window_bar.initialise(this, exit_button, minimize_button);
        AddChild(exit_button);
        AddChild(minimize_button);
        AddChild(&window_bar);
        parent->task_bar.window_list.push_back(this);
        taskbar_icon.initialise(&(parent->task_bar),(parent->task_bar.window_list.size())*14,1,13,13,colorIndex);
        parent->task_bar.AddChild(&(this->taskbar_icon));
    }
}

Window::Window() : CompositeWidget()
{
}

Window::~Window()
{
    window_bar.~WindowBar();
    this->~CompositeWidget();
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
        if(x>=320-w) x=320-w;
        if(y>=176) y=176;
        if(x<0) x=0;
        if(y<0) y=0;
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
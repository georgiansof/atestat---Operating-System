#ifndef __BLOCKOS__GUI__WIDGET_H
#define __BLOCKOS__GUI__WIDGET_H

#include <common/types.h>
#include <common/graphicscontext.h>
#include <drivers/keyboard.h>


namespace blockos
{
    namespace gui
    {
        class Widget : public blockos::drivers::KeyboardEventHandler
        {
        protected:
            Widget* parent;
            common::int32_t x;
            common::int32_t y;
            common::int32_t w;
            common::int32_t h;

            common::uint8_t r;
            common::uint8_t g;
            common::uint8_t b;
            bool isFocusable;
        public:
            Widget();
            Widget(Widget* parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b);
            Widget(Widget* parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, common::uint8_t colorIndex);
            ~Widget();

            void GetRGB(common::int8_t colorIndex,common::int8_t &r, common::int8_t &g, common::int8_t &b);
            void operator = (Widget *w2);
            virtual void GetFocus(Widget* widget);
            virtual void ModelToScreen(common::int32_t &x,common::int32_t &y);
            virtual bool ContainsCoordinate(common::int32_t x, common::int32_t y);

            virtual void Draw(common::GraphicsContext* gc);
            virtual void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button);
            virtual void OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button);
            virtual void OnMouseMove(common::int32_t oldx, common::int32_t oldy, common::int32_t newx, common::int32_t newy);
        };

        class Window;

        class CompositeWidget : public Widget
        {
            friend Window;
        protected:
            struct childrenList_node
            {
                childrenList_node *prev;
                childrenList_node *next;
                Widget *child;

                childrenList_node() {prev=next=nullptr; child=nullptr;}
            } *firstchild, *lastchild;

            Widget* focusedChild;
        public:
        
            CompositeWidget(Widget* parent, 
            common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
            common::uint8_t r, common::uint8_t g, common::uint8_t b);
            CompositeWidget(Widget* parent, 
            common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
            common::uint8_t colorIndex);
            CompositeWidget();
            ~CompositeWidget();

            virtual void GetFocus(Widget* widget);
            virtual void AddChild(Widget* child);
            virtual void RemoveChild(Widget* child);

            virtual void Draw(common::GraphicsContext* gc);
            virtual void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button);
            virtual void OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button);
            virtual void OnMouseMove(common::int32_t oldx, common::int32_t oldy, common::int32_t newx, common::int32_t newy);

            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
        };
    }
}

#endif

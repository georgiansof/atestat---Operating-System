#ifndef __BLOCKOS__GUI__WIDGET_H
#define __BLOCKOS__GUI__WIDGET_H

#include <common/types.h>
#include <common/graphicscontext.h>
#include <drivers/keyboard.h>
#include <common/containers/deque.h>

using namespace blockos::common::containers;


namespace blockos
{
    namespace gui
    {
        class CompositeWidget;

        class Widget : public blockos::drivers::KeyboardEventHandler
        {
            public:
            CompositeWidget* parent;
            common::int32_t x;
            common::int32_t y;
            common::int32_t w;
            common::int32_t h;

            common::uint8_t r;
            common::uint8_t g;
            common::uint8_t b;
            bool isFocusable;
        
            Widget();
            void SetWidget(Widget wgt);
            Widget(CompositeWidget* parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b);
            Widget(CompositeWidget* parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, common::uint8_t colorIndex);
            void init(CompositeWidget* parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b);
            void init(CompositeWidget* parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, common::uint8_t colorIndex);
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
            deque<CompositeWidget*> children;

            Widget* focusedChild;
        public:
        
            CompositeWidget(CompositeWidget* parent, 
            common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
            common::uint8_t r, common::uint8_t g, common::uint8_t b);
            CompositeWidget(CompositeWidget* parent, 
            common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
            common::uint8_t colorIndex);
            void init(CompositeWidget* parent, 
            common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
            common::uint8_t r, common::uint8_t g, common::uint8_t b);
            void init(CompositeWidget* parent, 
            common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
            common::uint8_t colorIndex);
            CompositeWidget();
            ~CompositeWidget();

            virtual void GetFocus(Widget* widget);
            virtual void AddChild(CompositeWidget* child);
            virtual void RemoveChild(CompositeWidget* child);

            virtual void Draw(common::GraphicsContext* gc);
            virtual void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button);
            virtual void OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button);
            virtual void OnMouseMove(common::int32_t oldx, common::int32_t oldy, common::int32_t newx, common::int32_t newy);

            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);

            void operator = (Widget* wgt) {this->SetWidget(*wgt);}
        };
    }
}

#endif

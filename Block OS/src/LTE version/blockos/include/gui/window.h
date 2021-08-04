#ifndef __BLOCKOS__GUI__WINDOW_H
#define __BLOCKOS__GUI__WINDOW_H

#include <drivers/mouse.h>
#include <gui/widget.h>
#include <gui/colors.h>

namespace blockos
{
    namespace gui
    {
        class Window;

        class Button : public CompositeWidget
        {
        private:
            bool type; // 0 - exit, 1 - minimize;
            Window *parent;
        public:
            Button();
            void initialise(Window* parent, bool type);
            ~Button();
            void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button);
        };

        class Window : public CompositeWidget
        {
        protected:
            bool Dragging;
        public:
            Button exit_button;
            Button minimize_button;
            Window(CompositeWidget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common:: uint8_t b);
            Window(CompositeWidget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t colorIndex);
            Window();
            ~Window();
            void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button);
            void OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button);
            void OnMouseMove(common::int32_t oldx, common::int32_t oldy, common::int32_t newx, common::int32_t newy);
        };

    }

}

#endif

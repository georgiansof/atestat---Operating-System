#ifndef __BLOCKOS__GUI__DESKTOP_H
#define __BLOCKOS__GUI__DESKTOP_H

#include <drivers/mouse.h>
#include <gui/widget.h>
#include <drivers/vga.h>

namespace blockos
{
    namespace gui
    {
        class Icon;

        class TaskBar : public CompositeWidget
        { 
        private:
        public:
            deque<Window*> window_list;
            TaskBar();
            void init(CompositeWidget *parent,uint8_t colorIndex);
            void RemoveWindow(Window *rmv);
        };


        class Desktop : public CompositeWidget, public blockos::drivers::MouseEventHandler
        {
        protected:
            common::uint32_t MouseX;
            common::uint32_t MouseY;
        public:
            TaskBar task_bar;
            drivers::VideoGraphicsArray *gc;
            Desktop(common::int32_t w, common::int32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b);
            Desktop(common::int32_t w, common::int32_t h, common::uint8_t colorIndex);
            ~Desktop();
            void Draw();
            void Update();

            void OnMouseDown(blockos::common::uint8_t button);
            void OnMouseUp(blockos::common::uint8_t button);
            void OnMouseMove(int x, int y);
        };
    }
}

#endif

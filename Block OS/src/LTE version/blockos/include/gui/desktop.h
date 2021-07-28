#ifndef __BLOCKOS__GUI__DESKTOP_H
#define __BLOCKOS__GUI__DESKTOP_H

#include <drivers/mouse.h>
#include <gui/widget.h>

namespace blockos
{
    namespace gui
    {
        class Desktop : public CompositeWidget, public blockos::drivers::MouseEventHandler
        {
        protected:
            common::uint32_t MouseX;
            common::uint32_t MouseY;
        public:
            Desktop(common::int32_t w, common::int32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~Desktop();
            void Draw(common::GraphicsContext* gc);
            void Update(common::GraphicsContext* gc);

            void OnMouseDown(blockos::common::uint8_t button);
            void OnMouseUp(blockos::common::uint8_t button);
            void OnMouseMove(int x, int y);
        };
    }
}

#endif

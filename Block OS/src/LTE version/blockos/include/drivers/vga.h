#ifndef __BLOCKOS__DRIVERS__VGA_H
#define __BLOCKOS__DRIVERS__VGA_H

#include <common/types.h>
#include <hardwarecomm/port.h>
#include <drivers/driver.h>
#include <hardwarecomm/interrupts.h>

namespace blockos
{
    namespace drivers
    {
        class VideoGraphicsArray
        {
        protected:
            common::int8_t pixelMap[200][320];
            common::uint8_t* FrameBufferSegment;
            hardwarecomm::Port8Bit miscPort;
            hardwarecomm::Port8Bit crtcIndexPort;
            hardwarecomm::Port8Bit crtcDataPort;
            hardwarecomm::Port8Bit sequencerIndexPort;
            hardwarecomm::Port8Bit sequencerDataPort;
            hardwarecomm::Port8Bit graphicsControllerIndexPort;
            hardwarecomm::Port8Bit graphicsControllerDataPort;
            hardwarecomm::Port8Bit attributeControllerIndexPort;
            hardwarecomm::Port8Bit attributeControllerReadPort;
            hardwarecomm::Port8Bit attributeControllerWritePort;
            hardwarecomm::Port8Bit attributeControllerResetPort;
            void WriteRegisters(common::uint8_t* registers);
            common::uint8_t* GetFrameBufferSegment();

            virtual common::uint8_t GetColorIndex(common::uint8_t r, common::uint8_t g, common::uint8_t b);
        public:
            VideoGraphicsArray();
            ~VideoGraphicsArray();
            void UpdateScreen();
            virtual bool SetMode(common::uint32_t width, common::uint32_t height, common::uint32_t colordepth);
            virtual bool SupportsMode(common::uint32_t width, common::uint32_t height, common::uint32_t colordepth);
            virtual void PutPixel(common::int32_t x, common::int32_t y, common::uint8_t r, common::uint8_t g, common::uint8_t b);
            virtual void PutPixel(common::int32_t x, common::int32_t y, common::uint8_t colorIndex);

            virtual void FillRectangle(common::uint32_t x, common::uint32_t y, common::uint32_t w, common::uint32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b);
        };

    }
}

#endif

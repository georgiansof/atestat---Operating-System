#include <drivers/vga.h>


using namespace blockos::drivers;
using namespace blockos::common;

VideoGraphicsArray::VideoGraphicsArray():
            miscPort(0x3c2),
            crtcIndexPort(0x3d4),
            crtcDataPort(0x3d5),
            sequencerIndexPort(0x3c4),
            sequencerDataPort(0x3c5),
            graphicsControllerIndexPort(0x3ce),
            graphicsControllerDataPort(0x3cf),
            attributeControllerIndexPort(0x3c0),
            attributeControllerReadPort(0x3c1),
            attributeControllerWritePort(0x3c0),
            attributeControllerResetPort(0x3da)
{

}



VideoGraphicsArray::~VideoGraphicsArray()
{
}

void VideoGraphicsArray::WriteRegisters(uint8_t* registers)
{
    //misc
    miscPort.Write(*(registers++));

    //sequencer
    for(uint8_t i=0; i<5; ++i)
    {
        sequencerIndexPort.Write(i);
        sequencerDataPort.Write(*(registers++));
    }
    // cathode ray tube controller (crtc)
    crtcIndexPort.Write(0x03);
    crtcDataPort.Write(crtcDataPort.Read() | 0x80);
    crtcIndexPort.Write(0x11);
    crtcDataPort.Write(crtcDataPort.Read() & ~0x80);

    registers[0x03] = registers[0x03] | 0x80;
    registers[0x11] = registers[0x11] & ~0x80;

    for(uint8_t i=0; i<25; ++i)
    {
        crtcIndexPort.Write(i);
        crtcDataPort.Write(*(registers++));
    }
    // graphics controller
    for(uint8_t i=0; i<9; ++i)
    {
        graphicsControllerIndexPort.Write(i);
        graphicsControllerDataPort.Write(*(registers++));
    }
    // attribute controller
    for(uint8_t i=0; i<21; ++i)
    {
        attributeControllerResetPort.Read();
        attributeControllerIndexPort.Write(i);
        attributeControllerWritePort.Write(*(registers++));
    }

    attributeControllerResetPort.Read();
    attributeControllerIndexPort.Write(0x20);
}


bool VideoGraphicsArray::SupportsMode(uint32_t width, uint32_t height, uint32_t colordepth)
{
    return width == 320 && height == 200 && colordepth == 8;
}

bool VideoGraphicsArray::SetMode(uint32_t width, uint32_t height, uint32_t colordepth)
{
    if(!SupportsMode(width, height, colordepth))
        return false;

    unsigned char g_320x200x256[] =
    {
    /* MISC */
        0x63,
    /* SEQ */
        0x03, 0x01, 0x0F, 0x00, 0x0E,
    /* CRTC */
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
        0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
        0xFF,
    /* GC */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
        0xFF,
    /* AC */
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x41, 0x00, 0x0F, 0x00,	0x00
    };


    WriteRegisters(g_320x200x256);
    FrameBufferSegment=GetFrameBufferSegment();
    return true;
}

uint8_t* VideoGraphicsArray::GetFrameBufferSegment()
{
    graphicsControllerIndexPort.Write(0x06);
    uint8_t segmentNumber = graphicsControllerDataPort.Read() & (3<<2);

    switch(segmentNumber)
    {
        default:
        case 0<<2: return (uint8_t*)0x00000;
        case 1<<2: return (uint8_t*)0xA0000;
        case 2<<2: return (uint8_t*)0xB0000;
        case 3<<2: return (uint8_t*)0xB8000;
    }
}

void VideoGraphicsArray::PutPixel(int32_t x, int32_t y, uint8_t colorIndex)
{
    if(x<0 || 320 <= x
    || y<0 || 200 <=y) 
        return;
        
    pixelMap[y][x] = colorIndex;
}

void VideoGraphicsArray::UpdateScreen()
{
    uint8_t *pixelAddress;
    for(uint32_t x=0;x<320;++x)
        for(uint32_t y=0;y<200;++y)
        {
            pixelAddress= FrameBufferSegment + 320*y + x;
            *pixelAddress = pixelMap[y][x];
        }
}

uint8_t VideoGraphicsArray::GetColorIndex(uint8_t r, uint8_t g, uint8_t b)
{
    // 00 57 A8 FF 
    /******************* COMPATIBILITY CONVERSION *******************/
    if(r!=0x00 && r!=0x57 && r!=0xA8 && r!=0xFF)
    {
        if(r>0x00 && r<0x57)
            if(r<=0x2B) r=0x00;
            else r=0x57;
        if(r>0x57 && r<0xA8)
            if(r<=0x7F) r=0x57;
            else r=0xA8;
        if(r>0xA8 && r<0xFF)
            if(r<=0xD3) r=0xA8;
            else r=0xFF;
    }

    if(g!=0x00 && g!=0x57 && g!=0xA8 && g!=0xFF)
    {
        if(g>0x00 && g<0x57)
            if(g<=0x2B) g=0x00;
            else g=0x57;
        if(g>0x57 && g<0xA8)
            if(g<=0x7F) g=0x57;
            else g=0xA8;
        if(g>0xA8 && g<0xFF)
            if(g<=0xD3) g=0xA8;
            else g=0xFF;
    }

    if(b!=0x00 && b!=0x57 && b!=0xA8 && b!=0xFF)
    {
        if(b>0x00 && b<0x57)
            if(b<=0x2B) b=0x00;
            else b=0x57;
        if(b>0x57 && b<0xA8)
            if(b<=0x7F) b=0x57;
            else b=0xA8;
        if(b>0xA8 && b<0xFF)
            if(b<=0xD3) b=0xA8;
            else b=0xFF;
    }
    /********************************************************/

    if(r == 0x00 && g == 0x00 && b == 0x00) return 0x00; 
    if(r == 0x00 && g == 0x00 && b == 0xA8) return 0x01; 
    if(r == 0x00 && g == 0xA8 && b == 0x00) return 0x02;
    if(r == 0x00 && g == 0xA8 && b == 0xA8) return 0x03; 
    if(r == 0xA8 && g == 0x00 && b == 0x00) return 0x04; 
    if(r == 0xA8 && g == 0x00 && b == 0xA8) return 0x05; 
    if(r == 0xA8 && g == 0xA8 && b == 0x00) return 0x06; 
    if(r == 0xA8 && g == 0xA8 && b == 0xA8) return 0x07; 
    if(r == 0x00 && g == 0x00 && b == 0x57) return 0x08; 
    if(r == 0x00 && g == 0x00 && b == 0xFF) return 0x09; 
    if(r == 0x00 && g == 0xA8 && b == 0x57) return 0x0A; 
    if(r == 0x00 && g == 0xA8 && b == 0xFF) return 0x0B; 
    if(r == 0xA8 && g == 0x00 && b == 0x57) return 0x0C; 
    if(r == 0xA8 && g == 0x00 && b == 0xFF) return 0x0D; 
    if(r == 0xA8 && g == 0xA8 && b == 0x57) return 0x0E; 
    if(r == 0xA8 && g == 0xA8 && b == 0xFF) return 0x0F; 


    if(r == 0x00 && g == 0x57 && b == 0x00) return 0x10; 
    if(r == 0x00 && g == 0x57 && b == 0xA8) return 0x11; 
    if(r == 0x00 && g == 0xFF && b == 0x00) return 0x12;
    if(r == 0x00 && g == 0xFF && b == 0xA8) return 0x13; 
    if(r == 0xA8 && g == 0x57 && b == 0x00) return 0x14; 
    if(r == 0xA8 && g == 0x57 && b == 0xA8) return 0x15; 
    if(r == 0xA8 && g == 0xFF && b == 0x00) return 0x16; 
    if(r == 0xA8 && g == 0xFF && b == 0xA8) return 0x17; 
    if(r == 0x00 && g == 0x57 && b == 0x57) return 0x18;
    if(r == 0x00 && g == 0x57 && b == 0xFF) return 0x19; 
    if(r == 0x00 && g == 0xFF && b == 0x57) return 0x1A; 
    if(r == 0x00 && g == 0xFF && b == 0xFF) return 0x1B; 
    if(r == 0xA8 && g == 0x57 && b == 0x57) return 0x1C; 
    if(r == 0xA8 && g == 0x57 && b == 0xFF) return 0x1D; 
    if(r == 0xA8 && g == 0xFF && b == 0x57) return 0x1E; 
    if(r == 0xA8 && g == 0xFF && b == 0xFF) return 0x1F; 


    if(r == 0x57 && g == 0x00 && b == 0x00) return 0x20; 
    if(r == 0x57 && g == 0x00 && b == 0xA8) return 0x21;
    if(r == 0x57 && g == 0xA8 && b == 0x00) return 0x22; 
    if(r == 0x57 && g == 0xA8 && b == 0xA8) return 0x23;
    if(r == 0xFF && g == 0x00 && b == 0x00) return 0x24; 
    if(r == 0xFF && g == 0x00 && b == 0xA8) return 0x25; 
    if(r == 0xFF && g == 0xA8 && b == 0x00) return 0x26;
    if(r == 0xFF && g == 0xA8 && b == 0xA8) return 0x27; 
    if(r == 0x57 && g == 0x00 && b == 0x57) return 0x28; 
    if(r == 0x57 && g == 0x00 && b == 0xFF) return 0x29; 
    if(r == 0x57 && g == 0xA8 && b == 0x57) return 0x2A;
    if(r == 0x57 && g == 0xA8 && b == 0xFF) return 0x2B; 
    if(r == 0xFF && g == 0x00 && b == 0x57) return 0x2C; 
    if(r == 0xFF && g == 0x00 && b == 0xFF) return 0x2D; 
    if(r == 0xFF && g == 0xA8 && b == 0x57) return 0x2E; 
    if(r == 0xFF && g == 0xA8 && b == 0xFF) return 0x2F;

    if(r == 0x57 && g == 0x57 && b == 0x00) return 0x30; 
    if(r == 0x57 && g == 0x57 && b == 0xA8) return 0x31; 
    if(r == 0x57 && g == 0xFF && b == 0x00) return 0x32; 
    if(r == 0x57 && g == 0xFF && b == 0xA8) return 0x33; 
    if(r == 0xFF && g == 0x57 && b == 0x00) return 0x34; 
    if(r == 0xFF && g == 0x57 && b == 0xA8) return 0x35; 
    if(r == 0xFF && g == 0xFF && b == 0x00) return 0x36; 
    if(r == 0xFF && g == 0xFF && b == 0xA8) return 0x37; 
    if(r == 0x57 && g == 0x57 && b == 0x57) return 0x38; 
    if(r == 0x57 && g == 0x57 && b == 0xFF) return 0x39; 
    if(r == 0x57 && g == 0xFF && b == 0x57) return 0x3A; 
    if(r == 0x57 && g == 0xFF && b == 0xFF) return 0x3B; 
    if(r == 0xFF && g == 0x57 && b == 0x57) return 0x3C; 
    if(r == 0xFF && g == 0x57 && b == 0xFF) return 0x3D; 
    if(r == 0xFF && g == 0xFF && b == 0x57) return 0x3E; 
    if(r == 0xFF && g == 0xFF && b == 0xFF) return 0x3F; 
    return 0x00;
}
void VideoGraphicsArray::PutPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b)
{
    PutPixel(x,y, GetColorIndex(r,g,b));
}

void VideoGraphicsArray::FillRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b)
{
    for(int32_t Y = y; Y<y+h; ++Y)
        for(int32_t X = x; X < x+w; ++X)
            PutPixel(X,Y,r,g,b);
}
void VideoGraphicsArray::FillRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t colorIndex)
{
    for(int32_t Y = y; Y<y+h; ++Y)
        for(int32_t X = x; X < x+w; ++X)
             PutPixel(X,Y,colorIndex);
}
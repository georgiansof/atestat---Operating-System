#include <filesystem/filemanager.h>

using namespace blockos;
using namespace blockos::drivers;
using namespace blockos::common;
using namespace blockos::filesystem;

void wait(float s);
void printf(uint32_t);
void printf(char);

FileManager::FileManager(AdvancedTechnologyAttachment *hdd, uint64_t size)
{
    this->hdd=hdd;
    this->size=size;
}

void FileManager::Format()
{
    uint8_t buffer[512];
    for(int i=0; i<512; ++i)
         buffer[i] = 0;
    
    for(uint64_t SIZE=this->size-1,i=0; SIZE > 0; SIZE-=512,++i)
    {
        hdd->Write28(0+i*512,buffer,512); 
        printf((uint32_t)i);
        printf('\n');
        //wait(1);
        hdd->Flush();
       // wait(1);
    }
}
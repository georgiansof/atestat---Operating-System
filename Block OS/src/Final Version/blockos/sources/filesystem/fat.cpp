 
#include <filesystem/fat.h>

using namespace blockos;
using namespace blockos::common;
using namespace blockos::filesystem;
using namespace blockos::drivers;


void printf(const char*);
void printf(uint32_t);
void printfHex(uint8_t);
bool isalphanum(char c);

bool streq(char* a, char*b)
{
    for(int i=0;i<8;++i)
        if(isalphanum(a[i]) && isalphanum(b[i]))
                if(a[i]!=b[i]) return false;
                else ;
        else if(!isalphanum(a[i]) && !isalphanum(b[i])) break;
                else return false;
    return true;
}

void StringAppend(char* data,char* buffer,int &dataLen,int maxLen,int offset)
{
    for(int j=offset; dataLen < maxLen && (j==0 || (j>0 && buffer[j-1]!='\0'));++dataLen, ++j)
        data[dataLen]=buffer[j];
}

FileManager::FileManager(AdvancedTechnologyAttachment *hd, uint32_t partitionOffset)
{
    this->hd=hd;
    this->partitionOffset=partitionOffset;
}

void FileManager::ReadFile(char* fileName, int offset, char* data, int count) // count = -512 = entire file from offset ahead
{
    int dataLen = 0;
    BiosParameterBlock32 bpb;
    hd->Read28(partitionOffset, (uint8_t*)&bpb, sizeof(BiosParameterBlock32));

    
    //printf("sectors per cluster: ");
   // printfHex(bpb.sectorsPerCluster);
   // printf("\n");
    
    
    uint32_t fatStart = partitionOffset + bpb.reservedSectors;
    uint32_t fatSize = bpb.tableSize;
    
    uint32_t dataStart = fatStart + fatSize*bpb.fatCopies;
    
    uint32_t rootStart = dataStart + bpb.sectorsPerCluster*(bpb.rootCluster - 2);
    
    
    DirectoryEntryFat32 dirent[bpb.rootDirEntries+2];
    hd->Read28(rootStart, (uint8_t*)&dirent[0], bpb.rootDirEntries*sizeof(DirectoryEntryFat32));
    
    for(int i = 0; i < bpb.rootDirEntries; i++)
    {
        if(streq((char*)dirent[i].name,fileName)!=0)
            continue;
        
        if((dirent[i].attributes & 0x0F) == 0x0F)
            continue;

        char* foo = "             ";
        int j = 0;
        for(; j < 8 && isalphanum(dirent[i].name[j]); j++)
            foo[j] = dirent[i].name[j];
        foo[j++]='.';
        for(int k=0; k<3; ++k,++j)
            foo[j]=dirent[i].ext[k];
        foo[j]='\n';
        foo[++j]=0;
        //printf(foo);
        
        
        if((dirent[i].attributes & 0x10) == 0x10) // directory
        // ReadDirectory(); TODO
            continue;
        
        uint32_t firstFileCluster = ((uint32_t)dirent[i].firstClusterHi) << 16
                             | ((uint32_t)dirent[i].firstClusterLow);
        
        
                             
                             
        int32_t SIZE = dirent[i].size;
        int32_t nextFileCluster = firstFileCluster;
        uint8_t buffer[513];
        uint8_t fatbuffer[513];
        
        
        while(SIZE > 0)
        {
            uint32_t fileSector = dataStart + bpb.sectorsPerCluster * (nextFileCluster-2);
            int sectorOffset = 0;
        
            for(; SIZE > 0; SIZE -= 512)
            {
                hd->Read28(fileSector+sectorOffset, buffer, 512);
                
                buffer[SIZE > 512 ? 512 : SIZE] = '\0';
                if(offset < 512) 
                {
                    StringAppend(data,(char*)buffer,dataLen,count,offset);
                    offset=0;
                }
                
                if(++sectorOffset > bpb.sectorsPerCluster)
                    break;
            }
            
            uint32_t fatSectorForCurrentCluster = nextFileCluster / (512/sizeof(uint32_t));
            hd->Read28(fatStart+fatSectorForCurrentCluster, fatbuffer, 512);
            uint32_t fatOffsetInSectorForCurrentCluster = nextFileCluster % (512/sizeof(uint32_t));
            nextFileCluster = ((uint32_t*)&fatbuffer)[fatOffsetInSectorForCurrentCluster] & 0x0FFFFFFF;
        }
        
    }
}
void FileManager::WriteToFile(char* fileName, int offset, char* data, int count)
{

}
void FileManager::AppendToFile(char* fileName, char* data, int count)
{

}

void blockos::filesystem::ReadBiosBlock(blockos::drivers::AdvancedTechnologyAttachment *hd, common::uint32_t partitionOffset)
{
    BiosParameterBlock32 bpb;
    hd->Read28(partitionOffset, (uint8_t*)&bpb, sizeof(BiosParameterBlock32));
    
    //printf("sectors per cluster: ");
   // printfHex(bpb.sectorsPerCluster);
   // printf("\n");
    
    
    uint32_t fatStart = partitionOffset + bpb.reservedSectors;
    uint32_t fatSize = bpb.tableSize;
    
    uint32_t dataStart = fatStart + fatSize*bpb.fatCopies;
    
    uint32_t rootStart = dataStart + bpb.sectorsPerCluster*(bpb.rootCluster - 2);
    
    
    DirectoryEntryFat32 dirent[32];
    hd->Read28(rootStart, (uint8_t*)&dirent[0], 32*sizeof(DirectoryEntryFat32));
    for(int i = 0; i < 32; i++)
    {
        if(dirent[i].name[0] == 0x00)
            continue;
        
        if((dirent[i].attributes & 0x0F) == 0x0F)
            continue;

        char* foo = "             ";
        int j = 0;
        for(; j < 8 && isalphanum(dirent[i].name[j]); j++)
            foo[j] = dirent[i].name[j];
        foo[j++]='.';
        for(int k=0; k<3; ++k,++j)
            foo[j]=dirent[i].ext[k];
        foo[j]='\n';
        foo[++j]=0;
        printf(foo);
        
        
        if((dirent[i].attributes & 0x10) == 0x10) // directory
        // ReadDirectory(); TODO
            continue;
        
        uint32_t firstFileCluster = ((uint32_t)dirent[i].firstClusterHi) << 16
                             | ((uint32_t)dirent[i].firstClusterLow);
        
        
                             
                             
        int32_t SIZE = dirent[i].size;
        int32_t nextFileCluster = firstFileCluster;
        uint8_t buffer[513];
        uint8_t fatbuffer[513];
                             
        
        while(SIZE > 0)
        {
            uint32_t fileSector = dataStart + bpb.sectorsPerCluster * (nextFileCluster-2);
            int sectorOffset = 0;
        
            for(; SIZE > 0; SIZE -= 512)
            {
                hd->Read28(fileSector+sectorOffset, buffer, 512);
                
                buffer[SIZE > 512 ? 512 : SIZE] = '\0';
                printf((char*)buffer);
                
                if(++sectorOffset > bpb.sectorsPerCluster)
                    break;
            }
            
            uint32_t fatSectorForCurrentCluster = nextFileCluster / (512/sizeof(uint32_t));
            hd->Read28(fatStart+fatSectorForCurrentCluster, fatbuffer, 512);
            uint32_t fatOffsetInSectorForCurrentCluster = nextFileCluster % (512/sizeof(uint32_t));
            nextFileCluster = ((uint32_t*)&fatbuffer)[fatOffsetInSectorForCurrentCluster] & 0x0FFFFFFF;
        }
        
    }
}

#include <filesystem/msdospart.h>
#include <filesystem/fat.h>

using namespace blockos;
using namespace blockos::common;
using namespace blockos::drivers;
using namespace blockos::filesystem;
 

void printf(const char*);
void printfHex(uint8_t);

void MSDOSPartitionTable::ReadPartitions(blockos::drivers::AdvancedTechnologyAttachment *hd)
{
    
    MasterBootRecord mbr;
    
    //printf("MBR: ");
    
    hd->Read28(0, (uint8_t*)&mbr, sizeof(MasterBootRecord));

    /*
    for(int i = 0x1BE; i <= 0x01FF; i++)
    {
        printfHex(((uint8_t*)&mbr)[i]);
        printf(" ");
    }
    printf("\n");
    */
    
    if(mbr.magicnumber != 0xAA55)
    {
        printf("illegal MBR");
        return;
    }
    
    
    for(int i = 0; i < 4; i++)
    {
        if(mbr.primaryPartition[i].partition_id == 0x00)
            continue;
        
       // printf(" Partition ");
       // printfHex(i & 0xFF);
        
        
        //if(mbr.primaryPartition[i].bootable == 0x80)
        //    printf(" bootable. Type");
       // else
       //     printf(" not bootable. Type ");
        
        
        //printfHex(mbr.primaryPartition[i].partition_id);
        
     
        ReadBiosBlock(hd, mbr.primaryPartition[i].start_lba);
    }
    
}

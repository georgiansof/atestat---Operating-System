#ifndef __BLOCKOS__FILESYSTEM__FILEMANAGER_H
#define __BLOCKOS__FILESYSTEM__FILEMANAGER_H

#include <common/types.h>
#include <drivers/ata.h>

namespace blockos
{
    namespace filesystem
    {
        class FileManager
        {
            private:
                drivers::AdvancedTechnologyAttachment *hdd;
            public:
                FileManager(drivers::AdvancedTechnologyAttachment *hdd);
                void Format();
        };
    }
}

#endif

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
                common::uint64_t size;
            public:
                FileManager(drivers::AdvancedTechnologyAttachment *hdd, common::uint64_t size);
                void Format();
        };
    }
}

#endif

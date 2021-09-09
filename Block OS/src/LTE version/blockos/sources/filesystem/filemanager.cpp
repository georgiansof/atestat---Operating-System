#include <filesystem/filemanager.h>

using namespace blockos;
using namespace blockos::drivers;
using namespace blockos::common;
using namespace blockos::filesystem;

FileManager::FileManager(AdvancedTechnologyAttachment *hdd)
{
    this->hdd=hdd;
}

void FileManager::Format()
{
    
}
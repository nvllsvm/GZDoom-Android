#include <android/asset_manager.h>
#include <stdint.h>
#include <vector>
#include <stdio.h>


class PageData
{
public:

    bool inUse;
    char* data;

    PageData( uint32_t size )
    {
        inUse = false;
        data = new char[size];
    }

    ~PageData()
    {
        delete[] data;
    }
};

class Page
{
public:
    uint32_t lastAccess;
    PageData *pageData;

    Page()
    {
        lastAccess = 0;
        pageData = NULL;
    }

    ~Page()
    {

    }
};


class AssetFileCache
{

public:
    AssetFileCache(  AAsset* asset );
    ~AssetFileCache();

    uint32_t read( char* buf, int size );
    uint32_t seek( fpos_t offset, int origin );

private:

    AAsset* asset;

    uint32_t fileSize;
    uint32_t pageSize;
    uint32_t nbrPageData;
    uint32_t totalPages;

    uint32_t time;

    fpos_t pos;

    std::vector<Page*> pages;

    std::vector<PageData*> pageData;

    PageData* getPage();

    uint32_t posToPageNbr( fpos_t pos );

    uint32_t readDataFromPage( char* buf, uint32_t size );
};
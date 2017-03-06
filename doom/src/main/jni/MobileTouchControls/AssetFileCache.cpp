#include "AssetFileCache.h"
#include <string>

#include <android/log.h>


#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,"AssetFileCache", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AssetFileCache", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,"AssetFileCache", __VA_ARGS__))


AssetFileCache::AssetFileCache ( AAsset* asset ) : asset ( asset )
{
    fileSize = AAsset_getLength ( asset );

    pageSize = 1024 * 1024;
    nbrPageData = 8;

    pos = 0;

    uint32_t totalPages = fileSize / pageSize;
    if ( fileSize % pageSize != 0 )
    {
        totalPages++;
    }

    this->totalPages = totalPages;

    if ( totalPages < nbrPageData )
    {
        nbrPageData = totalPages;
    }

    for ( int32_t n = 0; n < totalPages; n++ )
    {
        pages.push_back ( new Page() );
    }

    for ( int32_t n = 0; n < nbrPageData; n++ )
    {
        pageData.push_back ( new PageData ( pageSize ) );
    }

    LOGI ( "Created, asset = %p, fileSize = %d, totalPages = %d, nbrPageData = %d", asset, fileSize, totalPages, nbrPageData );
}

AssetFileCache::~AssetFileCache()
{
    AAsset_close ( asset );

    for ( int32_t n = 0; n < nbrPageData; n++ )
    {
        delete pageData[n];
    }

    for ( int32_t n = 0; n < totalPages; n++ )
    {
        delete pages[n];
    }
}


uint32_t AssetFileCache::read ( char* buf, int size )
{
    // Out 'time' is incremented everytime we get a read
    time++;

    LOGI ( "READ %p pos = %ld, size = %d", asset, pos, size );

    uint32_t written = 0;
    while ( written < size )
    {
        //LOGI("Read size = %d", temp);
        written += readDataFromPage ( buf + written, size - written );
    }

    return size;
}

uint32_t AssetFileCache::seek ( fpos_t offset, int origin )
{
    // LOGI("seek %ld %d", offset, origin );
    switch ( origin )
    {
    case SEEK_SET:
        pos = offset;
        break;
    case SEEK_CUR:
        pos += offset;
        break;
    case SEEK_END:
        pos = fileSize;
    default:
        break;
    }

    return pos;
}

PageData*  AssetFileCache::getPage()
{
    PageData *data = NULL;

    // First see if any data not in use
    for ( int32_t n = 0; n < nbrPageData; n++ )
    {
        if ( pageData.at ( n )->inUse == false )
        {
            data = pageData.at ( n );
            // LOGI("getPage found unused at %d (%p)",n, data);
            pageData.at ( n )->inUse = true;
            break;
        }
    }

    // Find the oldest used page
    if ( data == NULL )
    {
        uint32_t oldestTime = 0xFFFFFFFF;
        Page *oldestPage = NULL;
        for ( int32_t n = 0; n < totalPages; n++ )
        {
            Page *page = pages[n];
            if ( page->pageData != NULL )
            {
                if ( page->lastAccess < oldestTime )
                {
                    oldestTime = page->lastAccess;
                    oldestPage = page;
                }
            }
        }

        //Unlink data from page
        data = oldestPage->pageData;
        oldestPage->pageData = NULL;
    }

    return data;

}

uint32_t AssetFileCache::posToPageNbr ( fpos_t pos )
{
    return pos / pageSize;
}

void printBytes ( char *buf, int size )
{
    for ( int n = 0; n < size; n++ )
    {
        LOGI ( "0x%02x", buf[n] );
    }
}

uint32_t AssetFileCache::readDataFromPage ( char* buf, uint32_t size )
{
    uint32_t maxFromPage = ( pageSize - ( pos % pageSize ) );

    uint32_t bytesToRead;

    if ( size > maxFromPage )
    {
        bytesToRead = maxFromPage;
    }
    else
    {
        bytesToRead = size;
    }

    uint32_t pageNbr = posToPageNbr ( pos );

    Page *page = pages[pageNbr];
    page->lastAccess = time;

    // Check if page is already cached
    if ( page->pageData )
    {
        LOGI ( "USING CACHE asset = %p, data = %p", asset, page->pageData->data );
    }
    else
    {

        // Get a data block and cache the data
        PageData *data = getPage();
        AAsset_seek ( asset, pageNbr * pageSize, SEEK_SET );
        AAsset_read ( asset, data->data, pageSize );
        LOGI ( "CACHE MISS asset = %p, Cached data at %p", asset,  data->data );
        page->pageData = data;
        //printBytes( data->data, 50 );
    }


    char * dataSource =  page->pageData->data + ( pos % pageSize );
    //LOGI("buf = %p, dataSource = %p, bytes = %d", buf, dataSource, bytesToRead);
    memcpy ( buf, dataSource, bytesToRead );

    // printBytes( buf, 50 );

    pos += bytesToRead;

    return bytesToRead;

}
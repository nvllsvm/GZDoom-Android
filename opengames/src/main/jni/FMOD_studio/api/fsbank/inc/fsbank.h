#ifndef _FSBANK_H
#define _FSBANK_H

#if defined(_WIN32)
    #define FB_API __declspec(dllexport) _stdcall
    #define FSBANK_CALLBACK _stdcall
#elif defined(__linux__) || defined(__MACH__)
    #define FB_API __attribute__ ((visibility("default")))
    #define FSBANK_CALLBACK
#else
    #define FB_API
    #define FSBANK_CALLBACK
#endif

/*
    FSBank types
*/
typedef unsigned int FSBANK_INITFLAGS;
typedef unsigned int FSBANK_BUILDFLAGS;


/*
[DEFINE]
[
    [NAME]
    FSBANK_INITFLAGS

    [DESCRIPTION]
    Bit fields to use with FSBank_Init to control the general operation of the library.

    [REMARKS]

    [SEE_ALSO]
    FSBank_Init
]
*/
#define FSBANK_INIT_NORMAL                  0x00000000  /* Initialize normally. */
#define FSBANK_INIT_IGNOREERRORS            0x00000001  /* Ignore individual subsound build errors, continue building for as long as possible. */
#define FSBANK_INIT_WARNINGSASERRORS        0x00000002  /* Treat any warnings issued as errors. */
#define FSBANK_INIT_CREATEINCLUDEHEADER     0x00000004  /* Create C header files with #defines defining indices for each member of the FSB. */
#define FSBANK_INIT_DONTLOADCACHEFILES      0x00000008  /* Ignore existing cache files. */
#define FSBANK_INIT_GENERATEPROGRESSITEMS   0x00000010  /* Generate status items that can be queried by another thread to monitor the build progress and give detailed error messages. */
/* [DEFINE_END] */


/*
[DEFINE]
[
    [NAME]
    FSBANK_BUILDFLAGS

    [DESCRIPTION]
    Bit fields to use with FSBank_Build and in FSBANK_SUBSOUND to control how subsounds are encoded.

    [REMARKS]

    [SEE_ALSO]
    FSBank_Init
    FSBANK_SUBSOUND
]
*/
#define FSBANK_BUILD_DEFAULT                0x00000000  /* Build with default settings. */
#define FSBANK_BUILD_DISABLESYNCPOINTS      0x00000001  /* Disable the storing of syncpoints in the output */
#define FSBANK_BUILD_DONTLOOP               0x00000002  /* Disable perfect loop encoding and sound stretching. Removes chirps from the start of oneshot MP2, MP3 and IMAADPCM sounds. */
#define FSBANK_BUILD_FILTERHIGHFREQ         0x00000004  /* XMA only. Enable high frequency filtering. */
#define FSBANK_BUILD_DISABLESEEKING         0x00000008  /* XMA only. Disable seek tables to save memory. */
#define FSBANK_BUILD_OPTIMIZESAMPLERATE     0x00000010  /* Attempt to optimize the sample rate down. Ignored if format is MP2, MP3 or CELT or if FSB4 basic headers flag is used. */
#define FSBANK_BUILD_DONTINTERLEAVE         0x00000020  /* VAG and GCADPCM only. Don't interleave the data as LRLRLRLR... instead use LLLLRRRR type encoding. Speeds up loading for samples (not streams, streams must remained interleaved), allows FMOD_OPENMEMORY_POINT. */
#define FSBANK_BUILD_FSB4_USEBASICHEADERS   0x00000040  /* FSB4 format only. Generate FSBs with small sample header data. They only contain basic information such as sample length, and everything else has its attributes inherited from the first sample (for example the default frequency). */
#define FSBANK_BUILD_FSB5_DONTWRITENAMES    0x00000080  /* FSB5 format only. Do not write out a names chunk to the FSB to reduce file size. */
#define FSBANK_BUILD_NOGUID                 0x00000100  /* FSB5 format only. Write out a null GUID for the FSB header.  The runtime will not use header caching for these FSB files. */
#define FSBANK_BUILD_WRITEPEAKVOLUME        0x00000200  /* FSB5 format only. Write peak volume for all subsounds. */

#define FSBANK_BUILD_OVERRIDE_MASK          (FSBANK_BUILD_DISABLESYNCPOINTS | FSBANK_BUILD_DONTLOOP | FSBANK_BUILD_FILTERHIGHFREQ | FSBANK_BUILD_DISABLESEEKING | FSBANK_BUILD_OPTIMIZESAMPLERATE | FSBANK_BUILD_DONTINTERLEAVE | FSBANK_BUILD_WRITEPEAKVOLUME) /* Build flag mask that specifies which settings can be overridden per subsound. */
#define FSBANK_BUILD_CACHE_VALIDATION_MASK  (FSBANK_BUILD_DONTLOOP | FSBANK_BUILD_FILTERHIGHFREQ | FSBANK_BUILD_OPTIMIZESAMPLERATE | FSBANK_BUILD_DONTINTERLEAVE) /* Build flag mask that specifies which settings (when changed) invalidate a cache file. */
/* [DEFINE_END] */


/*
[ENUM]
[
    [DESCRIPTION]
    Error codes returned from every function.

    [REMARKS]

    [SEE_ALSO]
]
*/
typedef enum FSBANK_RESULT
{
    FSBANK_OK,                              /* No errors. */
    FSBANK_ERR_CACHE_CHUNKNOTFOUND,         /* An expected chunk is missing from the cache, perhaps try deleting cache files. */
    FSBANK_ERR_CANCELLED,                   /* The build process was cancelled during compilation by the user. */
    FSBANK_ERR_CANNOT_CONTINUE,             /* The build process cannot continue due to previously ignored errors. */
    FSBANK_ERR_ENCODER,                     /* Encoder for chosen format has encountered an unexpected error. */
    FSBANK_ERR_ENCODER_INIT,                /* Encoder initialization failed. */
    FSBANK_ERR_ENCODER_NOTSUPPORTED,        /* Encoder for chosen format is not supported on this platform. */
    FSBANK_ERR_FILE_OS,                     /* An operating system based file error was encountered. */
    FSBANK_ERR_FILE_NOTFOUND,               /* A specified file could not be found. */
    FSBANK_ERR_FMOD,                        /* Internal error from FMOD sub-system. */
    FSBANK_ERR_INITIALIZED,                 /* Already initialized. */
    FSBANK_ERR_INVALID_FORMAT,              /* The format of the source file is invalid, see output for details. */
    FSBANK_ERR_INVALID_FORMAT_PCMBITS,      /* The format of the source file is invalid, the source bit formats (i.e. PCM8, PCM16, etc) must match for all sub-sounds */
    FSBANK_ERR_INVALID_PARAM,               /* An invalid parameter has been passed to this function. */
    FSBANK_ERR_MEMORY,                      /* Ran out of memory. */
    FSBANK_ERR_UNINITIALIZED,               /* Not initialized yet. */
    FSBANK_ERR_WRITER_FORMAT,               /* Chosen encode format is not supported by this FSB version. */
    FSBANK_WARN_CANNOTLOOP,                 /* Source file is too short for seamless looping. Looping disabled. */
    FSBANK_WARN_IGNORED_OPTIMIZESAMPLERATE, /* FSBANK_BUILD_OPTIMIZESAMPLERATE flag ignored: MP2, MP3 and CELT formats, and the USEBASICHEADERS flag make this option irrelevant. */
    FSBANK_WARN_IGNORED_FILTERHIGHFREQ,     /* FSBANK_BUILD_FILTERHIGHFREQ flag ignored: feature only supported by XMA format. */
    FSBANK_WARN_IGNORED_DISABLESEEKING,     /* FSBANK_BUILD_DISABLESEEKING flag ignored: feature only supported by XMA format. */
    FSBANK_WARN_IGNORED_DONTINTERLEAVE,     /* FSBANK_BUILD_DONTINTERLEAVE flag ignored: feature only supported by VAG and GCADPCM formats. */
    FSBANK_ERR_ENCODER_FILE_NOTFOUND,       /* External encoder dynamic library not found */
    FSBANK_ERR_ENCODER_FILE_BAD,            /* External encoder dynamic library could not be loaded, possibly incorrect binary format, incorrect architecture, file corruption */
} FSBANK_RESULT;


/*
[ENUM]
[
    [DESCRIPTION]
    Compression formats available for encoding

    [REMARKS]

    [SEE_ALSO]
    FSBank_Build
]
*/
typedef enum FSBANK_FORMAT
{
    FSBANK_FORMAT_PCM,              /* PCM                                 (1:1)   All platforms. */
    FSBANK_FORMAT_PCM_BIGENDIAN,    /* PCM Big Endian                      (1:1)   Xbox360 and PS3 only. */
    FSBANK_FORMAT_IMAADPCM,         /* IMA ADPCM                           (3.5:1) All platforms. */
    FSBANK_FORMAT_MP2,              /* MPEG Layer 2                        (CBR)   All platforms except PS3.            Depends on toolame. */
    FSBANK_FORMAT_MP3,              /* MPEG Layer 3                        (CBR)   All platforms.                       Depends on libmp3lame. */
    FSBANK_FORMAT_XMA,              /* XMA                                 (VBR)   Xbox360 / XboxOne only (hardware).   Depends on xmaencoder. */
    FSBANK_FORMAT_CELT,             /* Constrained Energy Lapped Transform (CBR)   All platforms.                       Depends on celt_encoder. */
    FSBANK_FORMAT_AT9_PSVITA,       /* ATRAC9                              (CBR)   PSVita only (hardware).              Depends on libatrac9. */
    FSBANK_FORMAT_AT9_PS4,          /* ATRAC9                              (CBR)   PS4 only (hardware).                 Depends on libatrac9. */
    FSBANK_FORMAT_XWMA,             /* XWMA                                (VBR)   Xbox360 only.                        Depends on xwmaencoder. */
    FSBANK_FORMAT_VORBIS,           /* Vorbis                              (VBR)   All platforms.                       Depends on libvorbis. */

    FSBANK_FORMAT_MAX               /* Upper bound for this enumeration, for use with validation. */
} FSBANK_FORMAT;


/*
[ENUM]
[
    [DESCRIPTION]
    Version of FSB to write out.

    [REMARKS]

    [SEE_ALSO]
    FSBank_Init
]
*/
typedef enum FSBANK_FSBVERSION
{
    FSBANK_FSBVERSION_FSB5,         /* Produce FSB version 5 files. */

    FSBANK_FSBVERSION_MAX           /* Upper bound for this enumeration, for use with validation. */
} FSBANK_FSBVERSION;


/*
[ENUM]
[
    [DESCRIPTION]
    Speaker maps to define the layout of multichannel subsounds.

    [REMARKS]

    [SEE_ALSO]
    FSBANK_SUBSOUND
]
*/
typedef enum FSBANK_SPEAKERMAP
{
    FSBANK_SPEAKERMAP_DEFAULT,      /* Sample uses default FMOD speaker mapping. */
    FSBANK_SPEAKERMAP_ALLMONO,      /* Sample is a collection of mono channels. */
    FSBANK_SPEAKERMAP_ALLSTEREO,    /* Sample is a collection of stereo channel pairs. */
    FSBANK_SPEAKERMAP_PROTOOLS,     /* Sample is 6ch and uses L C R LS RS LFE standard. */   

    FSBANK_SPEAKERMAP_MAX           /* Upper bound for this enumeration, for use with validation. */
} FSBANK_SPEAKERMAP;


/*
[ENUM]
[
    [DESCRIPTION]
    Current state during the build process.

    [REMARKS]

    [SEE_ALSO]
    FSBANK_PROGRESSITEM
]
*/
typedef enum FSBANK_STATE
{
    FSBANK_STATE_DECODING,      /* Decode a file to usable raw sample data. */
    FSBANK_STATE_ANALYSING,     /* Scan sound data for details (such as optimized sample rate). */
    FSBANK_STATE_PREPROCESSING, /* Prepares sound data for encoder. */ 
    FSBANK_STATE_ENCODING,      /* Pass the sample data to the chosen encoder. */
    FSBANK_STATE_WRITING,       /* Write encoded data into an FSB. */
    FSBANK_STATE_FINISHED,      /* Process complete. */
    FSBANK_STATE_FAILED,        /* An error has occurred, check data (as FSBANK_STATEDATA_FAILED) for details. */
    FSBANK_STATE_WARNING,       /* A warning has been issued, check data (as FSBANK_STATEDATA_WARNING) for details. */
} FSBANK_STATE;


/*
[STRUCTURE]
[
    [DESCRIPTION]
    Representation of how to encode a single subsound in the final FSB.

    [REMARKS]

    [SEE_ALSO]
    FSBank_Build
    FSBANK_BUILD_OPTIMIZESAMPLERATE
    FSBANK_SPEAKERMAP
    FSBANK_BUILDFLAGS
]
*/
typedef struct FSBANK_SUBSOUND
{
    const char* const  *fileNames;              /* List of file names used to produce an interleaved sound. */
    unsigned int        numFileNames;           /* Number of files in above file name list, up to 16. */
    FSBANK_SPEAKERMAP   speakerMap;             /* Setting to define the mapping and order of channels. */
    FSBANK_BUILDFLAGS   overrideFlags;          /* Flags that will reverse the equivalent flags passed to FSBank_Build. */
    unsigned int        overrideQuality;        /* Override the quality setting passed to FSBank_Build. */
    float               desiredSampleRate;      /* Resample to this sample rate (ignores optimize sample rate setting), up to 192000Hz. */
    float               percentOptimizedRate;   /* If using FSBANK_BUILD_OPTIMIZESAMPLERATE, this is the percentage of that rate to be used, up to 100.0%. */
} FSBANK_SUBSOUND;


/*
[STRUCTURE]
[
    [DESCRIPTION]
    Status information describing the progress of a build.

    [REMARKS]

    [SEE_ALSO]
    FSBank_Build
    FSBank_FetchNextProgressItem
    FSBank_ReleaseProgressItem
    FSBANK_STATE
]
*/
typedef struct FSBANK_PROGRESSITEM
{
    int             subSoundIndex;  /* Index into the subsound list passed to FSBank_Build that this update relates to (-1 indicates no specific subsound). */
    int             threadIndex;    /* Which thread index is serving this update (-1 indicates FSBank_Build / main thread). */
    FSBANK_STATE    state;          /* Progress through the encoding process. */
    const void     *stateData;      /* Cast to state specific data structure for extra information. */
} FSBANK_PROGRESSITEM;


/*
[STRUCTURE]
[
    [DESCRIPTION]
    Extra state data for FSBANK_STATE_FAILED

    [REMARKS]
    Cast stateData in FSBANK_PROGRESSITEM to this struct if the state is FSBANK_STATE_FAILED

    [SEE_ALSO]
    FSBANK_STATE_FAILED
    FSBANK_PROGRESSITEM
]
*/
typedef struct FSBANK_STATEDATA_FAILED
{
    FSBANK_RESULT errorCode;    /* Error result code. */
    char errorString[256];      /* Description for error code. */
} FSBANK_STATEDATA_FAILED;


/*
[STRUCTURE]
[
    [DESCRIPTION]
    Extra state data for FSBANK_STATEDATA_WARNING

    [REMARKS]
    Cast stateData in FSBANK_PROGRESSITEM to this struct if the state is FSBANK_STATE_WARNING

    [SEE_ALSO]
    FSBANK_STATE_WARNING
    FSBANK_PROGRESSITEM
]
*/
typedef struct FSBANK_STATEDATA_WARNING
{
    FSBANK_RESULT warnCode;     /* Warning result code. */
    char warningString[256];    /* Description for warning code. */
} FSBANK_STATEDATA_WARNING;


#ifdef __cplusplus
extern "C" {
#endif  

typedef void* (FSBANK_CALLBACK *FSBANK_MEMORY_ALLOC_CALLBACK)(unsigned int size, unsigned int, const char *sourceStr);
typedef void* (FSBANK_CALLBACK *FSBANK_MEMORY_REALLOC_CALLBACK)(void *ptr, unsigned int size, unsigned int, const char *sourceStr);
typedef void  (FSBANK_CALLBACK *FSBANK_MEMORY_FREE_CALLBACK)(void *ptr, unsigned int, const char *sourceStr);

FSBANK_RESULT FB_API FSBank_MemoryInit(FSBANK_MEMORY_ALLOC_CALLBACK userAlloc, FSBANK_MEMORY_REALLOC_CALLBACK userRealloc, FSBANK_MEMORY_FREE_CALLBACK userFree);
FSBANK_RESULT FB_API FSBank_Init(FSBANK_FSBVERSION version, FSBANK_INITFLAGS flags, unsigned int numSimultaneousJobs, const char *cacheDirectory);
FSBANK_RESULT FB_API FSBank_Release();
FSBANK_RESULT FB_API FSBank_Build(const FSBANK_SUBSOUND *subSounds, unsigned int numSubSounds, FSBANK_FORMAT encodeFormat, FSBANK_BUILDFLAGS buildFlags, unsigned int quality, const char *encryptKey, const char *outputFileName);
FSBANK_RESULT FB_API FSBank_BuildCancel();
FSBANK_RESULT FB_API FSBank_FetchNextProgressItem(const FSBANK_PROGRESSITEM **progressItem);
FSBANK_RESULT FB_API FSBank_ReleaseProgressItem(const FSBANK_PROGRESSITEM *progressItem);
FSBANK_RESULT FB_API FSBank_MemoryGetStats(unsigned int *currentAllocated, unsigned int *maximumAllocated);

#ifdef __cplusplus
}
#endif

#endif  // _FSBANK_H

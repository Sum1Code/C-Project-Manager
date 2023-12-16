#pragma once
#ifndef __CPM_AVAIL
#define __CPM_AVAIL
#define AUTO __auto_type // use at your own risk

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
// =========================== //
// ENUMS & STRUCTS DECLARATION //
// =========================== //

typedef struct ProtoStringType
{
    char *m_inner_ptr;
    size_t m_cap;
    size_t m_cursize;
} String_t;

typedef struct BuildProperties
{
    String_t compiler;
    String_t CompilerFlags;
    String_t buildPath;
    String_t sourcesPath;
    String_t includePath;
    String_t linkerFlags;
    String_t libraryPath;
    String_t additionalLibrary;
} BuildProperties_t;

typedef enum LOG_LEVEL
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
} logLevel_e;

typedef enum directory_operation
{
    DIR_CREATE,
    DIR_DELETE
} dirOps_e;


typedef struct STATUS
{
    bool message_available;
    char* message;
    int code; 
} status_t;

// =========================== //
//      STRING FUNCTIONS       //
// =========================== //

String_t string_from_cstr(const char* cstr)
{
    String_t res; 
    size_t strsize = strlen(cstr);
    res.m_cap = strsize + 5;
    res.m_inner_ptr = (char*) malloc(res.m_cap);
    strcpy(res.m_inner_ptr, cstr);
    res.m_cursize = strsize;
    return res;
}

status_t append_cstr(String_t* str, const char* cstr)
{
    size_t cstrsize = strlen(cstr);
    if(str->m_cap < str->m_cursize + cstrsize)
    {
        str->m_cap *= 2;
    
        while(str->m_cap < str->m_cursize + cstrsize)
        {
            str->m_cap *= 2;
        }
        char* new_inner = (char*) realloc(str->m_inner_ptr, str->m_cap);
        if (!new_inner)
        {
            
        }

    }
}


/* PRIMARY FUNCTIONS*/

void cpm_configure_compiler(BuildProperties_t *bp, const char *compiler, const char *sources, 
                            const char *build_path, const char *name, const char *extra_compiler_flags)
{

}
                
void cpm_configure_linker(BuildProperties_t *bp, const char *Include_path, const char *library_path, 
                          const char *additional_library, const char *linker_flags)
{

}
void cpm_build(BuildProperties_t *bp)
{

}
void cpm_build_async(BuildProperties_t *bp)
{

}
void cpm_build_async_poll(BuildProperties_t *bp)
{

}



/* MISC FUNCTIONS*/
void cpm_log(logLevel_e loglvl, const char *message, ...);

/* FILE & DIRECTORY OPERATIONS */
void dir_ops(dirOps_e directory_operations, const char *dir_name);

#endif

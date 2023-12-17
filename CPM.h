#pragma once
#ifndef __CPM_AVAIL
#define __CPM_AVAIL
#define NOT_IMPLEMENTED  cpm_log(CPM_LOG_ERROR, "%s is not implemented yet!\n", __func__);\
                         exit(1); 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#define KERR "\x1B[31m" // RED
#define KWAR "\x1B[33m" // YELLOW
#define KMSG "\x1B[35m" // MAGENTA
#define KNRM "\x1B[0m"  // NORMAL COLOR

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
    String_t object_name;
    String_t linker;
    bool compiler_configured;
    bool linker_configured;
} BuildProperties_t;

typedef enum LOG_LEVEL
{
    CPM_LOG_INFO,
    CPM_LOG_WARNING,
    CPM_LOG_ERROR,
} logLevel_e;

typedef enum directory_operation
{
    DIR_CREATE,
    DIR_DELETE
} dirOps_e;

// =========================== //
//      STRING FUNCTIONS       //
// =========================== //

String_t string_from_cstr(const char *cstr)
{
    String_t res;
    size_t strsize = strlen(cstr);
    res.m_cap = strsize + 5;
    res.m_inner_ptr = (char *)calloc(1, res.m_cap);
    strcpy(res.m_inner_ptr, cstr);
    res.m_cursize = strsize;
    return res;
}

String_t string_new()
{
    String_t res;
    res.m_cursize = 0;
    res.m_cap = 0;
    res.m_inner_ptr = NULL;
    return res;
}

String_t string_cpy(String_t* str)
{
    String_t res; 
    res.m_cursize = str->m_cursize;
    res.m_cap = str->m_cap;
    res.m_inner_ptr = (char*) malloc(str->m_cap);
    strcpy(res.m_inner_ptr, str->m_inner_ptr);
    return res;
}

const char *string_get(String_t *string)
{
    return string->m_inner_ptr;
}

void string_append_cstr(String_t *str, const char *cstr, bool append_space_ending)
{

    if (NULL == str->m_inner_ptr)
    {
        str->m_cap = 5;
        str->m_inner_ptr = (char*) calloc(1, str->m_cap);
    }
    size_t cstrsize = strlen(cstr);
    char *new_inner = NULL;
    if (str->m_cap < str->m_cursize + cstrsize + 2)
    {

        while (str->m_cap < str->m_cursize + cstrsize + 2)
        {
            str->m_cap += 5;
        }
        new_inner = (char *)realloc(str->m_inner_ptr, str->m_cap);
        if (!new_inner)
        {
            perror("String Error, failed to append to string_t: NULL POINTER");
            exit(1);
        }
        str->m_inner_ptr = new_inner;
    }
    strcat(str->m_inner_ptr, cstr);
    str->m_cursize += cstrsize;
    if (append_space_ending)
    {
        strcat(str->m_inner_ptr, " ");
        str->m_cursize += 1;
    }
}

/* MISC FUNCTIONS*/
void cpm_log(logLevel_e loglvl, const char *fmt, ...)
{
    time_t current_time;
    struct tm *time_info;

    time(&current_time);
    time_info = localtime(&current_time);

    String_t modstr = string_new();
    char timestr[128];
    snprintf(timestr, 128, "(%02d:%02d:%02d)", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    switch (loglvl)
    {
    case CPM_LOG_INFO:
        string_append_cstr(&modstr, KMSG, false);
        string_append_cstr(&modstr, "LOG", true);
        string_append_cstr(&modstr, timestr, false);
        string_append_cstr(&modstr, ":", true);
        fprintf(stdout, "%s", string_get(&modstr));
        break;
    case CPM_LOG_WARNING:
        string_append_cstr(&modstr, KWAR, false);
        string_append_cstr(&modstr, "WARNING", true);
        string_append_cstr(&modstr, timestr, false);
        string_append_cstr(&modstr, ":", true);
        fprintf(stdout, "%s", string_get(&modstr));
        break;
    case CPM_LOG_ERROR:
        string_append_cstr(&modstr, KERR, false);
        string_append_cstr(&modstr, "ERROR", true);
        string_append_cstr(&modstr, timestr, false);
        string_append_cstr(&modstr, ":", true);
        fprintf(stdout, "%s", string_get(&modstr));
        break;

    default:
        break;
    }
    va_list args;
    va_start(args, fmt);

    vfprintf(stdout, fmt, args);
    va_end(args);
    fprintf(stdout, "%s", KNRM);
    free(modstr.m_inner_ptr);
}

// =========================== //
//      PRIMARY FUNCTIONS      //
// =========================== //


void cpm_configure_compiler(BuildProperties_t *bp, const char *compiler, const char *sources,
                            const char *build_path, const char *name, const char *extra_compiler_flags)
{
    

    if (NULL != build_path) bp->buildPath = string_from_cstr(build_path);
    else bp->buildPath = string_from_cstr(".");
    if (NULL != extra_compiler_flags) bp->CompilerFlags = string_from_cstr(extra_compiler_flags);
    else bp->CompilerFlags =  string_new();
    
    if(NULL != compiler || NULL != sources || NULL != name)
    {
        bp->compiler = string_from_cstr(compiler);
        bp->sourcesPath = string_from_cstr(sources);
        bp->object_name = string_from_cstr(name);
    }
    else
    {
        cpm_log(CPM_LOG_ERROR, "compiler, sourcesPath, and object_name should not be NULL\n");
        exit(1);
    }
    bp->compiler_configured = true;
}

void cpm_configure_linker(BuildProperties_t *bp, const char* linker,  const char *Include_path, const char *library_path,
                          const char *additional_library, const char *linker_flags)
{
    if (!bp->compiler_configured){
        cpm_log(CPM_LOG_ERROR, "please run cpm_configure_compiler before cpm_configure_linker\n");
        exit(1);
    }
    if(NULL == linker) // if the linker is NULL then use the compiler as linker 
        bp->linker = string_cpy(&bp->compiler);
    else bp->linker = string_from_cstr(linker); 
   
    if(NULL == Include_path)
        bp->includePath = string_new();
    else bp->includePath = string_from_cstr(Include_path);

    if(NULL == additional_library)
        bp->additionalLibrary = string_new();
    else bp->additionalLibrary = string_from_cstr(additional_library);

    if(NULL == library_path)
        bp->libraryPath = string_new();
    else bp->libraryPath = string_from_cstr(library_path);

    if(NULL == linker_flags)
        bp->linkerFlags = string_new();
    else bp->linkerFlags = string_from_cstr(linker_flags);

    bp->linker_configured = true;
}

/*
    Consumes the bp passed onto it and free it.
*/
void cpm_build(BuildProperties_t *bp)
{
    String_t compile = string_new();
    string_append_cstr(&compile, string_get(&bp->compiler), true);
    string_append_cstr(&compile, "-c", true);
}
void cpm_build_async(BuildProperties_t *bp)
{
    
}
void cpm_build_async_poll(BuildProperties_t *bp)
{
}

/* FILE & DIRECTORY OPERATIONS */
void dir_ops(dirOps_e directory_operations, const char *dir_name);

#endif

/* MACROS */
#define CPM_REBUILD_SELF(argc, argv)
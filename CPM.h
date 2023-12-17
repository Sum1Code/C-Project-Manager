#pragma once
#ifndef __CPM_AVAIL
#define __CPM_AVAIL
#define AUTO __auto_type // use at your own risk

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
    res.m_cap = 5;
    res.m_inner_ptr = (char *)calloc(1, res.m_cap);
    return res;
}

const char *string_get(String_t *string)
{
    return string->m_inner_ptr;
}

void string_append_cstr(String_t *str, const char *cstr, bool append_space_ending)
{
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

/* PRIMARY FUNCTIONS*/

void cpm_configure_compiler(BuildProperties_t *bp, const char *compiler, const char *sources,
                            const char *build_path, const char *name, const char *extra_compiler_flags)
{
    bp->compiler = string_from_cstr(compiler);
    bp->compiler = string_from_cstr(compiler);
    bp->compiler = string_from_cstr(compiler);
    bp->compiler = string_from_cstr(compiler);
    bp->compiler = string_from_cstr(compiler);
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

/* FILE & DIRECTORY OPERATIONS */
void dir_ops(dirOps_e directory_operations, const char *dir_name);

#endif

/* MACROS */
#define CPM_REBUILD_SELF(argc, argv)
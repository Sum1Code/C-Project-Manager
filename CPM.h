#pragma once
#ifndef CPM_AVAIL
#define CPM_AVAIL
#define NOT_IMPLEMENTED                                               \
    cpm_log(CPM_LOG_ERROR, "%s is not implemented yet!\n", __func__); \
    exit(1);

#define YES true
#define NO false
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <dirent.h>
#include <fnmatch.h>
#include <errno.h>
#include <unistd.h>

#include <sys/stat.h>

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

/*Immutable Iterator*/
typedef struct StringIterator
{
    char **str_array;
    size_t count;
    size_t cap;
    size_t index;
} StringIterator_t; 

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
    int compiler_configured;
    int linker_configured;
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
    DIR_DELETE,
    DIR_CHECK
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

String_t string_cpy(String_t *str)
{
    String_t res;
    res.m_cursize = str->m_cursize;
    res.m_cap = str->m_cap;
    res.m_inner_ptr = (char *)malloc(str->m_cap);
    strcpy(res.m_inner_ptr, str->m_inner_ptr);
    return res;
}

/*gets the inner string from string struct, note return value is supposed to be immutable*/
const char *string_get(String_t *string)
{
    return string->m_inner_ptr;
}

/*Frees the internal string, do not use string after it's freed*/
void string_free(String_t *string)
{
    string->m_cap = 0;
    string->m_cursize = 0;
    free(string->m_inner_ptr);
    string->m_inner_ptr = NULL;
}

void string_append_cstr(String_t *str, const char *cstr, bool append_space_ending)
{

    size_t cstrsize = strlen(cstr);
    char *new_inner = NULL;
    if (str->m_cap < str->m_cursize + cstrsize + 2)
    {

        while (str->m_cap < str->m_cursize + cstrsize + 2)
        {
            str->m_cap += 2;
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

StringIterator_t string_split_at(const String_t* input, const char* delimiter)
{
    StringIterator_t res;
    res.count = 0;
    res.cap = 5;
    res.index = 0;
    res.str_array = (char **) malloc(sizeof(char *) * 5);
    char* src_cpy = strdup(input->m_inner_ptr);
    char* token = strtok(src_cpy, delimiter);
    while (token){
        if(res.count == res.cap){
            res.cap += 5;
            res.str_array = realloc(res.str_array, res.cap * sizeof(char *));
        }
        ++res.count;
        res.str_array[res.count - 1 ] = (char *) malloc(strlen(token) * sizeof(char) + 1);
        res.str_array[res.count - 1 ] = strdup(token);
        token = strtok(NULL, delimiter);
    }               
    free(token);
    free(src_cpy);                  
    return res;
}
const char* string_iterate_next(StringIterator_t* iterator)
{
    if (iterator->index < iterator->count){
    iterator->index++;
    return iterator->str_array[iterator->index - 1];
    }
    return NULL;
}

void string_iterator_free(StringIterator_t* iterator)
{
    for(size_t x = 0; x < iterator->count; x++){
        free(iterator->str_array[x]);
    }
    free(iterator->str_array);
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
        string_append_cstr(&modstr, KMSG, NO);
        string_append_cstr(&modstr, "LOG", YES);
        string_append_cstr(&modstr, timestr, NO);
        string_append_cstr(&modstr, ":", YES);
        fprintf(stdout, "%s", string_get(&modstr));
        break;
    case CPM_LOG_WARNING:
        string_append_cstr(&modstr, KWAR, NO);
        string_append_cstr(&modstr, "WARNING", YES);
        string_append_cstr(&modstr, timestr, NO);
        string_append_cstr(&modstr, ":", YES);
        fprintf(stdout, "%s", string_get(&modstr));
        break;
    case CPM_LOG_ERROR:
        string_append_cstr(&modstr, KERR, NO);
        string_append_cstr(&modstr, "ERROR", YES);
        string_append_cstr(&modstr, timestr, NO);
        string_append_cstr(&modstr, ":", YES);
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

void cpm_configure_compiler(BuildProperties_t *bp, const char *compiler, const char *sources, const char *Include_path,
                            const char *build_path, const char *name, const char *extra_compiler_flags)
{

    if (NULL != build_path)
        bp->buildPath = string_from_cstr(build_path);
    else
        bp->buildPath = string_from_cstr(".");
    if (NULL != extra_compiler_flags)
        bp->CompilerFlags = string_from_cstr(extra_compiler_flags);
    else
        bp->CompilerFlags = string_new();

    if (NULL != compiler || NULL != sources || NULL != name)
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
    if (NULL == Include_path)
        bp->includePath = string_from_cstr("./");
    else
        bp->includePath = string_from_cstr(Include_path);
    bp->compiler_configured = YES;
}

void cpm_configure_linker(BuildProperties_t *bp, const char *linker, const char *library_path,
                          const char *additional_library, const char *linker_flags)
{
    if (!bp->compiler_configured)
    {
        cpm_log(CPM_LOG_ERROR, "please run cpm_configure_compiler before cpm_configure_linker\n");
        exit(1);
    }
    if (NULL == linker) // if the linker is NULL then use the compiler as linker
        bp->linker = string_cpy(&bp->compiler);
    else
        bp->linker = string_from_cstr(linker);

    if (NULL == additional_library)
        bp->additionalLibrary = string_new();
    else
        bp->additionalLibrary = string_from_cstr(additional_library);

    if (NULL == library_path)
        bp->libraryPath = string_from_cstr("./");
    else
        bp->libraryPath = string_from_cstr(library_path);

    if (NULL == linker_flags)
        bp->linkerFlags = string_new();
    else
        bp->linkerFlags = string_from_cstr(linker_flags);

    bp->linker_configured = YES;
}

/*
    Consumes the bp passed onto it and free it.
*/
void cpm_build(BuildProperties_t *bp)
{
    if (1 != bp->compiler_configured)
    {
        cpm_log(CPM_LOG_ERROR, "buildProperties hasn't been configured properly!\n");
        cpm_log(CPM_LOG_ERROR, "make sure to run cpm_configure_compiler!\n");
        exit(1);
    }
    String_t compile = string_new();
    string_append_cstr(&compile, string_get(&bp->compiler), YES);
    string_append_cstr(&compile, "-c", YES);
    string_append_cstr(&compile, string_get(&bp->CompilerFlags), YES);
    string_append_cstr(&compile, string_get(&bp->sourcesPath), YES);
    string_append_cstr(&compile, "-o", YES);
    string_append_cstr(&compile, string_get(&bp->buildPath), NO);
    string_append_cstr(&compile, "/", NO);
    string_append_cstr(&compile, string_get(&bp->object_name), NO);
    string_append_cstr(&compile, ".o", YES);
    cpm_log(CPM_LOG_INFO, "compiling: %s\n", string_get(&compile));
    if (system(string_get(&compile)))
    {
        cpm_log(CPM_LOG_ERROR, "Build command failed!\n");
        exit(1);
    }

    string_free(&compile);
}

void cpm_link(BuildProperties_t *bp)
{
    int compiler_is_linker = strcmp(bp->compiler.m_inner_ptr, bp->linker.m_inner_ptr);
#ifndef CPM_SUPPRESS_LINKER_WARNING
    if (0 != compiler_is_linker)
    {
        cpm_log(CPM_LOG_WARNING, "using custom linker, you will need to provide all extra arguments from the linker_flag\n");
        cpm_log(CPM_LOG_WARNING, "supress this warning by defining CPM_SUPPRESS_LINKER_WARNING before including CPM.h\n\n");
    }
#endif
    String_t link = string_new();
    string_append_cstr(&link, string_get(&bp->linker), YES);
    string_append_cstr(&link, string_get(&bp->buildPath), NO);
    string_append_cstr(&link, "/", NO);
    string_append_cstr(&link, string_get(&bp->object_name), NO);
    string_append_cstr(&link, ".o", YES);
    string_append_cstr(&link, "-o", YES);
    string_append_cstr(&link, string_get(&bp->buildPath), NO);
    string_append_cstr(&link, "/", NO);
    string_append_cstr(&link, string_get(&bp->object_name), YES);
    string_append_cstr(&link, "-L", NO);
    string_append_cstr(&link, string_get(&bp->libraryPath), YES);
    string_append_cstr(&link, string_get(&bp->additionalLibrary), YES);
    if (!compiler_is_linker)
    {
        string_append_cstr(&link, "-I", NO);
        string_append_cstr(&link, string_get(&bp->includePath), YES);
    }

    cpm_log(CPM_LOG_INFO, "linking: %s\n", string_get(&link));
    if (system(string_get(&link)))
    {
        cpm_log(CPM_LOG_ERROR, "Build command failed!\n");
        exit(1);
    }
    string_free(&link);
}

void cpm_buildprop_cleanup(BuildProperties_t *bp)
{
    string_free(&bp->additionalLibrary);
    string_free(&bp->buildPath);
    string_free(&bp->sourcesPath);
    string_free(&bp->compiler);
    string_free(&bp->CompilerFlags);
    string_free(&bp->includePath);
    string_free(&bp->libraryPath);
    string_free(&bp->linker);
    string_free(&bp->linkerFlags);
    string_free(&bp->object_name);
}

void cpm_build_async(BuildProperties_t *bp)
{
    NOT_IMPLEMENTED
}
void cpm_build_async_poll(BuildProperties_t *bp)
{
    NOT_IMPLEMENTED
}

// ============================== //
//      FILE & DIRECTORY OPS      //
// ============================== //

bool dir_ops(dirOps_e directory_operations, const char *dir_path)
{
    switch (directory_operations)
    {
    case DIR_CHECK:
    {
        DIR* opdir = opendir(dir_path);
            if(opdir)
            {
                closedir(opdir);
                return true;
            } else return false;
    }
        break;
    case DIR_CREATE:
    {
    struct stat st; 
    if(-1 == stat(dir_path, &st)){
        if(0 == mkdir(dir_path, 0700))
            return true;
        else return false;
    }}
    break;
    case DIR_DELETE:
    {
        String_t cmd = string_from_cstr("rmdir -rf ");
        string_append_cstr(&cmd, dir_path, NO);
        if(system(string_get(&cmd))){
            cpm_log(CPM_LOG_ERROR, "failed to delete directory");
        }
        string_free(&cmd);
    }
    default:
        break;
    }
    return false;
}

String_t dir_glob(const char *dir_path, const char *pattern)
{
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(dir_path)) == NULL)
    {
        cpm_log(CPM_LOG_ERROR, "Directory failed to open!\n");
        perror("reason:");
        exit(1);
    }
    String_t res = string_new();

    while ((entry = readdir(dir)) != NULL)
    {
        if (fnmatch(pattern, entry->d_name, 0) == 0)
        {
            string_append_cstr(&res, dir_path, NO);
            string_append_cstr(&res, "/", NO);
            string_append_cstr(&res, entry->d_name, YES);
        }
    }
    free(dir);
    free(entry);
    return res;
}

// ========================= //
//      GIT INTEGRATION      //
// ========================= //

void git_clone(const char *repo_url, const char *target_dir)
{
    String_t cmd = string_from_cstr("git ");
    string_append_cstr(&cmd, "clone", YES);
    string_append_cstr(&cmd, repo_url, YES);
    if(NULL != target_dir)
    {
        string_append_cstr(&cmd, target_dir, YES);
    } else
    {
        string_append_cstr(&cmd, ".", YES);
    }
    
    if(system(string_get(&cmd)))
    {
        cpm_log(CPM_LOG_WARNING, "Git failed to clone %s maybe the repo already existed!", repo_url);
    }
}

void git_clone_folder(const char *repo_url, const char *folder_name, bool clone_root)
{
    NOT_IMPLEMENTED;
}

// ========================== //
//     MODULE INTERGRATION    //
// ========================== //

/* MACROS */

bool cmp_file_modtime(const char *file1, const char *file2)
{
    struct stat oneInfo, twoInfo;
    if (-1 == stat(file1, &oneInfo))
    {
        cpm_log(CPM_LOG_ERROR, "Failed to stat file1 at %d\n", __LINE__);
        perror("stat: ");
        exit(1);
    }
    if (-1 == stat(file2, &twoInfo))
    {
        cpm_log(CPM_LOG_ERROR, "Failed to stat file2 at %d\n", __LINE__);
        perror("stat: ");
        exit(1);
    }

    return oneInfo.st_mtime > twoInfo.st_mtime;
}
#define GET_FILE_NAME(name) (strrchr(name, '/') ? strrchr(name, '/') + 1 : name)
#define CPM_REBUILD_SELF(argc, argv)                                                                      \
    if (cmp_file_modtime(__FILE__, argv[0]))                                                              \
    {                                                                                                     \
        BuildProperties_t _rebuild_prop;                                                                  \
        String_t _change_name_cmd = string_from_cstr("mv ");                                              \
        string_append_cstr(&_change_name_cmd, argv[0], YES);                                             \
        string_append_cstr(&_change_name_cmd, argv[0], NO);                                            \
        string_append_cstr(&_change_name_cmd, ".old", NO);                                             \
        cpm_log(CPM_LOG_WARNING, "changing %s to %s.old\n", argv[0], argv[0]);                            \
        cpm_log(CPM_LOG_INFO, "running: %s\n", string_get(&_change_name_cmd));                            \
        system(string_get(&_change_name_cmd));                                                            \
        cpm_configure_compiler(&_rebuild_prop, "cc", __FILE__, NULL, NULL, GET_FILE_NAME(argv[0]), NULL); \
        cpm_configure_linker(&_rebuild_prop, NULL, NULL, NULL, NULL);                                     \
        cpm_log(CPM_LOG_WARNING, "rebuilding %s\n", __FILE__);                                            \
        cpm_build(&_rebuild_prop);                                                                        \
        cpm_link(&_rebuild_prop);                                                                         \
        cpm_log(CPM_LOG_WARNING, "running new builder\n===========================================\n\n");   \
        if (system(argv[0]))                                                                              \
        {                                                                                                 \
            cpm_log(CPM_LOG_ERROR, "failed to run new builder!\n");                                       \
            exit(1);                                                                                      \
        }                                                                                                 \
        exit(0);                                                                                          \
    }
#endif // CPM_AVAIL
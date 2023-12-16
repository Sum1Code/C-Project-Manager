# CPM DOCUMENTATION

## STRUCTS:
```c
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

```

## ENUMS
```c
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
```

## FUNCTIONS
### WIP
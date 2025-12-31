#include "utils/utils.h"
#include "sds/sds.h"

sds read_entire_file(const char* filename)
{
    FILE* file = fopen(filename, "r");
    assert(file);

    sds content = sdsempty();
    char *line = NULL;
    size_t len = 0; // allocated buffer size, which may be larger than the line
    ssize_t nread; // number of characters actually read (including \n)

    while ((nread = getline(&line, &len, file)) != -1)
    {
        content = sdscatlen(content, line, nread);
    }

    free(line);
    fclose(file);

    return content;
}

// String_Builder read_entire_file(const char* filename)
// {
//     FILE* file = fopen(filename, "r");
//     assert(file);
//
//     String_Builder shader = {0};
//     char *line = NULL;
//     size_t len = 0; // allocated buffer size, which may be larger than the line
//     ssize_t nread; // number of characters actually read (including \n)
//
//     while ((nread = getline(&line, &len, file)) != -1)
//     {
//         for (size_t i = 0; i < (size_t)nread; i++)
//         {
//             daAppend(shader, line[i]);
//         }
//     }
//
//     // Null-terminate string
//     daAppend(shader, '\0');
//
//     free(line);
//     fclose(file);
//
//     return shader;
// }

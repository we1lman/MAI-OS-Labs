#ifndef PROCESS_HANDLER_H
#define PROCESS_HANDLER_H

#include <windows.h>

HANDLE create_named_pipe(const char *pipe_name);

PROCESS_INFORMATION create_child_process(const char *process_name, const char *pipe_name, const char *output_file);

#endif // PROCESS_HANDLER_H

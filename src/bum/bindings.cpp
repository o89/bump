#include "lean/object.h"
#include "lean/io.h"

#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define ERROR(val) return lean_io_result_mk_error(lean_mk_string(val))

typedef lean::object obj;

extern "C" obj* lean_io_set_env(obj* envname, obj* envval, obj* r) {
    auto res = setenv(lean::string_cstr(envname),
                      lean::string_cstr(envval), true);
    return lean_io_result_mk_ok(lean::box(res));
}

extern "C" obj* lean_io_chdir(obj* str, obj* r) {
    auto res = chdir(lean::string_cstr(str));
    return lean_io_result_mk_ok(lean::box_uint32(res));
}

extern "C" obj* lean_io_remove(obj* str, obj* r) {
    auto res = remove(lean::string_cstr(str));
    return lean_io_result_mk_ok(lean::box_uint32(res));
}

extern "C" obj* lean_get_last_write_time(obj* path, obj* r) {
    struct stat buff;
    switch (stat(lean::string_cstr(path), &buff)) {
        case EACCES:       ERROR("getLastWriteTime: permission denied");
        case EBADF:        ERROR("getLastWriteTime: invalid file descriptor");
        case EFAULT:       ERROR("getLastWriteTime: bad address");
        case ELOOP:        ERROR("getLastWriteTime: too many symbolic links encountered while traversing the path");
        case ENAMETOOLONG: ERROR("getLastWriteTime: pathname is too long");
        case ENOENT:       ERROR("getLastWriteTime: path not found");
        case ENOMEM:       ERROR("getLastWriteTime: out of memory");
        case ENOTDIR:      ERROR("getLastWriteTime: a component of the path prefix of pathname is not a directory");
        case EOVERFLOW:    ERROR("getLastWriteTime: descriptor overflow");
    }

    return lean_io_result_mk_ok(lean::box_uint64(buff.st_mtim.tv_sec));
}

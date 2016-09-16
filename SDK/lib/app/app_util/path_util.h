/*
 * path_util.h
 *
 *  Created on: Oct 6, 2014
 *      Author: Morgoth
 */

#ifndef PATH_UTIL_H_
#define PATH_UTIL_H_

#include "lib/string_lib.h"
//#######################################################
String_t *path_append_parse(String_t *path, String_t *rel_path);
//#######################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "path_util.c"
#endif
//#######################################################
#endif /* PATH_UTIL_H_ */

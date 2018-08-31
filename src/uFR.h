/*
 * uFR.h
 */

#ifndef UFR_H_
#define UFR_H_

#include "ini.h"
//------------------------------------------------------------------------------
typedef const char * sz_ptr;
//------------------------------------------------------------------------------
bool CheckDependencies(void);
sz_ptr GetDlTypeName(uint8_t dl_type_code);
//------------------------------------------------------------------------------

#endif /* UFR_H_ */

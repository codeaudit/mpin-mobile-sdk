#include "version.h"


/*! \brief Print version number and information about the build
 *
 *  Print version number and information about the build
 * 
 */
void version(char* info)
{
  sprintf(info,"Version: %d.%d.%d OS: %s FIELD CHOICE: %s CURVE TYPE: %s WORD_LENGTH: %d", CLINT_VERSION_MAJOR, CLINT_VERSION_MINOR, CLINT_VERSION_PATCH, OS, FIELD_CHOICE, CURVE_TYPE, CHUNK);
}


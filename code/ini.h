#ifndef _INI_H_
#define _INI_H_

char iniBuffer[2048];

void ini_write(STRING *filename, char* section, STRING *entry, STRING *value);
void ini_write_int(STRING *filename, char* section, STRING *entry, int value);
void ini_write_var(STRING *filename, char* section, STRING *entry, var value);
void ini_write_float(STRING *filename, char* section, STRING *entry, float value);
int ini_read_sections(TEXT *txt, STRING *filename);
int ini_read(STRING *targetValue, STRING *filename, char* section, STRING *entry, STRING *defaultValue);
int ini_read_int(STRING *filename, char* section, STRING *entry, int defaultValue);
var ini_read_var(STRING *filename, char* section, STRING *entry, var defaultValue);
float ini_read_float(STRING *filename, char* section, STRING *entry, float defaultValue);

#include "ini.c"

#endif /* _INI_H_ */
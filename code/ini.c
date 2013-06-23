#ifndef _INI_C_
#define _INI_C_

#include "ini.h"
#include <windows.h>

void ini_write(STRING *filename, char* section, STRING *entry, STRING *value)
{	
	WritePrivateProfileString(section, _chr(entry), _chr(value), _chr(filename));
}

void ini_write_int(STRING *filename, char* section, STRING *entry, int value)
{
	ini_write(filename, section, entry, str_for_int(NULL, value));
}

void ini_write_var(STRING *filename, char* section, STRING *entry, var value)
{
	ini_write(filename, section, entry, str_for_num(NULL, value));
}

void ini_write_float(STRING *filename, char* section, STRING *entry, float value)
{
	ini_write(filename, section, entry, str_for_float(NULL, value));
}

int ini_read_sections(TEXT *txt, STRING *filename)
{	
	int length = GetPrivateProfileString(NULL, NULL, NULL, iniBuffer, 2047, _chr(filename));
	int i = 0;
	char *str = iniBuffer;
	while(str < (iniBuffer + length))
	{
		(txt->pstring)[i] = str_create(str);
		str = str + strlen(str) + 1;
		i++;
	}
	return i;
}

int ini_read(STRING *targetValue, STRING *filename, char* section, STRING *entry, STRING *defaultValue)
{	
	int length = GetPrivateProfileString(section, _chr(entry), _chr(defaultValue), iniBuffer, 2047, _chr(filename));
	if(targetValue != NULL)
		str_cpy(targetValue, iniBuffer);
	return length;
}

int ini_read_int(STRING *filename, char* section, STRING *entry, int defaultValue)
{
	STRING *tmp = "#64";
	STRING *def = "#64";
	ini_read(tmp, filename, section, entry, str_for_int(def, defaultValue));
	return str_to_int(tmp);
}

var ini_read_var(STRING *filename, char* section, STRING *entry, var defaultValue)
{
	STRING *tmp = "#64";
	STRING *def = "#64";
	ini_read(tmp, filename, section, entry, str_for_num(def, defaultValue));
	return str_to_num(tmp);
}

float ini_read_float(STRING *filename, char* section, STRING *entry, float defaultValue)
{
	STRING *tmp = "#64";
	STRING *def = "#64";
	ini_read(tmp, filename, section, entry, str_for_float(def, defaultValue));
	return str_to_float(tmp);
}

#endif /* _INI_C_ */
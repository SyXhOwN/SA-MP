
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "../amx/amx.h"
#include "sqlite3.h"
#include "simplearray.h"

extern iDbLogging;
void logprintf(char* format, ...);

SimpleArray<sqlite3*> sqlite3Databases;

// native SQLiteDB:sqlite_open(name[]);
static cell AMX_NATIVE_CALL n_open(AMX* amx, cell* params)
{
	char *szDBName, szPathName[260];
	int errorCode, i;
	sqlite3 *sqlite=NULL;

	// Get the db filename
	amx_StrParam(amx, params[1], szDBName);

	if (szDBName == NULL || szDBName == "")
	{
		return 0;
	}

	// ensure the name doesn't start with a '/' or '\\' on win32
	if (szDBName[0] == '\\' || szDBName[0] == '/')
	{
		//logprintf(NATIVE_PREFIX "open - Warning: Database name can not start with %c", szDBName[0]);
		return 0;
	}

	// ensure there are no ".." in dbname, and no :
	for(i=1; szDBName[i]!=0; i++)
	{
		if (szDBName[i-1] == '.' && szDBName[i] == '.')
		{
			//logprintf(NATIVE_PREFIX "open - Warning: Database name can not contain '..'");
			return 0;
		}
	}

	// ensure there are no : in the file
	for(i=0; szDBName[i]!=0; i++)
	{
		if (szDBName[i] == ':')
		{
			//logprintf(NATIVE_PREFIX "open - Warning: Database name can not contain ':'");
			return 0;
		}
	}

	// Create the pathname string
	sprintf(szPathName, "scriptfiles/%s", szDBName);

	// Open the database
	errorCode = sqlite3_open(szPathName, &sqlite);

	if (errorCode != SQLITE_OK)
	{
		if (iDbLogging)
			logprintf("[Error] Can't open sqlite database %s.", szPathName);
		sqlite3_close(sqlite);
		return 0;
	}

	int index = sqlite3Databases.Insert(sqlite);
	if(index < 0)
		return 0;
	return index + 1;
}

// native sqlite_close(SQLiteDB:db);
static cell AMX_NATIVE_CALL n_close(AMX* amx, cell* params)
{
	// TODO: n_close
	return 0;
}

static cell AMX_NATIVE_CALL n_query(AMX* amx, cell* params)
{
	// TODO: n_query
	return 0;
}

static cell AMX_NATIVE_CALL n_free_result(AMX* amx, cell* params)
{
	// TODO: n_free_result
	return 0;
}

static cell AMX_NATIVE_CALL n_num_rows(AMX* amx, cell* params)
{
	// TODO: n_num_rows
	return 0;
}

static cell AMX_NATIVE_CALL n_next_row(AMX* amx, cell* params)
{
	// TODO: n_next_row
	return 0;
}

static cell AMX_NATIVE_CALL n_num_fields(AMX* amx, cell* params)
{
	// TODO: n_num_fields
	return 0;
}

static cell AMX_NATIVE_CALL n_field_name(AMX* amx, cell* params)
{
	// TODO: n_field_name
	return 0;
}

static cell AMX_NATIVE_CALL n_get_field(AMX* amx, cell* params)
{
	// TODO: n_get_field
	return 0;
}

static cell AMX_NATIVE_CALL n_get_field_int(AMX* amx, cell* params)
{
	// TODO: n_get_field_int
	return 0;
}

static cell AMX_NATIVE_CALL n_get_field_float(AMX* amx, cell* params)
{
	// TODO: n_get_field_float
	return 0;
}

static cell AMX_NATIVE_CALL n_get_field_assoc(AMX* amx, cell* params)
{
	// TODO: n_get_field_assoc
	return 0;
}

static cell AMX_NATIVE_CALL n_get_field_assoc_int(AMX* amx, cell* params)
{
	// TODO: n_get_field_assoc_int
	return 0;
}

static cell AMX_NATIVE_CALL n_get_field_assoc_float(AMX* amx, cell* params)
{
	// TODO: n_get_field_assoc_float
	return 0;
}

static cell AMX_NATIVE_CALL n_debug_openfiles(AMX* amx, cell* params)
{
	// TODO: n_debug_openfiles
	return 0;
}

static cell AMX_NATIVE_CALL n_debug_openresults(AMX* amx, cell* params)
{
	// TODO: n_debug_openresults
	return 0;
}

static cell AMX_NATIVE_CALL n_get_mem_handle(AMX* amx, cell* params)
{
	// TODO: n_get_mem_handle
	return 0;
}

static cell AMX_NATIVE_CALL n_get_result_mem_handle(AMX* amx, cell* params)
{
	// TODO: n_get_result_mem_handle
	return 0;
}

#if defined __cplusplus
  extern "C"
#endif
AMX_NATIVE_INFO sampDb_Natives[] = {
  { "db_open",					n_open },
  { "db_close",					n_close },
  { "db_query",					n_query },
  { "db_free_result",			n_free_result },
  { "db_num_rows",				n_num_rows },
  { "db_next_row",				n_next_row },
  { "db_num_fields",			n_num_fields },
  { "db_field_name",			n_field_name },
  { "db_get_field",				n_get_field },
  { "db_get_field_int",			n_get_field_int },
  { "db_get_field_float",		n_get_field_float },
  { "db_get_field_assoc",		n_get_field_assoc },
  { "db_get_field_assoc_int",	n_get_field_assoc_int },
  { "db_get_field_assoc_float",	n_get_field_assoc_float },
  { "db_debug_openfiles",		n_debug_openfiles },
  { "db_debug_openresults",		n_debug_openresults },
  { "db_get_mem_handle",		n_get_mem_handle },
  { "db_get_result_mem_handle",	n_get_result_mem_handle },
  { NULL, NULL }        /* terminator */
};

int AMXEXPORT amx_sampDbInit(AMX *amx)
{
	return amx_Register(amx, sampDb_Natives, -1);
}

int AMXEXPORT amx_sampDbCleanup(AMX *amx)
{
	return AMX_ERR_NONE;
}

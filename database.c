/* $Id: database.c,v 1.1.1.1 2003/02/10 22:45:49 tim Exp $
 *
 * Database related functions
 */

#include "database.h"

DmOpenRef gDatabase=NULL;
UInt16    gCategory=0;

/*****************************************************************************
* Function:  OpenDatabase
*
* Description:  Opens/creates the application's database.
*****************************************************************************/
Err OpenDatabase(void) {
	Err err = errNone;

  if (! gDatabase) {
    gDatabase = DmOpenDatabaseByTypeCreator(DATABASE_TYPE, APP_CREATOR, dmModeReadWrite);
    if (!gDatabase)  return dmErrCantOpen;
  }

  return err;
} // OpenDatabase



/*****************************************************************************
* Function:  CloseDatabase
*
* Description:  Closes the application's database.
*****************************************************************************/
void CloseDatabase(void) {
	if (gDatabase)
		DmCloseDatabase(gDatabase);
  gDatabase = NULL;
}

/*****************************************************************************
* Function:  DatabaseGetRef
*
* Description: Returns the database pointer
*****************************************************************************/
DmOpenRef DatabaseGetRef(void) {
  OpenDatabase();
  return gDatabase;
}


/*****************************************************************************
* Function:  DatabaseGetCat
*
* Description: Returns the current Category
*****************************************************************************/
UInt16 DatabaseGetCat(void) {
  return gCategory;
}

/*****************************************************************************
* Function:  DatabaseSetCat
*
* Description: Sets the current Category
*****************************************************************************/
void DatabaseSetCat(UInt16 newcat) {
  gCategory=newcat;
}


/* $Id: database.c,v 1.2 2003/10/08 23:10:46 tim Exp $
 *
 * Database related functions
 */

#include "database.h"

DmOpenRef gDatabase=NULL;
UInt16    gCategory=0;
UInt32    gDatabaseCurDB=0;
UInt16    gDatabaseCardNo=0;

/*****************************************************************************
* Function:  OpenDatabase
*
* Description:  Opens/creates the application's database.
*****************************************************************************/
Err OpenDatabase(void) {
	Err err = errNone;

  if (! gDatabase) {
    LocalID dbID=0;
    UInt16 cardNo=0;
    UInt32 dbType='appl';
    Boolean started=true;
    DmSearchStateType *searchState;
    searchState = MemPtrNew(sizeof(DmSearchStateType));
    while (dbType == 'appl') {

      if (DmGetNextDatabaseByTypeCreator(started, searchState, 0, APP_CREATOR, false, &cardNo, &dbID) != errNone) {
        break;
      }
      started = false;
      DmDatabaseInfo(cardNo, dbID, NULL, NULL, NULL, NULL, NULL,
                     NULL, NULL, NULL, NULL, &dbType, NULL);
      if (dbType != 'appl') {
        gDatabase = DmOpenDatabase(cardNo, dbID, dmModeReadWrite);
        gDatabaseCurDB = dbType;
        gDatabaseCardNo = cardNo;
      }
    }
    // gDatabase = DmOpenDatabaseByTypeCreator(DATABASE_TYPE, APP_CREATOR, dmModeReadWrite);
    MemPtrFree(searchState);
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

/*****************************************************************************
* Function:  DatabaseGetType
*
* Description: Returns the currently opened database type (aka university)
*****************************************************************************/
UInt32 DatabaseGetType() {
  return gDatabaseCurDB;
}

/*****************************************************************************
* Function:  DatabaseGetCardNo
*
* Description: Returns the card no of the opened DB
*****************************************************************************/
UInt32 DatabaseGetCardNo() {
  return gDatabaseCardNo;
}

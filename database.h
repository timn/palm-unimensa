/* $Id: database.h,v 1.2 2003/10/08 23:10:46 tim Exp $
 *
 * Database stuff header file
 */

#ifndef __DATABASE_H_
#define __DATABASE_H_

#include "UniMensa.h"

typedef struct {
	UInt16	renamedCategories; 
	Char	categoryLabels[16][16];
	UInt8	categoryUniqIDs[16];
	UInt8	lastUniqID;
	UInt8	padding;
  DateType fromDate;
  DateType toDate;
  Char university[16]; // Keep exact length or BadBugs(TM) will happen
} UMENAppInfoType;

typedef struct {
  UInt8 weekday;
  char text[1];
} MensaDBRecord;



extern Err OpenDatabase(void);
extern void CloseDatabase(void);
extern DmOpenRef DatabaseGetRef(void);
extern UInt16 DatabaseGetCat(void);
extern void DatabaseSetCat(UInt16 newcat);
extern UInt32 DatabaseGetType();
extern UInt32 DatabaseGetCardNo();

#endif /* __DATABASE_H_ */

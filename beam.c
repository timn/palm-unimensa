/* $Id: beam.c,v 1.2 2003/10/08 23:07:38 tim Exp $
 *
 * Beam related code
 * Basically taken from palmsource.com code recipes
 */

#include "UniMensa.h"
#include "beam.h"
#include "database.h"


static Err BeamWriteProc(const void *dataP, UInt32 *sizeP, void *userDataP) {
  Err err;

  *sizeP = ExgSend((ExgSocketPtr) userDataP, dataP, *sizeP, &err);
  return err;
}


void BeamDatabase(void) {
  ExgSocketType exgSocket;
  Err err = 0;

  Char *appName = "UniMensa";
  LocalID id;
  UInt16  cardNumber = 0; // better not to hardcode this
  Char label[2*longDateStrLength+4], *str;
  DateFormatType dateFormat=(DateFormatType)PrefGetPreference(prefLongDateFormat);
  UMENAppInfoType *appInfo;

  id = DmFindDatabase(cardNumber, "UniMensaUMEN");
  if (id) {
    // important to init structure to zeros...
    MemSet(&exgSocket,sizeof(exgSocket),0);
 
    appInfo = (UMENAppInfoType *)MemLocalIDToLockedPtr(DmGetAppInfoID(DatabaseGetRef()), DatabaseGetCardNo());

    MemSet(label, 2*dateStringLength+4, 0);
    str = label;
    DateToAscii((UInt8)(appInfo->fromDate.month), (UInt8)(appInfo->fromDate.day),
                (UInt16)(appInfo->fromDate.year+1904), dateFormat, str);
    str += StrLen (str);
    *str++ = spaceChr;
    *str++ = '-';
    *str++ = spaceChr;
    DateToAscii((UInt8)(appInfo->toDate.month), appInfo->toDate.day, appInfo->toDate.year+1904, dateFormat, str);

    exgSocket.description = label;
    exgSocket.name = "UniMensa.pdb";

    err = ExgPut(&exgSocket);
    if (err == 0) {
      if (id)  err = ExgDBWrite(BeamWriteProc, &exgSocket, appName, id, cardNumber);
      err = ExgDisconnect(&exgSocket,err);
    }
  }
}

/* $Id: dbinfo.c,v 1.1 2003/10/08 23:06:39 tim Exp $
 *
 * Database info
 */

#include "UniMensa.h"
#include "dbinfo.h"
#include "database.h"

Char gDBinfoDateLabel[2*longDateStrLength+4];

/*****************************************************************************
* Functions for showing database info
*****************************************************************************/

static void DBinfoFormInit(FormType *frm) {
  DateFormatType dateFormat=(DateFormatType)PrefGetPreference(prefLongDateFormat);
  Char *str;
  UMENAppInfoType *appInfo;
  ControlType *ctl;

  // Get AppInfo block for the dates
  appInfo = (UMENAppInfoType *)MemLocalIDToLockedPtr(DmGetAppInfoID(DatabaseGetRef()), DatabaseGetCardNo());

  // Set the header string
  MemSet(gDBinfoDateLabel, 2*dateStringLength+4, 0);
  str = gDBinfoDateLabel;
  DateToAscii((UInt8)(appInfo->fromDate.month), (UInt8)(appInfo->fromDate.day),
              (UInt16)(appInfo->fromDate.year+1904), dateFormat, str);
  str += StrLen (str);
  *str++ = spaceChr; *str++ = '-'; *str++ = spaceChr;
  DateToAscii((UInt8)(appInfo->toDate.month), appInfo->toDate.day, appInfo->toDate.year+1904, dateFormat, str);

  FrmCopyLabel(frm, DBINFO_uni, appInfo->university);

  ctl = GetObjectPtr(DBINFO_valid);
  FrmHideObject(frm, FrmGetObjectIndex(frm, DBINFO_valid));
  CtlSetLabel(ctl, gDBinfoDateLabel);
  FrmShowObject(frm, FrmGetObjectIndex(frm, DBINFO_valid));
  FrmDrawForm(frm);

}

Boolean DBinfoFormHandleEvent(EventPtr event) {
  Boolean handled = false;
  FormType *frm=FrmGetActiveForm();

  if (event->eType == ctlSelectEvent) {
    switch (event->data.ctlSelect.controlID) {

      case DBINFO_ok:
        FrmReturnToForm(FORM_main);
        handled=true;
        break;

      default:
        break;
    }   
  } else if (event->eType == frmUpdateEvent) {
      // redraws the form if needed
      frm = FrmGetActiveForm();
      FrmDrawForm (frm);
      handled = true;
    } else if (event->eType == frmOpenEvent) {
      // initializes and draws the form at program launch
      frm = FrmGetActiveForm();
      DBinfoFormInit(frm);
      FrmDrawForm (frm);
      handled = true;
    } else if (event->eType == frmCloseEvent) {
      // this is done if program is closed
      // NOT called, since we use FrmReturnToForm!
    }



  return handled;
}

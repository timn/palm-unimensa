/* $Id: prefs.c,v 1.1 2003/10/08 23:06:15 tim Exp $
 *
 * Preferences
 */

#include "UniMensa.h"
#include "prefs.h"
#include "database.h"

extern UniMensaPrefs gPrefs;
Char gPrefsMensaName[dmCategoryLength];
UInt16 gPrefsDefMensa=0;

void PrefLoadPrefs(UniMensaPrefs *prefs) {
  UInt16 prefsSize=0;
  Int16 version;

  version = PrefGetAppPreferences(APP_CREATOR, PREFS_ID, NULL, &prefsSize, false);
  
  if (version == noPreferenceFound) {
    MemSet(prefs, sizeof(UniMensaPrefs), 0);
  } else if (version != PREFS_VERSION) {
  } else {
    // Load
    PrefGetAppPreferences(APP_CREATOR, PREFS_ID, prefs, &prefsSize, false);
  }

}

void PrefSavePrefs(UniMensaPrefs *prefs) {
  PrefSetAppPreferences(APP_CREATOR, PREFS_ID, PREFS_VERSION, prefs, sizeof(UniMensaPrefs), false);
}


/*****************************************************************************
* Functions for editing SETTINGS
*****************************************************************************/

static void PrefsFormInit(FormType *frm) {
  ControlType *ctl;

  ctl = GetObjectPtr(PREFS_lastseen);
  CtlSetValue(ctl, gPrefs.lastSeen);

  ctl = GetObjectPtr(PREFS_defmen);
  CtlSetValue(ctl, gPrefs.remMensa);

  ctl = GetObjectPtr(PREFS_mentrig);
  gPrefsDefMensa = gPrefs.defMensa;
  CategoryGetName(DatabaseGetRef(), gPrefsDefMensa, gPrefsMensaName); 
  CategorySetTriggerLabel(ctl, gPrefsMensaName); 

}

static Boolean PrefsFormSave(FormType *frm) {
  ControlType *ctl;
  
  ctl=GetObjectPtr(PREFS_lastseen);
  gPrefs.lastSeen = (CtlGetValue(ctl)) ? 1 : 0;

  ctl=GetObjectPtr(PREFS_defmen);
  gPrefs.remMensa = (CtlGetValue(ctl)) ? 1 : 0;

  gPrefs.defMensa = gPrefsDefMensa;

  PrefSavePrefs(&gPrefs);

  return true;
}


Boolean PrefsFormHandleEvent(EventPtr event) {
  Boolean handled = false;
  FormType *frm=FrmGetActiveForm();
  UInt16 category=0;
  ControlType *ctl=NULL;

  if (event->eType == ctlSelectEvent) {
    switch (event->data.ctlSelect.controlID) {

      case PREFS_ok:
        if (PrefsFormSave(frm)) {
          FrmReturnToForm(FORM_main);
          FrmUpdateForm(FORM_main, frmRedrawUpdateCode);
        }
        handled=true;
        break;

      case PREFS_cancel:
        FrmReturnToForm(FORM_main);
        FrmUpdateForm(FORM_main, frmRedrawUpdateCode);
        handled=true;
        break;

      case PREFS_lastseen:
        if (CtlGetValue(event->data.ctlSelect.pControl)) {
          CtlSetValue(GetObjectPtr(PREFS_defmen), 0);
        }
        break;

      case PREFS_defmen:
        if (CtlGetValue(event->data.ctlSelect.pControl)) {
          CtlSetValue(GetObjectPtr(PREFS_lastseen), 0);
        }
        break;
        
      case PREFS_mentrig:
        category=gPrefsDefMensa;
        CategorySelect(DatabaseGetRef(), frm,
                       PREFS_mentrig, PREFS_menlist, false,
                       &category, gPrefsMensaName, 0,
                       categoryHideEditCategory);
        if (category != gPrefsDefMensa) {
          gPrefsDefMensa = category;
          ctl = GetObjectPtr(PREFS_mentrig);
          CategoryGetName(DatabaseGetRef(), gPrefsDefMensa, gPrefsMensaName); 
          CategorySetTriggerLabel(ctl, gPrefsMensaName); 
        }
        handled = true;
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
      PrefsFormInit(frm);
      FrmDrawForm (frm);
      handled = true;
    } else if (event->eType == frmCloseEvent) {
      // this is done if program is closed
      // NOT called, since we use FrmReturnToForm!
    }



  return handled;
}

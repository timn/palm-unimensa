/* $Id: UniMensa.c,v 1.2 2003/10/08 23:04:54 tim Exp $
 *
 * UniMensa main and event handling
 */

#include "UniMensa.h"
#include "database.h"
#include "tnglue.h"
#include "gadget.h"
#include "beam.h"
#include "prefs.h"
#include "dbinfo.h"

Char gCategoryName[dmCategoryLength];
Char gHeadLabel[2*longDateStrLength+4], *str;
UniMensaPrefs gPrefs;

/***********************************************************************
 * function is called at program start
 * you can put your own initialization jobs there
 ***********************************************************************/
static UInt16 StartApplication (void) {
  Err err = 0;
  // UInt16 cat=0, catSize=sizeof(cat);
  

	// Initialize the random number seed;
	SysRandom( TimGetSeconds() );

  err = TNGlueColorInit();
  
  // Open Database
  err = OpenDatabase();

  // Load Preferences
  PrefLoadPrefs(&gPrefs);

  if (gPrefs.lastDB == DatabaseGetType()) {
    if (gPrefs.lastSeen) {
      DatabaseSetCat(gPrefs.curMensa);
    } else if (gPrefs.remMensa) {
      DatabaseSetCat(gPrefs.defMensa);
    } else {
      DatabaseSetCat(0);
    }
  } else {
    DatabaseSetCat(0);
  }

  return (err);
}

/***********************************************************************
 * function is checking ROM version of the palmos operating system
 * if ROM version is less then required the result of this function is
 * sysErrRomIncompatible
 ***********************************************************************/
static Err RomVersionCompatible (UInt32 requiredVersion, UInt16 launchFlags) {
	UInt32 romVersion;

	// See if we're on in minimum required version of the ROM or later.
	FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
	if (romVersion < requiredVersion) {
		if ((launchFlags & (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
  			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) {
			FrmAlert (ALERT_ROMIncompatible);

			// PalmOS before 3.5 will continuously relaunch this app unless we switch to
			// another safe one.
			AppLaunchWithCommand(sysFileCDefaultApp, sysAppLaunchCmdNormalLaunch, NULL);
    }

    return (sysErrRomIncompatible);
  }

  return (0);
}


/***********************************************************************
 * function for main form initialization
 * you can put your initialization stuff there
 * e.g. initial settings for controls and labels
 ***********************************************************************/
static void MainFormInit (FormPtr frm){

  if (DatabaseGetRef() != NULL) {
    // We have a database
    ControlType *ctl, *button;
    DateFormatType dateFormat=(DateFormatType)PrefGetPreference(prefLongDateFormat);
    UMENAppInfoType *appInfo;
    Int16 lineWidth;
    FontID oldFont;
    DateTimeType now;
    Int16 dow;

    GadgetSet(frm, GADGET_text, SCROLL_text);
    FrmSetGadgetHandler(frm, FrmGetObjectIndex(frm, GADGET_text), GadgetHandler);

    ctl = GetObjectPtr(LIST_cat_trigger);
    CategoryGetName(DatabaseGetRef(), DatabaseGetCat(), gCategoryName); 
    CategorySetTriggerLabel(ctl, gCategoryName); 

    // Initialize time constants and highlight current day's button
    TimSecondsToDateTime(TimGetSeconds(), &now);
    dow = DayOfWeek(now.month, now.day, now.year);
    // we will subtract 1, so we would get BadBugs...
    if (dow == 0)  dow=1;
    button=GetObjectPtr(BUTTON_mon+(dow-1));
    CtlSetValue(button, 1);
    GadgetSetWeekday(dow-1);

    // Get AppInfo block for the dates
    appInfo = (UMENAppInfoType *)MemLocalIDToLockedPtr(DmGetAppInfoID(DatabaseGetRef()), DatabaseGetCardNo());

    // Set the header string
    MemSet(gHeadLabel, 2*dateStringLength+4, 0);
    str = gHeadLabel;
    DateToAscii((UInt8)(appInfo->fromDate.month), (UInt8)(appInfo->fromDate.day),
                (UInt16)(appInfo->fromDate.year+1904), dateFormat, str);
    str += StrLen (str);
    *str++ = spaceChr; *str++ = '-'; *str++ = spaceChr;
    DateToAscii((UInt8)(appInfo->toDate.month), appInfo->toDate.day, appInfo->toDate.year+1904, dateFormat, str);

    // Calc string width. We have bold font for the label so switch font
    oldFont = FntSetFont(boldFont);
    lineWidth=FntLineWidth(gHeadLabel, StrLen(gHeadLabel));
    FntSetFont(oldFont);

    // Assign the string to the label and position
    CtlSetLabel(GetObjectPtr(LABEL_head), gHeadLabel);
    FrmSetObjectPosition(frm, FrmGetObjectIndex(frm, LABEL_head), (160-lineWidth)/2, DATE_TOP_Y);

    MemPtrUnlock(appInfo);
  } else {
    // No database on Palm
    FormType *frm=FrmGetActiveForm();
    UInt16 i;
    for (i=BUTTON_mon; i <= BUTTON_sat; ++i)
      FrmHideObject(frm, FrmGetObjectIndex(frm, i));

    FrmHideObject(frm, FrmGetObjectIndex(frm, GADGET_text));
    FrmHideObject(frm, FrmGetObjectIndex(frm, LIST_cat_trigger));

    FrmShowObject(frm, FrmGetObjectIndex(frm, LABEL_nodb));
    
  }

}


/***********************************************************************
 * handling for the main drop down menu actions
 ***********************************************************************/
Boolean HandleMenuEvent (UInt16 command){
  Boolean handled = false;

  MenuEraseStatus(0);

  switch (command) {
    case MENUITEM_about:
      FrmDoDialog(FrmInitForm(FORM_about));
      handled=true;
      break;

    case MENUITEM_beam:
      BeamDatabase();
      handled=true;
      break;

    case MENUITEM_chat:
			AppLaunchWithCommand(UNICHAT_APP_CREATOR, sysAppLaunchCmdNormalLaunch, NULL);
      handled=true;
      break;

    case MENUITEM_matrix:
			AppLaunchWithCommand(UNIMATRIX_APP_CREATOR, sysAppLaunchCmdNormalLaunch, NULL);
      handled=true;
      break;

    case MENUITEM_prefs:
      FrmPopupForm(PREFS_form);
      handled=true;
      break;

    case MENUITEM_dbinfo:
      FrmPopupForm(DBINFO_form);
      handled=true;
      break;

    default:
      break;
  }

  return handled;
}


Boolean HandleMenuOpenEvent(EventType *event) {
  DmSearchStateType searchState;
  UInt16 cardNo;
  LocalID dbID;
  Boolean needsSeparator=false;

  if (DmGetNextDatabaseByTypeCreator(true, &searchState, 0, UNICHAT_APP_CREATOR, true, &cardNo, &dbID) != dmErrCantFind) {
    MenuAddItem(MENUITEM_beam, MENUITEM_chat, 'C', "UniChat");
    needsSeparator=true;
  }
  if (DmGetNextDatabaseByTypeCreator(true, &searchState, 0, UNIMATRIX_APP_CREATOR, true, &cardNo, &dbID) != dmErrCantFind) {
    MenuAddItem(MENUITEM_beam, MENUITEM_matrix, 'X', "UniMatrix");
    needsSeparator=true;
  }

  if (needsSeparator) {
    Char tmpSep[2];
    tmpSep[0]=MenuSeparatorChar;
    tmpSep[1]=0;
    MenuAddItem(MENUITEM_beam, MENUITEM_addsep, 0, tmpSep);
}

  return true;
}


/***********************************************************************
 * handling for form and control actions
 * menu actions are forwarded to MainFormDoCommand
 ***********************************************************************/
static Boolean MainFormHandleEvent (EventPtr event){
  FormType *frm=FrmGetActiveForm();
  Boolean handled = false, categoryEdited=false;
  UInt16 category;
  ControlType *ctl;

  if (event->eType == ctlSelectEvent) {
    // button handling
    handled = true;
    switch (event->data.ctlSelect.controlID) {
      // the ok button - this leaves the application

      case LIST_cat_trigger:
        category=DatabaseGetCat();
        categoryEdited = CategorySelect(DatabaseGetRef(), frm,
                                        LIST_cat_trigger, LIST_cat, false,
                                        &category, gCategoryName, 0,
                                        categoryHideEditCategory); // categoryDefaultEditCategoryString
        if (categoryEdited || (category != DatabaseGetCat())) {
          DatabaseSetCat(category);
          ctl = GetObjectPtr(LIST_cat_trigger);
          CategoryGetName(DatabaseGetRef(), DatabaseGetCat(), gCategoryName); 
          CategorySetTriggerLabel(ctl, gCategoryName); 
          GadgetResetScrollbar();
          GadgetRedraw();
        }
        break;

      /* BUTTONS
      case BUTTON_main_search:
        fldP = GetObjectPtr(FIELD_search);
        SetSearchQuery(FldGetTextPtr(fldP));

        FrmGotoForm(FORM_search);
        break;

      case BUTTON_beam:
        // BeamCourse(GadgetGetHintCourseIndex());
        break;
*/

      case BUTTON_mon:
      case BUTTON_tue:
      case BUTTON_wed:
      case BUTTON_thu:
      case BUTTON_fri:
      case BUTTON_sat:
        switch (FrmGetObjectId(frm, FrmGetControlGroupSelection(frm, GROUP_day))) {
          case BUTTON_mon: GadgetSetWeekday(GADGET_DAY_MON); break;
          case BUTTON_tue: GadgetSetWeekday(GADGET_DAY_TUE); break;
          case BUTTON_wed: GadgetSetWeekday(GADGET_DAY_WED); break;
          case BUTTON_thu: GadgetSetWeekday(GADGET_DAY_THU); break;
          case BUTTON_fri: GadgetSetWeekday(GADGET_DAY_FRI); break;
          case BUTTON_sat: GadgetSetWeekday(GADGET_DAY_SAT); break;
        }
        break;

      default:
        break;
      }
    } else if (event->eType == keyDownEvent) {
      if (EvtKeydownIsVirtual(event)) {
        // Up or down keys pressed
        switch (event->data.keyDown.chr) {
          case vchrPageUp:
            GadgetScrollPage(winUp);
            handled=true;
            break;
          
          case vchrPageDown:
            GadgetScrollPage(winDown);
            handled=true;
            break;

          case vchrSendData:
            BeamDatabase();
            handled=true;
            break;

          default:
            break;
        }
      }
    } else if (event->eType == sclRepeatEvent) {
      GadgetRedraw();
      handled = false; // return false so that scrolling can continue
    } else if (event->eType == menuOpenEvent) {
      return HandleMenuOpenEvent(event);
    } else if (event->eType == menuEvent) {
      // forwarding of menu events
      return HandleMenuEvent(event->data.menu.itemID);
    } else if (event->eType == frmUpdateEvent) {
      // redraws the form if needed
      FrmDrawForm (frm);
      handled = true;
    } else if (event->eType == frmOpenEvent) {
      // initializes and draws the form at program launch
      MainFormInit (frm);
      FrmDrawForm (frm);

      handled = true;
    } else if (event->eType == frmCloseEvent) {
      // this is done if program is closed
    }

  return (handled);
}


/***********************************************************************
 * chooses an displays the requested form
 ***********************************************************************/
static Boolean AppHandleEvent( EventPtr eventP) {
	UInt16 formId;
	FormPtr frmP;


	switch (eventP->eType) {
  	case frmLoadEvent:
      // Load the form resource.
		  formId = eventP->data.frmLoad.formID;
		  frmP = FrmInitForm(formId);
		  FrmSetActiveForm(frmP);

      // Set the event handler for the form.  The handler of the currently
      // active form is called by FrmHandleEvent each time is receives an
      // event.
		  switch (formId) {
			  case FORM_main: FrmSetEventHandler(frmP, MainFormHandleEvent); break;
			  case PREFS_form: FrmSetEventHandler(frmP, PrefsFormHandleEvent); break;
			  case DBINFO_form: FrmSetEventHandler(frmP, DBinfoFormHandleEvent); break;

        default:
  				ErrNonFatalDisplay("Invalid Form Load Event");
		  		break;
			}
		  break;

	  default:
		  return false;
	}
	return true;
}


/***********************************************************************
 * main event loop; loops until appStopEvent is caught
 ***********************************************************************/
static void AppEventLoop(void){
	UInt16 error;
	EventType event;


	do {
		EvtGetEvent(&event, evtWaitForever);


		if (! SysHandleEvent(&event))
			if (! MenuHandleEvent(0, &event, &error))
				if (! AppHandleEvent(&event))
					FrmDispatchEvent(&event);

// Check the heaps after each event
		#if EMULATION_LEVEL != EMULATION_NONE
			MemHeapCheck(0);
			MemHeapCheck(1);
		#endif

	} while (event.eType != appStopEvent);

}

/***********************************************************************
 * application is finished, so we have to clean the desktop behind us
 ***********************************************************************/
static void StopApplication (void){
  // UInt16 cat=DatabaseGetCat();

	FrmCloseAllForms ();

  // Save Preferences
  gPrefs.curMensa = DatabaseGetCat();
  gPrefs.lastDB = DatabaseGetType();
  PrefSavePrefs(&gPrefs);

	CloseDatabase();
  // PrefSetAppPreferences(APP_CREATOR, PREFS_CURCAT, PREFS_VERSION, &cat, sizeof(cat), false);
}


/***********************************************************************
 * main function
 ***********************************************************************/
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags){
	UInt16 error;
	error = RomVersionCompatible (MINVERSION, launchFlags);
	if (error) return (error);


  /***
  * NORMAL STARTUP
  ****/
	if ( cmd == sysAppLaunchCmdNormalLaunch ) {
    error = StartApplication();

    FrmGotoForm(FORM_main);

		AppEventLoop ();
		StopApplication ();

  /***
  * FIND
  ****/
/*
	} else if (cmd == sysAppLaunchCmdSaveData) {
    FrmSaveAllForms();
  } else if (cmd == sysAppLaunchCmdFind) {
    PalmGlobalFind((FindParamsPtr)cmdPBP);
*/

  /***
  * GoTo
  ****/
/*	} else if (cmd == sysAppLaunchCmdGoTo) {
    Boolean launched = launchFlags & sysAppLaunchFlagNewGlobals;

    if (launched) {
      error = StartApplication();
      if (! error) {
        GoToItem((GoToParamsPtr)cmdPBP, launched);
        AppEventLoop();
        StopApplication();
      }
    } else {
      GoToItem((GoToParamsPtr)cmdPBP, launched);
    }
*/

  /***
  * BEAMING
  ****/
/*
	} else if (cmd == sysAppLaunchCmdSyncNotify) {
    // Register with the Exchange Manager
    ExgRegisterData(APP_CREATOR, exgRegExtensionID, "ohs");
	} else if (cmd == sysAppLaunchCmdExgReceiveData) {
    DmOpenRef dbP=NULL;
    // Is app active?
    if (launchFlags & sysAppLaunchFlagSubCall) {
      // Quit Forms
      FrmSaveAllForms();

      dbP = DatabaseGetRef();
      error = ReceiveBeam(dbP, (ExgSocketPtr) cmdPBP);
      GadgetDrawComplete();

    } else {
      // Another app was running when we were called
      dbP = DmOpenDatabaseByTypeCreator(DATABASE_TYPE, APP_CREATOR, dmModeReadWrite);
      error=ReceiveBeam(dbP, (ExgSocketPtr)cmdPBP);
      DmCloseDatabase(dbP);
    }
*/
	}

	return 0;
}

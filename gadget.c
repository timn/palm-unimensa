/* $Id: gadget.c,v 1.1.1.1 2003/02/10 22:45:49 tim Exp $
 *
 * Code for TextGadget
 */

#include "gadget.h"
#include "tnglue.h"

FormPtr gForm=NULL;
UInt16 gGadgetID=0, gHintGadgetID=0, gGadgetScrollID=0, gGadgetMaxLines=0;
UInt8 gGadgetWeekday;

/*****************************************************************************
* Function: GadgetRedraw
*
* Description: Redraws without erasing first, avoids flicker and does not
*              select next item, behaves more like the user expects
*****************************************************************************/
void GadgetRedraw() {
  RectangleType bounds, rect;
  UInt16 gadgetIndex, index=0, curLength=0;
  FormType *frm=gForm;
  MemHandle m=NULL;
  UInt8 curLine=0, numLines=0;
  MensaDBRecord *menu=NULL;
  Char *string;
  Boolean found=false;
  ScrollBarType *scl;
  Int16 value=0, max=0, min=0, pageSize=0;
  
  // Check if GadgetSet has already been called. If not => die
  if (! frm) return;

  gadgetIndex = FrmGetObjectIndex(frm, gGadgetID);
  FrmGetObjectBounds(frm, gadgetIndex, &bounds);
  numLines = bounds.extent.y / FntLineHeight();
  scl = GetObjectPtr(gGadgetScrollID);

  // Erase background
  WinEraseRectangle(&bounds, 0);

  SclGetScrollBar(scl, &value, &min, &max, &pageSize);

  // Find string
  while( !found &&  ((m = DmQueryNextInCategory(DatabaseGetRef(), &index, DatabaseGetCat())) != NULL) ) {
    menu=(MensaDBRecord *)MemHandleLock(m);
    if (menu->weekday != gGadgetWeekday) {
      MemHandleUnlock(m);
      index += 1;
    } else found=true;
  }

  // Nothing to be drawn when not found
  if (! found) {
    // Should display not found here...
    return;
  }

  string=menu->text;

  // Draw string
  while (StrLen(string) > 0) {
    Char *temp;

    curLength = FntWordWrap(string, bounds.extent.x);

    if ( (curLine >= value) && ((curLine-value) < numLines) ) {
      RctSetRectangle(&rect, bounds.topLeft.x, bounds.topLeft.y+(curLine-value)*FntLineHeight(),
                             bounds.extent.x, FntLineHeight());

      temp=(Char *)MemPtrNew(curLength+1);
      MemSet(temp, MemPtrSize(temp), 0);
      StrNCopy(temp, string, curLength);

      TNDrawCharsToFitWidth(temp, &rect);

      MemPtrFree(temp);
    }

    curLine += 1;
    string += curLength;
  }

  if (curLine > numLines) {
    max = (curLine - numLines);
  } else max = value;
  
  SclSetScrollBar(scl, value, 0, max, numLines);
  gGadgetMaxLines=curLine;

  MemHandleUnlock(m);
}


/*****************************************************************************
* Function: GadgetHandler
*
* Description: We use extended Gadgeting => We need an event handler for it
*****************************************************************************/
Boolean GadgetHandler(FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP) {
  Boolean handled = false; 
  EventType *event = (EventType *)paramP;
  
  switch (cmd) { 
    case formGadgetDrawCmd: 
      //Sent to active gadgets any time form is  
      //drawn or redrawn. 
      GadgetRedraw();
      gadgetP->attr.visible = true; 
      handled = true; 
      break; 
       
    case formGadgetHandleEventCmd: 
      //Sent when form receives a gadget event.  
      //paramP points to EventType structure.

      if (event->eType == frmGadgetEnterEvent) { 
        // penDown in gadget's bounds.
        // We do not care, not interesting for this mini app
        // GadgetTap((FormGadgetType *)gadgetP, event);
        handled = true;
      }
      if (event->eType == frmGadgetMiscEvent) {
        //This event is sent by your application
        //when it needs to send info to the gadget
      }
      break;

    case formGadgetDeleteCmd:
      //Perform any cleanup prior to deletion.
      break;

    case formGadgetEraseCmd:
      //FrmHideObject takes care of this if you 
      //return false.
       handled = false;
       break;

    default:
      break;

  }
  return handled;
}


Boolean GadgetHandleScrollEvent(EventType *event) {
  Boolean handled=false;
  ScrollBarType *scl;

  if (event->eType == sclRepeatEvent) {
    scl = event->data.sclRepeat.pScrollBar;
    // event->data.sclRepeat.newValue - event->data.sclRepeat.value;
  }

  return handled;
}

void GadgetResetScrollbar(void) {
  SclSetScrollBar(GetObjectPtr(gGadgetScrollID), 0, 0, 0, 0);
}

void GadgetScrollPage(WinDirectionType direction) {
  Int16 value, min, max, pageSize;
  ScrollBarType *scl=GetObjectPtr(gGadgetScrollID);

  SclGetScrollBar(scl, &value, &min, &max, &pageSize);

  if ( (direction == winUp) && (value > 0) ) {
    // Scroll up and there is room to scroll...
    if ( (value-pageSize) > 0) {
      value -= pageSize;
    } else {
      value = 0;
    }
  } else if ( (direction == winDown) && (value < max) ) {
    if ( (value+pageSize) <= gGadgetMaxLines) {
      value += pageSize;
    } else {
      value = gGadgetMaxLines;
    }
  }

  if (gGadgetMaxLines > pageSize) {
    max = (gGadgetMaxLines - pageSize);
  } else max = value;

  SclSetScrollBar(scl, value, min, max, pageSize);

  GadgetRedraw();
}



/*****************************************************************************
* Function: GadgetSet
*
* Description: Must be called before draw to set the form the gadget is in
*              and the IDs of the Gadget and the hint gadget
*****************************************************************************/
void GadgetSet(FormPtr frm, UInt16 gadgetID, UInt16 scrollbarID) {
  gForm=frm;
  gGadgetID=gadgetID;
  gGadgetScrollID=scrollbarID;
}

void GadgetSetWeekday(UInt8 new) {
  gGadgetWeekday=new;
  GadgetResetScrollbar();
  GadgetRedraw();
}

UInt8 GadgetGetWeekday(void) {
  return gGadgetWeekday;
}

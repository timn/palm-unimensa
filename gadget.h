/* $Id: gadget.h,v 1.1.1.1 2003/02/10 22:45:49 tim Exp $
 *
 * Text Gadget headers
 */

#include "database.h"

#define GADGET_DAY_MON 0
#define GADGET_DAY_TUE 1
#define GADGET_DAY_WED 2
#define GADGET_DAY_THU 3
#define GADGET_DAY_FRI 4
#define GADGET_DAY_SAT 5


extern void GadgetRedraw(void);
extern Boolean GadgetHandler(FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP);
extern Boolean GadgetHandleScrollEvent(EventType *event);
extern void GadgetSet(FormPtr frm, UInt16 gadgetID, UInt16 scrollbarID);
extern void GadgetSetWeekday(UInt8 new);
extern UInt8 GadgetGetWeekday(void);
extern void GadgetResetScrollbar(void);
extern void GadgetScrollPage(WinDirectionType direction);

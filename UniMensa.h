/* $Id: UniMensa.h,v 1.1 2003/02/10 22:45:49 tim Exp $
 *
 * Basic defines and "Global" function prototypes for menu handling and
 * pointer retrieval
 */

#ifndef __UNIMENSA_H_
#define __UNIMENSA_H_

#include <PalmOS.h>
#include "resources.h"


#define APP_CREATOR 'UMEN'
#define MINVERSION		0x03500000

#define DATE_TOP_Y 18

#define UNICHAT_APP_CREATOR 'UCHT'
#define UNIMATRIX_APP_CREATOR 'UMTX'


extern Boolean HandleMenuEvent (UInt16 command);
extern Boolean HandleMenuOpenEvent(EventType *event);
extern void *GetObjectPtr(UInt16 objectID);


#endif /* __UNIMENSA_H_ */

/* $Id: prefs.h,v 1.1 2003/10/08 23:06:15 tim Exp $
 *
 * Preferences code
 */

#ifndef __PREFS_H
#define __PREFS_H

#define PREFS_VERSION 1
#define PREFS_ID 1

typedef struct {
  UInt16  curMensa;
  UInt16  defMensa;
  UInt32  lastDB;

  UInt16  lastSeen :1;
  UInt16  remMensa :1;
  UInt16  reserved :14;

} UniMensaPrefs;

extern Boolean PrefsFormHandleEvent(EventPtr event);
extern void PrefLoadPrefs(UniMensaPrefs *prefs);
extern void PrefSavePrefs(UniMensaPrefs *prefs);

#endif // __PREFS_H

/* $Id: getUMENpdbUniDate.c,v 1.1 2003/02/10 22:45:50 tim Exp $
 *
 * Extract university and date from a UniMensa PDB
 */

#include "genUMENpdb.h"

#pragma pack(2)
#pragma align 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *pdbfile_name = "unimensa.pdb";

// Prototypes
unsigned short SSwap(unsigned short s);
unsigned long LSwap(unsigned long l);



// Main
int main(int argc, char * argv[]) {

  FILE *pdbfile;
  FILE *outfile;
  char temp[128];
  DateType date_from, date_to;
  unsigned int date;
  AppInfoType appInfo;
  DatabaseHdrType dbhead;

  pdbfile = fopen(pdbfile_name, "rb");
  if (pdbfile) {

    fread(&dbhead, sizeof(DatabaseHdrType), 1, pdbfile);
    fseek(pdbfile, LSwap(dbhead.appInfoID), 0);
    fread(&appInfo, sizeof(AppInfoType), 1, pdbfile);

    *(UInt16 *) &date_from = SSwap(*(UInt16 *) &appInfo.fromDate);
    *(UInt16 *) &date_to = SSwap(*(UInt16 *) &appInfo.toDate);

    printf("%s::%02u.%02u.%u - %02u.%02u.%u", appInfo.university, date_from.day, date_from.month, date_from.year+1904,
                                    date_to.day, date_to.month, date_to.year+1904);


    fclose(pdbfile);
    
  }
}



/****************************************************************************
* FUNCTION:     SSwap
*
* DESCRIPTION:  transforms an INTEL-architecture short int into a MOTOROLLA-
*               architecture short int.
****************************************************************************/
unsigned short SSwap(unsigned short s) {
  unsigned short temp;

 temp = ((s & 0xff00) >> 8) + ((s & 0x00ff) << 8);
return(temp);
}

/****************************************************************************
* FUNCTION:     LSwap
*
* DESCRIPTION:  transforms an INTEL-architecture long int into a MOTOROLLA-
*               architecture long int.
****************************************************************************/
unsigned long LSwap(unsigned long l) {
 unsigned short temp_s1, temp_s2;
 unsigned long temp_l;

 temp_s1 = SSwap((unsigned short)((l & 0xffff0000) >> 16));
 temp_s2 = SSwap((unsigned short)(l & 0x0000ffff));
 temp_l = ((unsigned long)temp_s2 << 16) + temp_s1;
return(temp_l);
}

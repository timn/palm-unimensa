/****************************************************************************
 *
 * FILE:         genUMENpdb.c
 *
 * Copyright (c) 2002 by Tim Niemueller <tim@niemueller.de>
 * PURPOSE:      Create
 * DESCRIPTION:  Creates a PDB file with Mensa plan
 *
 * $Id: genUMENpdb.c,v 1.1 2003/02/10 22:45:50 tim Exp $
 *
 ****************************************************************************/

#pragma pack(2)
#pragma align 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "genUMENpdb.h"

/* the linefeed character */
#define LF 0x0a

/* the file names */
char *outfile_name = FILENAME;
char *infile_name = INFILE;

/* there shouldn't be any need to change these initial values */
UInt8  db_type[4] = {'F', 'o', 'o', 'd'};
UInt8  db_creator[4] = {'U', 'M', 'E', 'N'};

char university[16];

unsigned char rec_attributes = 0;


/* local function prototypes */
unsigned long LSwap(unsigned long l);
unsigned short SSwap(unsigned short s);



/***************************************************************************
 * main()
 ***************************************************************************/
int main(int argc, char * argv[]) {


  FILE *outfile;
  FILE *infile;
  unsigned short i=0, j=0;
  long pad = 0;
  unsigned short debug=0;
  char buffer[BUFFER_MAXLEN];

  unsigned long  offset = 80; // Basic PDB Header has 80 bytes
  unsigned char  temp_char;
  unsigned short temp_short;
  unsigned long  temp_long;
  unsigned int num_mensen;
  unsigned short records_expected;
  unsigned int year;
  unsigned int month;
  unsigned int day;
  long menu_start=0;
  unsigned int count=0;
  DateType temp_date;

  DatabaseHdrType dbhead;
  AppInfoType     appInfo;
  RecordEntryType rcle; // record list entry
  MensaDBRecord   *mensa_record;

  if ((argc > 1) && (strcmp(argv[1], "--debug") == 0)) debug=1;
  
  memset(&appInfo, 0, sizeof(AppInfoType));
  memset(&dbhead, 0, sizeof(DatabaseHdrType));
  memset(&university, 0, sizeof(university));
  
  memset(dbhead.name, 0, DB_NAME_SIZE);
  strcpy(dbhead.name, "UniMensaUMEN");
  dbhead.attributes=SSwap(0);
  dbhead.version=SSwap(UMEN_DB_VERSION);
  dbhead.creationDate=LSwap(0x32cb1cc2);
  dbhead.modificationDate=LSwap(0x32cb1cc2);
  dbhead.lastBackupDate=LSwap(0);
  dbhead.modificationNumber=LSwap(1);
//  dbhead.appInfoID=LSwap(0);
  dbhead.sortInfoID=LSwap(0);
  dbhead.type[0] = db_type[0];
  dbhead.type[1] = db_type[1];
  dbhead.type[2] = db_type[2];
  dbhead.type[3] = db_type[3];
  dbhead.creator[0]=db_creator[0];
  dbhead.creator[1]=db_creator[1];
  dbhead.creator[2]=db_creator[2];
  dbhead.creator[3]=db_creator[3];
  dbhead.uniqueIDSeed=LSwap(0);


  infile = fopen(infile_name, "r");
  if (! infile)  printf("Failed to open infile");

  // Get university name from file
  fgets(university, 15, infile);
  // Remove trailing \n
  university[strlen(university)-1]='\0';
  strncpy(appInfo.university, university, 15);

  if (fgets(buffer, BUFFER_MAXLEN-1, infile) != NULL) {
    num_mensen=atoi(buffer);
  } else {
    printf("Failed to open infile %s\n", infile_name);
    exit(1);
  }

  if (fscanf(infile, "%u/%u/%u\n", &year, &month, &day) != 3) {
    printf("File format error: Could not read date (%u.%u.%u)", day, month, year);
    exit(2);
  }
  temp_date.year=year-1904; // Damn Mac conversion
  temp_date.month=month;
  temp_date.day=day;
  appInfo.fromDate=SSwap(*(UInt16 *) &temp_date); //SSwap(*(UInt16 *) &temp_date);

  if (fscanf(infile, "%u/%u/%u\n", &year, &month, &day) != 3) {
    printf("File format error 2: Could not read date (%u.%u.%u)", day, month, year);
    exit(2);
  }
  temp_date.year=year-1904; // Damn Mac conversion
  temp_date.month=month;
  temp_date.day=day;
  appInfo.toDate=SSwap(*(UInt16 *) &temp_date);


  fgets(buffer, BUFFER_MAXLEN-1, infile);
  if (strstr(buffer, "---") != buffer) {
    printf("File format error. --- missing after first block. Aborting.(%s)\n", buffer);
    exit(2);
  }

  records_expected=num_mensen * NUM_DAYS;
  offset += (sizeof(RecordEntryType) * records_expected);
  if (debug)  printf("Num Records: %u\n", records_expected);
  dbhead.appInfoID=LSwap(offset);
  offset += sizeof(AppInfoType);

  outfile = fopen(outfile_name, "wb");
  if (outfile) {
    // File successfully opened

    // Write header and record list
    fwrite(&dbhead, sizeof(dbhead), 1, outfile);
    // fwrite(&rlist, sizeof(rlist), 1, outfile);

    temp_long = LSwap(0);
    fwrite(&temp_long, sizeof(temp_long), 1, outfile);

    temp_short = SSwap(records_expected);
    fwrite(&temp_short, sizeof(temp_short), 1, outfile);

    // Read names of mensen
    for (i=0; i < num_mensen; ++i) {
      fgets(buffer, BUFFER_MAXLEN-1, infile);
      buffer[strlen(buffer)-1]='\0'; // This removes newline that fgets appends at the end
      strncpy(appInfo.categoryLabels[i], buffer,15);
      if (debug) printf("Mensa %u: %s\n", i, appInfo.categoryLabels[i]);
        appInfo.categoryUniqIDs[i]=i;
    }

    fgets(buffer, BUFFER_MAXLEN-1, infile);
    if (strstr(buffer, "---") != buffer) {
      printf("File format error. --- missing after second block. Aborting.(%s)\n", buffer);
      exit(2);
      fclose(infile);
      fclose(outfile);
    }

    menu_start=ftell(infile);
    
    // Write all the record entries here
    // Write record list entries
    for (i=0; i < num_mensen; ++i) {
      for (j=0; j < NUM_DAYS; ++j) {
        unsigned int rec_len=0;
        count += 1;
        memset(buffer, 0, BUFFER_MAXLEN);
        memset(&rcle, 0, sizeof(rcle));
        fgets(buffer, BUFFER_MAXLEN-1, infile);
        // if (debug) printf("M %u D %u: %lu\n", i, j, offset);
        rcle.attributes=i;
        temp_long=LSwap(offset);
        rcle.localChunkID=temp_long;
        /* Note to myself:
         * Don't be dumb next time and change your code if it worked before...
         * We MUST assign REAL unique IDs
         */
        temp_char = 20; rcle.uniqueID[0] = temp_char;
        temp_char = (count & 0xff00) >> 8; rcle.uniqueID[1] = temp_char;
        temp_char = (count & 0x00ff); rcle.uniqueID[2] = temp_char;
        //  1 for MensaDBRecord.weekday, strlen for strlen, since we shorten the string by one to get rid of \n we do not
        // add 1 for the terminating zero
        rec_len = 1 + strlen(buffer)/* +1 */;
        if ( (rec_len % 2) == 1) {
          // odd number of bytes. Increase by one
          rec_len += 1;
        }
        offset += rec_len;
        fwrite(&rcle, sizeof(RecordEntryType), 1, outfile);
      }
      fgets(buffer, BUFFER_MAXLEN-1, infile);
      if (strstr(buffer, "---") != buffer) {
        printf("File format error. --- missing after mensa block %u. Aborting.(%s)\n", i, buffer);
        exit(2);
        fclose(infile);
        fclose(outfile);
      }
    }

    // The 2 Byte padding after the Record List
    fwrite(&pad, 2, 1, outfile);

    // Write AppInfo Block
    fwrite(&appInfo, sizeof(appInfo), 1, outfile);

    // Jump back to beginning of third block
    fseek(infile, menu_start, SEEK_SET);


    // Write all records
    // Write data for food
    for (i=0; i < num_mensen; ++i) {
      for (j=0; j < NUM_DAYS; ++j) {
        unsigned int rec_len;
        char *tmp;
        memset(buffer, 0, BUFFER_MAXLEN);
        fgets(buffer, BUFFER_MAXLEN-1, infile);
        // if (i==0) printf("M %u; D %u: %s\n", i, j, buffer);
        rec_len = 1 + strlen(buffer)/* +1 */;
        if ( (rec_len % 2) == 1) {
          // odd number of bytes. Increase by one
          rec_len += 1;
        }
        while ((tmp = strstr(buffer, "::")) != NULL) {
          strncpy(tmp, "\n\n", 2);
        }
        mensa_record=(MensaDBRecord *)malloc( rec_len );
        memset(mensa_record, 0, rec_len);
        mensa_record->weekday=j;
        strncpy(mensa_record->text, buffer, strlen(buffer)-1);
        // if (debug && (i==0)) printf("Mensa %u day %u: %s\nWritten: %s", i, j, buffer, (char *)mensa_record);
        fwrite(mensa_record, rec_len, 1, outfile);
        free(mensa_record);
      }
      fgets(buffer, BUFFER_MAXLEN-1, infile);
      if (strstr(buffer, "---") == NULL) {
        printf("File format error. --- missing after mensa block %u. Aborting.(%s)\n", i, buffer);
        exit(2);
        fclose(infile);
        fclose(outfile);
      }
      // printf("--- found: %s\n", buffer);
    }



    fclose(outfile);
  } else {
    printf("Could not open outfile %s!\n", outfile_name);
  }

  if (debug) printf("Done.\nRecords really written: %u\n", count);
  fclose(infile);
  
  return 0;
}




/****************************************************************************
* FUNCTION:     LSwap
*
* DESCRIPTION:  transforms an INTEL-architecture long int into a MOTOROLLA-
*               architecture long int.
****************************************************************************/
unsigned long LSwap(unsigned long l){
 unsigned short temp_s1, temp_s2;
 unsigned long temp_l;

 temp_s1 = SSwap((unsigned short)((l & 0xffff0000) >> 16));
 temp_s2 = SSwap((unsigned short)(l & 0x0000ffff));
 temp_l = ((unsigned long)temp_s2 << 16) + temp_s1;
return(temp_l);
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



/* $Id: genUMENpdb.h,v 1.1 2003/02/10 22:45:50 tim Exp $
 *
 * genUMENpdb headers (typedefs, data structures etc.)
 */

/* this is a PALM OS standard -- do not change */
#define DB_NAME_SIZE 32

#define FILENAME "unimensa.pdb"
#define INFILE "mensa.pre"

#define BUFFER_MAXLEN 2047
#define NUM_DAYS 6

#define UNINAME_MAXLENGTH 16
#define UMEN_DB_VERSION 1

typedef unsigned char   Boolean;
typedef unsigned char   UInt8;
typedef unsigned short  UInt16;
typedef unsigned long   UInt32;
typedef UInt32          LocalID; // local (card relative) chunk ID
typedef char            Char;


typedef struct{ 
  UInt16 day    :5; 
  UInt16 month  :4; 
  UInt16 year   :7; 
} DateType;

typedef struct {
  LocalID	localChunkID;
  UInt8	attributes;
  UInt8	uniqueID[3];
} RecordEntryType;

// Unusable when records available.. Would need to set
// firstEntry... More theoretical usage, keep for comprehension
// of database structure
typedef struct {
  LocalID nextRecordListID;
  UInt16  numRecords;
  UInt16  firstEntry;
} RecordListType;


typedef struct {
  UInt8   name[DB_NAME_SIZE];
  UInt16  attributes;
  UInt16  version;
  UInt32  creationDate;
  UInt32  modificationDate;
  UInt32  lastBackupDate;
  UInt32  modificationNumber;
  LocalID appInfoID;
  LocalID sortInfoID;
  UInt8   type[4];
  UInt8   creator[4];
  UInt32  uniqueIDSeed;
  // RecordListType recordList;  // We do that "by hand"
} DatabaseHdrType;


typedef struct {
	UInt16	renamedCategories; 
	Char	categoryLabels[16][16];
	UInt8	categoryUniqIDs[16];
	UInt8	lastUniqID;
	UInt8	padding;
  UInt16 fromDate;
  UInt16 toDate;
  Char university[16]; // Keep exact length or BadBugs(TM) will happen
} AppInfoType;

typedef struct {
  UInt8 weekday;
  char text[2];
} MensaDBRecord;

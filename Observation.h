#include "stdio.h"
#define MissingData -888888.0
#define EndingData -777777.0
typedef struct 
/* The fields in this record uniquely identify the source of the
!  data, so that duplicates of data can be merged or discarded.
!  The horizontal location of this report (assumed constant, even
!  for balloon ascent) is geven by the lat/lon of the site.     */
{
float Latitude;  // latitude (+ degrees east)
float Longitude; // longitude (+ degrees north)
char ID[40];     // 5 digit identifier, consisting of a 2 digit block number and a 3 digit identifier (for soundings) for WMO sttn; non digit for other sources
char Name[40];   // The name corresponds to the id is obtained from id in the program that is source of data
}STATIONLOCATION;

typedef struct 
{
char PlatForm[40]; // description of the measurement device
char Source[40];   // GTS data, NCAR ADP files, bogus information, etc
float Elevation;   // station elevation
/* During the decoding process, how many errors (non conforming
!  codes) were encountered, and how many warnings (this is a subjective
!  call based on repeated incorrect -- but probably not garbled --
!  GTS codes). If a bulletin is completely garbled, the logical
!  flag to not consider this report is set.                       */
int ValidFieldsNumber;// number of valid fields in the entire report; used as the first tie-breaker in deciding which conflicting data items to keep if have duplicate rpts
int ErrorNumber;      // number of errors encountered during the decoding process
int WarningNumber;    // number of warnings encountered during the decoding process
int SequenceNumber;   // sequence numbers that tell which of 2 reports is more recent.
int DuplicatesNumber; // number of duplicates found of this observation
unsigned IsSound;     // is-a-sounding tells whether the observation possibly has multiple levels vs having only one level for srfc ob.
unsigned IsBogus;     // T/F if this is a bogus observation
unsigned Discard;     // Tells whether this observation has been found to be a dup AND has been discarded or merged.
}SOURCEINFORMATION;

typedef struct
/* Defines a data type consisting of a paired data value (real) with a
!  quality control flag that holds a binary-coded combination of error
!  codes; the codes identify possible problems with the data.        */
{
float Data;
int QualityControlFlag; // Quality control flags that are 0 if data is good, or different integers depending upon what error(s) occured
}OBSERVATIONFIELD;
typedef struct
/* The data that will occur, at most, once during a report is
!  listed here. These are typically terrestrial measured values. The
!  surface met fields are stored in a separate TYPE, to allow a
!  POINTER to the next level (if one exists). This needs to be a
!  separate TYPE so as to allow a POINTER to it                 */
{
OBSERVATIONFIELD SeaLevelPressure;              // sea level pressure
OBSERVATIONFIELD ReferencePressure;             // reference pres level for the thickness
OBSERVATIONFIELD GroundTemperature;             // ground temperature
OBSERVATIONFIELD SeaSurfaceTemperature;         // sea surface temperature
OBSERVATIONFIELD SurfacePressure;               // surface pressure
OBSERVATIONFIELD Precipitation;                 //  precipitation accumulation
OBSERVATIONFIELD DailyTemperatureMax;           // daily temperature max
OBSERVATIONFIELD DailyTemperatureMin;           // daily temperature min
OBSERVATIONFIELD MinOvernightTemperature;       // min overnight temperature
OBSERVATIONFIELD PressureTendencyIn3hr;         // pressure tendency in 3hr
OBSERVATIONFIELD PressureTendencyIn24hr;        // pressure tendency in 24hr
OBSERVATIONFIELD TotalCloudCover;               // total cloud cover (oktas)
OBSERVATIONFIELD CloudCeiling;                  // height of lowest cloud base
}TERRESTRIALOBSERVATION;

typedef struct
/* GTS report time: the valid time of the report. The largest INTEGER values
!  require only 8 digits, so that this should function properly with
!  32-bit INTEGERS.                                                      */
{
int SecondsNumber;       // number of seconds since 1 Jan
                         // 0000 UTC 1970
int JulianDay;           // Julian day
char DateCharacter[15];  // CCYYMMDDHHmmss date
}TIMEINFORMATION;

typedef struct 
/* The met data involved with this program is defined in this TYPE. The
!  standard state variables (wind, temperature, moisture, with pressure
!  and/or height to fix the vertical location) are stored here. For
!  single level observations, only one of these records is used per
!  observation. For multi-level reports, a linked list of these
!  measurement TYPEs is generated.                                     */
{
OBSERVATIONFIELD Pressure;         // pressure of observation
OBSERVATIONFIELD Height;           // height (above sea level)
OBSERVATIONFIELD Temperature;
OBSERVATIONFIELD DewPoint;
OBSERVATIONFIELD WindSpeed;
OBSERVATIONFIELD WindDirection;
OBSERVATIONFIELD WindUComponent;   // u and v components of wind
OBSERVATIONFIELD WindVComponent;   // are derived from spd and dir
OBSERVATIONFIELD RelativityHumidity;
OBSERVATIONFIELD Thickness;
}MEASUREDATA;

typedef struct _MEASUREMENT
{
MEASUREDATA Measure;           // contains data and qc code
struct _MEASUREMENT *Next;     // the met info is handled as a linked list of the measurement type
}MEASUREMENT;

class OBSERVATIONREPORT
{private:
// this is the entire report
STATIONLOCATION Location;                // for a single time, from a
SOURCEINFORMATION Information;           // single reporting platform,
TIMEINFORMATION ValidTime;               // a sounding, surface, buoy,
TERRESTRIALOBSERVATION Ground;           // aircraft or ship report
MEASUREMENT *Surface;
public:
int OutputReport(FILE *fp);
int AddMeasure(MEASUREMENT *meas);
OBSERVATIONREPORT(STATIONLOCATION,TIMEINFORMATION,SOURCEINFORMATION);
~OBSERVATIONREPORT();
};

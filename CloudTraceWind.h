typedef struct 
{
char FileName[12];
short int ByteOrder;
short int HeaderLength1;
short int HeaderLength2;
short int FilledLength;
short int RecordLength;
short int HeaderUseRecord;
short int DataUseRecord;
short int DataType;
short int CompressType;
char FormatDescriptor[8];
short int DataQuality;
}STARHEADER;

typedef struct
{
char SatelliteName[8];
short int FieldType;
short int BytesOfOneRecord;
short int TotalPointNumber;
short int StartYear;
short int StartMonth;
short int StartDay;
short int StartHour;
short int StartMinute;
short int EndYear;
short int EndMonth;
short int EndDay;
short int EndHour;
short int EndMinute;
short int MethodType;
short int FirstGuessFieldType;
short int DefaultValue;
}SCATTERFIELDHEADER;

typedef struct
{
short int PointLatitude;       // Degree x 100
short int PointLongitude;      // Degree x 100
short int ExploreLevel;        // 
short int WindDirection;       // 0~360
short int WindSpeed;           // m/s
short int CloudTopPressure;    // 
short int CloudTopTemperature; // K x 100
short int Reserved[12];
short int Discard;
}CLOUDTRACEWINDDATA;

typedef struct
{
STARHEADER *BigHeader;
SCATTERFIELDHEADER *SubHeader;
CLOUDTRACEWINDDATA *pData;
}STARCLOUDTRACEWIND;

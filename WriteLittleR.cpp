#include "stdio.h"
#include "string.h"
#include "Observation.h"
#include "CloudTraceWind.h"
STARCLOUDTRACEWIND *CloudTraceWind(char []);
char *int2str(int, char[],int);
int main(int argc,char *argv[])
{
STARCLOUDTRACEWIND *CloudTraceWindData;
FILE *fp;
STATIONLOCATION DefaultStationLoc;
SOURCEINFORMATION DefaultSourceInfo;
TIMEINFORMATION DefaultTimeInfo;
OBSERVATIONFIELD MissingField;
MEASUREMENT *BlankMeasure;
short i;
OBSERVATIONREPORT *ObsReport;
int ncount;

if(argc!=3)
{
   printf("Usage:%s readfilename writefilename\n",argv[0]);
   return 1;
}

MissingField.Data=MissingData;
MissingField.QualityControlFlag=0;

BlankMeasure=new MEASUREMENT;
//for(i=0;i<1;i++)
//{
BlankMeasure->Measure.DewPoint=MissingField;
BlankMeasure->Measure.Height=MissingField;
BlankMeasure->Measure.Pressure=MissingField;
BlankMeasure->Measure.RelativityHumidity=MissingField;
BlankMeasure->Measure.Temperature=MissingField;
BlankMeasure->Measure.Thickness=MissingField;
BlankMeasure->Measure.WindDirection=MissingField;
BlankMeasure->Measure.WindSpeed=MissingField;
BlankMeasure->Measure.WindUComponent=MissingField;
BlankMeasure->Measure.WindVComponent=MissingField;
BlankMeasure->Next=NULL;
//}

strcpy(DefaultTimeInfo.DateCharacter,"              ");
DefaultTimeInfo.JulianDay=int(MissingData);
DefaultTimeInfo.SecondsNumber=int(MissingData);

strcpy(DefaultStationLoc.Name,"Point                           ");
DefaultStationLoc.Latitude=MissingData;
DefaultStationLoc.Longitude=MissingData;
strcpy(DefaultStationLoc.ID,"                                       ");

DefaultSourceInfo.Discard=0;
DefaultSourceInfo.DuplicatesNumber=0;
DefaultSourceInfo.ErrorNumber=0;
DefaultSourceInfo.Elevation=MissingData;
DefaultSourceInfo.IsBogus=0;
DefaultSourceInfo.IsSound=1;
strcpy(DefaultSourceInfo.PlatForm,"FM-88 SATOB                            ");
DefaultSourceInfo.SequenceNumber=1;
strcpy(DefaultSourceInfo.Source,"Cloud Trace Wind From Satellite        ");
DefaultSourceInfo.ValidFieldsNumber=0;
DefaultSourceInfo.WarningNumber=0;

CloudTraceWindData=CloudTraceWind(argv[1]);
if(CloudTraceWindData==NULL) return 1;

int2str(CloudTraceWindData->SubHeader->EndYear,DefaultTimeInfo.DateCharacter,4);
int2str(CloudTraceWindData->SubHeader->EndMonth,&(DefaultTimeInfo.DateCharacter[4]),2);
int2str(CloudTraceWindData->SubHeader->EndDay,&(DefaultTimeInfo.DateCharacter[6]),2);
int2str(CloudTraceWindData->SubHeader->EndHour,&(DefaultTimeInfo.DateCharacter[8]),2);
int2str(CloudTraceWindData->SubHeader->EndMinute,&(DefaultTimeInfo.DateCharacter[10]),2);
int2str(0,&(DefaultTimeInfo.DateCharacter[12]),2);

sprintf(DefaultStationLoc.ID, "%s",CloudTraceWindData->SubHeader->SatelliteName);

//printf("%s\n%s\n",DefaultStationLoc.ID,DefaultStationLoc.Name);
if((fp=fopen(argv[2],"w+"))==NULL) return 1;

ncount=0;
for(i=0;i<CloudTraceWindData->SubHeader->TotalPointNumber;i++)
{
//		printf("%d\n",i);
    //printf("%i,%i\n",i,CloudTraceWindData->pData[i].Discard);
    if(CloudTraceWindData->pData[i].Discard ==-1 ){
//        printf("WRITE Report %i .\n",i);
	DefaultStationLoc.Latitude=float(CloudTraceWindData->pData[i].PointLatitude)/100.0;
	DefaultStationLoc.Longitude=float(CloudTraceWindData->pData[i].PointLongitude)/100.0;
        int2str(i,&(DefaultStationLoc.Name[5]),5);

	ObsReport=new OBSERVATIONREPORT(DefaultStationLoc,DefaultTimeInfo,DefaultSourceInfo);

	BlankMeasure->Measure.Pressure.Data=float(CloudTraceWindData->pData[i].ExploreLevel)*100.0;
	BlankMeasure->Measure.Temperature.Data=float(CloudTraceWindData->pData[i].CloudTopTemperature)/100.0;
	BlankMeasure->Measure.WindSpeed.Data=float(CloudTraceWindData->pData[i].WindSpeed);
	BlankMeasure->Measure.WindDirection.Data=float(CloudTraceWindData->pData[i].WindDirection);

	ObsReport->AddMeasure(BlankMeasure);
	ObsReport->OutputReport(fp);
	delete ObsReport;
        ncount++;
    }
}

printf("Write %i point to file: %s.\n",ncount, argv[2]);
fclose(fp);
return 0;
}



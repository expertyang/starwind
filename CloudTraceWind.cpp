#include "stdio.h"
#include "string.h"
#include "CloudTraceWind.h"
//unsigned long TotalRecordNumber;
STARCLOUDTRACEWIND *CloudTraceWind(char FileName[])
{
int i;
FILE *fp;
STARCLOUDTRACEWIND *CloudWind; 
STARHEADER *bigheader;
SCATTERFIELDHEADER *subheader;
CLOUDTRACEWINDDATA *data;

bigheader=new STARHEADER;
subheader=new SCATTERFIELDHEADER;
if((fp=fopen(FileName,"rb"))==NULL) return NULL;

    printf("Read AWX file: %s\n",FileName);
 
    printf("size of STARHEADER is   : %i\n",sizeof(STARHEADER));

//printf("%i\n",ftell(fp));
    fread(bigheader,sizeof(STARHEADER),1,fp);
    printf("\tFilled Length   : %i\n"
           "\tRecord Length   : %i\n"
           "\tHeaderUseRecord : %i\n"
           "\tDataUseRecord   : %i\n"
           "\tCompressType    : %i\n", 
           bigheader->FilledLength,
           bigheader->RecordLength,
           bigheader->HeaderUseRecord,
           bigheader->DataUseRecord,
           bigheader->CompressType,
           bigheader->FilledLength);

if(bigheader->DataType!=4) return NULL;
//printf("%i\n",ftell(fp));
    printf("size of SCATTERFIELDHEADER is : %i\n",sizeof(SCATTERFIELDHEADER));
    fread(subheader,sizeof(SCATTERFIELDHEADER),1,fp);
    printf("\tSatellite Name        : %s\n"
           "\tStart Time            : %i,%i,%i,%i,%i\n"
           "\tEnd Time              : %i,%i,%i,%i,%i\n"
           "\tPoint Number          : %i\n",
           subheader->SatelliteName,
           subheader->StartYear,subheader->StartMonth,subheader->StartDay,
           subheader->StartHour,subheader->StartMinute,
           subheader->EndYear,subheader->EndMonth,subheader->EndDay,
           subheader->EndHour,subheader->EndMinute, 
           subheader->TotalPointNumber);

//if(subheader->FieldType!=101 || strcmp(bigheader->FormatDescriptor,"SAT2004") != 0 ) return NULL;
if(subheader->FieldType!=101) return NULL;
//printf("%i\n",ftell(fp));
//fseek(fp,bigheader->FilledLength,1);
fseek(fp,bigheader->RecordLength*bigheader->HeaderUseRecord,0);
//subheader->TotalPointNumber=bigheader->DataUseRecord - bigheader->HeaderUseRecord;

	printf("size of CLOUDTRACEWINDDATA is : %i\n",sizeof(CLOUDTRACEWINDDATA));
	data=new CLOUDTRACEWINDDATA[subheader->TotalPointNumber];
	printf("\tTotal Ponit Number is : %i\n", subheader->TotalPointNumber);
	for(i=0;i<subheader->TotalPointNumber;i++)
	{
		fseek(fp,bigheader->RecordLength*(bigheader->HeaderUseRecord+i),0);
                // printf("%i\n",ftell(fp));
		fread(&data[i],sizeof(CLOUDTRACEWINDDATA),1,fp);

                /*
  		printf("Point:%i\n\t Lat: %i Lon: %i\n\t Lev: %i Prs: %i Tmp:%i \n\t Dir: %i Spd: %i\n",
  		i,data[i].PointLatitude, data[i].PointLongitude,
  		data[i].ExploreLevel,
  		data[i].CloudTopPressure,data[i].CloudTopTemperature,
  		data[i].WindDirection,data[i].WindSpeed);
                printf("\t Reserved: %i %i %i %i %i %i %i %i %i %i %i %i\n\t Discard: %i\n",
                data[i].Reserved[0],
                data[i].Reserved[1],
                data[i].Reserved[2],
                data[i].Reserved[3],
                data[i].Reserved[4],
                data[i].Reserved[5],
                data[i].Reserved[6],
                data[i].Reserved[7],
                data[i].Reserved[8],
                data[i].Reserved[9],
                data[i].Reserved[10],
                data[i].Reserved[11],
                data[i].Discard); */

	}

CloudWind=new STARCLOUDTRACEWIND;
CloudWind->BigHeader=bigheader;
CloudWind->SubHeader=subheader;
CloudWind->pData=data;
return CloudWind;
printf("%i\n",ftell(fp));
}

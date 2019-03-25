#include "Observation.h"
#include "stdio.h"
void PrintLogicalMark(FILE *fp, int len, int Mark)
{
int i;
for(i=0;i<len-1;i++) fprintf(fp, " ");
if(Mark != 0)
{
	fprintf(fp,"T");
}else{
	fprintf(fp,"F");
}
}

int OBSERVATIONREPORT::OutputReport(FILE *fp)
{
MEASUREMENT *CurrentReport;

if(this->Surface->Next==NULL) return 1;

fprintf(fp,"%20.5f%20.5f%-40s%-40s%-40s%-40s%20.5f%10d%10d%10d%10d%10d",
		this->Location.Latitude,           this->Location.Longitude,
		this->Location.ID,                 this->Location.Name,
		this->Information.PlatForm,        this->Information.Source,
		this->Information.Elevation,       this->Information.ValidFieldsNumber,
		this->Information.ErrorNumber,     this->Information.WarningNumber,
		this->Information.SequenceNumber,  this->Information.DuplicatesNumber);
PrintLogicalMark(fp,10,this->Information.IsSound);
PrintLogicalMark(fp,10,this->Information.IsBogus);
PrintLogicalMark(fp,10,this->Information.Discard);

fprintf(fp,"%10d%10d%20s",  this->ValidTime.SecondsNumber, 
							this->ValidTime.JulianDay,
							this->ValidTime.DateCharacter);
fprintf(fp,"%13.5f%7d%13.5f%7d%13.5f%7d%13.5f%7d%13.5f%7d%13.5f%7d",
		this->Ground.SeaLevelPressure.Data,       this->Ground.SeaLevelPressure.QualityControlFlag,
		this->Ground.ReferencePressure.Data,      this->Ground.ReferencePressure.QualityControlFlag,
		this->Ground.GroundTemperature.Data,      this->Ground.GroundTemperature.QualityControlFlag,
		this->Ground.SeaSurfaceTemperature.Data,  this->Ground.SeaSurfaceTemperature.QualityControlFlag,
		this->Ground.SurfacePressure.Data,        this->Ground.SurfacePressure.QualityControlFlag,
		this->Ground.Precipitation.Data,          this->Ground.Precipitation.QualityControlFlag);
fprintf(fp,"%13.5f%7d%13.5f%7d%13.5f%7d%13.5f%7d%13.5f%7d%13.5f%7d%13.5f%7d\n",
		this->Ground.DailyTemperatureMax.Data,    this->Ground.DailyTemperatureMax.QualityControlFlag,
		this->Ground.DailyTemperatureMin.Data,    this->Ground.DailyTemperatureMin.QualityControlFlag,
		this->Ground.MinOvernightTemperature.Data,this->Ground.MinOvernightTemperature.QualityControlFlag,
		this->Ground.PressureTendencyIn3hr.Data,  this->Ground.PressureTendencyIn3hr.QualityControlFlag,
		this->Ground.PressureTendencyIn24hr.Data, this->Ground.PressureTendencyIn24hr.QualityControlFlag,
		this->Ground.TotalCloudCover.Data,        this->Ground.TotalCloudCover.QualityControlFlag,
		this->Ground.CloudCeiling.Data,           this->Ground.CloudCeiling.QualityControlFlag);

CurrentReport=this->Surface;
while(CurrentReport!=NULL)
{
	fprintf(fp,"%13.5f%7d%13.5f%7d%13.5f%7d%13.5f%7d%13.5f%7d%13.5f%7d%13.5f%7d%13.5f%7d%13.5f%7d%13.5f%7d\n",
			CurrentReport->Measure.Pressure.Data,CurrentReport->Measure.Pressure.QualityControlFlag,
			CurrentReport->Measure.Height.Data,CurrentReport->Measure.Height.QualityControlFlag,
			CurrentReport->Measure.Temperature.Data,CurrentReport->Measure.Temperature.QualityControlFlag,
			CurrentReport->Measure.DewPoint.Data,CurrentReport->Measure.DewPoint.QualityControlFlag,
			CurrentReport->Measure.WindSpeed.Data,CurrentReport->Measure.WindSpeed.QualityControlFlag,
			CurrentReport->Measure.WindDirection.Data,CurrentReport->Measure.WindDirection.QualityControlFlag,
			CurrentReport->Measure.WindUComponent.Data,CurrentReport->Measure.WindUComponent.QualityControlFlag,
			CurrentReport->Measure.WindVComponent.Data,CurrentReport->Measure.WindVComponent.QualityControlFlag,
			CurrentReport->Measure.RelativityHumidity.Data,CurrentReport->Measure.RelativityHumidity.QualityControlFlag,
			CurrentReport->Measure.Thickness.Data,CurrentReport->Measure.Thickness.QualityControlFlag);
	CurrentReport=CurrentReport->Next;
};

fprintf(fp,"%7d%7d%7d\n",this->Information.ValidFieldsNumber,this->Information.ErrorNumber,this->Information.WarningNumber);
return 0;
}
OBSERVATIONREPORT::OBSERVATIONREPORT(STATIONLOCATION StnLoc, 
									 TIMEINFORMATION TimeInfo, 
									 SOURCEINFORMATION SrcInfo)
{
MEASUREMENT *EndOfReport;
OBSERVATIONFIELD EndField;
OBSERVATIONFIELD MissingField;

EndField.Data=EndingData;
EndField.QualityControlFlag=0;

MissingField.Data=MissingData;
MissingField.QualityControlFlag=0;

EndOfReport = new MEASUREMENT;
EndOfReport->Measure.Pressure=EndField;
EndOfReport->Measure.Height=EndField;
EndOfReport->Measure.DewPoint=MissingField;
EndOfReport->Measure.RelativityHumidity=MissingField;
EndOfReport->Measure.Temperature=MissingField;
EndOfReport->Measure.Thickness=MissingField;
EndOfReport->Measure.WindDirection=MissingField;
EndOfReport->Measure.WindSpeed=MissingField;
EndOfReport->Measure.WindUComponent=MissingField;
EndOfReport->Measure.WindVComponent=MissingField;
EndOfReport->Measure.Temperature.Data=0;
EndOfReport->Next=NULL;

this->Location=StnLoc;
this->Information=SrcInfo;
this->ValidTime=TimeInfo;
this->Ground.CloudCeiling=MissingField;
this->Ground.DailyTemperatureMax=MissingField;
this->Ground.DailyTemperatureMin=MissingField;
this->Ground.GroundTemperature=MissingField;
this->Ground.MinOvernightTemperature=MissingField;
this->Ground.Precipitation=MissingField;
this->Ground.PressureTendencyIn24hr=MissingField;
this->Ground.PressureTendencyIn3hr=MissingField;
this->Ground.ReferencePressure=MissingField;
this->Ground.SeaLevelPressure=MissingField;
this->Ground.SeaSurfaceTemperature=MissingField;
this->Ground.SurfacePressure=MissingField;
this->Ground.TotalCloudCover=MissingField;

this->Surface=EndOfReport;
}
int OBSERVATIONREPORT::AddMeasure(MEASUREMENT *Measurement)
{
MEASUREMENT *meas;
MEASUREMENT *PreviousReport;
MEASUREMENT *CurrentReport;
//int num;
int validfields;
//num=0;
meas=new MEASUREMENT;
*meas=*Measurement;
validfields=(meas->Measure.DewPoint.Data!=MissingData) +
			(meas->Measure.Height.Data!=MissingData) +
			(meas->Measure.Pressure.Data!=MissingData) +
			(meas->Measure.RelativityHumidity.Data!=MissingData) +
			(meas->Measure.Temperature.Data!=MissingData) +
			(meas->Measure.Thickness.Data!=MissingData) +
			(meas->Measure.WindDirection.Data!=MissingData) +
			(meas->Measure.WindSpeed.Data!=MissingData) +
			(meas->Measure.WindUComponent.Data!=MissingData) +
			(meas->Measure.WindVComponent.Data!=MissingData);

PreviousReport=NULL;
CurrentReport=this->Surface;

while(CurrentReport->Next!=NULL)//Measure.Pressure.Data != EndingData &&
	  //CurrentReport->Measure.Height.Data != EndingData )
{

	PreviousReport=CurrentReport;
	CurrentReport=CurrentReport->Next;
	if( (PreviousReport->Measure.Pressure.Data >= meas->Measure.Pressure.Data &&
	   	CurrentReport->Measure.Pressure.Data <= meas->Measure.Pressure.Data) || 
		(PreviousReport->Measure.Height.Data <= meas->Measure.Height.Data &&
		CurrentReport->Measure.Height.Data >= meas->Measure.Height.Data) ) break;
}
//printf("%d\n",validfields);
	meas->Next=CurrentReport;
	if(PreviousReport!=NULL) 
	{
		PreviousReport->Next=meas;
	}else{
		this->Surface=meas;
	}
this->Information.ValidFieldsNumber+=validfields;

while(CurrentReport->Next != NULL)//CurrentReport->Measure.Pressure.Data != EndingData &&
	  //CurrentReport->Measure.Height.Data != EndingData )
{
//	num+=1;
	PreviousReport=CurrentReport;
	CurrentReport=CurrentReport->Next;
}//find End of Report

//printf("%d\n",num);
CurrentReport->Measure.Temperature.Data=float(this->Information.ValidFieldsNumber);
//printf("%d\n",validfields);
return 0;

}

OBSERVATIONREPORT::~OBSERVATIONREPORT()
{
MEASUREMENT *PreviousReport;
MEASUREMENT *CurrentReport;

CurrentReport=this->Surface;
this->Surface=NULL;
while(CurrentReport->Next!=NULL)
{

	PreviousReport=CurrentReport;
	CurrentReport=CurrentReport->Next;
	PreviousReport->Next=NULL;
	delete PreviousReport;
}
}



 func_ OperatingSystem
OperatingSystemContext(void)
{
 OperatingSystem Result = OperatingSystem_Null;
#if OS_WINDOWS
 Result = OperatingSystem_Windows;
#elif OS_LINUX
 Result = OperatingSystem_Linux;
#elif OS_MAC
 Result = OperatingSystem_Mac;
#endif
 return(Result);
}


func_ Architecture 
ArchitectureContext(void)
{
 OperatingSystem Result = Architecture_Null;
#if ARCH_X64
 Result = Architecture_X64;
#elif ARCH_X86
 Result = Architecture_X86;
#elif ARCH_ARM
 Result = Architecture_Arm;
#endif
 return(Result);
}

func_ char *
StringFromOperatingSistem(OperatingSystem OS)
{
 char *Result = 0;
 switch(OS) {
  case OperatingSystem_Null: {
   Result = "Null";
  } break;
  case OperatingSystem_Windows: {
   Result = "Windows";
  } break;
  case OperatingSystem_Linux: {
   Result = "Linux";
  } break;
  case OperatingSystem_Mac: {
   Result = "Mac";
  } break;
  
  OperatingSystem;
 }
 return(Result);
}

func_ char *
StringFromArchitecture(Architecture Arch)
{
 char *Result = 0;
 switch(Arch)
 {
  case Architecture_Null: {
   Result = "Null";
  } break;
  case Architecture_X64: {
   Result = "X64";
  } break;
  case Architecture_X86: {
   Result = "X86";
  } break;
  case Architecture_Arm: {
   Result = "Arm";
  } break;
  
  InvalidCase;
 }
 return(Result);
}

func_ char *
StringFromMonth(Month month)
{
 char *Result = 0;
 switch(month)
 {
  case Month_Jan: {
   Result = "January";
  } break;
  case Month_Feb: {
   Result = "Feburary";
  } break;
  case Month_Mar: {
   Result = "March";
  } break;
  case Month_Apr: {
   Result = "April";
  } break;
  case Month_May: {
   Result = "May";
  } break;
  case Month_Jun: {
   Result = "June";
  } break;
  case Month_Jul: {
   Result = "July";
  } break;
  case Month_Aug: {
   Result = "August";
  } break;
  case Month_Sep: {
   Result = "September";
  } break;
  case Month_Oct: {
   Result = "October";
  } break;
  case Month_Nov: {
   Result = "November";
  } break;
  case Month_Dec: {
   Result = "Decemeber";
  } break;
  
  InvalidCase;
 }
 return(Result);
}

func_ char *
StringFromWeekDay(WeekDay day)
{
 char *Result = 0;
 switch(day)
 {
  case WeekDay_Monday: {
   Result = "Monday";
  } break;
  case WeekDay_Tuesday: {
   Result = "Tuesday";
  } break;
  case WeekDay_Wednsday: {
   Result = "Wednsday";
  } break;
  case WeekDay_Thursday: {
   Result = "Thursday";
  } break;
  case WeekDay_Friday: {
   Result = "Friday";
  } break;
  case WeekDay_Saturday: {
   Result = "Saturday";
  } break;
  case WeekDay_Sunday: {
   Result = "Sunday";
  } break;
  
  InvalidCase;
 }
 return(Result);
}

////////////////////////////////////
// NOTE(nates): Time functions

func_ DenseTime
DenseTimeFromDateTime(DateTime *time)
{
 U64 result = 0;
 U32 year_decoded = (U32)((S32)time->year + 0x8000);
 result += year_decoded;
 result *= 12;
 result += (time->mon + 1);
 result *= 31;
 result += time->day;
 result *= 24;
 result += time->hour;
 result *= 60;
 result += time->min;
 result *= 61;
 result += time->sec;
 result *= 1000;
 result += time->ms;
 return(result);
}

func_ DateTime
DateTimeFromDenseTime(DenseTime time)
{
 DateTime result = {0};
 result.ms = time%1000;
 time /= 1000;
 result.sec = time%61;
 time /= 61;
 result.min = time%60;
 time /= 60;
 result.hour = time%24;
 time /= 24;
 result.day = time%31;
 time /= 31;
 result.mon = (time%12) + 1;
 time /= 12;
 S32 year_encoded = (S32)time;
 result.year = (U32)(year_encoded - 0x8000);
 return(result);
}
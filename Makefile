default:	star2little_r
star2little_r:	CloudTraceWind.cpp  Observation.cpp  WriteLittleR.cpp int2str.cpp 
		g++ CloudTraceWind.cpp  Observation.cpp  WriteLittleR.cpp \
			 int2str.cpp  -o star2little_r

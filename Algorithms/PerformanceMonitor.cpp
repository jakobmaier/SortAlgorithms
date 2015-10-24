#include "PerformanceMonitor.h"

#include <math.h>



PerformanceMonitor::PerformanceMonitor() {
	QueryPerformanceFrequency(&mFrequency);
}


void PerformanceMonitor::start() {
	QueryPerformanceCounter(&mStartTicks);
}

void PerformanceMonitor::stop() {
	QueryPerformanceCounter(&mStopTicks);
	mMillis = -(mStartTicks.QuadPart - mStopTicks.QuadPart) * 1000.0 / mFrequency.QuadPart;;
}


double PerformanceMonitor::millis() {
	return mMillis;
}

std::string PerformanceMonitor::toString() {	//hh:mm:ss.mmm
	long millis = static_cast<long>(floor(mMillis));
	long sek = millis / 1000;
	long min = sek / 60;
	long h = min / 60;

	char str[12 + 1];
	sprintf(str, "%02d:%02d:%02d.%03d", h % 100, min % 60, sek % 60, millis % 1000);

	return std::string(str);
}
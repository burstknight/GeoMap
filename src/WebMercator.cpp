#include "../includes/WebMercator.h"
#include <cmath>
#include <math.h>

#define Math_PI 3.1415926

int lat2TileY(double dLat, int iZoom){
	double dLatRad = dLat*Math_PI/180.0;
	return (int)(floor((1.0 - asinh(tan(dLatRad))) / Math_PI) / 2.0 * (1 << iZoom));
} // End of lat2TileY

int long2TileX(double dLong, int iZoom){
	return (int)(floor(dLong + 180.0) /360.0 * (1 << iZoom) );
} // End of long2TileX

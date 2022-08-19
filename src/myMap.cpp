#include "../includes/myMap.h"
#include "../includes/WebMercator.h"
#include "opencv2/core/types.hpp"
#include <cassert>
#include <cmath>
#include <math.h>

myMap::myMap(double dLat, double dLong, int iZoom, int iWidth, int iHeight){
	this->m_dLat = dLat;
	this->m_dLong = dLong;
	this->m_iZoom = iZoom;
	this->m_iWidth = iWidth;
	this->m_iHeight = iHeight;

	assert(this->m_iZoom <= Zoom_Max);
} // End of constructor

void myMap::getTilesCoord(std::vector<cv::Point2i> &vTileCoords){
	vTileCoords.clear();

	cv::Point2i oDelta;
	oDelta.x = (int)ceil(0.5*m_iWidth/Tile_Width);
	oDelta.y = (int)ceil(0.5*m_iHeight/Tile_Height);

	for(int x = -oDelta.x; x <= oDelta.x; x++){
		for(int y = -oDelta.y; y <= oDelta.y; y++){
			cv::Point2i oPoint;
			oPoint.x = x;
			oPoint.y = y;
			vTileCoords.push_back(oPoint);
		} // End of for-loop
	} // End of for-loop
} // End of getTilesCoord


#include "../includes/myMap.h"
#include "../includes/WebMercator.h"
#include "opencv2/core/mat.hpp"
#include "opencv2/core/matx.hpp"
#include "opencv2/core/types.hpp"
#include <curl/curl.h>
#include <math.h>
#include <string>
#include <vector>
#include <opencv2/core/hal/interface.h>

myMap::myMap(double dLat, double dLong, int iZoom, int iWidth, int iHeight){
	this->m_dLat = dLat;
	this->m_dLong = dLong;
	this->m_iZoom = iZoom;
	this->m_iWidth = iWidth;
	this->m_iHeight = iHeight;

	assert(this->m_iZoom <= Zoom_Max);
} // End of constructor

myMap::~myMap(){
	m_mMap.release();
} // End of destructor

void myMap::getTilesCoord(std::vector<cv::Point2i> &vTileCoords, cv::Size2i &oTileSize){
	vTileCoords.clear();
	oTileSize.width = 0;
	oTileSize.height = 0;

	cv::Point2i oDelta;
	oDelta.x = (int)ceil(0.5*m_iWidth/Tile_Width);
	oDelta.y = (int)ceil(0.5*m_iHeight/Tile_Height);

	for(int x = -oDelta.x; x <= oDelta.x; x++){
		for(int y = -oDelta.y; y <= oDelta.y; y++){
			cv::Point2i oPoint;
			oPoint.x = x;
			oPoint.y = y;
			vTileCoords.push_back(oPoint);
			oTileSize.height += 256;
		} // End of for-loop

		oTileSize.width += 256;
	} // End of for-loop
} // End of getTilesCoord

size_t myMap::writeMemoryCallback(void *pContents, size_t iSize, size_t nmemb, void *userp){
	size_t iRealSize = iSize*nmemb;
	BufferStruct *poBuffer = (BufferStruct*)userp;
	char *pcNewBuffer = (char*)realloc(poBuffer->pcBuffer, iRealSize + poBuffer->iSize);
	if(NULL == pcNewBuffer){
		printf("Error: Out of memory to get OSM map!\n");
		return 0;
	} // End of if-condition

	poBuffer->pcBuffer = pcNewBuffer;
	memcpy(&(poBuffer->pcBuffer[poBuffer->iSize]), pContents, iRealSize);
	poBuffer->iSize += iRealSize;
	return iRealSize;
} // End of writeMemoryCallback

cv::Mat myMap::getMap(){
	std::vector<cv::Point2i> vTileCoords;
	BufferStruct oBuffer = {.pcBuffer = NULL, .iSize = 0};
	cv::Size2i oTileSize;

	getTilesCoord(vTileCoords, oTileSize);
	m_mMap.create(oTileSize, CV_8UC3);

	curl_global_init(CURL_GLOBAL_ALL);
	CURL *curlHandle = curl_easy_init();
	CURLcode iRes;

	char* pcUrl = (char*)malloc(sizeof(char)*1024);

	curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
	curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &oBuffer);
	curl_easy_setopt(curlHandle, CURLOPT_USERAGENT, "GeoMap/1.0");
	curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, 1L);
	for(int i = 0; i < vTileCoords.size(); i++){
		sprintf(pcUrl, "http://a.tile.openstreetmap.org/%d/%d/%d.png", 
				m_iZoom, vTileCoords[i].x, vTileCoords[i].y);	

		curl_easy_setopt(curlHandle, CURLOPT_URL, pcUrl);

		iRes = curl_easy_perform(curlHandle);

		if(CURLE_OK != iRes){
			printf("\nError: curl_easy_perform() falied: %s\n", curl_easy_strerror(iRes));
		} else {
			printf("\n%lu bytes retrived\n", (unsigned long)oBuffer.iSize);
			cv::Mat mRoi;
			mRoi.create(256, 256, CV_8UC3);
			memcpy(mRoi.data, &(oBuffer.pcBuffer[oBuffer.iSize]), oBuffer.iSize);
			int iRow = 0, iCol = 0;

			for(int r = vTileCoords[i].y; r < vTileCoords[i].y + 256; r++){
				iCol = 0;
				for(int c = vTileCoords[i].x; c < vTileCoords[i].x + 256; c++){
					m_mMap.at<cv::Vec3b>(r, c) = mRoi.at<cv::Vec3b>(iRow, iCol);
					iCol++;
				} // End of for-loop

				iRow++;
			} // End of for-loop
		} // End of if-condition

	} // End of for-loop
	
	curl_easy_cleanup(curlHandle);
	curl_global_cleanup();
	free(pcUrl);
	pcUrl = NULL;
	free(oBuffer.pcBuffer);
	oBuffer.pcBuffer = NULL;

	cv::Rect2i oRoi;
	oRoi.width = m_iWidth;
	oRoi.height = m_iHeight;
	oRoi.x = (m_mMap.cols - m_iWidth)/2;
	oRoi.y = (m_mMap.rows - m_iHeight)/2;

	cv::Mat mTarget = m_mMap(oRoi);

	return mTarget.clone();
} // End of myMap::getMap

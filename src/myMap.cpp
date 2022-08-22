#include "../includes/myMap.h"
#include "../includes/WebMercator.h"
#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <curl/curl.h>
#include <curl/easy.h>
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
	size_t realSize = iSize*nmemb;
	realloc(userp, realSize);
	memcpy(userp, pContents, realSize);
	return realSize;
} // End of writeMemoryCallback

cv::Mat myMap::getMap(){
	std::vector<cv::Point2i> vTileCoords;
	char* pcBuffer = NULL;
	cv::Size2i oTileSize;

	getTilesCoord(vTileCoords, oTileSize);
	m_mMap.create(oTileSize, CV_8UC3);

	curl_global_init(CURL_GLOBAL_ALL);
	CURL *curlHandle = curl_easy_init();
	CURLcode iRes;

	char* pcUrl = (char*)malloc(sizeof(char)*1024);
	char* ppcSubDomain[] = {"a", "b", "c"};
	int index = 0;
	for(int i = 0; i < vTileCoords.size(); i++){
		sprintf(pcUrl,"http://%s.tile.openstreetmap.org/%d/%d/%d.png", 
				ppcSubDomain[index], m_iZoom, vTileCoords[i].x, vTileCoords[i].y);	

		curl_easy_setopt(curlHandle, CURLOPT_URL, pcUrl);
		curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
		curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, pcBuffer);
		curl_easy_setopt(curlHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curlHandle, CURLOPT_HTTPPROXYTUNNEL, 1L);
		curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 1L);

		iRes = curl_easy_perform(curlHandle);

		if(CURLE_OK != iRes){
			printf("Error: curl_easy_perform() falied: %s\n", curl_easy_strerror(iRes));
		} else {
			cv::Mat mRoi;
			mRoi.create(256, 256, CV_8UC3);
			memcpy(mRoi.data, pcBuffer, sizeof(char)*256*256);
			int iRow = 0, iCol = 0;
			
			for(int r = vTileCoords[i].y; r < vTileCoords[i].y + 256; r++){
				iCol = 0;
				for(int c = vTileCoords[i].x; c < vTileCoords[i].x + 256; c++){
					m_mMap.at<uchar>(r, c) = mRoi.at<uchar>(iRow, iCol);
					iCol++;
				} // End of for-loop

				iRow++;
			} // End of for-loop
		} // End of if-condition

	} // End of for-loop
	
	curl_easy_cleanup(curlHandle);
	curl_global_cleanup();
	if(NULL == pcBuffer){
		free(pcBuffer);
		pcBuffer = NULL;
	} // End of if-condition
	free(pcUrl);
	pcUrl = NULL;

	cv::Rect2i oRoi;
	oRoi.width = m_iWidth;
	oRoi.height = m_iHeight;
	oRoi.x = (m_mMap.cols - m_iWidth)/2;
	oRoi.y = (m_mMap.rows - m_iHeight)/2;

	cv::Mat mTarget = m_mMap(oRoi);

	return mTarget.clone();
} // End of myMap::getMap

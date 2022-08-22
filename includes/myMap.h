#ifndef MYMAP_H
#define MYMAP_H

#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"
#include <cstddef>
#include <opencv2/opencv.hpp>
#include <utility>
#include <vector>
#include <curl/curl.h>

#define Zoom_Max  19
#define Tile_Width 256
#define Tile_Height 256

/**
 * Desciption: This class can parse GPS position to get map from OSM
 */
class myMap{
	// private fields
	private:
		cv::Mat m_mMap;
		double m_dLat;
		double m_dLong;
		int m_iZoom;
		int m_iWidth;
		int m_iHeight;

	// public method
	public:
		/** 
		 * Desciption: Constructor
		 * @param dLat: The center latitude
		 * @param dLong: Thte center longitude
		 * @param iZoom: The level layer of tile map
		 * @param iWidth: The map image width
		 * @param iHeight: The map image height
		 */
		myMap(double dLat, double dLong, int iZoom, int iWidth = 1900, int iHeight = 1000);

		/** 
		 * Desciption: Get map image
		 * @return Return the map image
		 */
		cv::Mat getMap();
	
	// private methods
	private:
		/** 
		 * @brief Desciption: Get the list of the coordinates for tiles
		 * @param vTileCoords[out]: The list of the coordinates for tiles
		 * @param oTileSize[out]: The size of the tiles
		 */
		void getTilesCoord(std::vector<cv::Point2i> &vTileCoords, cv::Size2i &oTileSize);

		static size_t writeMemoryCallback(void *pContents, size_t iSize, size_t nmemb, void *userp);
}; // End of class myMap

#endif

#ifndef MYMAP_H
#define MYMAP_H

#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"
#include <opencv4/opencv2/opencv.hpp>
#include <utility>
#include <vector>

#define Zoom_Max  19
#define Tile_Width 256
#define Tile_Height 256

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
		/* Desciption: Constructor
		 * @param dLat: The center latitude
		 * @param dLong: Thte center longitude
		 * @param iZoom: The level layer of tile map
		 * @param iWidth: The map image width
		 * @param iHeight: The map image height
		 */
		myMap(double dLat, double dLong, int iZoom, int iWidth = 1900, int iHeight = 1000);

		/* Desciption: Get map image
		 * @return Return the map image
		 */
		cv::Mat getMap() const;
	
	// private methods
	private:
		/* Desciption: Get the list of the coordinates for tiles
		 * @param vTileCoords[out]: The list of the coordinates for tiles
		 */
		void getTilesCoord(std::vector<cv::Point2i> &vTileCoords);
}; // End of class myMap

#endif

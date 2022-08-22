#ifndef WEBMERCATOR_H
#define WEBMERCATOR_H

/**
 * Description: Convert the degree latitude to coordinate for map tile
 * @param dLat: The degree unit latitude
 * @param iZoom: The zoom level of map tile
 * @return Return the y coordinate in the map tile
 */
int lat2TileY(double dLat, int iZoom);

/**
 * Description: Convert the degree longitude to coordinate for map tile
 * @param dLat: The degree unit longitude
 * @param iZoom: The zoom level of map tile
 * @return Return the x coordinate in the map tile
 */
int long2TileX(double dLong, int iZoom);

#endif

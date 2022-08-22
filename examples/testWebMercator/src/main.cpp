#include "../../../includes/WebMercator.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief This struct is to store the valus from parsing the arguments by user.
 */
struct Position{
	double dLat; 	///< The GPS latitude
	double dLong; 	///< The GPS longitude
	int iZoom; 		///< The tile map level
};

/**
 * @brief Parse arguments from user.
 * @param argc: The number of the arguments.
 * @param argv: The list of the arguments.
 * @retval 0: Succeed to parse the arguments.
 * @retval -1: Fail to parse the arguments.
 * @retval 1: Show usage.
 */
int parseArgs(int argc, char ** argv, Position &oPosition);

/**
 * @brief Parse GPS point to connvert the coordinate system.
 * @param oPosition: This struct contains GPS point.
 */
void parseGPS(Position &oPosition);

int main(int argc, char **argv){
	Position oPosition;
	oPosition.dLat = 0.0;
	oPosition.dLong = 0.0;
	oPosition.iZoom = 1;
	
	if(0 == parseArgs(argc, argv, oPosition)){
		parseGPS(oPosition);
	} // End of if-condition

	return 0;
} // End of main

int parseArgs(int argc, char ** argv, Position &oPosition){
	while(true){
		int iArg = getopt(argc, argv, "hz:x:y:");
		if(-1 == iArg)
			break;

		switch (iArg) {
			case 'h':
				printf("Usage:\n"
						"\t-z\tSet the tile map level.\n"
						"\t-x\tSet the longitude.\n"
						"\t-y\tSet the latitude.\n"
						"\t-h\tShow the usage.\n");
				return 1;
			case 'x':
				oPosition.dLat = atof(optarg);
				break;
			case 'y':
				oPosition.dLong = atof(optarg);
				break;
			case 'z':
				oPosition.iZoom = atoi(optarg);
				break;
			default:
				printf("Error: The argument '-%c' is invalid!"
						"Please see the usage.\n", iArg);
				return -1;
		} // End of switch
	} // End of while-loop

	return 0;
} // End of parseArgs

void parseGPS(Position &oPosition){
	printf("The raw GPS position: (%.6f, %.6f, %d)\n", oPosition.dLat, oPosition.dLong, oPosition.iZoom);	
	printf("The tile map position: (%d, %d, %d)\n", 
			long2TileX(oPosition.dLong, oPosition.iZoom), 
			lat2TileY(oPosition.dLat, oPosition.iZoom), 
			oPosition.iZoom);
} // End of parseGPS


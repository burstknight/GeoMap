#include <cstddef>
#include <stdlib.h>
#include <stdio.h>
#include "../../../includes/myMap.h"
#include "opencv2/imgcodecs.hpp"
#include <unistd.h>

/**
 * @brief This struct can store the datum that parsing from arguments.
 */
struct Params{
	double dLat; 	///< The GPS latitude
	double dLong; 	///< The GPS longitude
	int iZoom; 		///< The tile map level
	char *pcPath; 	///< The map image path to store
};

/**
 * @brief Parse arguments
 * @param argc[in]: The number of the arguments
 * @param argv[in]: The list of the arguments
 * @param oParms[out]: The parsed arguments
 * @retval 0: Succeed to parse arguments
 * @retval 1: Show the usage
 * @retval -1: Fail to parse arguments
 */
int parseArgs(int argc, char** argv, Params &oParms);

/**
 * @brief Test the class `myMap` to download map image from OSM.
 * @param oParms[in]: The arguments is given by user.
 */
void testDownloadMap(Params &oParms);

int main(int argc, char** argv){
	Params oParms = {0.0, 0.0, 19, (char*)malloc(sizeof(char)*1024)};

	sprintf(oParms.pcPath, "./map.bmp");

	if(0 == parseArgs(argc, argv, oParms)){
		testDownloadMap(oParms);
	} // End of if-condition

	free(oParms.pcPath);
	oParms.pcPath = NULL;
	return 0;
} // End of main

int parseArgs(int argc, char** argv, Params &oParms){
	while (1) {
		int iArg = getopt(argc, argv, "hx:y:z:o:");
		if(-1 == iArg)
			break;

		switch (iArg) {
			case 'h':
				printf("This program is to test the class myMap.\n\n"
						"Usage:\n"
						"\t-h:\tShow the usage.\n"
						"\t-z:\tSet the tile map level.\n"
						"\t-x:\tSet the longitude.\n"
						"\t-y:\tSet the latitude.\n"
						"\t-o:\tSet the path to store map image.\n");
				return 1;
			case 'z':
				oParms.iZoom = atoi(optarg);
				break;
			case 'x':
				oParms.dLong = atoi(optarg);
				break;
			case 'y':
				oParms.dLat = atoi(optarg);
				break;
			case 'o':
				sprintf(oParms.pcPath, "%s", optarg);
				break;
			default:
				printf("Error: The argument '-%c' is invalid! Please use '-h' to see the usage.", iArg);
				return -1;
		} // End of switch
	} // End of while-loop
	
	return  0;;
} // End of parseArgs

void testDownloadMap(Params &oParms){
	myMap* poMap = new myMap(oParms.dLat, oParms.dLong, oParms.iZoom);
	
	printf("Downloading map from OSM...");
	cv::Mat mImage = poMap->getMap();
	printf(" Done!\n");

	printf("Storing image...");
	cv::imwrite(oParms.pcPath, mImage);
	printf(" Done!\n");

	delete poMap;
	poMap = NULL;
} // End of testDownloadMap

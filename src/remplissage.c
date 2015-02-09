#include<stdlib.h>
#include<stdio.h>

#include "utils.h"
#include "remplissage.h"

#define MAX_POLY_CORNERS 256

#define IMAGE_TOP 0
#define IMAGE_BOT 500
#define IMAGE_LEFT 0
#define IMAGE_RIGHT 500


PointsToFill remplissageLCA(CustomPolygon p) {
	int polyCorners = p.nbVertices;
	PointsToFill pointsToFill;
	pointsToFill.nbPointsToFill = 0;

	//  public-domain code by Darel Rex Finley, 2007

	int  nodes, nodeX[MAX_POLY_CORNERS], /*pixelX,*/ pixelY, i, j, swap;

	//  Loop through the rows of the image.
	for(pixelY = IMAGE_TOP; pixelY < IMAGE_BOT; pixelY++) {

		//  Build a list of nodes.
		nodes = 0;
		j = polyCorners - 1;
		for(i = 0; i < polyCorners; i++) {
			if(p.vertices[i].y < (double) pixelY && p.vertices[j].y >= (double) pixelY ||
			   p.vertices[j].y < (double) pixelY && p.vertices[i].y >= (double) pixelY) {
				nodeX[nodes++] = (int) (p.vertices[i].x + (pixelY - p.vertices[i].y) / (p.vertices[j].y - p.vertices[i].y) * (p.vertices[j].x - p.vertices[i].x));
			}
			j = i;
		}

		//  Sort the nodes, via a simple “Bubble” sort.
		i = 0;
		while(i < nodes - 1) {
			if(nodeX[i] > nodeX[i + 1]) {
				swap = nodeX[i]; nodeX[i] = nodeX[i + 1]; nodeX[i + 1] = swap; if(i) i--;
			}
			else {
				i++;
			}
		}

		//  Fill the pixels between node pairs.
		for(i = 0; i < nodes; i += 2) { // i+=2
			if(nodeX[i] >= IMAGE_RIGHT)
				break;
			if(nodeX[i + 1] > IMAGE_LEFT) {
				if(nodeX[i] < IMAGE_LEFT)
					nodeX[i] = IMAGE_LEFT;
				if(nodeX[i + 1] > IMAGE_RIGHT)
					nodeX[i + 1] = IMAGE_RIGHT;
				for(j = nodeX[i]; j < nodeX[i + 1]; j++) {
					Point p = {j, pixelY};
					pointsToFill.pointsToFill[pointsToFill.nbPointsToFill] = p;
					pointsToFill.nbPointsToFill++;
				}
			}
		}
	}

	return pointsToFill;
}

PointsToFill remplissageLCANotre(CustomPolygon p) {
	int polyCorners = p.nbVertices;
	PointsToFill pointsToFill;
	pointsToFill.nbPointsToFill = 0;

	int  nodes, nodeX[MAX_POLY_CORNERS], /*pixelX,*/ pixelY, i, j, swap;

	//  Loop through the rows of the image.
	for(pixelY = IMAGE_TOP; pixelY < IMAGE_BOT; pixelY++) {

		//  Build a list of nodes.
		nodes = 0;
		j = polyCorners - 1;
		for(i = 0; i < polyCorners; i++) {
			if(p.vertices[i].y < (double) pixelY && p.vertices[j].y >= (double) pixelY ||
			   p.vertices[j].y < (double) pixelY && p.vertices[i].y >= (double) pixelY) {
				nodeX[nodes++] = (int) (p.vertices[i].x + (pixelY - p.vertices[i].y) / (p.vertices[j].y - p.vertices[i].y) * (p.vertices[j].x - p.vertices[i].x));
			}
			j = i;
		}

		//  Sort the nodes, via a simple “Bubble” sort.
		i = 0;
		while(i < nodes - 1) {
			if(nodeX[i] > nodeX[i + 1]) {
				swap = nodeX[i]; nodeX[i] = nodeX[i + 1]; nodeX[i + 1] = swap; if(i) i--;
			}
			else {
				i++;
			}
		}

		//  Fill the pixels between node pairs.
		for(i = 0; i < nodes; i += 2) { // i+=2
			if(nodeX[i] >= IMAGE_RIGHT)
				break;
			if(nodeX[i + 1] > IMAGE_LEFT) {
				if(nodeX[i] < IMAGE_LEFT)
					nodeX[i] = IMAGE_LEFT;
				if(nodeX[i + 1] > IMAGE_RIGHT)
					nodeX[i + 1] = IMAGE_RIGHT;
				for(j = nodeX[i]; j < nodeX[i + 1]; j++) {
					Point p = {j, pixelY};
					pointsToFill.pointsToFill[pointsToFill.nbPointsToFill] = p;
					pointsToFill.nbPointsToFill++;
				}
			}
		}
	}

	return pointsToFill;
}
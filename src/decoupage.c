#include "decoupage.h"

void normale(Point p1, Point p2, Point * normal)
{
	normal->x = -(p2.y - p1.y);
	normal->y = p2.x - p1.x;
}


int coupe(Point s, Point pj, Point fi1, Point fi2)
{
	int mat11 = pj.x - s.x, mat12 = fi1.x - fi2.x, mat21 = pj.y - s.y, mat22 = (fi1.y - fi2.y);

	int det = (mat11 * mat22 - mat21 * mat12);
	if (det != 0)
	{
		float invmat11 = (1.0f / det) * mat22, invmat12 = (1.0f / det) * -mat12;
		float t = invmat11 * (fi1.x - s.x) + invmat12 * (fi1.y - s.y);
		if (t >= 0 && t <= 1)
			return 0;
	}
	return 1;
}

Point intersection(Point s, Point pj, Point fi1, Point fi2)
{
	int mat11 = pj.x - s.x, mat12 = fi1.x - fi2.x, mat21 = pj.y - s.y, mat22 = (fi1.y - fi2.y);

	int det = (mat11 * mat22 - mat21 * mat12);

	float invmat11 = (1.0f / det) * mat22, invmat12 = (1.0f / det) * -mat12;
	float t = invmat11 * (fi1.x - s.x) + invmat12 * (fi1.y - s.y);
	
	Point ret;
	ret.x = (1 - t) * s.x + t * pj.x;
	ret.y = (1 - t) * s.y + t * pj.y;

	return ret;
}

int visible(Point s, Point fi1, Point fi2)
{
	Point n;
	normale(fi1, fi2, &n);
	int totoscal = n.x * (fi2.x - fi1.x) + n.y * (fi2.y - fi1.y);
	int pscal = n.x * (s.x - fi1.x) + n.y * (s.y - fi1.y);
	if (pscal >= 0)
		return 0;

	return 1;
}

void SutherlandHodgman(CustomPolygon PL, CustomPolygon PW, CustomPolygon *PS)
{
	int i, j, n2;
	Point S, F, I;
	for (i = 0; i < PW.nbVertices; ++i)
	{
		n2 = 0;
		PS->nbVertices = 0;
		for (j = 0; j < PL.nbVertices; ++j)
		{
			if (j == 0)
				F = PL.vertices[0];
			else
			{
				if (coupe(S, PL.vertices[j], PW.vertices[i], PW.vertices[(i + 1) % PW.nbVertices]) == 0)
				{
					I = intersection(S, PL.vertices[j], PW.vertices[i], PW.vertices[(i + 1) % PW.nbVertices]);
					//Charger(I, PS)
					PS->vertices[PS->nbVertices++] = I;
					n2++;
				}
			}
			S = PL.vertices[j];
			if (visible(S, PW.vertices[i], PW.vertices[(i + 1) % PW.nbVertices]) == 0)
			{
				//Charger(S, PS)
				PS->vertices[PS->nbVertices++] = S;
				n2++;
			}
		}
		if (n2 > 0)
		{
			if (coupe(S, F, PW.vertices[i], PW.vertices[(i + 1) % PW.nbVertices]) == 0)
			{
				I = intersection(S, F, PW.vertices[i], PW.vertices[(i + 1) % PW.nbVertices]);
				//Charger(I, PS)
				PS->vertices[PS->nbVertices++] = I;
				n2++;
			}
			PL = *PS;
			PL.nbVertices = n2;
		}
	}
}
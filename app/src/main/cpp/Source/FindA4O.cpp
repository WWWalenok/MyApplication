//
//	Version 1.0.14
//
//	Date 05.07
//

#include "../Include/FindA4.h"
#include "../Include/Vector.h"
#include "../Include/Tabu.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <cmath>
#include <map>
#include <utility>

namespace fa4
{
	std::string GetVersion()
	{
		return std::string(version);
	}

	typedef int int32_t;
#define PI 3.141592653589793238462643383279502884197169399
#define LineDisp 15
#define LineDeep 3
#define RandomCount 4096
#define max(a,b) ((a > b) ? a : b)
#define min(a,b) ((a < b) ? a : b)
#define in(a,x,b) max(min(x,b),a)

#define MIN_2(a,b) ((a > b) ? b : a)
#define MIN_3(a,b,c) MIN_2(a,MIN_2(b,c))
#define MIN_4(a,b,c,d) MIN_2(a,MIN_3(b,c,d))
#define MAX_2(a,b) ((a < b) ? b : a)
#define MAX_3(a,b,c) MAX_2(a,MAX_2(b,c))
#define MAX_4(a,b,c,d) MAX_2(a,MAX_3(b,c,d))

	float arandom[RandomCount];
	int32_t seed = 0;
	int32_t seedcount = 0;
	int32_t useangel = 0;
	int32_t maxthreadcount = 1;
	double** inout = 0;
	int32_t KadrNumber = 0;
	int32_t Right = -1;
#define random (arandom[(seed >= RandomCount - 100) ? seed = 0 : ++seed])

	const float baseK = 43.26661530556787151743 / 800.0;

	using namespace std;

	struct CameraStats
	{
		double xK; // RCamera
		double yK;
		double zK;
		double xL; // Cent pola zreniya
		double yL;
		double zum; // Focus
		double fiK; // Ugol

		CameraStats(double _xK, double _yK, double _zK, double _xL, double _yL, double _zum, double _fiK)
		{
			xK = _xK;
			yK = _yK;
			zK = _zK;
			xL = _xL;
			yL = _yL;
			zum = _zum;
			fiK = _fiK;
		}

		CameraStats()
		{
			xK = 0;
			yK = 0;
			zK = 0;
			xL = 0;
			yL = 0;
			zum = 0;
			fiK = 0;
		}
	};

	struct ListStat
	{
		double xA, xB, xC, xD, yA, yB, yC, yD;

		ListStat(double xa, double ya, double xb, double yb, double xc, double yc, double xd, double yd)
		{
			xA = xa;
			xB = xb;
			xC = xc;
			xD = xd;
			yA = ya;
			yB = yb;
			yC = yc;
			yD = yd;
		}

		ListStat()
		{
			xA = 0;
			xB = 0;
			xC = 0;
			xD = 0;
			yA = 0;
			yB = 0;
			yC = 0;
			yD = 0;
		}

	};

	double KritFunc(double f);

	double** GetNewCamPos(uint8_t** img, double w, double h, double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy);

	void BruteScan(uint8_t** img, int32_t itercount, int32_t sublenght, CameraStats& outstats, ListStat& lstat, double& Kritbb, int32_t& SheetFound, int32_t& SheetFoundProbability, int32_t& SheetProbablyFound, int32_t);

	void MakeBest(uint8_t** img, ListStat& lstat, double disp, float* Krits);

	double tacos[2001];
	double tr[2000];
	double ironangel;
	float gcams[5][3];
	float immaxX, immaxY, K, DK, GGama, GBeta, GFocal;
	//float LAx, LAy, LBx, LBy, LCx, LCy, LDx, LDy;

#define GetInterPixel(x, y, dx, dy) (img[int32_t(x)][int32_t(y)] * (1 - dx) * (1 - dy) + img[int32_t(x + 1)][int32_t(y)] * (dx) * (1 - dy) + img[int32_t(x)][int32_t(y + 1)] * (1 - dx) * (dy) + img[int32_t(x + 1)][int32_t(y + 1)] * (dx) * (dy))

	int32_t Kadr;

	double TAcos(double f)
	{
		return tacos[int32_t(f * 1000 + 1000)];
	}

	double KritFunc(double f)
	{
		return exp(1.0 / 30.0 * log(abs(f) + 1e-100)) * f / abs(f + 1e-100);
	}

	void BruteScan(uint8_t** img, int32_t itercount, int32_t sublenght, CameraStats& outstats, ListStat& lstat, double& Kritbb, int32_t& SheetFound, int32_t& SheetFoundProbability, int32_t& SheetProbablyFound, int32_t thnum)
	{
#pragma region Vars
		//srand(img[immaxY / 13][immaxX / 11] + thnum + 1);
		double G11i = -550, G12i = 550;
		double G21i = -550, G22i = 550;
		double G31i = 350, G32i = 700;
		double G41i = -100, G42i = 100;
		double G51i = -140, G52i = 140;
		double G61i = 500, G62i = 900;
		double G71i = (-180 - 20) / 180.0 * PI, G72i = (-180 + 20) / 180.0 * PI;
		double G11 = G11i, G12 = G12i, DI1 = G12 - G11;
		double G21 = G21i, G22 = G22i, DI2 = G22 - G21;
		double G31 = G31i, G32 = G32i, DI3 = G32 - G31;
		double G41 = G41i, G42 = G42i, DI4 = G42 - G41;
		double G51 = G51i, G52 = G52i, DI5 = G52 - G51;
		double G61 = G61i, G62 = G62i, DI6 = G62 - G61;
		double G71 = G71i, G72 = G72i, DI7 = G72 - G71;
		double G11g, G12g;
		double G21g, G22g;
		double G31g, G32g;
		double G41g, G42g;
		double G51g, G52g;
		double G61g, G62g;
		double G71g, G72g;

		double xKbb = 0, xKb = 0, xK = 0; // RCamera
		double yKbb = 0, yKb = 0, yK = 0;
		double zKbb = 500, zKb = 500, zK = 500;
		double xLbb = 0, xLb = 0, xL = 0; // Cent pola zreniya
		double yLbb = 0, yLb = 0, yL = 0;
		double zumbb = 700, zumb = 700, zum = 700; // Focus
		double fiKbb = PI, fiKb = PI, fiK = PI; // Ugol

		double xPA = 297 / 2.0, yPA = 210 / 2.0;
		double xPB = -297 / 2.0, yPB = 210 / 2.0;
		double xPC = -297 / 2.0, yPC = -210 / 2.0;
		double xPD = 297 / 2.0, yPD = -210 / 2.0;

		Kritbb = -1e20; double Kritb = Kritbb, Krit;
		double Krit1, Krit2, Krit3, Krit4, Krit1e, Krit2e, Krit3e, Krit4e;
		double Krit1b, Krit2b, Krit3b, Krit4b;
		double Krit1bb, Krit2bb, Krit3bb, Krit4bb;
		double Krit1eb, Krit2eb, Krit3eb, Krit4eb;
		double Krit1ebb, Krit2ebb, Krit3ebb, Krit4ebb;

		double xAb, yAb, xBb, yBb, xCb, yCb, xDb, yDb;
		double xAbb, yAbb, xBbb, yBbb, xCbb, yCbb, xDbb, yDbb;

		double dol[7];

		dol[4] = 7 * DK; dol[5] = 14 * DK;  dol[6] = 23 * DK;
		dol[3] = 0;
		dol[2] = -7 * DK; dol[1] = -14 * DK;  dol[0] = -23 * DK;

		double Line[50][7];

		int32_t count = sublenght * 2;


		DI1 = G12 - G11;
		DI2 = G22 - G21;
		DI3 = G32 - G31;
		DI4 = G42 - G41;
		DI5 = G52 - G51;
		DI6 = G62 - G61;
		DI7 = G72 - G71;


#pragma endregion

		int32_t iterc = (itercount / sublenght + 1) * sublenght;
		float deltaAngl = PI * 120.0 / 180.0 / 3.0, maxbreaked = 10;
		for (int32_t u = 1, totu = 0, breaked = 0; u < iterc; u++, totu++)
		{
			if (totu > iterc * 2) u += 2;
			if (count >= sublenght)
			{
				count = 0;
				Kritb = -1e20;

				G11 = G11i; G12 = G12i; DI1 = G12 - G11;
				G21 = G21i; G22 = G22i; DI2 = G22 - G21;
				G31 = G31i; G32 = G32i; DI3 = G32 - G31;
				G41 = G41i; G42 = G42i; DI4 = G42 - G41;
				G51 = G51i; G52 = G52i; DI5 = G52 - G51;
				G61 = G61i; G62 = G62i; DI6 = G62 - G61;
				G71 = G71i; G72 = G72i; DI7 = G72 - G71;

				xKb = G11 + (G12 - G11) * 0.5;
				yKb = G21 + (G22 - G21) * 0.5;
				zKb = G31 + (G32 - G31) * 0.5;
				zKb = G31 + (G32 - G31) * 0.5;
				xLb = G41 + (G42 - G41) * 0.5;
				yLb = G51 + (G52 - G51) * 0.5;
				zumb = G61 + (G62 - G61) * 0.5;
				fiKb = G71 + (G72 - G71) * 0.5;
			}
			count = count + 1;

			double r = 0.6;
			if (random < r) { G11g = G11; G12g = G12; }
			else { G11g = xKb; G12g = xKb; };
			if (random < r) { G21g = G21; G22g = G22; }
			else { G21g = yKb; G22g = yKb; };
			if (random < r) { G31g = G31; G32g = G32; }
			else { G31g = zKb; G32g = zKb; };
			if (random < r) { G41g = G41; G42g = G42; }
			else { G41g = xLb; G42g = xLb; };
			if (random < r) { G51g = G51; G52g = G52; }
			else { G51g = yLb; G52g = yLb; };
			if (random < r) { G61g = G61; G62g = G62; }
			else { G61g = zumb; G62g = zumb; };
			if (random < r) { G71g = G71; G72g = G72; }
			else { G71g = fiKb; G72g = fiKb; };

			//RegenCameraStats(&select, minStatsGen, maxStatsGen);

			xK = G11g + (G12g - G11g) * random;
			yK = G21g + (G22g - G21g) * random;
			zK = G31g + (G32g - G31g) * random;
			xL = G41g + (G42g - G41g) * random;
			yL = G51g + (G52g - G51g) * random;
			zum = G61g + (G62g - G61g) * random;
			fiK = G71g + (G72g - G71g) * random;

			if (u % 2 == 0)
			{
				r = tr[int32_t(max(1, min((count * 1998) / sublenght, 1998)))];

				if (xKb < ((G11 + G12) / 2.0)) G12 = G11 + r * DI1; else G11 = G12 - r * DI1;
				if (yKb < ((G21 + G22) / 2.0)) G22 = G21 + r * DI2; else G21 = G22 - r * DI2;
				if (zKb < ((G31 + G32) / 2.0)) G32 = G31 + r * DI3; else G31 = G32 - r * DI3;
				if (xLb < ((G41 + G42) / 2.0)) G42 = G41 + r * DI4; else G41 = G42 - r * DI4;
				if (yLb < ((G51 + G52) / 2.0)) G52 = G51 + r * DI5; else G51 = G52 - r * DI5;
				if (zumb < ((G61 + G62) / 2.0)) G62 = G61 + r * DI6; else G61 = G62 - r * DI6;
				if (fiKb < ((G71 + G72) / 2.0)) G72 = G71 + r * DI7; else G71 = G72 - r * DI7;

			}

			r = xK * xK + yK * yK + zK * zK;
			if (r > 800 * 800)
			{
				u--;
				count--;
				continue;
			}

			r = sqrt(xK * xK + yK * yK);
			if (r < 350)
			{
				u--;
				count--;
				continue;
			}
			if (KadrNumber > 0 && KadrNumber <= 7 && useangel == 1)
			{
				float dx = xK / r;
				float dy = yK / r;
				float ax, ay;
				switch (Right)
				{
				case 1:
					ax = t_cos((111 - 37 * (KadrNumber - 1)) / 180.0 * PI);
					ay = t_sin((111 - 37 * (KadrNumber - 1)) / 180.0 * PI);
					break;
				case 0:
					ax = t_cos((-111 + 37 * (KadrNumber - 1)) / 180.0 * PI);
					ay = t_sin((-111 + 37 * (KadrNumber - 1)) / 180.0 * PI);
					break;
				}
				switch (Right)
				{
				case 1:
				case 0:
					if (abs(dx * ax + dy * ay) < 0.71)
					{
						u--;
						count--;
						continue;
					}
					break;
				case -1:
					float bx, by;
					ax = t_cos((111 - 37 * (KadrNumber - 1)) / 180.0 * PI);
					ay = t_sin((111 - 37 * (KadrNumber - 1)) / 180.0 * PI);
					bx = t_cos((-111 + 37 * (KadrNumber - 1)) / 180.0 * PI);
					by = t_sin((-111 + 37 * (KadrNumber - 1)) / 180.0 * PI);
					if (abs(dx * ax + dy * ay) < 0.71 && abs(dx * bx + dy * by) < 0.71)
					{
						u--;
						count--;
						continue;
					}
					break;
				}
			}
			double PLA = xL - xK, PLB = yL - yK, PLC = -zK;
			if (zum < 0) zum *= -1;
			if (zum < G61i) zum = zumb;
			if (zum > G62i) zum = zumb;
			r = sqrt((PLA * PLA) + (PLB * PLB) + (PLC * PLC)) + 1e-20;
			double fi = zum / r;
			double PLD = -(xL - xK) * (xK + fi * (xL - xK)) - (yL - yK) * (yK + fi * (yL - yK)) + zK * (zK - fi * zK);

			// Находим точки-проекции углов листа A4
			double q = -PLD - PLA * xK - PLB * yK - PLC * zK;
			fi = PLC * (-zK) + 1e-20;
			r = q / (PLA * (xL - xK) + PLB * (yL - yK) + fi);
			double xML = xK + (xL - xK) * r;
			double yML = yK + (yL - yK) * r;
			double zML = zK + (0 - zK) * r;
			r = q / (PLA * (xPA - xK) + PLB * (yPA - yK) + fi);
			double xMA = xK + (xPA - xK) * r;
			double yMA = yK + (yPA - yK) * r;
			double zMA = zK + (0 - zK) * r;
			r = q / (PLA * (xPB - xK) + PLB * (yPB - yK) + fi);
			double xMB = xK + (xPB - xK) * r;
			double yMB = yK + (yPB - yK) * r;
			double zMB = zK + (0 - zK) * r;
			r = q / (PLA * (xPC - xK) + PLB * (yPC - yK) + fi);
			double xMC = xK + (xPC - xK) * r;
			double yMC = yK + (yPC - yK) * r;
			double zMC = zK + (0 - zK) * r;
			r = q / (PLA * (xPD - xK) + PLB * (yPD - yK) + fi);
			double xMD = xK + (xPD - xK) * r;
			double yMD = yK + (yPD - yK) * r;
			double zMD = zK + (0 - zK) * r;
			// Делаем вектора осей на плоскости-экране
			double v1x = yL - yK, v1y = xK - xL, v1z = 0;
			double v2x = (xK - xL) * (0 - zK), v2y = -(yL - yK) * (0 - zK), v2z = (yL - yK) * (yL - yK) + (xL - xK) * (xL - xK);
			// Находим проекции всех точек
			double v1 = sqrt(v1x * v1x + v1y * v1y + v1z * v1z);
			double v2 = sqrt(v2x * v2x + v2y * v2y + v2z * v2z);
			double xVA = ((xMA - xML) * v1x + (yMA - yML) * v1y + (zMA - zML) * v1z) / v1;
			double yVA = ((xMA - xML) * v2x + (yMA - yML) * v2y + (zMA - zML) * v2z) / v2;
			double xVB = ((xMB - xML) * v1x + (yMB - yML) * v1y + (zMB - zML) * v1z) / v1;
			double yVB = ((xMB - xML) * v2x + (yMB - yML) * v2y + (zMB - zML) * v2z) / v2;
			double xVC = ((xMC - xML) * v1x + (yMC - yML) * v1y + (zMC - zML) * v1z) / v1;
			double yVC = ((xMC - xML) * v2x + (yMC - yML) * v2y + (zMC - zML) * v2z) / v2;
			double xVD = ((xMD - xML) * v1x + (yMD - yML) * v1y + (zMD - zML) * v1z) / v1;
			double yVD = ((xMD - xML) * v2x + (yMD - yML) * v2y + (zMD - zML) * v2z) / v2;

			// Здесь нужно сделать пересчет точек на пиксели кадра
			double xA = xVC + immaxX / 2.0, yA = -yVC + immaxY / 2.0;
			double xB = xVD + immaxX / 2.0, yB = -yVD + immaxY / 2.0;
			double xC = xVA + immaxX / 2.0, yC = -yVA + immaxY / 2.0;
			double xD = xVB + immaxX / 2.0, yD = -yVB + immaxY / 2.0;
			// Поворот через табулированные синус и косинус
			if (abs(fiK) > PI) fiK = fiKb;
			double x = t_cos(fiK), y = t_sin(fiK);
			r = xA - immaxX / 2.0; q = yA - immaxY / 2.0; xA = immaxX / 2.0 + r * x - q * y; yA = immaxY / 2.0 + r * y + q * x;
			r = xB - immaxX / 2.0; q = yB - immaxY / 2.0; xB = immaxX / 2.0 + r * x - q * y; yB = immaxY / 2.0 + r * y + q * x;
			r = xC - immaxX / 2.0; q = yC - immaxY / 2.0; xC = immaxX / 2.0 + r * x - q * y; yC = immaxY / 2.0 + r * y + q * x;
			r = xD - immaxX / 2.0; q = yD - immaxY / 2.0; xD = immaxX / 2.0 + r * x - q * y; yD = immaxY / 2.0 + r * y + q * x;

			///////////////////////////////////////////////////////////////////////////
			// Конец поиска углов листа на изображении по параметрам камеры
			////////////////////////////////////////////////////////////////////////////

			// Устранение привязки прямоугольника вне ноги
			int32_t xr = (xA + xB + xC + xD) / 4.0, yr = (yA + yB + yC + yD) / 4.0;
			if ((xr < 1 + 18) || (xr >= immaxX - 20) || (yr < 1 + 18) || (yr >= immaxY - 20))
			{
				u--;
				count--;
				continue;
			}
			if ((img[yr][xr] > 85) &&
				(img[yr + 16][xr] > 85) &&
				(img[yr - 16][xr] > 85) &&
				(img[yr][xr + 16] > 85) &&
				(img[yr][xr - 16] > 85))
			{
				u--;
				count--;
				continue;
			}

			int32_t tr = 19;
			int32_t tr2 = tr * 1, tr3 = tr * 1;
			int32_t NR = 15;
			//w1=1; w2=0.5; w3=0.25;
			double w1 = 1, w2 = 0.7, w3 = 0.2;

			double rAB = sqrt((xA - xB) * (xA - xB) + (yA - yB) * (yA - yB)) + 1e-20;
			double rBC = sqrt((xB - xC) * (xB - xC) + (yB - yC) * (yB - yC)) + 1e-20;
			double rCD = sqrt((xC - xD) * (xC - xD) + (yC - yD) * (yC - yD)) + 1e-20;
			double rAD = sqrt((xA - xD) * (xA - xD) + (yA - yD) * (yA - yD)) + 1e-20;


			double xA1, yA1, xB1, yB1;
			int32_t xT, yT;
			for (int32_t i = 1; i <= NR; i++) Line[i][3] = 0;
			for (int32_t j = 0; j < 7; j++) if (j != 3)
			{
				xA1 = xA + dol[j] * (xD - xA) / rAD;
				yA1 = yA + dol[j] * (yD - yA) / rAD;
				xB1 = xB + dol[j] * (xC - xB) / rBC;
				yB1 = yB + dol[j] * (yC - yB) / rBC;
				for (int32_t i = 1; i <= NR; i++)
				{
					xT = xA1 + (xB1 - xA1) / NR * (i - 0.5);
					yT = yA1 + (yB1 - yA1) / NR * (i - 0.5);
					if ((xT < 0) || (xT > immaxX - 1) || (yT < 0) || (yT > immaxY - 1)) Line[i][3] = -1;
					else
					{
						Line[i][j] = img[yT][xT];
					}
				}
			}
			Krit1 = 0; Krit1e = 0;
			for (int32_t i = 1; i <= NR; i++) if (Line[i][3] != -1)
			{
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if (r > tr3)
				{
					Krit1 = Krit1 + w3;
					if (abs(i - NR / 2.0) >= (NR * 0.25))  Krit1e = Krit1e + w3;
				}
				r = ((Line[i][4] + Line[i][5]) / 2.0 - (Line[i][2] + Line[i][1]) / 2.0);
				if (r > tr2)
				{
					Krit1 = Krit1 + w2;
					if (abs(i - NR / 2.0) >= (NR * 0.25))  Krit1e = Krit1e + w2;
				}
				r = Line[i][4] - Line[i][2];
				if (r > tr)
				{
					Krit1 = Krit1 + w1;
					if (abs(i - NR / 2.0) >= (NR * 0.25))  Krit1e = Krit1e + w1;
				}
			}

			for (int32_t i = 1; i <= NR; i++) Line[i][3] = 0;
			for (int32_t j = 0; j < 7; j++) if (j != 3)
			{
				xA1 = xB + dol[j] * (xA - xB) / rAB;
				yA1 = yB + dol[j] * (yA - yB) / rAB;
				xB1 = xC + dol[j] * (xD - xC) / rCD;
				yB1 = yC + dol[j] * (yD - yC) / rCD;
				for (int32_t i = 1; i < NR; i++)
				{
					xT = xA1 + (xB1 - xA1) / NR * (i - 0.5); yT = yA1 + (yB1 - yA1) / NR * (i - 0.5);
					if ((xT < 0) || (xT > immaxX - 1) || (yT < 0) || (yT > immaxY - 1)) Line[i][3] = -1;
					else
					{
						Line[i][j] = img[yT][xT];
					}
				}
			}
			Krit2 = 0; Krit2e = 0;
			for (int32_t i = 1; i <= NR; i++) if (Line[i][3] != -1)
			{
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if (r > tr3)
				{
					Krit2 = Krit2 + w3;
					if (abs(i - NR / 2.0) >= (NR * 0.25))  Krit2e = Krit2e + w3;
				}
				r = ((Line[i][4] + Line[i][5]) / 2.0 - (Line[i][2] + Line[i][1]) / 2.0);
				if (r > tr2)
				{
					Krit2 = Krit2 + w2;
					if (abs(i - NR / 2.0) >= (NR * 0.25))  Krit2e = Krit2e + w2;
				}
				r = Line[i][4] - Line[i][2];
				if (r > tr)
				{
					Krit2 = Krit2 + w1;
					if (abs(i - NR / 2.0) >= (NR * 0.25))  Krit2e = Krit2e + w1;
				}
			}

			for (int32_t i = 1; i <= NR; i++) Line[i][3] = 0;
			for (int32_t j = 0; j < 7; j++) if (j != 3)
			{
				xA1 = xD + dol[j] * (xA - xD) / rAD;
				yA1 = yD + dol[j] * (yA - yD) / rAD;
				xB1 = xC + dol[j] * (xB - xC) / rBC;
				yB1 = yC + dol[j] * (yB - yC) / rBC;
				for (int32_t i = 1; i <= NR; i++)
				{
					xT = xA1 + (xB1 - xA1) / NR * (i - 0.5); yT = yA1 + (yB1 - yA1) / NR * (i - 0.5);
					if ((xT < 0) || (xT > immaxX - 1) || (yT < 0) || (yT > immaxY - 1)) Line[i][3] = -1;
					else
					{
						Line[i][j] = img[yT][xT];
					}
				}
			}
			Krit3 = 0; Krit3e = 0;
			for (int32_t i = 1; i <= NR; i++) if (Line[i][3] != -1)
			{
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if (r > tr3)
				{
					Krit3 = Krit3 + w3;
					if (abs(i - NR / 2.0) >= (NR * 0.25))  Krit3e = Krit3e + w3;
				}
				r = ((Line[i][4] + Line[i][5]) / 2.0 - (Line[i][2] + Line[i][1]) / 2.0);
				if (r > tr2)
				{
					Krit3 = Krit3 + w2;
					if (abs(i - NR / 2.0) >= (NR * 0.25))  Krit3e = Krit3e + w2;
				}
				r = Line[i][4] - Line[i][2];
				if (r > tr)
				{
					Krit3 = Krit3 + w1;
					if (abs(i - NR / 2.0) >= (NR * 0.25))  Krit3e = Krit3e + w1;
				}
			}

			for (int32_t i = 1; i <= NR; i++) Line[i][3] = 0;
			for (int32_t j = 0; j < 7; j++) if (j != 3)
			{
				xA1 = xA + dol[j] * (xB - xA) / rAB;
				yA1 = yA + dol[j] * (yB - yA) / rAB;
				xB1 = xD + dol[j] * (xC - xD) / rCD;
				yB1 = yD + dol[j] * (yC - yD) / rCD;
				for (int32_t i = 1; i <= NR; i++)
				{
					xT = xA1 + (xB1 - xA1) / NR * (i - 0.5); yT = yA1 + (yB1 - yA1) / NR * (i - 0.5);
					if ((xT < 0) || (xT > immaxX - 1) || (yT < 0) || (yT > immaxY - 1)) Line[i][3] = -1;
					else
					{
						Line[i][j] = img[yT][xT];
					}
				}
			}
			Krit4 = 0; Krit4e = 0;
			for (int32_t i = 1; i <= NR; i++) if (Line[i][3] != -1)
			{
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if (r > tr3)
				{
					Krit4 = Krit4 + w3;
					if (abs(i - NR / 2.0) >= (NR * 0.25))  Krit4e = Krit4e + w3;
				}
				r = ((Line[i][4] + Line[i][5]) / 2.0 - (Line[i][2] + Line[i][1]) / 2.0);
				if (r > tr2)
				{
					Krit4 = Krit4 + w2;
					if (abs(i - NR / 2.0) >= (NR * 0.25))  Krit4e = Krit4e + w2;
				}
				r = Line[i][4] - Line[i][2];
				if (r > tr)
				{
					Krit4 = Krit4 + w1;
					if (abs(i - NR / 2.0) >= (NR * 0.25))  Krit4e = Krit4e + w1;
				}
			}

			Krit = KritFunc(Krit1) + KritFunc(Krit2) + KritFunc(Krit3) + KritFunc(Krit4);

			if (Krit > Kritb || Kritb < -1e10)
			{
				Kritb = Krit;
				Krit1b = Krit1; Krit2b = Krit2; Krit3b = Krit3; Krit4b = Krit4;
				Krit1eb = Krit1e; Krit2eb = Krit2e; Krit3eb = Krit3e; Krit4eb = Krit4e;
				xKb = xK; yKb = yK; zKb = zK; xLb = xL; yLb = yL; zumb = zum; fiKb = fiK;
				xAb = xA; yAb = yA; xBb = xB; yBb = yB; xCb = xC; yCb = yC; xDb = xD; yDb = yD;
			}

			if (Kritb > Kritbb || Kritbb < -1e10)
			{
				Kritbb = Kritb;
				Krit1bb = Krit1b; Krit2bb = Krit2b; Krit3bb = Krit3b; Krit4bb = Krit4b;
				Krit1ebb = Krit1eb; Krit2ebb = Krit2eb; Krit3ebb = Krit3eb; Krit4ebb = Krit4eb;
				xKbb = xKb; yKbb = yKb; zKbb = zKb; xLbb = xLb; yLbb = yLb; zumbb = zumb; fiKbb = fiKb;
				xAbb = xAb; yAbb = yAb; xBbb = xBb; yBbb = yBb; xCbb = xCb; yCbb = yCb; xDbb = xDb; yDbb = yDb;
			}
		}

		try
		{
			Krit = Kritbb;
			if (Krit < -1e10)
			{
				SheetFound = 0;
				SheetFoundProbability = 0;
				SheetProbablyFound = 0;
				return;
			}
		}
		catch (const std::exception&)
		{
			if (Krit < -1e10)
			{
				SheetFound = 0;
				SheetFoundProbability = 0;
				SheetProbablyFound = 0;
				return;
			}
		}


		// Превращаем bb в b
		Kritb = Kritbb;
		//Krit1b=Krit1bb; Krit2b=Krit2bb; Krit3b=Krit3bb; Krit4b=Krit4bb;
		xKb = xKbb; yKb = yKbb; zKb = zKbb; xLb = xLbb; yLb = yLbb; zumb = zumbb; fiKb = fiKbb;
		xAb = xAbb; yAb = yAbb; xBb = xBbb; yBb = yBbb; xCb = xCbb; yCb = yCbb; xDb = xDbb; yDb = yDbb;

		double r = 0, fi = 0;
		// Проверяем, не выкинуть ли этот кадр
		// Находим минимальный критерий
		int32_t Sort[4];
		Sort[0] = Krit1bb;
		Sort[1] = Krit2bb;
		Sort[2] = Krit3bb;
		Sort[3] = Krit4bb;
		for (int32_t i = 1; i < 4; i++) if (Sort[i] > Sort[0]) { r = Sort[0]; Sort[0] = Sort[i]; Sort[i] = r; }
		for (int32_t i = 2; i < 4; i++) if (Sort[i] > Sort[1]) { r = Sort[1]; Sort[1] = Sort[i]; Sort[i] = r; }
		if (Sort[3] > Sort[2]) { r = Sort[1]; Sort[1] = Sort[2]; Sort[2] = r; }

		fi = Sort[3];

		Sort[0] = Krit1ebb;
		Sort[1] = Krit2ebb;
		Sort[2] = Krit3ebb;
		Sort[3] = Krit4ebb;
		for (int32_t i = 1; i < 4; i++) if (Sort[i] > Sort[0]) { r = Sort[0]; Sort[0] = Sort[i]; Sort[i] = r; }
		for (int32_t i = 2; i < 4; i++) if (Sort[i] > Sort[1]) { r = Sort[1]; Sort[1] = Sort[i]; Sort[i] = r; }
		if (Sort[3] > Sort[2]) { r = Sort[1]; Sort[1] = Sort[2]; Sort[2] = r; }

		if (Sort[3] < 5.5) SheetFound = 0; else SheetFound = 1;
		if (Sort[3] < 3.75) SheetProbablyFound = 0; else; SheetProbablyFound = 1;
		if (Kritbb > 4.403) SheetProbablyFound = 1;
		SheetFoundProbability = round((Sort[3] - 2.5) / 5.0 * 100); if (SheetFoundProbability < 0)  SheetFoundProbability = 0; if (SheetFoundProbability > 100) SheetFoundProbability = 100;

		outstats = CameraStats(xKbb, yKbb, zKbb, xLbb, yLbb, zumbb, fiKbb);

		lstat = ListStat(xAb, yAb, xBb, yBb, xCb, yCb, xDb, yDb);
	};

	void MakeBest(uint8_t** img, ListStat& lstat, double disp, float* OKrits)
	{
#pragma region Vars

		float Krits[4];

		double dol[7];
		bool ResetNewPos = false;
		float DD = disp / 0.2 * DK;

		ResetNewPos = false;
		int32_t count = 0;
		double Kritbb = -1e20, Kritb = Kritbb, Krit, Okritb, Okrit;

		double xAb = lstat.xA, yAb = lstat.yA, xBb = lstat.xB, yBb = lstat.yB, xCb = lstat.xC, yCb = lstat.yC, xDb = lstat.xD, yDb = lstat.yD;
		double xAbb = lstat.xA, yAbb = lstat.yA, xBbb = lstat.xB, yBbb = lstat.yB, xCbb = lstat.xC, yCbb = lstat.yC, xDbb = lstat.xD, yDbb = lstat.yD;

		int32_t o1 = 400, o2 = o1 / 20;
		dol[4] = max(0.71 * DD, 0.71); dol[5] = max(10.224 * DD, 0.71 * 3);  dol[6] = max(18.744 * DD, 0.71 * 5);
		dol[3] = 0;
		dol[2] = -dol[4]; dol[1] = -dol[5]; dol[0] = -dol[6];
		int32_t tr = 18;
		int32_t NR = 35;
		double w1 = 1, w2 = 0.5, w3 = 0.25;
		double xA = xAb, yA = yAb, xB = xBb, yB = yBb, xC = xCb, yC = yCb, xD = xDb, yD = yDb, xBsb, yBsb, xCsb, yCsb, rBC, rAD;
		double r; double fi;

		double Line[50][7];
#pragma endregion

#pragma region
		Kritb = -10e20;
		float G11 = -disp; float  G12 = disp;
		float G21 = -disp; float  G22 = disp;

		double sA = 0, sB = 0, sC = 0, sD = 0, sAb, sBb, sCb, sDb, rAB, rCD, xB1, yB1, xC1, yC1, xAsb2, yAsb2, xDsb2, yDsb2, xBsb2, yBsb2;
		int32_t xT, yT, dX, dY;

		for (int32_t o = 1; o < o1; o++)
		{
			sB = G11 + (G12 - G11) * random;
			sC = G21 + (G22 - G21) * random;
			if (o % o2 == 0)
			{
				r = 0.93;
				if (sBb < 0.5 * (G11 + G12)) G12 = G11 + r * (G12 - G11); else G11 = G12 - r * (G12 - G11);
				if (sCb < 0.5 * (G21 + G22)) G22 = G21 + r * (G22 - G21); else G21 = G22 - r * (G22 - G21);
			}

			xB = xBbb + sB * (xBbb - xAbb); yB = yBbb + sB * (yBbb - yAbb);
			xC = xCbb + sC * (xCbb - xDbb); yC = yCbb + sC * (yCbb - yDbb);
			// Считаем критерий

			rAB = sqrt((xAbb - xB) * (xAbb - xB) + (yAbb - yB) * (yAbb - yB)) + 1e-20;
			rCD = sqrt((xC - xDbb) * (xC - xDbb) + (yC - yDbb) * (yC - yDbb)) + 1e-20;
			for (int32_t i = 0; i < NR; i++) Line[i][3] = 0;
			for (int32_t g = 0; g < 7; g++) if (g != 3)
			{
				xB1 = xB + dol[g] * (xA - xB) / rAB;
				yB1 = yB + dol[g] * (yA - yB) / rAB;
				xC1 = xC + dol[g] * (xD - xC) / rCD;
				yC1 = yC + dol[g] * (yD - yC) / rCD;
				for (int32_t i = 2; i < NR - 2; i++)
				{
					xT = xB1 + (xC1 - xB1) / NR * (i - 0.5);
					yT = yB1 + (yC1 - yB1) / NR * (i - 0.5);
					//dX = xT - int32_t(xT); dY = yT - int32_t(yT);
					if (((xT < 1.5) || (xT > immaxX - 2.5) || (yT < 1.5) || (yT > immaxY - 2.5)))
						Line[i][3] = -1; else Line[i][g] = img[yT][xT];
				}
			}
			Krit = -0;
			double q = 0, u = 0, n = 0;
			for (int32_t i = 2; i < NR - 2; i++) if (Line[i][3] != -1)
			{
				fi = 0;
				n++;
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if (r > tr)
					Krit = Krit + w3;
				r = ((Line[i][4] + Line[i][5]) - (Line[i][2] + Line[i][1])) / 2.0;
				if (r > tr)
					Krit = Krit + w2;
				r = Line[i][4] - Line[i][2];
				if (r > tr)
					Krit = Krit + w1;
				// Количество превышений интенсивности должно быть не менее 35 %
				r = ((Line[i][5] + Line[i][6]) / 2.0 - (Line[i][1] + Line[i][0]) / 2.0); if (r > tr) q = q + 1;
				r = ((Line[i][5] + Line[i][6] + Line[i][1] + Line[i][0]) / 4.0); if (r < 40) u = u + 1;
			}
			if (q < (n) * 0.35) Krit = Krit * 0.7;
			Okrit = Krit;
			if (u < (n) * 0.7) Okrit = Krit * n / (n - u);
			if (u < (n) * 0.4) Krit = Krit * n / (n - u);

			if (Krit > Kritb)
			{
				Okritb = Okrit;
				Kritb = Krit; sBb = sB; sCb = sC; xBsb = xB; yBsb = yB; xCsb = xC; yCsb = yC;
			}

		}
		OKrits[0] = Okritb;
		Krits[0] = Kritb;
#pragma endregion

#pragma region

		Kritb = -10e20;
		xA = xAb; yA = yAb; xB = xBsb; yB = yBsb; xC = xCsb; yC = yCsb; xD = xDb; yD = yDb;
		G11 = -disp; G12 = disp;
		G21 = -disp; G22 = disp;

		for (int32_t o = 1; o < o1; o++)
		{
			sA = G11 + (G12 - G11) * random;
			sD = G21 + (G22 - G21) * random;
			if (o % o2 == 0)
			{

				r = 0.93;
				if (sAb < 0.5 * (G11 + G12)) G12 = G11 + r * (G12 - G11); else G11 = G12 - r * (G12 - G11);
				if (sDb < 0.5 * (G21 + G22)) G22 = G21 + r * (G22 - G21); else G21 = G22 - r * (G22 - G21);
			}

			xA = xAbb + sA * (xAbb - xB); yA = yAbb + sA * (yAbb - yB);
			xD = xDbb + sD * (xDbb - xC); yD = yDbb + sD * (yCbb - yC);
			// Считаем критерий
			rAB = sqrt((xA - xBsb) * (xA - xBsb) + (yA - yBsb) * (yA - yBsb)) + 1e-20;
			rCD = sqrt((xCsb - xD) * (xCsb - xD) + (yCsb - yD) * (yCsb - yD)) + 1e-20;
			for (int32_t i = 0; i < NR; i++) Line[i][3] = 0;
			for (int32_t g = 0; g < 7; g++) if (g != 3)
			{
				xB1 = xA + dol[g] * (xB - xA) / rAB;
				yB1 = yA + dol[g] * (yB - yA) / rAB;
				xC1 = xD + dol[g] * (xC - xD) / rCD;
				yC1 = yD + dol[g] * (yC - yD) / rCD;
				for (int32_t i = 2; i < NR - 2; i++)
				{
					xT = xB1 + (xC1 - xB1) / NR * (i - 0.5);
					yT = yB1 + (yC1 - yB1) / NR * (i - 0.5);
					//dX = xT - int32_t(xT); dY = yT - int32_t(yT);
					if (((xT < 1.5) || (xT > immaxX - 2.5) || (yT < 1.5) || (yT > immaxY - 2.5)))
						Line[i][3] = -1; else Line[i][g] = img[yT][xT];
				}
			}
			Krit = -0;
			double q = 0, u = 0, n = 0;
			for (int32_t i = 2; i < NR - 2; i++) if (Line[i][3] != -1)
			{
				fi = 0;
				n++;
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if (r > tr)
					Krit = Krit + w3;
				r = ((Line[i][4] + Line[i][5]) - (Line[i][2] + Line[i][1])) / 2.0;
				if (r > tr)
					Krit = Krit + w2;
				r = Line[i][4] - Line[i][2];
				if (r > tr)
					Krit = Krit + w1;
				// Количество превышений интенсивности должно быть не менее 35 %
				r = ((Line[i][5] + Line[i][6]) / 2.0 - (Line[i][1] + Line[i][0]) / 2.0); if (r > tr) q = q + 1;
				r = ((Line[i][5] + Line[i][6] + Line[i][1] + Line[i][0]) / 4.0); if (r < 40) u = u + 1;
			}
			if (q < (n) * 0.35) Krit = Krit * 0.7;
			Okrit = Krit;
			if (u < (n) * 0.7) Okrit = Krit * n / (n - u);
			if (u < (n) * 0.4) Krit = Krit * n / (n - u);

			if (Krit > Kritb)
			{
				Okritb = Okrit;
				Kritb = Krit; sAb = sA; sDb = sD; xAsb2 = xA; yAsb2 = yA; xDsb2 = xD; yDsb2 = yD;
			}

		}
		OKrits[1] = Okritb;
		Krits[1] = Kritb;
		double xAsb = xAsb2, yAsb = yAsb2, xDsb = xDsb2, yDsb = yDsb2;
#pragma endregion

#pragma region
		Kritb = -10e20;
		xA = xAsb; yA = yAsb; xB = xBsb; yB = yBsb; xC = xCsb; yC = yCsb; xD = xDsb; yD = yDsb;
		G11 = -disp; G12 = disp;
		G21 = -disp; G22 = disp;

		for (int32_t o = 1; o < o1; o++)
		{
			sA = G11 + (G12 - G11) * random;
			sB = G21 + (G22 - G21) * random;
			if (o % o2 == 0)
			{

				r = 0.93;
				if (sAb < 0.5 * (G11 + G12)) G12 = G11 + r * (G12 - G11); else G11 = G12 - r * (G12 - G11);
				if (sBb < 0.5 * (G21 + G22)) G22 = G21 + r * (G22 - G21); else G21 = G22 - r * (G22 - G21);
			}

			xA = xAsb + sA * (xAsb - xD); yA = yAsb + sA * (yAsb - yD);
			xB = xBsb + sB * (xBsb - xC); yB = yBsb + sB * (yBsb - yC);
			// Считаем критерий

			rBC = sqrt((xCsb - xB) * (xCsb - xB) + (yCsb - yB) * (yCsb - yB)) + 1e-20;
			rAD = sqrt((xA - xDsb) * (xA - xDsb) + (yA - yDsb) * (yA - yDsb)) + 1e-20;
			for (int32_t i = 0; i < NR; i++) Line[i][3] = 0;
			for (int32_t g = 0; g < 7; g++) if (g != 3)
			{
				double j = g - 3;
				xB1 = xA + dol[g] * (xD - xA) / rAD;
				yB1 = yA + dol[g] * (yD - yA) / rAD;
				xC1 = xB + dol[g] * (xC - xB) / rBC;
				yC1 = yB + dol[g] * (yC - yB) / rBC;
				for (int32_t i = 2; i < NR - 2; i++)
				{
					xT = xB1 + (xC1 - xB1) / NR * (i - 0.5);
					yT = yB1 + (yC1 - yB1) / NR * (i - 0.5);
					//dX = xT - int32_t(xT); dY = yT - int32_t(yT);
					if (((xT < 1.5) || (xT > immaxX - 2.5) || (yT < 1.5) || (yT > immaxY - 2.5)))
						Line[i][3] = -1; else Line[i][g] = img[yT][xT];
				}
			}
			Krit = -0;
			double q = 0, u = 0, n = 0;
			for (int32_t i = 2; i < NR - 2; i++) if (Line[i][3] != -1)
			{
				fi = 0;
				n++;
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if (r > tr)
					Krit = Krit + w3;
				r = ((Line[i][4] + Line[i][5]) - (Line[i][2] + Line[i][1])) / 2.0;
				if (r > tr)
					Krit = Krit + w2;
				r = Line[i][4] - Line[i][2];
				if (r > tr)
					Krit = Krit + w1;
				// Количество превышений интенсивности должно быть не менее 35 %
				r = ((Line[i][5] + Line[i][6]) / 2.0 - (Line[i][1] + Line[i][0]) / 2.0); if (r > tr) q = q + 1;
				r = ((Line[i][5] + Line[i][6] + Line[i][1] + Line[i][0]) / 4.0); if (r < 40) u = u + 1;
			}
			if (q < (n) * 0.35) Krit = Krit * 0.7;
			Okrit = Krit;
			if (u < (n) * 0.7) Okrit = Krit * n / (n - u);
			if (u < (n) * 0.4) Krit = Krit * n / (n - u);

			if (Krit > Kritb)
			{
				Okritb = Okrit;
				Kritb = Krit; sAb = sA; sBb = sB; xAsb2 = xA; yAsb2 = yA; xBsb2 = xB; yBsb2 = yB;
			}

		}
		OKrits[2] = Okritb;
		xAsb = xAsb2; yAsb = yAsb2; xBsb = xBsb2; yBsb = yBsb2;
		Krits[2] = Kritb;
#pragma endregion

#pragma region

		double xCsb2, yCsb2;
		Kritb = -10e20;
		xA = xAsb; yA = yAsb; xB = xBsb; yB = yBsb; xC = xCsb; yC = yCsb; xD = xDsb; yD = yDsb;
		G11 = -disp; G12 = disp;
		G21 = -disp; G22 = disp;

		for (int32_t o = 1; o < o1; o++)
		{
			sC = G11 + (G12 - G11) * random;
			sD = G21 + (G22 - G21) * random;
			if (o % o2 == 0)
			{

				r = 0.93;
				if (sCb < 0.5 * (G11 + G12)) G12 = G11 + r * (G12 - G11); else G11 = G12 - r * (G12 - G11);
				if (sDb < 0.5 * (G21 + G22)) G22 = G21 + r * (G22 - G21); else G21 = G22 - r * (G22 - G21);
			}
			xC = xCsb + sC * (xCsb - xB); yC = yCsb + sC * (yCsb - yB);
			xD = xDsb + sD * (xDsb - xA); yD = yDsb + sD * (yDsb - yA);
			// Считаем критерий

			rBC = sqrt((xC - xBsb) * (xC - xBsb) + (yC - yBsb) * (yC - yBsb)) + 1e-20;
			rAD = sqrt((xAsb - xD) * (xAsb - xD) + (yAsb - yD) * (yAsb - yD)) + 1e-20;
			for (int32_t i = 0; i < NR; i++) Line[i][3] = 0;
			for (int32_t g = 0; g < 7; g++) if (g != 3)
			{
				double j = g - 3;
				xB1 = xD + dol[g] * (xA - xD) / rAD;
				yB1 = yD + dol[g] * (yA - yD) / rAD;
				xC1 = xC + dol[g] * (xB - xC) / rBC;
				yC1 = yC + dol[g] * (yB - yC) / rBC;
				for (int32_t i = 2; i < NR - 2; i++)
				{
					xT = xB1 + (xC1 - xB1) / NR * (i - 0.5);
					yT = yB1 + (yC1 - yB1) / NR * (i - 0.5);
					//dX = xT - int32_t(xT); dY = yT - int32_t(yT);
					if (((xT < 1.5) || (xT > immaxX - 2.5) || (yT < 1.5) || (yT > immaxY - 2.5)))
						Line[i][3] = -1; else Line[i][g] = img[yT][xT];
					//GetInterPixel(yT, xT, dX, dY);
				}
			}
			Krit = -0;
			double q = 0, u = 0, n = 0;
			for (int32_t i = 2; i < NR - 2; i++) if (Line[i][3] != -1)
			{
				fi = 0;
				n++;
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if (r > tr)
					Krit = Krit + w3;
				r = ((Line[i][4] + Line[i][5]) - (Line[i][2] + Line[i][1])) / 2.0;
				if (r > tr)
					Krit = Krit + w2;
				r = Line[i][4] - Line[i][2];
				if (r > tr)
					Krit = Krit + w1;
				// Количество превышений интенсивности должно быть не менее 35 %
				r = ((Line[i][5] + Line[i][6]) / 2.0 - (Line[i][1] + Line[i][0]) / 2.0); if (r > tr) q = q + 1;
				r = ((Line[i][5] + Line[i][6] + Line[i][1] + Line[i][0]) / 4.0); if (r < 40) u = u + 1;
			}
			if (q < (n) * 0.35) Krit = Krit * 0.7;
			Okrit = Krit;
			if (u < (n) * 0.7) Okrit = Krit * n / (n - u);
			if (u < (n) * 0.4) Krit = Krit * n / (n - u);

			if (Krit > Kritb)
			{
				Okritb = Okrit;
				Kritb = Krit; sCb = sC; sDb = sD; xCsb2 = xC; yCsb2 = yC; xDsb2 = xD; yDsb2 = yD;
			}

		}
		OKrits[3] = Okritb;

		xCsb = xCsb2; yCsb = yCsb2; xDsb = xDsb2; yDsb = yDsb2;
		Krits[3] = Kritb;
#pragma endregion

		xAb = xAsb; yAb = yAsb;
		xBb = xBsb; yBb = yBsb;
		xCb = xCsb; yCb = yCsb;
		xDb = xDsb; yDb = yDsb;

		lstat = ListStat(xAb, yAb, xBb, yBb, xCb, yCb, xDb, yDb);
	};

	float GetKrit(uint8_t** img, Vector2 A, Vector2 B, float disp)
	{
#pragma region Vars

		float Krits[4];

		double dol[9];
		bool ResetNewPos = false;
		float DD = disp / 0.2 * DK;

		ResetNewPos = false;
		int32_t count = 0;
		double Kritbb = -1e20, Kritb = Kritbb, Krit, Okritb, Okrit;

		int32_t o1 = 200, o2 = o1 / 20;
		dol[5] = 0.75; dol[6] = 1.5;  dol[7] = 2.5;  dol[8] = 4;
		dol[4] = 0;
		dol[0] = -dol[8]; dol[1] = -dol[7]; dol[2] = -dol[6]; dol[3] = -dol[5];
		int32_t tr = 12;
		int32_t NR = 35;
		double w1 = 1, w2 = 0.5, w3 = 0.25;
		double r; double fi;

		float Line[50][7];
#pragma endregion

		Kritb = -10e20;
		float G11 = -2; float  G12 = 2;
		float G21 = -3; float  G22 = 3;
		float G31 = -2; float  G32 = 2;

		int32_t xT, yT, dX, dY;

		float k1 = 0, k2 = 0, k3 = 0, k1b = 0, k2b = 0, k3b = 0;

		Vector2 TA, TB, TX;

		float TAx, TAy, TBx, TBy, TCx, TCy, Tx, Ty, TTx, TTy;


		TAx = A.x + k1 * TX.x;
		TBx = B.x + k3 * TX.x - TAx;
		TCx = -4 * k2 * TX.x;

		TAy = A.y + k1 * TX.y;
		TBy = B.y + k3 * TX.y - TAy;
		TCy = -4 * k2 * TX.y;

		float maxI = 0, minI = 255;
		float medL = 0, LC = 0;

		for (int32_t i = 0; i < NR; i++) Line[i][3] = 0;
		for (int32_t i = 0; i < NR; i++)
		{
			float k = (i + 1) / float(NR + 1);
			for (int32_t g = 0; g < 9; g++) if (g != 5)
			{
				xT = TAx + k * TBx + k * (K - 1) * TCx + dol[g] * TX.x * 2;
				yT = TAy + k * TBy + k * (K - 1) * TCy + dol[g] * TX.y * 2;
				if (((xT < 1.5) || (xT > immaxX - 2.5) || (yT < 1.5) || (yT > immaxY - 2.5)));
				else
				{
					if (maxI < img[yT][xT])maxI = img[yT][xT];
					if (minI > img[yT][xT])minI = img[yT][xT];
					if (g < 4)
					{
						medL += img[yT][xT];
						LC++;
					}
				}
			}
		}

		medL /= LC;

		float down = 255 / (maxI - minI / 2.0);

		TX = !(A - B); TX.SetLenght(1);
		for (int32_t E = 0; E < 5; E++)
		{
			k1 = k1b; k2 = k2b; k3 = k3b;
			for (int32_t o = 1; o < o1; o++)
			{
				if (0 != 1)
				{
					k1 = G11 + (G12 - G11) * random;
					k2 = G21 + (G22 - G21) * random;
					k3 = G31 + (G32 - G31) * random;
				}
				if (o % o2 == 0)
				{
					r = 0.93;

					if (abs(0.5 * (G11 + G12) - k1) / (G12 - G11) < 0.3)
					{
						G12 = G11 + r * (G12 - G11) / 2.0; G11 = G12 - r * (G12 - G11) / 2.0;
					}
					else if (k1 < 0.5 * (G11 + G12))
						G12 = G11 + r * (G12 - G11);
					else
						G11 = G12 - r * (G12 - G11);


					if (abs(0.5 * (G21 + G22) - k2) / (G22 - G21) < 0.3)
					{
						G22 = G21 + r * (G22 - G21) / 2.0; G21 = G22 - r * (G22 - G21) / 2.0;
					}
					else if (k2 < 0.5 * (G21 + G22))
						G22 = G21 + r * (G22 - G21);
					else
						G21 = G22 - r * (G22 - G21);


					if (abs(0.5 * (G31 + G32) - k3) / (G32 - G31) < 0.3)
					{
						G32 = G31 + r * (G32 - G31) / 2.0; G31 = G32 - r * (G32 - G31) / 2.0;
					}
					else if (k3 < 0.5 * (G31 + G32))
						G32 = G31 + r * (G32 - G31);
					else
						G31 = G32 - r * (G32 - G31);
				}

				TAx = A.x + k1 * TX.x;
				TBx = B.x + k3 * TX.x - TAx;
				TCx = -4 * k2 * TX.x;

				TAy = A.y + k1 * TX.y;
				TBy = B.y + k3 * TX.y - TAy;
				TCy = -4 * k2 * TX.y;
				for (int32_t i = 0; i < NR; i++) Line[i][3] = 0;
				for (int32_t i = 0; i < NR; i++)
				{
					float k = (i + 1) / float(NR + 1);
					for (int32_t g = 0; g < 9; g++) if (g != 4)
					{
						xT = TAx + k * TBx + k * (K - 1) * TCx + dol[g] * TX.x;
						yT = TAy + k * TBy + k * (K - 1) * TCy + dol[g] * TX.y;
						if (((xT < 1.5) || (xT > immaxX - 2.5) || (yT < 1.5) || (yT > immaxY - 2.5)))
							Line[i][4] = -1;
						else
						{
							Line[i][g] = (img[yT][xT] - minI / 2.0) * down;
						}

					}
				}
				Okrit = 0;
				Krit = -0;
				double q = 0, u = 0, n = 0, r0;
				for (int32_t i = 0; i < NR; i++) if (Line[i][4] != -1)
				{
					fi = 0;
					n++;
					r0 = Line[i][5] - Line[i][3];
					if (r0 > tr)
					{
						Krit = Krit + 1.6;
						Okrit += 0.8;
					}
					r = ((Line[i][5] * 0.6 + Line[i][4] * 0.4) - (Line[i][1] + Line[i][2]) / 2.0);
					if (r > tr)
					{
						Krit = Krit + 0.25;
						if (r < 1.0 * r0)
						{
							Okrit += 0.25;
							Krit += 0.25;
						}
					}
					r = ((Line[i][5] * 0.6 + Line[i][6] * 0.4) - (Line[i][1] + Line[i][0]) / 2.0);
					if (r > tr)
					{
						Krit = Krit + 0.25;
						if (r < 1.0 * r0)
						{
							Okrit += 0.25;
							Krit += 0.25;
						}
					}
					r = ((Line[i][6]) - (Line[i][2]));
					if (r > tr)
					{
						Krit = Krit + 0.5;
						if (r < 1.0 * r0)
						{
							Okrit += 0.5;
							Krit += 0.5;
						}
					}

					// Количество превышений интенсивности должно быть не менее 35 %
					r = ((Line[i][5] + Line[i][6]) / 2.0 - (Line[i][1] + Line[i][0]) / 2.0);
					if (r > tr) q = q + 1;
					r = MIN_3(Line[i][5], Line[i][6], Line[i][6]);
					if (r < medL * 0.9) u = u + 1;
				}
				//if (q < (n) * 0.35) Krit = Krit * 0.7;
				//Okrit = Krit;
				if (u < (n) * 0.7)
					Okrit = Okrit * n / (n - u);
				if (u < (n) * 0.5)
					Krit = Krit * n / (n - u);

				if (Krit > Kritb)
				{
					Okritb = Okrit;
					Kritb = Krit;
					k1b = k1;
					k2b = k2;
					k3b = k3;
				}

			}
		}
		return Okritb;

	};

	float GetKrit2old(uint8_t** img, Vector2 A, Vector2 B, Vector2 C, Vector2 D)
	{
		float Xs[10][31], Ys[10][31], Is[10][31], IsB[10][31], SKS[31], Ux, Uy, Vx, Vy;
		for (int32_t i = 0; i < 10; ++i)
		{
			Ux = A.x + (i + 1) / 11.0 * (B.x - A.x); Uy = A.y + (i + 1) / 11.0 * (B.y - A.y);

			Vx = D.x + (i + 1) / 11.0 * (C.x - D.x); Vy = D.y + (i + 1) / 11.0 * (C.y - D.y);

			float r = sqrt((Ux - Vx) * (Ux - Vx) + (Uy - Vy) * (Uy - Vy));
			for (int32_t j = 0, _j = -15; j < 31; ++j, ++_j)
			{
				Xs[i][j] = Ux + _j / r * (Vx - Ux);
				Ys[i][j] = Uy + _j / r * (Vy - Uy);
				if (((Xs[i][j] < 1.5) || (Xs[i][j] > immaxX - 2.5) || (Ys[i][j] < 1.5) || (Ys[i][j] > immaxY - 2.5)))
					Is[i][j] = 0;
				else
					Is[i][j] = img[int32_t(Ys[i][j])][int32_t(Xs[i][j])];
			}
		}
		float KTable[10], STable[10];

		// Аппроксимируем сигмоидами
		for (int32_t j = 0; j < 20; ++j)
		{
			int32_t i = j % 10;
			float	G11 = 0, G12 = 230,
				G21 = 18, G22 = 255,
				G31 = 8, G32 = 23,
				G41 = 0.15, G42 = 2,
				Kritb = 1e20,
				Int1, Int2, Pos, Sig,
				Int1b, Int2b, Posb, Sigb


				;
			for (int32_t o = 1; o < 4000; ++o)
			{
				Int1 = G11 + (G12 - G11) * random;
				Int2 = G21 + (G22 - G21) * random;
				Pos = G31 + (G32 - G31) * random;
				if (random < 0.5) Sig = G41 + (G42 - G41) * random / 2;
				else Sig = G41 + (G42 - G41) * random;
				if (Int2 < Int1 + 18) continue;
				if (o % 100 == 0)
				{
					float r = 0.94;
					if (Int1b < (G11 + G12) / 2) G12 = G11 + r * (G12 - G11); else G11 = G12 - r * (G12 - G11);
					if (Int2b < (G21 + G22) / 2) G22 = G21 + r * (G22 - G21); else G21 = G22 - r * (G22 - G21);
					if (Posb < (G31 + G32) / 2) G32 = G31 + r * (G32 - G31); else G31 = G32 - r * (G32 - G31);
					if (Sigb < (G41 + G42) / 2) G42 = G41 + r * (G42 - G41); else G41 = G42 - r * (G42 - G41);
				};
				// Строим функцию
				int32_t i1 = round(Pos - Sig * 8); if (i1 < 0) i1 = 0;
				int32_t i2 = round(Pos + Sig * 8); if (i2 > 30) i2 = 30;
				for (int32_t j = 0; j <= 30; ++j)
					if ((j < i1) || (j > i2))
						SKS[j] = 0;
					else
						SKS[j] = Int1 + (Int2 - Int1) / (1 + expf(-(j - Pos) / Sig));
				// Считаем критерий
				float Krit = 0;
				for (int32_t j = 0; j <= 30; ++j)
					if (SKS[j] > 0)
						Krit = Krit +
						(Is[i][j] - SKS[j]) * (Is[i][j] - SKS[j]) * (Is[i][j] - SKS[j]) * (Is[i][j] - SKS[j])
						*
						expf((Pos - j) * (Pos - j) / (16 * Sig * Sig));

				Krit = Krit / (i2 - i1) / ((Int2 - Int1 + 1e-20) * (Int2 - Int1 + 1e-20) * (Int2 - Int1 + 1e-20) * (Int2 - Int1 + 1e-20));
				if (Krit < Kritb)
				{
					Kritb = Krit; Int1b = Int1; Int2b = Int2; Posb = Pos; Sigb = Sig;
					for (int32_t j = 0; j <= 30; ++j)  IsB[i][j] = SKS[j];
				};
			};
			if (i < 10)
			{
				KTable[i] = Kritb * 10000;
				STable[i] = Sigb * 6;
			}
			else if (KTable[i] > Kritb * 10000)
			{
				KTable[i] = Kritb * 10000;
				STable[i] = Sigb * 6;
			}
		};

		float o1 = 0;
		int32_t k = 0;
		for (int32_t i = 0; i < 10; ++i)
			if (KTable[i] < 10)
			{
				k = k + 1;
				o1 = o1 + STable[i];
			};
		if (k > 0) o1 = o1 / k; else
			o1 = 10;

		return o1;
	}

	float GetKrit2(uint8_t** img, const Vector2& A, const Vector2& B, const Vector2& C, const Vector2& D)
	{
		float Is[10][31];

		float Ux;
		float Uy;
		float Vx;
		float Vy;

		for (int32_t i = 0; i < 10; ++i) {
			Ux = A.x + (i + 1) / 11.0 * (B.x - A.x);
			Uy = A.y + (i + 1) / 11.0 * (B.y - A.y);
			Vx = D.x + (i + 1) / 11.0 * (C.x - D.x);
			Vy = D.y + (i + 1) / 11.0 * (C.y - D.y);

			float r = sqrt((Ux - Vx) * (Ux - Vx) + (Uy - Vy) * (Uy - Vy));

			for (int32_t j = 0, _j = -15; j < 31; ++j, ++_j) {
				const float x = Ux + _j / r * (Vx - Ux);
				const float y = Uy + _j / r * (Vy - Uy);

				if (x < 1.5 || x > immaxX - 2.5 || y < 1.5 || y > immaxY - 2.5)
					Is[i][j] = 0;
				else
					Is[i][j] = img[int32_t(y)][int32_t(x)];
			}
		}

		float KTable[10];
		float STable[10];

		// Аппроксимируем сигмоидами
		for (int32_t j = 0; j < 20; ++j) {
			int32_t i = j % 10;
			float G11 = 0, G12 = 230,
				G21 = 18, G22 = 255,
				G31 = 8, G32 = 23,
				G41 = 0.15, G42 = 2,
				Kritb = 1e20,
				Int1, Int2, Pos, Sig,
				Int1b, Int2b, Posb, Sigb;

			for (int32_t o = 1; o < 4000; ++o) {

				Int1 = G11 + (G12 - G11) * random;
				Int2 = G21 + (G22 - G21) * random;

				if (Int2 < Int1 + 18)
					continue;

				Pos = G31 + (G32 - G31) * random;
				if (random < 0.5)
					Sig = G41 + (G42 - G41) * random / 2;
				else
					Sig = G41 + (G42 - G41) * random;

				if (o % 100 == 0) {
					const float r = 0.94;
					if (Int1b < (G11 + G12) / 2) G12 = G11 + r * (G12 - G11); else G11 = G12 - r * (G12 - G11);
					if (Int2b < (G21 + G22) / 2) G22 = G21 + r * (G22 - G21); else G21 = G22 - r * (G22 - G21);
					if (Posb < (G31 + G32) / 2) G32 = G31 + r * (G32 - G31); else G31 = G32 - r * (G32 - G31);
					if (Sigb < (G41 + G42) / 2) G42 = G41 + r * (G42 - G41); else G41 = G42 - r * (G42 - G41);
				};

				// Строим функцию
				const int32_t i1 = max(0, round(Pos - Sig * 8));
				const int32_t i2 = min(30, round(Pos + Sig * 8));

				// Считаем критерий
				float Krit = 0;
				for (int32_t j = 0; j <= 30; ++j)
					if (i1 < j && j < i2) {
						const float coeffTemp = (Pos - j) / Sig;
						float coeff = Is[i][j] - (Int1 + (Int2 - Int1) / (1 + expf(coeffTemp)));
						coeff *= coeff;
						coeff *= coeff;
						Krit += coeff * expf(coeffTemp * coeffTemp / 16);
					}

				float coeff = Int2 - Int1 + 1e-20;
				coeff *= coeff;
				coeff *= coeff;
				Krit /= (i2 - i1) * coeff;

				if (Krit < Kritb) {
					Kritb = Krit;
					Int1b = Int1;
					Int2b = Int2;
					Posb = Pos;
					Sigb = Sig;
				}
			}

			KTable[i] = Kritb * 1e4;
			STable[i] = Sigb * 6;
		};

		float o1 = 0;
		int32_t k = 0;
		for (int32_t i = 0; i < 10; ++i)
			if (KTable[i] < 10) {
				++k;
				o1 += STable[i];
			}

		if (k > 0)
			o1 /= k;
		else
			o1 = 10;
		return o1;
	}

	double** GetNewCamPos(uint8_t** img, double w, double h, double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy)
	{
		Vector3 AT, BT, CT, DT, O;
		double dig = std::sqrt(w * w + h * h), matrixDig = sqrt(24 * 24 + 36 * 36);
		double t = matrixDig / dig;
		Vector3 A(ax, ay, 0), B(bx, by, 0), C(cx, cy, 0), D(dx, dy, 0), R1(h / 2.0, w / 2.0, 0), R2, R3, R4;
		A = (A - R1) * K; B = (B - R1) * K; C = (C - R1) * K; D = (D - R1) * K;
		R1 = A - B;
		R2 = B - C;
		R3 = C - D;
		R4 = D - A;
		double a, b, c, d, d1, d2, l1, l2, dmax = 0;
		double M[3][4], maxerr = 1e30;
		double lamda = 50;
		bool flip = false;
		if (min(R1 * R1, R3 * R3) < min(R2 * R2, R4 * R4))
		{
			flip = true;
			O = A;
			A = B; B = C; C = D; D = O;
		}
		int32_t rot = 0;
		d = 6;
		Vector3 AO = A, BO = B, CO = C, DO = D;
		M[0][0] = 1; M[0][1] = -1; M[0][2] = 1; M[0][3] = 1;
		M[1][0] = A.x; M[1][1] = -B.x; M[1][2] = C.x; M[1][3] = D.x;
		M[2][0] = A.y; M[2][1] = -B.y; M[2][2] = C.y; M[2][3] = D.y;
		for (int32_t j = 0; j < 3; j++)
			for (int32_t k = 0; k < 3; k++)if (k != j)
			{
				t = M[k][j] / M[j][j];
				for (int32_t i = j; i < 4; i++)
				{
					M[k][i] = M[k][i] - M[j][i] * t;
				}
			}
		for (int32_t j = 0; j < 3; j++)
		{
			t = 1 / M[j][j];
			M[j][j] = 1;
			M[j][3] *= t;
		}
		AO.z = 0; BO.z = 0; CO.z = 0; DO.z = 0;
		Vector3 BA, BB, BC, BD;
		double bx1 = 0, by1 = 0, bx2 = 0, by2 = 0, bx3 = 0, by3 = 0, bx4 = 0, by4 = 0, Kri = 1e10, dr = K * 0.05; int32_t counter = 0;

		Vector3 Olist[4]{ A,B,C,D };
		Vector3 list[4];

		for (int32_t l = 0; l < 20; l++)
		{
			double te = 1e10, tx1 = 0, ty1 = 0, tx2 = 0, ty2 = 0, tx3 = 0, ty3 = 0, tx4 = 0, ty4 = 0;
			float x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0, x4 = 0, y4 = 0;
			for (x2 = -1; x2 <= 1; x2++) for (y2 = -1; y2 <= 1; y2++)
			{
				A.x = AO.x; A.y = AO.y;
				B.x = BO.x; B.y = BO.y;
				C.x = CO.x; C.y = CO.y;
				D.x = DO.x; D.y = DO.y;
				A.z = 0; B.z = 0; C.z = 0; D.z = 0;
				B.x += bx2 + dr * x2; C.x += bx3 + dr * x3; D.x += bx4 + dr * x4;
				B.y += by2 + dr * y2; C.y += by3 + dr * y3; D.y += by4 + dr * y4;

				M[0][0] = 1; M[0][1] = -1; M[0][2] = 1; M[0][3] = 1;
				M[1][0] = A.x; M[1][1] = -B.x; M[1][2] = C.x; M[1][3] = D.x;
				M[2][0] = A.y; M[2][1] = -B.y; M[2][2] = C.y; M[2][3] = D.y;
				for (int32_t j = 0; j < 3; j++)
					for (int32_t k = 0; k < 3; k++)if (k != j)
					{
						t = M[k][j] / M[j][j];
						for (int32_t i = j; i < 4; i++)
						{
							M[k][i] = M[k][i] - M[j][i] * t;
						}
					}
				for (int32_t j = 0; j < 3; j++)
				{
					t = 1 / M[j][j];
					M[j][j] = 1;
					M[j][3] *= t;
				}

				a = M[0][3]; b = M[1][3]; c = M[2][3];
				lamda = -((D - A * a) * (D - C * c)) / (1 - a - c + a * c);
				lamda = sqrt(abs(lamda));
				A.z = lamda; B.z = lamda; C.z = lamda, D.z = lamda;
				R1 = A * M[0][3] - B * M[1][3]; R2 = C * M[2][3] - B * M[1][3];
				l1 = sqrt(R1 * R1);
				l2 = sqrt(R2 * R2);
				d = l1 / l2;
				R1 = Vector3(1, d, 0);
				R2 = Vector3(1, -d, 0);
				d1 = -(297 + d * 210) / (1 + d * d);
				d2 = -(297 - d * 210) / (1 + d * d);
				R1 = R1 * d1;
				R2 = R2 * d2;
				d1 = R1.GetLenght();
				d2 = R2.GetLenght();
				if (d1 < d2)
				{
					d1 = R1.x;
					d2 = R1.y;
				}
				else
				{
					d1 = R2.x;
					d2 = R2.y;
				}
				d = (abs(d1) + abs(d2)) / 2.0;

				float e = d * d;

				if (lamda < 25)
					e *= (25 - lamda) * 100;
				if (lamda > 45)
					e *= (lamda - 45) * 100;
				if (e < te)
				{
					te = e;
					tx2 = x2 * dr; ty2 = y2 * dr;
					tx4 = x4 * dr; ty4 = y4 * dr;
				}
			}
			if (te <= Kri)
			{
				Kri = te;
				bx2 += tx2; by2 += ty2;
				bx4 += tx4; by4 += ty4;
			}

			//dr = K * 0.1 * (Kri + 100) / (100 + Kri);

			//dr = dr * 0.7071;
		}
		A.x = AO.x; A.y = AO.y;
		B.x = BO.x; B.y = BO.y;
		C.x = CO.x; C.y = CO.y;
		D.x = DO.x; D.y = DO.y;
		A.z = 0; B.z = 0; C.z = 0; D.z = 0;
		B.x += bx2; C.x += bx3; D.x += bx4;
		B.y += by2; C.y += by3; D.y += by4;
		a = M[0][3]; b = M[1][3]; c = M[2][3];
		lamda = -((D - A * a) * (D - C * c)) / (1 - a - c + a * c);
		lamda = sqrt(abs(lamda));
		A.z = lamda; B.z = lamda; C.z = lamda, D.z = lamda;
		R1 = A * M[0][3] - B * M[1][3]; R2 = C * M[2][3] - B * M[1][3];
		l1 = sqrt(R1 * R1);
		l2 = sqrt(R2 * R2);
		d = l1 / l2;
		R1 = Vector3(1, d, 0);
		R2 = Vector3(1, -d, 0);
		d1 = -(297 + d * 210) / (1 + d * d);
		d2 = -(297 - d * 210) / (1 + d * d);
		R1 = R1 * d1;
		R2 = R2 * d2;
		d1 = R1.GetLenght();
		d2 = R2.GetLenght();
		if (d1 < d2)
		{
			d1 = R1.x;
			d2 = R1.y;
		}
		else
		{
			d1 = R2.x;
			d2 = R2.y;
		}
		double t1 = (297 + d1) / l1, t2 = (210 + d2) / l2;
		d = (t1 + t2) / 2.0;
		a = a * d;	b = b * d;	c = c * d;	d = d;
		A = AO; B = BO; C = CO; D = DO;
		A.z = lamda; B.z = lamda; C.z = lamda, D.z = lamda;
		if (flip)
		{
			O = D;
			D = C; C = B; B = A; A = O;
			t = d;
			d = c; c = b; b = a; a = t;
		}
		R1 = Vector3(w / 2.0, h / 2.0, 0);
		AO = (A / K + R1); BO = (B / K + R1); CO = (C / K + R1); DO = (D / K + R1);
		AT = A * a;
		BT = B * b;
		CT = C * c;
		DT = D * d;
		O = (AT + BT + CT + DT) / 4.0;
		R1 = AT - BT;
		R2 = AT - DT;
		R3 = R1 / R2; R3 = R3 / R3.z;
		R1.SetLenght(1);
		R2.SetLenght(1);
		R3.SetLenght(1);
		AT = AT - O;
		BT = BT - O;
		CT = CT - O;
		DT = DT - O;
		Vector3 WA(AT * R1, AT * R2, AT * R3), WB(BT * R1, BT * R2, BT * R3), WC(CT * R1, CT * R2, CT * R3), WD(DT * R1, DT * R2, DT * R3);
		O = Vector3(O * R1, O * R2, O * R3);
		t = (R1.x * R2.y * R3.z) - (R3.x * R2.y * R1.z) - (R2.x * R1.y * R3.z) - (R1.x * R3.y * R2.z) + (R3.x * R1.y * R2.z) + (R2.x * R3.y * R1.z);
		Vector3 X(R1.x, R2.x, R3.x), Y(R1.y, R2.y, R3.y), Z(R1.z, R2.z, R3.z);
		X.SetLenght(1); Y.SetLenght(1); Z.SetLenght(1);

		double** ret = new double* [8]; ret[0] = new double[5];
		for (int32_t i = 1; i < 8; i++) ret[i] = new double[3];

		ret[0][0] = O.x; ret[0][1] = O.y; ret[0][2] = O.z; ret[0][3] = lamda;
		ret[1][0] = X.x; ret[1][1] = X.y; ret[1][2] = X.z;
		ret[2][0] = Y.x; ret[2][1] = Y.y; ret[2][2] = Y.z;
		ret[3][0] = Z.x; ret[3][1] = Z.y; ret[3][2] = Z.z;
		ret[4][0] = WA.x; ret[4][1] = WA.y; ret[4][2] = WA.z;
		ret[5][0] = WB.x; ret[5][1] = WB.y; ret[5][2] = WB.z;
		ret[6][0] = WC.x; ret[6][1] = WC.y; ret[6][2] = WC.z;
		ret[7][0] = WD.x; ret[7][1] = WD.y; ret[7][2] = WD.z;

		for (int32_t i = 0; i < 4; i++) for (int32_t j = 0; j < 3; j++)
			gcams[i][j] = ret[i][j];
		gcams[4][0] = lamda;

		return ret;
	}

	void Th_UpdateOreint(uint8_t** img, int32_t& borr, float& krbb, Vector3 A, Vector3 B, Vector3 C, Vector3 D)
	{
#define Oreintdisp 40
		float r, t;
		borr = 1; krbb = -1;
		float CFxbb = 0, CFybb = 0, CFzbb = 0.5, CFfbb = 0;
		int32_t  blor = 1;
		int32_t CSh = 25;
		float _x, _y;
		int32_t buff[Oreintdisp][Oreintdisp][4];

		float avg = 0, h = 0, P[4];

		int xT, yT;

		for (int32_t x = 0; x < Oreintdisp; x++)
		{
			_x = (x + 1) / (Oreintdisp + 1.0);
			for (int32_t y = 0; y < Oreintdisp; y++)
			{
				t = -1;
				_y = (y + 1) / (Oreintdisp + 1.0);
				P[1] = A.x * (1 - _x) * (1 - _y) + B.x * (_x) * (1 - _y) + C.x * (_x) * (_y)+D.x * (1 - _x) * (_y);
				P[0] = A.y * (1 - _x) * (1 - _y) + B.y * (_x) * (1 - _y) + C.y * (_x) * (_y)+D.y * (1 - _x) * (_y);

				xT = A.x * (1 - _x) * (1 - _y) + B.x * (_x) * (1 - _y) + C.x * (_x) * (_y)+D.x * (1 - _x) * (_y);
				yT = A.y * (1 - _x) * (1 - _y) + B.y * (_x) * (1 - _y) + C.y * (_x) * (_y)+D.y * (1 - _x) * (_y);
				if (((xT < 1.5) || (xT > immaxX - 2.5) || (yT < 1.5) || (yT > immaxY - 2.5)))
					t = -1;
				else
				{
					t = img[yT][xT];
				}

				//if (P[0] > 3 && P[1] > 3 && P[0] < immaxY - 5 && P[1] < immaxX - 5)
				//	t = img[int32_t(P[0])][int32_t(P[1])];
				buff[x][y][2] = P[0];
				buff[x][y][3] = P[1];
				buff[x][y][0] = t;
				if (t >= 0)
				{
					avg += t;
					h++;
				}
			}
		}
		avg /= h;
		for (int32_t x = 0; x < Oreintdisp; x++)
		{
			for (int32_t y = 0; y < Oreintdisp; y++)if (buff[x][y][0] >= 0)
			{
				if (buff[x][y][0] > avg * 0.5) buff[x][y][0] = 255;
				else buff[x][y][0] = 0;
			}
		}
		float n[4], point[4], tpoint[4];
		float npoints[200][4];
		for (int32_t E = 0; E < 8; E++)
		{
			int32_t orr = (E % 2 == 0) ? 1 : -1, lor = ((E % 4) / 2) ? 1 : -1;
			float G11 = -80, G12 = 80, G21 = -40, G22 = 40, G31 = 0.5, G32 = 1, G41 = -15 / 180.0 * PI, G42 = 15 / 180.0 * PI;
			float CFx, CFy, CFz, CFf, CFxb = 0, CFyb = 0, CFzb = 1, CFfb = 0;
			float krb = -1;
			for (int32_t I = 0; I < 75; I++)
			{
				CFx = G11 + random * (G12 - G11);
				CFy = G21 + random * (G22 - G21);
				CFz = G31 + random * (G32 - G31);
				CFf = G41 + random * (G42 - G41);
				if (I % 3 == 2)
				{
					r = 0.93;
					if (CFxb < (G11 + G12) / 2.0) G12 = G11 + (G12 - G11) * r; else G11 = G12 - (G12 - G11) * r;
					if (CFyb < (G21 + G22) / 2.0) G22 = G21 + (G22 - G21) * r; else G21 = G22 - (G22 - G21) * r;
					if (CFzb < (G31 + G32) / 2.0) G32 = G31 + (G32 - G31) * r; else G31 = G32 - (G32 - G31) * r;
					if (CFfb < (G41 + G42) / 2.0) G42 = G41 + (G42 - G41) * r; else G41 = G42 - (G42 - G41) * r;
				}
				float co = cos(CFf), si = sin(CFf), tx, ty;
				for (int32_t i = 0; i < CSh; i++)
				{
					point[0] = CFz * (shariki[i][0] * co - lor * shariki[i][1] * si) * orr + CFx;
					point[1] = CFz * (shariki[i][0] * si + lor * shariki[i][1] * co) * orr + CFy;
					point[2] = CFz * shariki[i][2];
					point[0] -= gcams[0][0]; point[1] -= gcams[0][1]; point[2] -= gcams[0][2];
					npoints[i][0] = (point[0] * gcams[1][0] + point[1] * gcams[1][1] + point[2] * gcams[1][2]);
					npoints[i][1] = (point[0] * gcams[2][0] + point[1] * gcams[2][1] + point[2] * gcams[2][2]);
					npoints[i][2] = (point[0] * gcams[3][0] + point[1] * gcams[3][1] + point[2] * gcams[3][2]);
					npoints[i][3] = shariki[i][3] * CFz;
				}
				float kr = 0;
				for (int32_t x = 0; x < Oreintdisp; x++)
				{
					for (int32_t y = 0; y < Oreintdisp; y++)
					{
						n[2] = gcams[4][0] / K;
						n[1] = -(buff[x][y][2] - immaxY / 2.0);
						n[0] = -(buff[x][y][3] - immaxX / 2.0);
						float a = n[2] * n[2] + n[1] * n[1] + n[0] * n[0], b, c;
						bool isnotin = true;
						for (int32_t i = 0; i < CSh && isnotin; i++)
						{
							point[0] = npoints[i][0]; point[1] = npoints[i][1]; point[2] = npoints[i][2];

							b = n[2] * point[2] + n[1] * point[1] + n[0] * point[0];
							c = point[2] * point[2] + point[1] * point[1] + point[0] * point[0];
							if (c - b * b / a <= shariki[i][3] * shariki[i][3])
								isnotin = false;
						}
						if (!isnotin)
						{
							buff[x][y][1] = 255;

						}
						else
						{
							buff[x][y][1] = 0;

						}
						if (!(buff[x][y][1] == buff[x][y][0]))
						{
							kr++;
						}
					}
				}

				if (kr > krb)
				{
					krb = kr;
					CFxb = CFx; CFyb = CFy; CFzb = CFz; CFfb = CFf;

				}
				if (krb > krbb)
				{
					krbb = krb;
					blor = lor; borr = orr;
					CFxbb = CFxb; CFybb = CFyb; CFzbb = CFzb; CFfbb = CFfb;
				}
			}
		}
	}

	int32_t UpdateOreint(uint8_t** img, Vector3 A, Vector3 B, Vector3 C, Vector3 D)
	{
		//std::thread *ths[4];
		int32_t borrs[8]; float krits[8];
		//for (int32_t i = 0; i < 4; i++)
		//{
		//	ths[i] = new std::thread(Th_UpdateOreint, std::ref(borrs[i]), std::ref(krits[i]), A, B, C, D);
		//}
		//for (int32_t i = 0; i < 4; i++)
		//{
		//	ths[i]->join();
		//}
		//float b = -1000;
		int32_t borr = 1;
		//for (int32_t i = 0; i < 4; i++)
		//{
		//	if (krits[i] > b)
		//	{
		//		b = krits[i];
		//		borr = borrs[i];
		//	}
		//	delete ths[i];
		//}
		//delete borrs;
		//delete krits;
		Th_UpdateOreint(img, borrs[0], krits[0], A, B, C, D);
		return borrs[0];
	}

	string  Start(uint8_t** img);

	std::string GetA4FromImg(uint8_t** img, Prop* props, int32_t size)
	{
		Right = -1;
		for (int32_t i = 0; i < size; i++)
		{
			string str(props[i].name, props[i].lenght);
			if (str == "ImgX")
				immaxX = props[i].var;
			else if (str == "ImgY")
				immaxY = props[i].var;
			else if (str == "ThreadCount")
				maxthreadcount = props[i].var;
			else if (str == "Kadr")
				KadrNumber = props[i].var;
			else if (str == "Gamma")
				GGama = props[i].var;
			else if (str == "Beta")
				GBeta = props[i].var;
			else if (str == "Focal")
				GFocal = props[i].var;
			else if (str == "Right")
				Right = 1;
			else if (str == "Left")
				Right = 0;
			else if (str == "Side")
				Right = (props[i].var == 0) ? 0 : ((props[i].var == 1) ? 1 : -1);
		}

		double time = 0;
		useangel = (KadrNumber > 0) ? 0 : 1;
		fa4::KadrNumber = abs(KadrNumber);
		for (int32_t i = 0; i < RandomCount; i++)
			arandom[i] = (rand()) / double(RAND_MAX);
		{
			for (int32_t i = 0; i < 2001; i++)
			{
				double f = (i - 1000) / 100.0;
				tacos[i] = acos(f / 10.0);
			}

			for (int32_t i = 0; i < 2000; i++)
			{
				tr[i] = exp(0.83 * log(1 - i / (double)2000 + 1e-5));
			}
		}
		Kadr = KadrNumber;
		K = 43.26661530556787151743 / sqrt(immaxX * immaxX + immaxY * immaxY);
		DK = baseK / K;
		//fa4::img = img;

		return Start(img);
	}

	string GetA4FromImg(int32_t maxthreadcount, uint8_t** img, int32_t inpmaxY, int32_t inpmaxX, int32_t KadrNumber, int32_t sd)
	{
		Right = (sd == 0) ? 0 : ((sd == 1) ? 1 : -1);
		double time = 0;
		useangel = (KadrNumber > 0) ? 0 : 1;
		fa4::KadrNumber = abs(KadrNumber);
		for (int32_t i = 0; i < RandomCount; i++)
			arandom[i] = (rand()) / double(RAND_MAX);
		//srand(img[immaxY / 13][immaxX / 11]);
		{
			// Табуляция одной из функций
								 // Предварительно табулируем sin и cos
			for (int32_t i = 0; i < 2001; i++)
			{
				double f = (i - 1000) / 100.0;
				tacos[i] = acos(f / 10.0);
			}

			for (int32_t i = 0; i < 2000; i++)
			{
				tr[i] = exp(0.83 * log(1 - i / (double)2000 + 1e-5));
			}
		}
		immaxX = inpmaxX;
		immaxY = inpmaxY;
		Kadr = KadrNumber;
		K = 43.26661530556787151743 / sqrt(immaxX * immaxX + immaxY * immaxY);
		DK = baseK / K;
		//fa4::img = img;

		fa4::maxthreadcount = maxthreadcount;

		return Start(img);
	}

	string  Start(uint8_t** img)
	{
#pragma region

		int32_t orienta, SheetFound, SF[20], SPF[20], PF[20];
		double KritBB[20];
		CameraStats cstat2[20];
		ListStat lstat2[20];

		thread threads[20];

		if (maxthreadcount > 20) maxthreadcount = 20;
		if (maxthreadcount < 1) maxthreadcount = 1;

		for (int32_t i = 0; i < maxthreadcount - 1; i++)
		{

			threads[i] = thread{ BruteScan , std::ref(img), 12000 / maxthreadcount, 350, std::ref(cstat2[i + 1]), std::ref(lstat2[i + 1]), std::ref(KritBB[i + 1]), std::ref(SF[i + 1]), std::ref(PF[i + 1]), std::ref(SPF[i + 1]), i + 1 };

		}
		BruteScan(img, 12000 / maxthreadcount, 350, cstat2[0], lstat2[0], KritBB[0], SF[0], PF[0], SPF[0], 0);

		for (int32_t i = 0; i < maxthreadcount - 1; i++)
		{
			threads[i].join();
		}

		for (int32_t i = 1; i < maxthreadcount; i++) if (KritBB[0] < KritBB[i])
		{
			SF[0] = SF[i];
			SPF[0] = SPF[i];
			PF[0] = PF[i];
			lstat2[0] = lstat2[i];
			KritBB[0] = KritBB[i];
		}
		//srand(img[immaxY / 13][immaxX / 11]);
		SheetFound = SF[0];

		double out[30][10];

		int32_t SayPointCameraDown = 0,
			SayShiftRightBehindHeel_IfR = 0,
			SayShiftRightBehindHeel_IfL = 0,
			SayShiftLeftBehindHeel_IfR = 0,
			SayShiftLeftBehindHeel_IfL = 0,
			SayNotCenteredFoot = 0,
			NotCenteredSheet = 0,
			SayMoveFurther = 0,
			SayMoveCloser = 0,
			SayShiftDown = 0,
			SayShiftUp = 0,
			SayShiftLeft_IfL = 0,
			SayShiftLeft_IfR = 0,
			SayShiftRight_IfL = 0,
			SayShiftRight_IfR = 0,
			SayCrumpledSheet = 0,
			SayLowSock = 0,
			SayLowPants = 0,
			SayCurledFingers = 0,
			SayRotateDown = 0,
			SayRotateUp = 0;

#pragma endregion

#pragma region
		for (int32_t i = 0; i < RandomCount; i++)
			arandom[i] = (rand()) / double(RAND_MAX);
		float* B_Krits = new float[4];
		ListStat lstat = lstat2[0];
		lstat = ListStat(lstat.xB, lstat.yB, lstat.xC, lstat.yC, lstat.xD, lstat.yD, lstat.xA, lstat.yA);
		MakeBest(img, lstat, 0.2, B_Krits);
		lstat = ListStat(lstat.xD, lstat.yD, lstat.xA, lstat.yA, lstat.xB, lstat.yB, lstat.xC, lstat.yC);
		MakeBest(img, lstat, 0.11, B_Krits);
		lstat = ListStat(lstat.xD, lstat.yD, lstat.xA, lstat.yA, lstat.xB, lstat.yB, lstat.xC, lstat.yC);
		MakeBest(img, lstat, 0.075, B_Krits);
		lstat = ListStat(lstat.xD, lstat.yD, lstat.xA, lstat.yA, lstat.xB, lstat.yB, lstat.xC, lstat.yC);
		MakeBest(img, lstat, 0.05, B_Krits);

		double xAb = lstat.xA;
		double xBb = lstat.xB;
		double xCb = lstat.xC;
		double xDb = lstat.xD;

		double yAb = lstat.yA;
		double yBb = lstat.yB;
		double yCb = lstat.yC;
		double yDb = lstat.yD;


		B_Krits[0] = GetKrit(img, Vector2(xAb, yAb), Vector2(xBb, yBb), 0.05);
		B_Krits[1] = GetKrit(img, Vector2(xBb, yBb), Vector2(xCb, yCb), 0.05);
		B_Krits[2] = GetKrit(img, Vector2(xCb, yCb), Vector2(xDb, yDb), 0.05);
		B_Krits[3] = GetKrit(img, Vector2(xDb, yDb), Vector2(xAb, yAb), 0.05);

		float B_Krits2[4];

		B_Krits2[0] = GetKrit2(img, Vector2(xAb, yAb), Vector2(xBb, yBb), Vector2(xCb, yCb), Vector2(xDb, yDb));
		B_Krits2[1] = GetKrit2(img, Vector2(xDb, yDb), Vector2(xAb, yAb), Vector2(xBb, yBb), Vector2(xCb, yCb));
		B_Krits2[2] = GetKrit2(img, Vector2(xCb, yCb), Vector2(xDb, yDb), Vector2(xAb, yAb), Vector2(xBb, yBb));
		B_Krits2[3] = GetKrit2(img, Vector2(xBb, yBb), Vector2(xCb, yCb), Vector2(xDb, yDb), Vector2(xAb, yAb));

		float x = (xAb + xBb + xCb + xDb) / 4.0, y = (yAb + yBb + yCb + yDb) / 4.0;
		if ((abs(x - immaxX / 2.0) > (immaxX * 0.1)) || (abs(y - immaxY / 2.0) > (immaxY * 0.12)))  NotCenteredSheet = 1;

		if (KadrNumber == 1) if (SPF[0] == 0) SayNotCenteredFoot = 1;
		float K1 = MAX_4(B_Krits[0], B_Krits[1], B_Krits[2], B_Krits[3]);
		float K2 = MIN_4(MAX_3(B_Krits[0], B_Krits[1], B_Krits[2]), MAX_3(B_Krits[0], B_Krits[1], B_Krits[3]), MAX_3(B_Krits[0], B_Krits[3], B_Krits[2]), MAX_3(B_Krits[3], B_Krits[1], B_Krits[2]));
		float K3 = MAX_4(MIN_3(B_Krits[0], B_Krits[1], B_Krits[2]), MIN_3(B_Krits[0], B_Krits[1], B_Krits[3]), MIN_3(B_Krits[0], B_Krits[3], B_Krits[2]), MIN_3(B_Krits[3], B_Krits[1], B_Krits[2]));
		float K4 = MIN_4(B_Krits[0], B_Krits[1], B_Krits[2], B_Krits[3]);
		float K5 = (B_Krits[0] + B_Krits[1] + B_Krits[2] + B_Krits[3]) / 4.0;

		float K21 = MAX_4(B_Krits2[0], B_Krits2[1], B_Krits2[2], B_Krits2[3]);
		float K22 = MIN_4(MAX_3(B_Krits2[0], B_Krits2[1], B_Krits2[2]), MAX_3(B_Krits2[0], B_Krits2[1], B_Krits2[3]), MAX_3(B_Krits2[0], B_Krits2[3], B_Krits2[2]), MAX_3(B_Krits2[3], B_Krits2[1], B_Krits2[2]));
		float K23 = MAX_4(MIN_3(B_Krits2[0], B_Krits2[1], B_Krits2[2]), MIN_3(B_Krits2[0], B_Krits2[1], B_Krits2[3]), MIN_3(B_Krits2[0], B_Krits2[3], B_Krits2[2]), MIN_3(B_Krits2[3], B_Krits2[1], B_Krits2[2]));
		float K24 = MIN_4(B_Krits2[0], B_Krits2[1], B_Krits2[2], B_Krits2[3]);
		float K25 = (B_Krits2[0] + B_Krits2[1] + B_Krits2[2] + B_Krits2[3]) / 4.0;

		int32_t IsBlurred = ((K3 < 18 || K4 < 18 || K5 < 28 || K22 > 4.0) ? 1 : 0), IsProbBlurred = ((K3 < 40 || K4 < 32 || K5 < 42 || K22 > 3) ? 1 : 0);
		SheetFound = SPF[0];
		int32_t IsBadList = ((K4 < 15) ? 1 : 0);
		if (IsBlurred == 1) SheetFound = 0;
		if (NotCenteredSheet == 1) SheetFound = 0;
		if (IsBlurred == 1) SPF[0] = 0;
		if (NotCenteredSheet == 1) SPF[0] = 0;
		if (IsBlurred == 1) PF[0] = 0;
		if (NotCenteredSheet == 1) PF[0] = 0;
#pragma endregion

#pragma region Do poisk
		for (int32_t i = 0; i < RandomCount; i++)
			arandom[i] = (rand()) / double(RAND_MAX);

		double** cam = GetNewCamPos(img, immaxY, immaxX, lstat.xA, lstat.yA, lstat.xB, lstat.yB, lstat.xC, lstat.yC, lstat.xD, lstat.yD);


		for (int32_t i = 0; i < 30; i++)
			for (int32_t j = 0; j < 10; j++)
				out[i][j] = 0;

		out[0][3] = cam[0][3];
		for (int32_t i = 0; i < 8; i++)
			for (int32_t j = 0; j < 3; j++)
				out[i][j] = cam[i][j];
		out[8][0] = xAb; out[8][1] = yAb;
		out[9][0] = xBb; out[9][1] = yBb;
		out[10][0] = xCb; out[10][1] = yCb;
		out[11][0] = xDb; out[11][1] = yDb;


#pragma endregion

		// Определяем пространственные углы
		double AAlfa = atan(out[0][1] / out[0][0] + 1e-20) / PI * 180;
		if ((out[0][0] + 1e-20) < 0) AAlfa = AAlfa + 180;
		if ((out[0][0] + 1e-20) < 0 && (out[0][1] < 0)) AAlfa = AAlfa - 360;
		double r = sqrt(out[0][0] * out[0][0] + out[0][1] * out[0][1]);
		double ABeta = atan(out[0][2] / (r + 1e-20)) / PI * 180;
		Vector3 Z(out[3][0], out[3][1], out[3][2]), Y(0, 0, 1); Y = Z / Y;
		Y.SetLenght(1);
		Vector3 X(out[1][0], out[1][1], out[1][2]);
		X.SetLenght(1);
		r = X.GetLenght();
		r = Y.GetLenght();
		double AGamma = X * Y;
		if (AGamma < 1)
			AGamma = acos(AGamma) / PI * 180.0;
		else
			AGamma = 0;

		float CBeta = acos(abs(cam[3][2])) / PI * 180;

		if (CBeta < ABeta - 15) SayRotateDown = 0;
		if (CBeta > ABeta + 15) SayRotateUp = 0;

		for (int32_t i = 0; i < 8; i++)delete[] cam[i];
		delete[] cam;

		// Корректирую альфа, в зависимости от принятого номера кадра

		int32_t k = 0;
		int32_t Kadr;
		k = -UpdateOreint(img, Vector3(xAb, yAb, 0), Vector3(xBb, yBb, 0), Vector3(xCb, yCb, 0), Vector3(xDb, yDb, 0));
		if (AAlfa > 150 && KadrNumber != 1 && KadrNumber != 7) { k = 1; }
		else if (AAlfa < -150 && KadrNumber != 1 && KadrNumber != 7) { k = 1; }
		else if (AAlfa < 30 && AAlfa > -30 && (KadrNumber == 1 || KadrNumber == 7)) { k = 1; }
		if (k == 1)
		{
			r = xAb; xAb = xCb; xCb = r;
			r = yAb; yAb = yCb; yCb = r;
			r = xBb; xBb = xDb; xDb = r;
			r = yBb; yBb = yDb; yDb = r;
			out[0][0] *= -1; out[0][1] *= -1; out[1][0] *= -1; out[1][1] *= -1; out[2][0] *= -1; out[2][1] *= -1; out[3][0] *= -1; out[3][1] *= -1;
			if (AAlfa > 0)	AAlfa -= 180.0;
			else			AAlfa += 180.0;
		}
		if (AAlfa >= -180 && AAlfa < -95)
		{
			Kadr = 1;
		}
		else if (AAlfa < -65)
		{
			Kadr = 2;
		}
		else if (AAlfa < -30)
		{
			Kadr = 3;
		}
		else if (AAlfa < 30)
		{
			Kadr = 4;
		}
		else if (AAlfa < 65)
		{
			Kadr = 5;
		}
		else if (AAlfa < 95)
		{
			Kadr = 6;
		}
		else
		{
			Kadr = 7;
		}
		lstat.xA = xAb; lstat.yA = yAb;
		lstat.xB = xBb; lstat.yB = yBb;
		lstat.xC = xCb; lstat.yC = yCb;
		lstat.xD = xDb; lstat.yD = yDb;



		double q = 0;
		r = 1e20; if (xAb < r) r = xAb; if (xBb < r) r = xBb; if (xCb < r) r = xCb; if (xDb < r) r = xDb;
		q = -1e20; if (xAb > q) q = xAb; if (xBb > q) q = xBb; if (xCb > q) q = xCb; if (xDb > q) q = xDb;
		double fi = q - r;
		r = 1e20; if (yAb < r) r = yAb; if (yBb < r) r = yBb; if (yCb < r) r = yCb; if (yDb < r) r = yDb;
		q = -1e20; if (yAb > q) q = yAb; if (yBb > q) q = yBb; if (yCb > q) q = yCb; if (yDb > q) q = yDb;
		r = q - r;
		if (r > fi) fi = r;
		if (fi > 410 + 40) SayMoveFurther = 1;
		if (fi > 410 - 160) SayMoveCloser = 1;

		if ((KadrNumber == 2) || (KadrNumber == 6))
		{
			if (ABeta > 65) SayShiftDown = 1;
			if (ABeta < 30) SayShiftUp = 1;
		}

		if (AGamma > 30) SayPointCameraDown = 1;
		if (AGamma < -30) SayPointCameraDown = 1;

		r = 0;
		if (KadrNumber >= 1 && KadrNumber <= 7)
		{
			float A1, A2;
			switch (KadrNumber)
			{
			case 7: A1 = -180; A2 = -107; break;
			case 6: A1 = -107; A2 = -65; break;
			case 5: A1 = -65; A2 = -30; break;
			case 4: A1 = -30; A2 = 30; break;
			case 3: A1 = 30; A2 = 65; break;
			case 2: A1 = 65; A2 = 107; break;
			case 1: A1 = 107; A2 = 180; break;
			}
			if (AAlfa < A1) SayShiftLeft_IfR = 1;
			if (AAlfa > A2) SayShiftRight_IfR = 1;
			if (SayShiftLeft_IfR == 1) if (KadrNumber == 1 || KadrNumber == 7) { SayShiftLeftBehindHeel_IfR = 1; SayShiftLeft_IfR = 0; }
			if (SayShiftRight_IfR == 1) if (KadrNumber == 1 || KadrNumber == 7) { SayShiftRightBehindHeel_IfR = 1; SayShiftRight_IfR = 0; };
			r = 0;
			switch (KadrNumber)
			{
			case 1: A1 = -180; A2 = -107; break;
			case 2: A1 = -107; A2 = -65; break;
			case 3: A1 = -65; A2 = -30; break;
			case 4: A1 = -30; A2 = 30; break;
			case 5: A1 = 30; A2 = 65; break;
			case 6: A1 = 65; A2 = 107; break;
			case 7: A1 = 107; A2 = 180; break;
			}
			if (AAlfa < A1) SayShiftLeft_IfL = 1;
			if (AAlfa > A2) SayShiftRight_IfL = 1;
			if (SayShiftLeft_IfL == 1) if (KadrNumber == 1 || KadrNumber == 7) { SayShiftLeftBehindHeel_IfL = 1; SayShiftLeft_IfL = 0; }
			if (SayShiftRight_IfL == 1) if (KadrNumber == 1 || KadrNumber == 7) { SayShiftRightBehindHeel_IfL = 1; SayShiftRight_IfL = 0; };
		}

		if (IsBadList == 1)
		{
			SayPointCameraDown = 0;
			SayShiftRightBehindHeel_IfR = 0;
			SayShiftRightBehindHeel_IfL = 0;
			SayShiftLeftBehindHeel_IfR = 0;
			SayShiftLeftBehindHeel_IfL = 0;
			SayNotCenteredFoot = 0;
			NotCenteredSheet = 0;
			SayMoveFurther = 0;
			SayMoveCloser = 0;
			SayShiftDown = 0;
			SayShiftUp = 0;
			SayShiftLeft_IfL = 0;
			SayShiftLeft_IfR = 0;
			SayShiftRight_IfL = 0;
			SayShiftRight_IfR = 0;
			SayCrumpledSheet = 0;
			SayLowSock = 0;
			SayLowPants = 0;
			SayCurledFingers = 0;
			SayRotateDown = 0;
			SayRotateUp = 0;
		}

		float* utechka = new float;
		out[12][0] = K1;
		out[12][1] = K2;
		out[12][2] = K3;
		out[12][3] = K4;
		out[12][4] = K5;
		out[13][0] = IsBlurred;
		out[13][1] = IsProbBlurred;
		out[13][2] = IsBadList;
		out[14][0] = SheetFound;
		out[14][1] = SPF[0];
		out[14][2] = PF[0];
		out[15][0] = Kadr;
		out[16][0] = SayShiftLeft_IfL;
		out[16][1] = SayShiftRight_IfL;
		out[16][2] = SayShiftLeft_IfR;
		out[16][3] = SayShiftRight_IfR;
		out[16][4] = SayShiftDown;
		out[16][5] = SayShiftUp;
		out[16][6] = SayMoveCloser;
		out[16][7] = SayMoveFurther;
		out[16][8] = SayPointCameraDown;
		out[17][0] = K21;
		out[17][1] = K22;
		out[17][2] = K23;
		out[17][3] = K24;
		out[17][4] = K25;
		string Ret = "";
#ifdef __TEST__
		Ret = Ret
			+ "Blur " + to_string(IsBlurred) + ' '
			+ "PBlur " + to_string(IsProbBlurred) + ' '
			+ "Found " + to_string(SheetFound) + ' '
			+ "Bad " + to_string(IsBadList) + ' '
			+ "\nK1 " + to_string(B_Krits[0]) + ' '
			+ "K2 " + to_string(B_Krits[1]) + ' '
			+ "K3 " + to_string(B_Krits[2]) + ' '
			+ "K4 " + to_string(B_Krits[3]) + ' '
			+ "\nS1 " + to_string(B_Krits2[0]) + ' '
			+ "S2 " + to_string(B_Krits2[1]) + ' '
			+ "S3 " + to_string(B_Krits2[2]) + ' '
			+ "S4 " + to_string(B_Krits2[3]) + ' '
			+ "\nAlfa " + to_string(AAlfa)
			+ "Beta " + to_string(ABeta)
			+ "Gamma " + to_string(AGamma)
			+ "\nSayShiftLeft_IfL " + to_string(SayShiftLeft_IfL)
			+ ", SayShiftRight_IfL " + to_string(SayShiftRight_IfL)
			+ ", SayShiftLeft_IfR " + to_string(SayShiftLeft_IfR)
			+ ", SayShiftRight_IfR " + to_string(SayShiftRight_IfR)
			+ ", SayShiftLeftBehindHeel_IfL " + to_string(SayShiftLeftBehindHeel_IfL)
			+ ", SayShiftRightBehindHeel_IfL " + to_string(SayShiftRightBehindHeel_IfL)
			+ ", SayShiftLeftBehindHeel_IfR " + to_string(SayShiftLeftBehindHeel_IfR)
			+ ", SayShiftRightBehindHeel_IfR " + to_string(SayShiftRightBehindHeel_IfR)
			+ ", SayShiftDown " + to_string(SayShiftDown)
			+ ", SayShiftUp " + to_string(SayShiftUp)
			+ "\nOld SayTurnLeft " + to_string(0)
			+ ", SayTurnRight " + to_string(0)
			+ ", SayTurnDown " + to_string(0)
			+ ", SayTurnUp " + to_string(0)
			+ ", SayRotateClockwise " + to_string(0)
			+ ", SayRotateCounterClockwise " + to_string(0)
			+ ", SayMoveCloser " + to_string(SayMoveCloser)
			+ ", SayMoveFurther " + to_string(SayMoveFurther)
			+ ", SayPointCameraDown " + to_string(SayPointCameraDown)
			+ ", SayCrumpledSheet " + to_string(SayCrumpledSheet)
			+ ", SayNotCenteredFoot " + to_string(SayNotCenteredFoot)
			+ ", SayLowSock " + to_string(SayLowSock)
			+ ", SayLowPants " + to_string(SayLowPants)
			+ ", SayCurledFingers " + to_string(SayCurledFingers)
			+ "\nx1 " + to_string(xAb)
			+ ", y1 " + to_string(yAb)
			+ ", x2 " + to_string(xBb)
			+ ", y2 " + to_string(yBb)
			+ ", x3 " + to_string(xCb)
			+ ", y3 " + to_string(yCb)
			+ ", x4 " + to_string(xDb)
			+ ", y4 " + to_string(yDb)
			+ ", NotCenteredSheet " + to_string(NotCenteredSheet)
			// New Tags
			+ ", IsBlurred " + to_string(IsBlurred)
			+ ", IsProbBlurred " + to_string(IsProbBlurred)
			+ ", IsBadList " + to_string(IsBadList)
			// Old Tags

			// MY Tags
			+", KadrNumber " + to_string(KadrNumber)
			+ ", xK " + to_string(out[0][0])
			+ ", yK " + to_string(out[0][1])
			+ ", zK " + to_string(out[0][2])
			+ ", equivalentFocalLength " + to_string(out[0][3])
			+ ", KXx " + to_string(out[1][0])
			+ ", KXy " + to_string(out[1][1])
			+ ", KXz " + to_string(out[1][2])
			+ ", KYx " + to_string(out[2][0])
			+ ", KYy " + to_string(out[2][1])
			+ ", KYz " + to_string(out[2][2])
			+ ", KZx " + to_string(out[3][0])
			+ ", KZy " + to_string(out[3][1])
			+ ", KZz " + to_string(out[3][2])
			+ ", KritBorder1 " + to_string(B_Krits[0])
			+ ", KritBorder2 " + to_string(B_Krits[1])
			+ ", KritBorder3 " + to_string(B_Krits[2])
			+ ", KritBorder4 " + to_string(B_Krits[3])
			+ '\n';
#else
		Ret = Ret + "{"
			+ "\"x1\":" + to_string(xAb)
			+ ",\"y1\":" + to_string(yAb)
			+ ",\"x2\":" + to_string(xBb)
			+ ",\"y2\":" + to_string(yBb)
			+ ",\"x3\":" + to_string(xCb)
			+ ",\"y3\":" + to_string(yCb)
			+ ",\"x4\":" + to_string(xDb)
			+ ",\"y4\":" + to_string(yDb)
			+ ",\"SheetFound\":" + to_string(SheetFound)
			+ ",\"SheetProbablyFound\":" + to_string(SPF[0])
			+ ",\"SheetFoundProbability\":" + to_string(PF[0])
			+ ",\"Alfa\":" + to_string(AAlfa)
			+ ",\"Beta\":" + to_string(ABeta)
			+ ",\"Gamma\":" + to_string(AGamma)
			+ ",\"SayShiftLeft_IfL\":" + to_string(SayShiftLeft_IfL)
			+ ",\"SayShiftRight_IfL\":" + to_string(SayShiftRight_IfL)
			+ ",\"SayShiftLeft_IfR\":" + to_string(SayShiftLeft_IfR)
			+ ",\"SayShiftRight_IfR\":" + to_string(SayShiftRight_IfR)
			+ ",\"SayShiftLeftBehindHeel_IfL\":" + to_string(SayShiftLeftBehindHeel_IfL)
			+ ",\"SayShiftRightBehindHeel_IfL\":" + to_string(SayShiftRightBehindHeel_IfL)
			+ ",\"SayShiftLeftBehindHeel_IfR\":" + to_string(SayShiftLeftBehindHeel_IfR)
			+ ",\"SayShiftRightBehindHeel_IfR\":" + to_string(SayShiftRightBehindHeel_IfR)
			+ ",\"SayShiftDown\":" + to_string(SayShiftDown)
			+ ",\"SayShiftUp\":" + to_string(SayShiftUp)
			+ ",\"SayRotateDown\":" + to_string(SayRotateDown)
			+ ",\"SayRotateUp\":" + to_string(SayRotateUp)
			+ ",\"SayMoveCloser\":" + to_string(SayMoveCloser)
			+ ",\"SayMoveFurther\":" + to_string(SayMoveFurther)
			+ ",\"SayPointCameraDown\":" + to_string(SayPointCameraDown)
			+ ",\"NotCenteredSheet\":" + to_string(NotCenteredSheet)
			+ ",\"SayCrumpledSheet\":" + to_string(SayCrumpledSheet)
			+ ",\"SayNotCenteredFoot\":" + to_string(SayNotCenteredFoot)
			+ ",\"SayLowSock\":" + to_string(SayLowSock)
			+ ",\"SayLowPants\":" + to_string(SayLowPants)
			+ ",\"SayCurledFingers\":" + to_string(SayCurledFingers)
			// New Tags
			+ ",\"IsBlurred\":" + to_string(IsBlurred)
			+ ",\"IsProbBlurred\":" + to_string(IsProbBlurred)
			+ ",\"IsBadList\":" + to_string(IsBadList)
			// Old Tags
			+ ",\"SayTurnLeft\":" + to_string(0)
			+ ",\"SayTurnRight\":" + to_string(0)
			+ ",\"SayTurnDown\":" + to_string(0)
			+ ",\"SayTurnUp\":" + to_string(0)
			+ ",\"SayRotateClockwise\":" + to_string(0)
			+ ",\"SayRotateCounterClockwise\":" + to_string(0)
			// MY Tags
			+ ",\"KadrNumber\":" + to_string(KadrNumber)
			+ ",\"xK\":" + to_string(out[0][0])
			+ ",\"yK\":" + to_string(out[0][1])
			+ ",\"zK\":" + to_string(out[0][2])
			+ ",\"equivalentFocalLength\":" + to_string(out[0][3])
			+ ",\"KXx\":" + to_string(out[1][0])
			+ ",\"KXy\":" + to_string(out[1][1])
			+ ",\"KXz\":" + to_string(out[1][2])
			+ ",\"KYx\":" + to_string(out[2][0])
			+ ",\"KYy\":" + to_string(out[2][1])
			+ ",\"KYz\":" + to_string(out[2][2])
			+ ",\"KZx\":" + to_string(out[3][0])
			+ ",\"KZy\":" + to_string(out[3][1])
			+ ",\"KZz\":" + to_string(out[3][2])
			+ ",\"KritBorder1\":" + to_string(B_Krits[0])
			+ ",\"KritBorder2\":" + to_string(B_Krits[1])
			+ ",\"KritBorder3\":" + to_string(B_Krits[2])
			+ ",\"KritBorder4\":" + to_string(B_Krits[3])
			+ "}";
#endif // __TEST__	

		delete[] B_Krits;
		delete utechka;
		{
			for (int32_t i = 0; i < 30; i++)
			{
				if (inout != 0)
					for (int32_t j = 0; j < 10; j++) inout[i][j] = out[i][j];
			}
		}

		return Ret;
	};

	void SetOut(double** _out)
	{
		inout = _out;
	}
}

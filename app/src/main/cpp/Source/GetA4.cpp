//
//	Version 1.1.4_A
//
//	Date 14.07
//

#include "../Include/FindA4.hpp"

namespace fa4
{
	//BruteScan(12000, 350, cstat, list, KritBB, SheetFound, PF, SPF);
	void Finder::BruteScan(int32_t itercount, int32_t sublenght, float* outstats, float* lstat, double & Kritbb, int32_t & SheetFound, int32_t & SheetFoundProbability, int32_t & SheetProbablyFound)
	{
		int32_t orienta, SF[20], SPF[20], PF[20];
		double KritBB[20];
		float cstat2[20][7];
		float lstat2[20][8];

		std::thread threads[20];


		if(maxthreadcount > 20) maxthreadcount = 20;
		if(maxthreadcount < 1) maxthreadcount = 1;

		for(int32_t i = 0; i < maxthreadcount - 1; i++)
		{
			threads[i] = std::thread{&Finder::th_BruteScan, this, itercount / maxthreadcount + 1, sublenght, cstat2[i + 1], lstat2[i + 1], std::ref(KritBB[i + 1]), std::ref(SF[i + 1]), std::ref(PF[i + 1]), std::ref(SPF[i + 1]), i + 1};

		}
		th_BruteScan(itercount / maxthreadcount + 1, sublenght, cstat2[0], lstat2[0], KritBB[0], SF[0], PF[0], SPF[0], 0);

		for(int32_t i = 0; i < maxthreadcount - 1; i++)
		{
			threads[i].join();
		}

		for(int32_t i = 1; i < maxthreadcount; i++) if(KritBB[0] < KritBB[i])
		{
			SF[0] = SF[i];
			SPF[0] = SPF[i];
			PF[0] = PF[i];
			for(int j = 0; j < 8; j++)
				lstat2[0][j] = lstat2[i][j];
			for (int j = 0; j < 7; j++)
				cstat2[0][j] = cstat2[i][j];
			KritBB[0] = KritBB[i];
		}
		for (int j = 0; j < 8; j++)
			lstat[j] = lstat2[0][j];
		for (int j = 0; j < 7; j++)
			outstats[j] = cstat2[0][j];
		SheetFound = SF[0];
		SheetFoundProbability = PF[0];
		SheetProbablyFound = SPF[0];
	}

	void Finder::th_BruteScan(int32_t itercount, int32_t sublenght, float* outstats,float* lstat, double & Kritbb, int32_t & SheetFound, int32_t & SheetFoundProbability, int32_t & SheetProbablyFound, int32_t thnum)
	{
#pragma region Vars
		//srand(Get(immaxY / 13][immaxX / 11] + thnum + 1);
		double G11i = -550, G12i = 550;
		double G21i = -550, G22i = 550;
		double G31i = 350, G32i = 700;
		double G41i = -100, G42i = 100;
		double G51i = -140, G52i = 140;
		double G61i = 500, G62i = 900;
		double G71i = (-180 - 20) / 180.0*PI, G72i = (-180 + 20) / 180.0*PI;
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

		float Line[50][7];

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
		for(int32_t u = 1, totu = 0, breaked = 0; u < iterc; u++, totu++)
		{
			if(totu > iterc * 2) u += 2;
			if(count >= sublenght)
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

				xKb = G11 + (G12 - G11)*0.5;
				yKb = G21 + (G22 - G21)*0.5;
				zKb = G31 + (G32 - G31)*0.5;
				zKb = G31 + (G32 - G31)*0.5;
				xLb = G41 + (G42 - G41)*0.5;
				yLb = G51 + (G52 - G51)*0.5;
				zumb = G61 + (G62 - G61)*0.5;
				fiKb = G71 + (G72 - G71)*0.5;
			}
			count = count + 1;

			double r = 0.6;
			if(random < r) { G11g = G11; G12g = G12; }
			else { G11g = xKb; G12g = xKb; };
			if(random < r) { G21g = G21; G22g = G22; }
			else { G21g = yKb; G22g = yKb; };
			if(random < r) { G31g = G31; G32g = G32; }
			else { G31g = zKb; G32g = zKb; };
			if(random < r) { G41g = G41; G42g = G42; }
			else { G41g = xLb; G42g = xLb; };
			if(random < r) { G51g = G51; G52g = G52; }
			else { G51g = yLb; G52g = yLb; };
			if(random < r) { G61g = G61; G62g = G62; }
			else { G61g = zumb; G62g = zumb; };
			if(random < r) { G71g = G71; G72g = G72; }
			else { G71g = fiKb; G72g = fiKb; };

			//RegenCameraStats(&select, minStatsGen, maxStatsGen);

			xK = G11g + (G12g - G11g)*random;
			yK = G21g + (G22g - G21g)*random;
			zK = G31g + (G32g - G31g)*random;
			xL = G41g + (G42g - G41g)*random;
			yL = G51g + (G52g - G51g)*random;
			zum = G61g + (G62g - G61g)*random;
			fiK = G71g + (G72g - G71g)*random;

			if(u % 2 == 0)
			{
				r = tr[int32_t(max(1, min((count * 1998) / sublenght, 1998)))];

				if(xKb < ((G11 + G12) / 2.0)) G12 = G11 + r * DI1; else G11 = G12 - r * DI1;
				if(yKb < ((G21 + G22) / 2.0)) G22 = G21 + r * DI2; else G21 = G22 - r * DI2;
				if(zKb < ((G31 + G32) / 2.0)) G32 = G31 + r * DI3; else G31 = G32 - r * DI3;
				if(xLb < ((G41 + G42) / 2.0)) G42 = G41 + r * DI4; else G41 = G42 - r * DI4;
				if(yLb < ((G51 + G52) / 2.0)) G52 = G51 + r * DI5; else G51 = G52 - r * DI5;
				if(zumb < ((G61 + G62) / 2.0)) G62 = G61 + r * DI6; else G61 = G62 - r * DI6;
				if(fiKb < ((G71 + G72) / 2.0)) G72 = G71 + r * DI7; else G71 = G72 - r * DI7;

			}

			r = xK * xK + yK * yK + zK * zK;
			if(r > 800 * 800)
			{
				u--;
				count--;
				continue;
			}

			r = sqrt(xK * xK + yK * yK);
			if(r < 350)
			{
				u--;
				count--;
				continue;
			}
			if(KadrNumber > 0 && KadrNumber <= 7 && useangel == 1)
			{
				float dx = xK / r;
				float dy = yK / r;
				float ax, ay;
				switch(Right)
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
				switch(Right)
				{
					case 1:
					case 0:
						if(abs(dx * ax + dy * ay) < 0.71)
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
						if(abs(dx * ax + dy * ay) < 0.71 && abs(dx * bx + dy * by) < 0.71)
						{
							u--;
							count--;
							continue;
						}
						break;
				}
			}
			double PLA = xL - xK, PLB = yL - yK, PLC = -zK;
			if(zum < 0) zum *= -1;
			if(zum < G61i) zum = zumb;
			if(zum > G62i) zum = zumb;
			r = sqrt((PLA * PLA) + (PLB * PLB) + (PLC * PLC)) + 1e-20;
			double fi = zum / r;
			double PLD = -(xL - xK)*(xK + fi * (xL - xK)) - (yL - yK)*(yK + fi * (yL - yK)) + zK * (zK - fi * zK);

			// Находим точки-проекции углов листа A4
			double q = -PLD - PLA * xK - PLB * yK - PLC * zK;
			fi = PLC * (-zK) + 1e-20;
			r = q / (PLA*(xL - xK) + PLB * (yL - yK) + fi);
			double xML = xK + (xL - xK)*r;
			double yML = yK + (yL - yK)*r;
			double zML = zK + (0 - zK)*r;
			r = q / (PLA*(xPA - xK) + PLB * (yPA - yK) + fi);
			double xMA = xK + (xPA - xK)*r;
			double yMA = yK + (yPA - yK)*r;
			double zMA = zK + (0 - zK)*r;
			r = q / (PLA*(xPB - xK) + PLB * (yPB - yK) + fi);
			double xMB = xK + (xPB - xK)*r;
			double yMB = yK + (yPB - yK)*r;
			double zMB = zK + (0 - zK)*r;
			r = q / (PLA*(xPC - xK) + PLB * (yPC - yK) + fi);
			double xMC = xK + (xPC - xK)*r;
			double yMC = yK + (yPC - yK)*r;
			double zMC = zK + (0 - zK)*r;
			r = q / (PLA*(xPD - xK) + PLB * (yPD - yK) + fi);
			double xMD = xK + (xPD - xK)*r;
			double yMD = yK + (yPD - yK)*r;
			double zMD = zK + (0 - zK)*r;
			// Делаем вектора осей на плоскости-экране
			double v1x = yL - yK, v1y = xK - xL, v1z = 0;
			double v2x = (xK - xL)*(0 - zK), v2y = -(yL - yK)*(0 - zK), v2z = (yL - yK)*(yL - yK) + (xL - xK)*(xL - xK);
			// Находим проекции всех точек
			double v1 = sqrt(v1x*v1x + v1y * v1y + v1z * v1z);
			double v2 = sqrt(v2x*v2x + v2y * v2y + v2z * v2z);
			double xVA = ((xMA - xML)*v1x + (yMA - yML)*v1y + (zMA - zML)*v1z) / v1;
			double yVA = ((xMA - xML)*v2x + (yMA - yML)*v2y + (zMA - zML)*v2z) / v2;
			double xVB = ((xMB - xML)*v1x + (yMB - yML)*v1y + (zMB - zML)*v1z) / v1;
			double yVB = ((xMB - xML)*v2x + (yMB - yML)*v2y + (zMB - zML)*v2z) / v2;
			double xVC = ((xMC - xML)*v1x + (yMC - yML)*v1y + (zMC - zML)*v1z) / v1;
			double yVC = ((xMC - xML)*v2x + (yMC - yML)*v2y + (zMC - zML)*v2z) / v2;
			double xVD = ((xMD - xML)*v1x + (yMD - yML)*v1y + (zMD - zML)*v1z) / v1;
			double yVD = ((xMD - xML)*v2x + (yMD - yML)*v2y + (zMD - zML)*v2z) / v2;

			// Здесь нужно сделать пересчет точек на пиксели кадра
			double xA = xVC + immaxX / 2.0, yA = -yVC + immaxY / 2.0;
			double xB = xVD + immaxX / 2.0, yB = -yVD + immaxY / 2.0;
			double xC = xVA + immaxX / 2.0, yC = -yVA + immaxY / 2.0;
			double xD = xVB + immaxX / 2.0, yD = -yVB + immaxY / 2.0;
			// Поворот через табулированные синус и косинус
			if(abs(fiK) > PI) fiK = fiKb;
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
			if((xr < 1 + 18) || (xr >= immaxX - 20) || (yr < 1 + 18) || (yr >= immaxY - 20))
			{
				u--;
				count--;
				continue;
			}
			if((Get(yr,xr) > 85) &&
				(Get(yr + 16,xr) > 85) &&
			   (Get(yr - 16,xr) > 85) &&
			   (Get(yr,xr + 16) > 85) &&
			   (Get(yr,xr - 16) > 85))
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

			double rAB = sqrt((xA - xB)*(xA - xB) + (yA - yB)*(yA - yB)) + 1e-20;
			double rBC = sqrt((xB - xC)*(xB - xC) + (yB - yC)*(yB - yC)) + 1e-20;
			double rCD = sqrt((xC - xD)*(xC - xD) + (yC - yD)*(yC - yD)) + 1e-20;
			double rAD = sqrt((xA - xD)*(xA - xD) + (yA - yD)*(yA - yD)) + 1e-20;


			double xA1, yA1, xB1, yB1;
			int32_t xT, yT;
			for(int32_t i = 1; i <= NR; i++) Line[i][3] = 0;
			for(int32_t j = 0; j < 7; j++) if(j != 3)
			{
				xA1 = xA + dol[j] * (xD - xA) / rAD;
				yA1 = yA + dol[j] * (yD - yA) / rAD;
				xB1 = xB + dol[j] * (xC - xB) / rBC;
				yB1 = yB + dol[j] * (yC - yB) / rBC;
				for(int32_t i = 1; i <= NR; i++)
				{
					xT = xA1 + (xB1 - xA1) / NR * (i - 0.5);
					yT = yA1 + (yB1 - yA1) / NR * (i - 0.5);
					if((xT < 0) || (xT > immaxX - 1) || (yT < 0) || (yT > immaxY - 1)) Line[i][3] = -1;
					else
					{
						Line[i][j] = Get(yT,xT);
					}
				}
			}
			Krit1 = 0; Krit1e = 0;
			for(int32_t i = 1; i <= NR; i++) if(Line[i][3] != -1)
			{
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if(r > tr3)
				{
					Krit1 = Krit1 + w3;
					if(abs(i - NR / 2.0) >= (NR*0.25))  Krit1e = Krit1e + w3;
				}
				r = ((Line[i][4] + Line[i][5]) / 2.0 - (Line[i][2] + Line[i][1]) / 2.0);
				if(r > tr2)
				{
					Krit1 = Krit1 + w2;
					if(abs(i - NR / 2.0) >= (NR*0.25))  Krit1e = Krit1e + w2;
				}
				r = Line[i][4] - Line[i][2];
				if(r > tr)
				{
					Krit1 = Krit1 + w1;
					if(abs(i - NR / 2.0) >= (NR*0.25))  Krit1e = Krit1e + w1;
				}
			}

			for(int32_t i = 1; i <= NR; i++) Line[i][3] = 0;
			for(int32_t j = 0; j < 7; j++) if(j != 3)
			{
				xA1 = xB + dol[j] * (xA - xB) / rAB;
				yA1 = yB + dol[j] * (yA - yB) / rAB;
				xB1 = xC + dol[j] * (xD - xC) / rCD;
				yB1 = yC + dol[j] * (yD - yC) / rCD;
				for(int32_t i = 1; i < NR; i++)
				{
					xT = xA1 + (xB1 - xA1) / NR * (i - 0.5); yT = yA1 + (yB1 - yA1) / NR * (i - 0.5);
					if((xT < 0) || (xT > immaxX - 1) || (yT < 0) || (yT > immaxY - 1)) Line[i][3] = -1;
					else
					{
						Line[i][j] = Get(yT,xT);
					}
				}
			}
			Krit2 = 0; Krit2e = 0;
			for(int32_t i = 1; i <= NR; i++) if(Line[i][3] != -1)
			{
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if(r > tr3)
				{
					Krit2 = Krit2 + w3;
					if(abs(i - NR / 2.0) >= (NR*0.25))  Krit2e = Krit2e + w3;
				}
				r = ((Line[i][4] + Line[i][5]) / 2.0 - (Line[i][2] + Line[i][1]) / 2.0);
				if(r > tr2)
				{
					Krit2 = Krit2 + w2;
					if(abs(i - NR / 2.0) >= (NR*0.25))  Krit2e = Krit2e + w2;
				}
				r = Line[i][4] - Line[i][2];
				if(r > tr)
				{
					Krit2 = Krit2 + w1;
					if(abs(i - NR / 2.0) >= (NR*0.25))  Krit2e = Krit2e + w1;
				}
			}

			for(int32_t i = 1; i <= NR; i++) Line[i][3] = 0;
			for(int32_t j = 0; j < 7; j++) if(j != 3)
			{
				xA1 = xD + dol[j] * (xA - xD) / rAD;
				yA1 = yD + dol[j] * (yA - yD) / rAD;
				xB1 = xC + dol[j] * (xB - xC) / rBC;
				yB1 = yC + dol[j] * (yB - yC) / rBC;
				for(int32_t i = 1; i <= NR; i++)
				{
					xT = xA1 + (xB1 - xA1) / NR * (i - 0.5); yT = yA1 + (yB1 - yA1) / NR * (i - 0.5);
					if((xT < 0) || (xT > immaxX - 1) || (yT < 0) || (yT > immaxY - 1)) Line[i][3] = -1;
					else
					{
						Line[i][j] = Get(yT,xT);
					}
				}
			}
			Krit3 = 0; Krit3e = 0;
			for(int32_t i = 1; i <= NR; i++) if(Line[i][3] != -1)
			{
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if(r > tr3)
				{
					Krit3 = Krit3 + w3;
					if(abs(i - NR / 2.0) >= (NR*0.25))  Krit3e = Krit3e + w3;
				}
				r = ((Line[i][4] + Line[i][5]) / 2.0 - (Line[i][2] + Line[i][1]) / 2.0);
				if(r > tr2)
				{
					Krit3 = Krit3 + w2;
					if(abs(i - NR / 2.0) >= (NR*0.25))  Krit3e = Krit3e + w2;
				}
				r = Line[i][4] - Line[i][2];
				if(r > tr)
				{
					Krit3 = Krit3 + w1;
					if(abs(i - NR / 2.0) >= (NR*0.25))  Krit3e = Krit3e + w1;
				}
			}

			for(int32_t i = 1; i <= NR; i++) Line[i][3] = 0;
			for(int32_t j = 0; j < 7; j++) if(j != 3)
			{
				xA1 = xA + dol[j] * (xB - xA) / rAB;
				yA1 = yA + dol[j] * (yB - yA) / rAB;
				xB1 = xD + dol[j] * (xC - xD) / rCD;
				yB1 = yD + dol[j] * (yC - yD) / rCD;
				for(int32_t i = 1; i <= NR; i++)
				{
					xT = xA1 + (xB1 - xA1) / NR * (i - 0.5); yT = yA1 + (yB1 - yA1) / NR * (i - 0.5);
					if((xT < 0) || (xT > immaxX - 1) || (yT < 0) || (yT > immaxY - 1)) Line[i][3] = -1;
					else
					{
						Line[i][j] = Get(yT,xT);
					}
				}
			}
			Krit4 = 0; Krit4e = 0;
			for(int32_t i = 1; i <= NR; i++) if(Line[i][3] != -1)
			{
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if(r > tr3)
				{
					Krit4 = Krit4 + w3;
					if(abs(i - NR / 2.0) >= (NR*0.25))  Krit4e = Krit4e + w3;
				}
				r = ((Line[i][4] + Line[i][5]) / 2.0 - (Line[i][2] + Line[i][1]) / 2.0);
				if(r > tr2)
				{
					Krit4 = Krit4 + w2;
					if(abs(i - NR / 2.0) >= (NR*0.25))  Krit4e = Krit4e + w2;
				}
				r = Line[i][4] - Line[i][2];
				if(r > tr)
				{
					Krit4 = Krit4 + w1;
					if(abs(i - NR / 2.0) >= (NR*0.25))  Krit4e = Krit4e + w1;
				}
			}

			Krit = KritFunc(Krit1) + KritFunc(Krit2) + KritFunc(Krit3) + KritFunc(Krit4);

			if(Krit > Kritb || Kritb < -1e10)
			{
				Kritb = Krit;
				Krit1b = Krit1; Krit2b = Krit2; Krit3b = Krit3; Krit4b = Krit4;
				Krit1eb = Krit1e; Krit2eb = Krit2e; Krit3eb = Krit3e; Krit4eb = Krit4e;
				xKb = xK; yKb = yK; zKb = zK; xLb = xL; yLb = yL; zumb = zum; fiKb = fiK;
				xAb = xA; yAb = yA; xBb = xB; yBb = yB; xCb = xC; yCb = yC; xDb = xD; yDb = yD;
			}

			if(Kritb > Kritbb || Kritbb < -1e10)
			{
				Kritbb = Kritb;
				Krit1bb = Krit1b; Krit2bb = Krit2b; Krit3bb = Krit3b; Krit4bb = Krit4b;
				Krit1ebb = Krit1eb; Krit2ebb = Krit2eb; Krit3ebb = Krit3eb; Krit4ebb = Krit4eb;
				xKbb = xKb; yKbb = yKb; zKbb = zKb; xLbb = xLb; yLbb = yLb; zumbb = zumb; fiKbb = fiKb;
				xAbb = xAb; yAbb = yAb; xBbb = xBb; yBbb = yBb; xCbb = xCb; yCbb = yCb; xDbb = xDb; yDbb = yDb;
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
		for(int32_t i = 1; i < 4; i++) if(Sort[i] > Sort[0]) { r = Sort[0]; Sort[0] = Sort[i]; Sort[i] = r; }
		for(int32_t i = 2; i < 4; i++) if(Sort[i] > Sort[1]) { r = Sort[1]; Sort[1] = Sort[i]; Sort[i] = r; }
		if(Sort[3] > Sort[2]) { r = Sort[1]; Sort[1] = Sort[2]; Sort[2] = r; }

		fi = Sort[3];

		Sort[0] = Krit1ebb;
		Sort[1] = Krit2ebb;
		Sort[2] = Krit3ebb;
		Sort[3] = Krit4ebb;
		for(int32_t i = 1; i < 4; i++) if(Sort[i] > Sort[0]) { r = Sort[0]; Sort[0] = Sort[i]; Sort[i] = r; }
		for(int32_t i = 2; i < 4; i++) if(Sort[i] > Sort[1]) { r = Sort[1]; Sort[1] = Sort[i]; Sort[i] = r; }
		if(Sort[3] > Sort[2]) { r = Sort[1]; Sort[1] = Sort[2]; Sort[2] = r; }

		if(Sort[3] < 5.5) SheetFound = 0; else SheetFound = 1;
		if(Sort[3] < 3.75) SheetProbablyFound = 0; else; SheetProbablyFound = 1;
		if(Kritbb > 4.403) SheetProbablyFound = 1;
		SheetFoundProbability = round((Sort[3] - 2.5) / 5.0 * 100); if(SheetFoundProbability < 0)  SheetFoundProbability = 0; if(SheetFoundProbability > 100) SheetFoundProbability = 100;

		outstats[0] = (float)xKbb;
		outstats[1] = (float)yKbb;
		outstats[2] = (float)zKbb;
		outstats[3] = (float)xLbb;
		outstats[4] = (float)yLbb;
		outstats[5] = (float)zumbb;
		outstats[6] = (float)fiKbb;

		lstat [0] = (float)xAb;
		lstat[1] = (float)yAb;
		lstat[2] = (float)xBb;
		lstat[3] = (float)yBb;
		lstat[4] = (float)xCb;
		lstat[5] = (float)yCb;
		lstat[6] = (float)xDb;
		lstat[7] = (float)yDb;

	};
}

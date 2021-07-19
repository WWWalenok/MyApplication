//
//	Version 1.1.4_A
//
//	Date 14.07
//

#include "../Include/FindA4.hpp"

namespace fa4
{
	void Finder::MakeBest(float* lstat, double disp, float* OKrits)
	{
#pragma region Vars

		float Krits[4];

		double dol[7];
		bool ResetNewPos = false;
		float DD = disp / 0.2 * DK;

		ResetNewPos = false;
		int32_t count = 0;
		double Kritbb = -1e20, Kritb = Kritbb, Krit, Okritb, Okrit;

		double xAb = lstat[0], yAb = lstat[1], xBb = lstat[2], yBb = lstat[3], xCb = lstat[4], yCb = lstat[5], xDb = lstat[6], yDb = lstat[7];
		double xAbb = lstat[0], yAbb = lstat[1], xBbb = lstat[2], yBbb = lstat[3], xCbb = lstat[4], yCbb = lstat[5], xDbb = lstat[6], yDbb = lstat[7];

		int32_t o1 = 400, o2 = o1 / 20;
		dol[4] = max(0.71 * DD, 0.71); dol[5] = max(10.224 * DD, 0.71 * 3);  dol[6] = max(18.744 * DD, 0.71 * 5);
		dol[3] = 0;
		dol[2] = -dol[4]; dol[1] = -dol[5]; dol[0] = -dol[6];
		int32_t tr = 18;
		int32_t NR = 35;
		double w1 = 1, w2 = 0.5, w3 = 0.25;
		double xA = xAb, yA = yAb, xB = xBb, yB = yBb, xC = xCb, yC = yCb, xD = xDb, yD = yDb, xBsb, yBsb, xCsb, yCsb, rBC, rAD;
		double r; double fi;

		float Line[50][7];

#pragma endregion

#pragma region
		Kritb = -10e20;
		float G11 = -disp; float  G12 = disp;
		float G21 = -disp; float  G22 = disp;

		double sA = 0, sB = 0, sC = 0, sD = 0, sAb, sBb, sCb, sDb, rAB, rCD, xB1, yB1, xC1, yC1, xAsb2, yAsb2, xDsb2, yDsb2, xBsb2, yBsb2;
		int32_t xT, yT, dX, dY;

		for(int32_t o = 1; o < o1; o++)
		{
			sB = G11 + (G12 - G11)*random;
			sC = G21 + (G22 - G21)*random;
			if(o % o2 == 0)
			{
				r = 0.93;
				if(sBb < 0.5*(G11 + G12)) G12 = G11 + r * (G12 - G11); else G11 = G12 - r * (G12 - G11);
				if(sCb < 0.5*(G21 + G22)) G22 = G21 + r * (G22 - G21); else G21 = G22 - r * (G22 - G21);
			}

			xB = xBbb + sB * (xBbb - xAbb); yB = yBbb + sB * (yBbb - yAbb);
			xC = xCbb + sC * (xCbb - xDbb); yC = yCbb + sC * (yCbb - yDbb);
			// —читаем критерий

			rAB = sqrt((xAbb - xB)*(xAbb - xB) + (yAbb - yB)*(yAbb - yB)) + 1e-20;
			rCD = sqrt((xC - xDbb)*(xC - xDbb) + (yC - yDbb)*(yC - yDbb)) + 1e-20;
			for(int32_t i = 0; i < NR; i++) Line[i][3] = 0;
			for(int32_t g = 0; g < 7; g++) if(g != 3)
			{
				xB1 = xB + dol[g] * (xA - xB) / rAB;
				yB1 = yB + dol[g] * (yA - yB) / rAB;
				xC1 = xC + dol[g] * (xD - xC) / rCD;
				yC1 = yC + dol[g] * (yD - yC) / rCD;
				for(int32_t i = 2; i < NR - 2; i++)
				{
					xT = xB1 + (xC1 - xB1) / NR * (i - 0.5);
					yT = yB1 + (yC1 - yB1) / NR * (i - 0.5);
					//dX = xT - int32_t(xT); dY = yT - int32_t(yT);
					if(((xT < 1.5) || (xT > immaxX - 2.5) || (yT < 1.5) || (yT > immaxY - 2.5)))
						Line[i][3] = -1; else Line[i][g] = Get(yT,xT);
				}
			}
			Krit = -0;
			double q = 0, u = 0, n = 0;
			for(int32_t i = 2; i < NR - 2; i++) if(Line[i][3] != -1)
			{
				fi = 0;
				n++;
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if(r > tr)
					Krit = Krit + w3;
				r = ((Line[i][4] + Line[i][5]) - (Line[i][2] + Line[i][1])) / 2.0;
				if(r > tr)
					Krit = Krit + w2;
				r = Line[i][4] - Line[i][2];
				if(r > tr)
					Krit = Krit + w1;
				//  оличество превышений интенсивности должно быть не менее 35 %
				r = ((Line[i][5] + Line[i][6]) / 2.0 - (Line[i][1] + Line[i][0]) / 2.0); if(r > tr) q = q + 1;
				r = ((Line[i][5] + Line[i][6] + Line[i][1] + Line[i][0]) / 4.0); if(r < 40) u = u + 1;
			}
			if(q < (n) * 0.35) Krit = Krit * 0.7;
			Okrit = Krit;
			if(u < (n) * 0.7) Okrit = Krit * n / (n - u);
			if(u < (n) * 0.4) Krit = Krit * n / (n - u);

			if(Krit > Kritb)
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

		for(int32_t o = 1; o < o1; o++)
		{
			sA = G11 + (G12 - G11)*random;
			sD = G21 + (G22 - G21)*random;
			if(o % o2 == 0)
			{

				r = 0.93;
				if(sAb < 0.5*(G11 + G12)) G12 = G11 + r * (G12 - G11); else G11 = G12 - r * (G12 - G11);
				if(sDb < 0.5*(G21 + G22)) G22 = G21 + r * (G22 - G21); else G21 = G22 - r * (G22 - G21);
			}

			xA = xAbb + sA * (xAbb - xB); yA = yAbb + sA * (yAbb - yB);
			xD = xDbb + sD * (xDbb - xC); yD = yDbb + sD * (yCbb - yC);
			// —читаем критерий
			rAB = sqrt((xA - xBsb)*(xA - xBsb) + (yA - yBsb)*(yA - yBsb)) + 1e-20;
			rCD = sqrt((xCsb - xD)*(xCsb - xD) + (yCsb - yD)*(yCsb - yD)) + 1e-20;
			for(int32_t i = 0; i < NR; i++) Line[i][3] = 0;
			for(int32_t g = 0; g < 7; g++) if(g != 3)
			{
				xB1 = xA + dol[g] * (xB - xA) / rAB;
				yB1 = yA + dol[g] * (yB - yA) / rAB;
				xC1 = xD + dol[g] * (xC - xD) / rCD;
				yC1 = yD + dol[g] * (yC - yD) / rCD;
				for(int32_t i = 2; i < NR - 2; i++)
				{
					xT = xB1 + (xC1 - xB1) / NR * (i - 0.5);
					yT = yB1 + (yC1 - yB1) / NR * (i - 0.5);
					//dX = xT - int32_t(xT); dY = yT - int32_t(yT);
					if(((xT < 1.5) || (xT > immaxX - 2.5) || (yT < 1.5) || (yT > immaxY - 2.5)))
						Line[i][3] = -1; else Line[i][g] = Get(yT,xT);
				}
			}
			Krit = -0;
			double q = 0, u = 0, n = 0;
			for(int32_t i = 2; i < NR - 2; i++) if(Line[i][3] != -1)
			{
				fi = 0;
				n++;
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if(r > tr)
					Krit = Krit + w3;
				r = ((Line[i][4] + Line[i][5]) - (Line[i][2] + Line[i][1])) / 2.0;
				if(r > tr)
					Krit = Krit + w2;
				r = Line[i][4] - Line[i][2];
				if(r > tr)
					Krit = Krit + w1;
				//  оличество превышений интенсивности должно быть не менее 35 %
				r = ((Line[i][5] + Line[i][6]) / 2.0 - (Line[i][1] + Line[i][0]) / 2.0); if(r > tr) q = q + 1;
				r = ((Line[i][5] + Line[i][6] + Line[i][1] + Line[i][0]) / 4.0); if(r < 40) u = u + 1;
			}
			if(q < (n) * 0.35) Krit = Krit * 0.7;
			Okrit = Krit;
			if(u < (n) * 0.7) Okrit = Krit * n / (n - u);
			if(u < (n) * 0.4) Krit = Krit * n / (n - u);

			if(Krit > Kritb)
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

		for(int32_t o = 1; o < o1; o++)
		{
			sA = G11 + (G12 - G11)*random;
			sB = G21 + (G22 - G21)*random;
			if(o % o2 == 0)
			{

				r = 0.93;
				if(sAb < 0.5*(G11 + G12)) G12 = G11 + r * (G12 - G11); else G11 = G12 - r * (G12 - G11);
				if(sBb < 0.5*(G21 + G22)) G22 = G21 + r * (G22 - G21); else G21 = G22 - r * (G22 - G21);
			}

			xA = xAsb + sA * (xAsb - xD); yA = yAsb + sA * (yAsb - yD);
			xB = xBsb + sB * (xBsb - xC); yB = yBsb + sB * (yBsb - yC);
			// —читаем критерий

			rBC = sqrt((xCsb - xB)*(xCsb - xB) + (yCsb - yB)*(yCsb - yB)) + 1e-20;
			rAD = sqrt((xA - xDsb)*(xA - xDsb) + (yA - yDsb)*(yA - yDsb)) + 1e-20;
			for(int32_t i = 0; i < NR; i++) Line[i][3] = 0;
			for(int32_t g = 0; g < 7; g++) if(g != 3)
			{
				double j = g - 3;
				xB1 = xA + dol[g] * (xD - xA) / rAD;
				yB1 = yA + dol[g] * (yD - yA) / rAD;
				xC1 = xB + dol[g] * (xC - xB) / rBC;
				yC1 = yB + dol[g] * (yC - yB) / rBC;
				for(int32_t i = 2; i < NR - 2; i++)
				{
					xT = xB1 + (xC1 - xB1) / NR * (i - 0.5);
					yT = yB1 + (yC1 - yB1) / NR * (i - 0.5);
					//dX = xT - int32_t(xT); dY = yT - int32_t(yT);
					if(((xT < 1.5) || (xT > immaxX - 2.5) || (yT < 1.5) || (yT > immaxY - 2.5)))
						Line[i][3] = -1; else Line[i][g] = Get(yT,xT);
				}
			}
			Krit = -0;
			double q = 0, u = 0, n = 0;
			for(int32_t i = 2; i < NR - 2; i++) if(Line[i][3] != -1)
			{
				fi = 0;
				n++;
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if(r > tr)
					Krit = Krit + w3;
				r = ((Line[i][4] + Line[i][5]) - (Line[i][2] + Line[i][1])) / 2.0;
				if(r > tr)
					Krit = Krit + w2;
				r = Line[i][4] - Line[i][2];
				if(r > tr)
					Krit = Krit + w1;
				//  оличество превышений интенсивности должно быть не менее 35 %
				r = ((Line[i][5] + Line[i][6]) / 2.0 - (Line[i][1] + Line[i][0]) / 2.0); if(r > tr) q = q + 1;
				r = ((Line[i][5] + Line[i][6] + Line[i][1] + Line[i][0]) / 4.0); if(r < 40) u = u + 1;
			}
			if(q < (n) * 0.35) Krit = Krit * 0.7;
			Okrit = Krit;
			if(u < (n) * 0.7) Okrit = Krit * n / (n - u);
			if(u < (n) * 0.4) Krit = Krit * n / (n - u);

			if(Krit > Kritb)
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

		for(int32_t o = 1; o < o1; o++)
		{
			sC = G11 + (G12 - G11)*random;
			sD = G21 + (G22 - G21)*random;
			if(o % o2 == 0)
			{

				r = 0.93;
				if(sCb < 0.5*(G11 + G12)) G12 = G11 + r * (G12 - G11); else G11 = G12 - r * (G12 - G11);
				if(sDb < 0.5*(G21 + G22)) G22 = G21 + r * (G22 - G21); else G21 = G22 - r * (G22 - G21);
			}
			xC = xCsb + sC * (xCsb - xB); yC = yCsb + sC * (yCsb - yB);
			xD = xDsb + sD * (xDsb - xA); yD = yDsb + sD * (yDsb - yA);
			// —читаем критерий

			rBC = sqrt((xC - xBsb)*(xC - xBsb) + (yC - yBsb)*(yC - yBsb)) + 1e-20;
			rAD = sqrt((xAsb - xD)*(xAsb - xD) + (yAsb - yD)*(yAsb - yD)) + 1e-20;
			for(int32_t i = 0; i < NR; i++) Line[i][3] = 0;
			for(int32_t g = 0; g < 7; g++) if(g != 3)
			{
				double j = g - 3;
				xB1 = xD + dol[g] * (xA - xD) / rAD;
				yB1 = yD + dol[g] * (yA - yD) / rAD;
				xC1 = xC + dol[g] * (xB - xC) / rBC;
				yC1 = yC + dol[g] * (yB - yC) / rBC;
				for(int32_t i = 2; i < NR - 2; i++)
				{
					xT = xB1 + (xC1 - xB1) / NR * (i - 0.5);
					yT = yB1 + (yC1 - yB1) / NR * (i - 0.5);
					//dX = xT - int32_t(xT); dY = yT - int32_t(yT);
					if(((xT < 1.5) || (xT > immaxX - 2.5) || (yT < 1.5) || (yT > immaxY - 2.5)))
						Line[i][3] = -1; else Line[i][g] = Get(yT,xT);
					//GetInterPixel(yT, xT, dX, dY);
				}
			}
			Krit = -0;
			double q = 0, u = 0, n = 0;
			for(int32_t i = 2; i < NR - 2; i++) if(Line[i][3] != -1)
			{
				fi = 0;
				n++;
				r = ((Line[i][4] + Line[i][5] + Line[i][6]) / 3.0 - (Line[i][2] + Line[i][1] + Line[i][0]) / 3.0);
				if(r > tr)
					Krit = Krit + w3;
				r = ((Line[i][4] + Line[i][5]) - (Line[i][2] + Line[i][1])) / 2.0;
				if(r > tr)
					Krit = Krit + w2;
				r = Line[i][4] - Line[i][2];
				if(r > tr)
					Krit = Krit + w1;
				//  оличество превышений интенсивности должно быть не менее 35 %
				r = ((Line[i][5] + Line[i][6]) / 2.0 - (Line[i][1] + Line[i][0]) / 2.0); if(r > tr) q = q + 1;
				r = ((Line[i][5] + Line[i][6] + Line[i][1] + Line[i][0]) / 4.0); if(r < 40) u = u + 1;
			}
			if(q < (n) * 0.35) Krit = Krit * 0.7;
			Okrit = Krit;
			if(u < (n) * 0.7) Okrit = Krit * n / (n - u);
			if(u < (n) * 0.4) Krit = Krit * n / (n - u);

			if(Krit > Kritb)
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

		lstat[0] = xAb;
		lstat[1] = yAb;
		lstat[2] = xBb;
		lstat[3] = yBb;
		lstat[4] = xCb;
		lstat[5] = yCb;
		lstat[6] = xDb;
		lstat[7] = yDb;
	};
}
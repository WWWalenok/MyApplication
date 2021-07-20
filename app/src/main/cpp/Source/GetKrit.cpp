//
//	Version 1.1.6_A
//
//	Date 19.07
//

#include "../Include/FindA4.hpp"

namespace fa4
{

	float Finder::GetKrit(Vector2 A, Vector2 B, float disp)
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

		for(int32_t i = 0; i < NR; i++) Line[i][3] = 0;
		for(int32_t i = 0; i < NR; i++)
		{
			float k = (i + 1) / float(NR + 1);
			for(int32_t g = 0; g < 9; g++) if(g != 5)
			{
				xT = TAx + k * TBx + k * (K - 1) * TCx + dol[g] * TX.x * 2;
				yT = TAy + k * TBy + k * (K - 1) * TCy + dol[g] * TX.y * 2;
				if(((xT < 1.5) || (xT > immaxX - 2.5) || (yT < 1.5) || (yT > immaxY - 2.5)));
				else
				{
					uint8_t T = Get(yT, xT);
					if(maxI <T)maxI = T;
					if(minI > T)minI = T;
					if(g < 4)
					{
						medL += T;
						LC++;
					}
				}
			}
		}

		medL /= LC;

		float down = 255 / (maxI - minI / 2.0);

		TX = !(A - B); TX.SetLenght(1);
		for(int32_t E = 0; E < 5; E++)
		{
			k1 = k1b; k2 = k2b; k3 = k3b;
			for(int32_t o = 1; o < o1; o++)
			{
				if(0 != 1)
				{
					k1 = G11 + (G12 - G11)*random;
					k2 = G21 + (G22 - G21)*random;
					k3 = G31 + (G32 - G31)*random;
				}
				if(o % o2 == 0)
				{
					r = 0.93;

					if(abs(0.5*(G11 + G12) - k1) / (G12 - G11) < 0.3)
					{
						G12 = G11 + r * (G12 - G11) / 2.0; G11 = G12 - r * (G12 - G11) / 2.0;
					}
					else if(k1 < 0.5*(G11 + G12))
						G12 = G11 + r * (G12 - G11);
					else
						G11 = G12 - r * (G12 - G11);


					if(abs(0.5*(G21 + G22) - k2) / (G22 - G21) < 0.3)
					{
						G22 = G21 + r * (G22 - G21) / 2.0; G21 = G22 - r * (G22 - G21) / 2.0;
					}
					else if(k2 < 0.5*(G21 + G22))
						G22 = G21 + r * (G22 - G21);
					else
						G21 = G22 - r * (G22 - G21);


					if(abs(0.5*(G31 + G32) - k3) / (G32 - G31) < 0.3)
					{
						G32 = G31 + r * (G32 - G31) / 2.0; G31 = G32 - r * (G32 - G31) / 2.0;
					}
					else if(k3 < 0.5*(G31 + G32))
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
				for(int32_t i = 0; i < NR; i++) Line[i][3] = 0;
				for(int32_t i = 0; i < NR; i++)
				{
					float k = (i + 1) / float(NR + 1);
					for(int32_t g = 0; g < 9; g++) if(g != 4)
					{
						xT = TAx + k * TBx + k * (K - 1) * TCx + dol[g] * TX.x;
						yT = TAy + k * TBy + k * (K - 1) * TCy + dol[g] * TX.y;
						if(((xT < 1.5) || (xT > immaxX - 2.5) || (yT < 1.5) || (yT > immaxY - 2.5)))
							Line[i][4] = -1;
						else
						{
							Line[i][g] = (Get(yT,xT) - minI / 2.0) * down;
						}

					}
				}
				Okrit = 0;
				Krit = -0;
				double q = 0, u = 0, n = 0, r0;
				for(int32_t i = 0; i < NR; i++) if(Line[i][4] != -1)
				{
					fi = 0;
					n++;
					r0 = Line[i][5] - Line[i][3];
					if(r0 > tr)
					{
						Krit = Krit + 1.6;
						Okrit += 0.8;
					}
					r = ((Line[i][5] * 0.6 + Line[i][4] * 0.4) - (Line[i][1] + Line[i][2]) / 2.0);
					if(r > tr)
					{
						Krit = Krit + 0.25;
						if(r < 1.0 * r0)
						{
							Okrit += 0.25;
							Krit += 0.25;
						}
					}
					r = ((Line[i][5] * 0.6 + Line[i][6] * 0.4) - (Line[i][1] + Line[i][0]) / 2.0);
					if(r > tr)
					{
						Krit = Krit + 0.25;
						if(r < 1.0 * r0)
						{
							Okrit += 0.25;
							Krit += 0.25;
						}
					}
					r = ((Line[i][6]) - (Line[i][2]));
					if(r > tr)
					{
						Krit = Krit + 0.5;
						if(r < 1.0 * r0)
						{
							Okrit += 0.5;
							Krit += 0.5;
						}
					}

					// Количество превышений интенсивности должно быть не менее 35 %
					r = ((Line[i][5] + Line[i][6]) / 2.0 - (Line[i][1] + Line[i][0]) / 2.0);
					if(r > tr) q = q + 1;
					r = MIN_3(Line[i][5], Line[i][6], Line[i][6]);
					if(r < medL * 0.9) u = u + 1;
				}
				//if (q < (n) * 0.35) Krit = Krit * 0.7;
				//Okrit = Krit;
				if(u < (n) * 0.7)
					Okrit = Okrit * n / (n - u);
				if(u < (n) * 0.5)
					Krit = Krit * n / (n - u);

				if(Krit > Kritb)
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

	float Finder::GetKrit2(const Vector2& A, const Vector2& B, const Vector2& C, const Vector2& D)
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
					Is[i][j] = Get(int32_t(y),int32_t(x));
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


}

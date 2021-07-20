//
//	Version 1.1.6_A
//
//	Date 19.07
//

#include "../Include/FindA4.hpp"

namespace fa4
{

	float** Finder::GetNewCamPos(double w, double h, double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy)
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
		if(min(R1 * R1, R3 * R3) < min(R2 * R2, R4 * R4))
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
		for(int32_t j = 0; j < 3; j++)
			for(int32_t k = 0; k < 3; k++)if(k != j)
			{
				t = M[k][j] / M[j][j];
				for(int32_t i = j; i < 4; i++)
				{
					M[k][i] = M[k][i] - M[j][i] * t;
				}
			}
		for(int32_t j = 0; j < 3; j++)
		{
			t = 1 / M[j][j];
			M[j][j] = 1;
			M[j][3] *= t;
		}
		AO.z = 0; BO.z = 0; CO.z = 0; DO.z = 0;
		Vector3 BA, BB, BC, BD;
		double bx1 = 0, by1 = 0, bx2 = 0, by2 = 0, bx3 = 0, by3 = 0, bx4 = 0, by4 = 0, Kri = 1e10, dr = K * 0.05; int32_t counter = 0;

		Vector3 Olist[4]{A,B,C,D};
		Vector3 list[4];

		for(int32_t l = 0, _Q = 0; l < 100; l++)
		{
			double te = 1e10, tx1 = 0, ty1 = 0, tx2 = 0, ty2 = 0, tx3 = 0, ty3 = 0, tx4 = 0, ty4 = 0;
			float x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0, x4 = 0, y4 = 0;
			for(x2 = -1; x2 <= 1; x2++) for(y2 = -1; y2 <= 1; y2++) for (x4 = -1; x4 <= 1; x4++) for (y4 = -1; y4 <= 1; y4++)
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
				for(int32_t j = 0; j < 3; j++)
					for(int32_t k = 0; k < 3; k++)if(k != j)
					{
						t = M[k][j] / M[j][j];
						for(int32_t i = j; i < 4; i++)
						{
							M[k][i] = M[k][i] - M[j][i] * t;
						}
					}
				for(int32_t j = 0; j < 3; j++)
				{
					t = 1 / M[j][j];
					M[j][j] = 1;
					M[j][3] *= t;
				}

				a = M[0][3]; b = M[1][3]; c = M[2][3];
				lamda = -((D - A * a) * (D - C * c)) / (1 - a - c + a * c);
				lamda = sqrt(abs(lamda));
				A.z = lamda; B.z = lamda; C.z = lamda, D.z = lamda;
				R1 = A * a - B * b; R2 = C * c - B * b;
				l1 = sqrt(R1*R1);
				l2 = sqrt(R2*R2);
				
				d = 297 * l2 / l1 - 210;

				float e = d * d;

				if(lamda < 25)
					e *= (25 - lamda) * 100;
				if(lamda > 45)
					e *= (lamda - 45) * 100;
				if(e < te)
				{
					te = e;
					tx2 = x2 * dr; ty2 = y2 * dr;
					tx4 = x4 * dr; ty4 = y4 * dr;
				}
			}
			te = sqrtf(te);

			if (te == Kri && te > 2.1)
			{
				dr = dr * 0.7071 * 2;
			}

			if (te == Kri && te <= 2.1)
			{
				dr = dr * 0.7071;
			}

			if(te < Kri)
			{
				_Q = 0;
				Kri = te;
				bx2 += tx2; by2 += ty2;
				bx4 += tx4; by4 += ty4;
			}
			if (Kri < 1e-3)
				break;
			//dr = K * 0.1 * (Kri + 100) / (100 + Kri);

			//dr = dr * 0.7071;
		}

		A.z = 0; B.z = 0; C.z = 0; D.z = 0;
		BO.x += bx2; CO.x += bx3; DO.x += bx4;
		BO.y += by2; CO.y += by3; DO.y += by4;
		A.x = AO.x; A.y = AO.y;
		B.x = BO.x; B.y = BO.y;
		C.x = CO.x; C.y = CO.y;
		D.x = DO.x; D.y = DO.y;
		a = M[0][3]; b = M[1][3]; c = M[2][3];
		lamda = -((D - A * a) * (D - C * c)) / (1 - a - c + a * c);
		lamda = sqrt(abs(lamda));
		A.z = lamda; B.z = lamda; C.z = lamda, D.z = lamda;
		R1 = A * M[0][3] - B * M[1][3]; R2 = C * M[2][3] - B * M[1][3];
		l1 = sqrt(R1*R1);
		l2 = sqrt(R2*R2);
		d = 297 / l1;
		a = a * d;	b = b * d;	c = c * d;	d = d;
		A = AO; B = BO; C = CO; D = DO;
		A.z = lamda; B.z = lamda; C.z = lamda, D.z = lamda;
		if(flip)
		{
			O = D;
			D = C; C = B; B = A; A = O;
			t = d;
			d = c; c = b; b = a; a = t;
		}
		R1 = Vector3(h / 2.0, w / 2.0, 0);
		AO = (A / K + R1); BO = (B / K + R1); CO = (C / K + R1); DO = (D / K + R1);
		AT = A * a;
		BT = B * b;
		CT = C * c;
		DT = D * d;
		O = (AT + CT) / 2.0;
		R1 = AT - BT;
		R1.SetLenght(1);
		R2 = AT - DT;
		R2.SetLenght(1);
		R3 = R1 / R2; R3 = R3 / R3.z;
		R3.SetLenght(1);
		R2 = R1 / R3;
		R2.SetLenght(1);


		AT = AT - O;
		BT = BT - O;
		CT = CT - O;
		DT = DT - O;
		Vector3 WA(AT * R1, AT * R2, AT * R3), WB(BT * R1, BT * R2, BT * R3), WC(CT * R1, CT * R2, CT * R3), WD(DT * R1, DT * R2, DT * R3);
		O = Vector3(O * R1, O * R2, O * R3);
		t = (R1.x * R2.y * R3.z) - (R3.x * R2.y * R1.z) - (R2.x * R1.y * R3.z) - (R1.x * R3.y * R2.z) + (R3.x * R1.y * R2.z) + (R2.x * R3.y * R1.z);
		Vector3 X(R1.x, R2.x, R3.x), Y(R1.y, R2.y, R3.y), Z(R1.z, R2.z, R3.z);
		X.SetLenght(1); Y.SetLenght(1); Z.SetLenght(1);

		float** ret = new float*[8]; ret[0] = new float[5];
		for(int32_t i = 1; i < 8; i++) ret[i] = new float[3];

		Vector3 Err = (WA + WB + WC + WD) / 4.0;

		O = O - Err;
		WA = WA - Err;
		WB = WB - Err;
		WC = WC - Err;
		WD = WD - Err;

		ret[0][0] = O.x; ret[0][1] = O.y; ret[0][2] = O.z; ret[0][3] = lamda;
		ret[1][0] = X.x; ret[1][1] = X.y; ret[1][2] = X.z;
		ret[2][0] = Y.x; ret[2][1] = Y.y; ret[2][2] = Y.z;
		ret[3][0] = Z.x; ret[3][1] = Z.y; ret[3][2] = Z.z;
		ret[4][0] = WA.x; ret[4][1] = WA.y; ret[4][2] = WA.z;
		ret[5][0] = WB.x; ret[5][1] = WB.y; ret[5][2] = WB.z;
		ret[6][0] = WC.x; ret[6][1] = WC.y; ret[6][2] = WC.z;
		ret[7][0] = WD.x; ret[7][1] = WD.y; ret[7][2] = WD.z;

		return ret;
	}


}

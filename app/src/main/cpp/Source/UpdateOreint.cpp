//
//	Version 1.1.6_A
//
//	Date 19.07
//

#include "../Include/FindA4.hpp"
namespace fa4
{
#define initdarr3(arr, type, m1, m2, m3) \
	{\
		arr = new type**[m1];\
		for(int i1 = 0; i1 < m1; i1++) { \
			arr[i1] = new type*[m2];\
			for(int i2 = 0; i2 < m2; i2++) \
				arr[i1][i2] = new type[m3];\
		}\
	}\

#define deletedarr3(arr, m1, m2, m3) \
	{\
		for(int i1 = 0; i1 < m1; i1++) { \
			for(int i2 = 0; i2 < m2; i2++) \
				delete[] arr[i1][i2];\
			delete[] arr[i1];\
		}\
		delete[] arr;\
	}\


	void Finder::Th_UpdateOreint(int32_t &borr, float &krbb, Vector3 A, Vector3 B, Vector3 C, Vector3 D, float ** gcams)
	{
#define Oreintdisp 50

		float r, t;
		borr = 1; krbb = -1;
		float CFxbb = 0, CFybb = 0, CFzbb = 0.5, CFfbb = 0;
		int32_t  blor = 1;
		int32_t CSh = 25;
		float _x, _y;
		int16_t buff[Oreintdisp][Oreintdisp][4];
		//int16_t ***buff;
		//initdarr3(buff, int16_t, Oreintdisp, Oreintdisp, 4);

		float avg = 0, h = 0, P[4];

		int xT, yT;

		for(int32_t x = 0; x < Oreintdisp; x++)
		{
			_x = (x + 1) / (Oreintdisp + 1.0);
			for(int32_t y = 0; y < Oreintdisp; y++)
			{
				t = -1;
				_y = (y + 1) / (Oreintdisp + 1.0);
				P[1] = A.x * (1 - _x)*(1 - _y) + B.x * (_x)*(1 - _y) + C.x * (_x)*(_y)+D.x * (1 - _x)*(_y);
				P[0] = A.y * (1 - _x)*(1 - _y) + B.y * (_x)*(1 - _y) + C.y * (_x)*(_y)+D.y * (1 - _x)*(_y);

				xT = A.x * (1 - _x)*(1 - _y) + B.x * (_x)*(1 - _y) + C.x * (_x)*(_y)+D.x * (1 - _x)*(_y);
				yT = A.y * (1 - _x)*(1 - _y) + B.y * (_x)*(1 - _y) + C.y * (_x)*(_y)+D.y * (1 - _x)*(_y);
				if(((xT < 1.5) || (xT > immaxX - 2.5) || (yT < 1.5) || (yT > immaxY - 2.5)))
					t = -1;
				else
				{
					t = Get(yT,xT);
				}

				//if (P[0] > 3 && P[1] > 3 && P[0] < immaxY - 5 && P[1] < immaxX - 5)
				//	t = img[int32_t(P[0])][int32_t(P[1])];
				buff[x][y][2] = yT;
				buff[x][y][3] = xT;
				buff[x][y][0] = t;
				if(t >= 0)
				{
					avg += t;
					h++;
				}
			}
		}
		avg /= h;
		for(int32_t x = 0; x < Oreintdisp; x++)
		{
			for(int32_t y = 0; y < Oreintdisp; y++)if(buff[x][y][0] >= 0)
			{
				if(buff[x][y][0] > avg * 0.5) buff[x][y][0] = 1;
				else buff[x][y][0] = 0;
			}
		}
		float n[4], point[4], tpoint[4];
		float npoints[200][4];
		for(int32_t E = 0; E < 4; E++)
		{
			int32_t orr = (E % 2 == 0) ? 1 : -1, lor = ((E % 4) / 2) ? 1 : -1;
			float G11 = -80, G12 = 80, G21 = -40, G22 = 40, G31 = 0.5, G32 = 1, G41 = -15 / 180.0 * PI, G42 = 15 / 180.0 * PI;
			float CFx, CFy, CFz, CFf, CFxb = 0, CFyb = 0, CFzb = 1, CFfb = 0;
			float krb = -1;
			for(int32_t I = 0; I < 150; I++)
			{
				CFx = G11 + random * (G12 - G11);
				CFy = G21 + random * (G22 - G21);
				CFz = G31 + random * (G32 - G31);
				CFf = G41 + random * (G42 - G41);
				if(I % 3 == 2)
				{
					r = 0.93;
					if(CFxb < (G11 + G12) / 2.0) G12 = G11 + (G12 - G11)*r; else G11 = G12 - (G12 - G11)*r;
					if(CFyb < (G21 + G22) / 2.0) G22 = G21 + (G22 - G21)*r; else G21 = G22 - (G22 - G21)*r;
					if(CFzb < (G31 + G32) / 2.0) G32 = G31 + (G32 - G31)*r; else G31 = G32 - (G32 - G31)*r;
					if(CFfb < (G41 + G42) / 2.0) G42 = G41 + (G42 - G41)*r; else G41 = G42 - (G42 - G41)*r;
				}
				float co = cos(CFf), si = sin(CFf), tx, ty;
				for(int32_t i = 0; i < CSh; i++)
				{
					point[0] = CFz * (shariki[i][0] * co - lor * shariki[i][1] * si) * orr + CFx;
					point[1] = CFz * (shariki[i][0] * si + lor * shariki[i][1] * co) * orr + CFy;
					point[2] = CFz * shariki[i][2];
					point[0] -= gcams[0][0]; point[1] -= gcams[0][1]; point[2] -= gcams[0][2];
					tpoint[0] = (point[0] * gcams[1][0] + point[1] * gcams[1][1] + point[2] * gcams[1][2]);
					tpoint[1] = (point[0] * gcams[2][0] + point[1] * gcams[2][1] + point[2] * gcams[2][2]);
					tpoint[2] = (point[0] * gcams[3][0] + point[1] * gcams[3][1] + point[2] * gcams[3][2]);
					npoints[i][0] = tpoint[0];
					npoints[i][1] = tpoint[1];
					npoints[i][2] = tpoint[2];
					npoints[i][3] = shariki[i][3] * CFz;
				}
				float kr = 0;
				for(int32_t x = 0; x < Oreintdisp; x++)
				{
					for(int32_t y = 0; y < Oreintdisp; y++)
					{
						n[2] = gcams[4][0] / K;
						n[1] = -(buff[x][y][2] - immaxY / 2.0);
						n[0] = -(buff[x][y][3] - immaxX / 2.0);
						float a = n[2] * n[2] + n[1] * n[1] + n[0] * n[0], b, c;
						bool isnotin = true;
						for(int32_t i = 0; i < CSh && isnotin; i++)
						{
							point[0] = npoints[i][0]; point[1] = npoints[i][1]; point[2] = npoints[i][2];

							b = n[2] * point[2] + n[1] * point[1] + n[0] * point[0];
							c = point[2] * point[2] + point[1] * point[1] + point[0] * point[0];
							if(c - b * b / a <= shariki[i][3] * shariki[i][3])
								isnotin = false;
						}
						if(!isnotin)
						{
							buff[x][y][1] = 1;

						}
						else
						{
							buff[x][y][1] = 0;

						}
						if(!(buff[x][y][1] == buff[x][y][0]))
						{
							kr++;
						}
					}
				}

				if(kr > krb)
				{
					krb = kr;
					CFxb = CFx; CFyb = CFy; CFzb = CFz; CFfb = CFf;

				}
				if(krb > krbb)
				{
					krbb = krb;
					blor = lor; borr = orr;
					CFxbb = CFxb; CFybb = CFyb; CFzbb = CFzb; CFfbb = CFfb;
				}
			}
		}
		//deletedarr3(buff, Oreintdisp, Oreintdisp, 4);
	}

	int32_t Finder::UpdateOreint(Vector3 A, Vector3 B, Vector3 C, Vector3 D, float** gcams)
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
		Th_UpdateOreint(borrs[0], krits[0], A, B, C, D, gcams);
		return borrs[0];
	}

}

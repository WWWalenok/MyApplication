//
//	Version 1.1.4_A
//
//	Date 14.07
//
#pragma once
#if defined __cplusplus
extern "C++"
{
#include <string>
#include "Vector.hpp"
#include <vector>
#include "Tabu.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <cmath>
#include <map>
#include <utility>
	namespace fa4
	{
#define PI 3.141592653589793238462643383279502884197169399
#define LineDisp 15
#define LineDeep 3
#define RandomCount 4096
#define max(a,b) ((a > b) ? a : b)
#define min(a,b) ((a < b) ? a : b)
		//#define in(a,x,b) max(min(x,b),a)

#define MIN_2(a,b) ((a > b) ? b : a)
#define MIN_3(a,b,c) MIN_2(a,MIN_2(b,c))
#define MIN_4(a,b,c,d) MIN_2(a,MIN_3(b,c,d))
#define MAX_2(a,b) ((a < b) ? b : a)
#define MAX_3(a,b,c) MAX_2(a,MAX_2(b,c))
#define MAX_4(a,b,c,d) MAX_2(a,MAX_3(b,c,d))

#define random (arandom[(seed >= RandomCount - 100) ? seed = 0 : ++seed])

#define Get(c,r) (TARR == UI8 ? map8[c * rows + r] : (TARR == UI8_2 ? map8_2[c][r] : (TARR == UI32 ? uint8_t(map32[c * rows + r]) : uint8_t(map32_2[c][r]))))

		static std::string version = "1.1.0_A";

		std::string GetVersion();

		struct Prop
		{
			char* name;
			uint32_t lenght;
			float var;
			Prop(std::string str, float value)
			{
				lenght = str.length();
				name = (char*)malloc(sizeof(char) * lenght);
				for (int i = 0; i < lenght; i++)
					name[i] = str[i];
				var = value;
			}

			Prop(char* str, int32_t len, float value)
			{
				lenght = len;
				name = str;
				var = value;
			}
			Prop()
			{
				name = nullptr;
				lenght = 0;
				var = 0;
			}
			~Prop()
			{
				delete[] name;
			}
		};

		struct Finder
		{
			enum Typeofarray
			{
				UI8, // *uint8_t
				UI8_2, // **uint8_t
				UI32, // *uint32_t
				UI32_2 // **uint32_t
			};

			Typeofarray TARR = UI8_2;

			float K, DK;

			void SetOut(double** out);

			void InitImg(void* map, int32_t rows, int32_t cols, Typeofarray = UI8_2);

			/*
			props - Array of parametrs
			Parrametrts :
			"Rows" - image
			"Cols" - image
			"Kadr" - select view point
			"ThreadCount" - maxumum number of thread
			"Beta" - angle to floor
			"Gamma" - rotation angle
			"Focal" - focal lenght of phone camera
			"Right" - use is foot is right (value not used)
			"Left" - use is foot is left (value not used)
			"Side" - use to define foot side : 1 for right, 0 for left, other - side not define
			int32_t count - count of prop
			*/
			void Init(Prop* props, int32_t count);
			/*
			props - Array of parametrs
			Parrametrts :
			"Rows" - image
			"Cols" - image
			"Kadr" - select view point
			"ThreadCount" - maxumum number of thread
			"Beta" - angle to floor
			"Gamma" - rotation angle
			"Focal" - focal lenght of phone camera
			"Right" - use is foot is right (value not used)
			"Left" - use is foot is left (value not used)
			"Side" - use to define foot side : 1 for right, 0 for left, other - side not define
			*/
			void Init(std::vector<fa4::Prop>);

			/*
			Return
			*/
			std::string GetCorner(float* olist = 0 /*float[8]*/, int32_t* oisbadlist = 0/*int32_t*/);

			std::string GetCams(float* list/*float[8]*/, float** camspace = 0/*float[8][4]*/, bool reinit = false);

			std::string GetKrit(float* list/*float[8]*/, float* oKrits = 0/*float[5]*/, bool reinit = false);

			std::string UpdateOrientedCams(float* list/*float[8]*/, float** camspace/*float[8][4]*/, bool reinit = false);

			std::string GetTips(float* list/*float[8]*/, float** camspace/*float[8][4]*/, uint8_t* otips = 0, bool reinit = false);

			std::string StartAll();

			std::string FootCollor(float* list, uint8_t* out_I = 0);

			void ClearMap_8_2()
			{
				for (int32_t i = 0, k = 0; i < rows; i++)
				{
					delete[] map8_2[i];
				}
				delete[] map8_2;
			}

		private:
			uint8_t* map8, ** map8_2;

			uint32_t cols = 0, rows = 0;

			uint32_t* map32, ** map32_2;

			uint8_t GetElement(uint16_t c, uint16_t r);

			float arandom[RandomCount];
			int32_t seed = 0;
			int32_t seedcount = 0;
			int32_t useangel = 0;
			uint32_t maxthreadcount = 1;
			double** inout = 0;
			int32_t KadrNumber = 0;
			int32_t Right = -1;

			float tacos[2001];
			float tr[2000];
			float ironangel;
			float gcams[8][4]{ -1000000 }, list[8]{ -1000000 }, cstat[7]{ -1000000 };
			float immaxX, immaxY, GGama, GBeta, GFocal;

			const float baseK = 43.26661530556787151743 / 800.0;

			float KritFunc(float x);

			void MakeBest(float* list, double disp, float* Krits);

			//void th_MakeBest(float* list, double disp, float* Krits);

			float** GetNewCamPos(double w, double h, double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy);

			void BruteScan(int32_t itercount, int32_t thcount, float* outstats, float* lstat, double& Kritbb, int32_t& SheetFound, int32_t& SheetFoundProbability, int32_t& SheetProbablyFound);

			void th_BruteScan(int32_t itercount, int32_t sublenght, float* outstats, float* lstat, double& Kritbb, int32_t& SheetFound, int32_t& SheetFoundProbability, int32_t& SheetProbablyFound, int32_t thnum);

			float GetKrit(Vector2 A, Vector2 B, float disp);

			float GetKrit2(const Vector2& A, const Vector2& B, const Vector2& C, const Vector2& D);

			void Th_UpdateOreint(int32_t& borr, float& krbb, Vector3 A, Vector3 B, Vector3 C, Vector3 D, float** gcams);

			int32_t UpdateOreint(Vector3 A, Vector3 B, Vector3 C, Vector3 D, float** gcams);

		};		

		size_t InitNewFinder();

		void Remove(size_t id, int mode = 0);

		void SetOut(size_t id, double** out);

		void InitImg(size_t id, void* map, int32_t rows, int32_t cols, Finder::Typeofarray = Finder::UI8_2);

		/*
			props - Array of parametrs
			Parrametrts :
			"Rows" - image
			"Cols" - image
			"Kadr" - select view point
			"ThreadCount" - maxumum number of thread
			"Beta" - angle to floor
			"Gamma" - rotation angle
			"Focal" - focal lenght of phone camera
			"Right" - use is foot is right (value not used)
			"Left" - use is foot is left (value not used)
			"Side" - use to define foot side : 1 for right, 0 for left, other - side not define
			int32_t count - count of prop
			*/
		void Init(size_t id, Prop* props, int32_t count);

		/*
		props - Array of parametrs
		Parrametrts :
		"Rows" - image
		"Cols" - image
		"Kadr" - select view point
		"ThreadCount" - maxumum number of thread
		"Beta" - angle to floor
		"Gamma" - rotation angle
		"Focal" - focal lenght of phone camera
		"Right" - use is foot is right (value not used)
		"Left" - use is foot is left (value not used)
		"Side" - use to define foot side : 1 for right, 0 for left, other - side not define
		*/
		void Init(size_t id, std::vector<fa4::Prop>);

		std::string GetCorner(size_t id, float* olist = 0 /*float[8]*/, int32_t* oisbadlist = 0/*int32_t*/);

		std::string GetCams(size_t id, float* list/*float[8]*/, float** camspace = 0/*float[8][4]*/, bool reinit = false);

		std::string GetKrit(size_t id, float* list/*float[8]*/, float* oKrits = 0/*float[5]*/, bool reinit = false);

		std::string UpdateOrientedCams(size_t id, float* list/*float[8]*/, float** camspace/*float[8][4]*/, bool reinit = false);

		std::string GetTips(size_t id, float* list/*float[8]*/, float** camspace/*float[8][4]*/, uint8_t* otips = 0, bool reinit = false);

		std::string StartAll(size_t id);

		std::string FootCollor(size_t id, float* list, uint8_t* out_I = 0);
	}
}
#endif

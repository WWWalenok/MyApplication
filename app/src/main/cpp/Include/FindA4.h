//
//	Version 1.0.13
//
//	Date 05.07
//

#if defined __cplusplus
extern "C++"
{
#include <string>
#include "Vector.h"
	namespace fa4
	{
		static std::string version = "1.0.13";

		std::string GetVersion()
		{
			return std::string(version);
		}

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

			Prop(char*str, int32_t len, float value)
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

		/*
			Основная функция
			maxthreadcount - количество задействованых потоков (не менее 1 и не более 20)
			img - массив изображения, maxX/maxY - размер кадра
			imnum - номер кадра(если нет то просто ставим -1)
			maxX = ~640
			maxY = ~480
			 int32_t side = -1 : - use to define foot side : 1 for right, 0 for left, other - side not define
		*/
		std::string GetA4FromImg(int32_t maxthreadcount, uint8_t **img, int32_t maxX, int32_t maxY, int32_t imnum, int32_t side = -1);

		/*
			props - Array of parametrs
			Parrametrts :
			"ImgX" - image
			"ImgY" - image
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
		std::string GetA4FromImg(uint8_t **img, Prop* props, int32_t count);

		void SetOut(double** out);
	}
}
#endif

//
//	Version 1.1.4_A
//
//	Date 14.07
//

#include "../Include/FindA4.hpp"

namespace fa4
{
	float Finder::KritFunc(float f)
	{
		return exp(1.0 / 30.0 * log(abs(f) + 1e-100)) * f / abs(f + 1e-100);
	}

	void  Finder::InitImg(void* map, int32_t rows, int32_t cols, Typeofarray _Y)
	{
		map32 = (uint32_t*)(map);
		map32_2 = (uint32_t**)(map);
		map8 = (uint8_t*)(map);
		map8_2 = (uint8_t**)(map);

		Finder::TARR = _Y;

		Right = 0;
		double time = 0;
		useangel = 0;
		KadrNumber = 0;
		for(int32_t i = 0; i < RandomCount; i++)
			arandom[i] = (rand()) / double(RAND_MAX);
		//srand(img[immaxY / 13][immaxX / 11]);
		{
			// Табуляция одной из функций
			// Предварительно табулируем sin и cos
			for(int32_t i = 0; i < 2001; i++)
			{
				double f = (i - 1000) / 100.0;
				tacos[i] = acos(f / 10.0);
			}

			for(int32_t i = 0; i < 2000; i++)
			{
				tr[i] = exp(0.83*log(1 - i / (double)2000 + 1e-5));
			}
		}
		immaxX = cols;
		immaxY = rows;
		K = 43.26661530556787151743 / sqrt(immaxX*immaxX + immaxY * immaxY);
		DK = baseK / K;
		//fa4::img = img;
		maxthreadcount = 0;

	}

	void  Finder::Init(Prop* props, int32_t count)
	{
		Right = -1;
		for(int32_t i = 0; i < count; i++)
		{
			std::string str(props[i].name, props[i].lenght);
			if(str == "ImgX")
				immaxX = props[i].var;
			else if(str == "ImgY")
				immaxY = props[i].var;
			else if(str == "ThreadCount")
				maxthreadcount = props[i].var;
			else if(str == "Kadr")
				KadrNumber = props[i].var;
			else if(str == "Gamma")
				GGama = props[i].var;
			else if(str == "Beta")
				GBeta = props[i].var;
			else if(str == "Focal")
				GFocal = props[i].var;
			else if(str == "Right")
				Right = 1;
			else if(str == "Left")
				Right = 0;
			else if(str == "Side")
				Right = (props[i].var == 0) ? 0 : ((props[i].var == 1) ? 1 : -1);
		}

		double time = 0;
		useangel = (KadrNumber > 0) ? 0 : 1;
		KadrNumber = abs(KadrNumber);
		for(int32_t i = 0; i < RandomCount; i++)
			arandom[i] = (rand()) / double(RAND_MAX);
		{
			for(int32_t i = 0; i < 2001; i++)
			{
				double f = (i - 1000) / 100.0;
				tacos[i] = acos(f / 10.0);
			}

			for(int32_t i = 0; i < 2000; i++)
			{
				tr[i] = exp(0.83*log(1 - i / (double)2000 + 1e-5));
			}
		}
		K = 43.26661530556787151743 / sqrt(immaxX*immaxX + immaxY * immaxY);
		DK = baseK / K;
		//fa4::img = img;

	}

	void  Finder::Init(std::vector<fa4::Prop> props)
	{
		Right = -1;
		for(int32_t i = 0; i < props.size(); i++)
		{
			std::string str(props[i].name, props[i].lenght);
			if(str == "ImgX")
				immaxX = props[i].var;
			else if(str == "ImgY")
				immaxY = props[i].var;
			else if(str == "ThreadCount")
				maxthreadcount = props[i].var;
			else if(str == "Kadr")
				KadrNumber = props[i].var;
			else if(str == "Gamma")
				GGama = props[i].var;
			else if(str == "Beta")
				GBeta = props[i].var;
			else if(str == "Focal")
				GFocal = props[i].var;
			else if(str == "Right")
				Right = 1;
			else if(str == "Left")
				Right = 0;
			else if(str == "Side")
				Right = (props[i].var == 0) ? 0 : ((props[i].var == 1) ? 1 : -1);
		}

		double time = 0;
		useangel = (KadrNumber > 0) ? 0 : 1;
		KadrNumber = abs(KadrNumber);
		for(int32_t i = 0; i < RandomCount; i++)
			arandom[i] = (rand()) / double(RAND_MAX);
		{
			for(int32_t i = 0; i < 2001; i++)
			{
				double f = (i - 1000) / 100.0;
				tacos[i] = acos(f / 10.0);
			}

			for(int32_t i = 0; i < 2000; i++)
			{
				tr[i] = exp(0.83*log(1 - i / (double)2000 + 1e-5));
			}
		}
		K = 43.26661530556787151743 / sqrt(immaxX*immaxX + immaxY * immaxY);
		DK = baseK / K;
		//fa4::img = img;
	}

	void Finder::SetOut(double** _out)
	{
		inout = _out;
	}

	size_t maxid = 1;

	Finder** stack = new Finder * [1]{ 0 };

	size_t InitNewFinder()
	{
		size_t id = 0;
		while (stack[id] != 0)
		{
			id++;
			if (id == maxid)
			{
				Finder** t = new Finder * [maxid + 100];
				for (size_t i = 0; i < maxid + 100; i++)
					t[i] = i < maxid ? stack[i] : 0;
				delete[] stack;
				stack = t;
				maxid += 100;
			}
		}
		stack[id] = new Finder();
		return id;
	}

	void Remove(size_t id, int mode)
	{

		if (id < maxid && stack[id] != 0)
		{
			if (mode & 1 > 0)
			{
				stack[id]->ClearMap_8_2();
			}
			delete stack[id];
			stack[id] = 0;
		}
	}

	std::string GetCorner(size_t id, float* olist /*float[8]*/, int32_t* oisbadlist/*int32_t*/)
	{
		if (id < maxid && stack[id] != 0)
		{
			return stack[id]->GetCorner(olist, oisbadlist);
		}
		return "Error : Finder NotInit";
	}

	std::string GetCams(size_t id, float* list/*float[8]*/, float** camspace/*float[8][4]*/, bool reinit)
	{
		if (id < maxid && stack[id] != 0)
		{
			return stack[id]->GetCams(list, camspace, reinit);
		}
		return "Error : Finder NotInit";
	}

	std::string GetKrit(size_t id, float* list/*float[8]*/, float* oKrits/*float[5]*/, bool reinit)
	{
		if (id < maxid && stack[id] != 0)
		{
			return stack[id]->GetKrit(list, oKrits, reinit);
		}
		return "Error : Finder NotInit";
	}

	std::string UpdateOrientedCams(size_t id, float* list/*float[8]*/, float** camspace/*float[8][4]*/, bool reinit)
	{
		if (id < maxid && stack[id] != 0)
		{
			return stack[id]->UpdateOrientedCams(list, camspace, reinit);
		}
		return "Error : Finder NotInit";
	}

	std::string GetTips(size_t id, float* list/*float[8]*/, float** camspace/*float[8][4]*/, uint8_t* otips, bool reinit)
	{
		if (id < maxid && stack[id] != 0)
		{
			return stack[id]->GetTips(list, camspace, otips, reinit);
		}
		return "Error : Finder NotInit";
	}

	std::string StartAll(size_t id)
	{
		if (id < maxid && stack[id] != 0)
		{
			return stack[id]->StartAll();
		}
		return "Error : Finder NotInit";
	}

	void SetOut(size_t id, double** out)
	{
		if (id < maxid && stack[id] != 0)
		{
			return stack[id]->SetOut(out);
		}
	}

	void InitImg(size_t id, void* map, int32_t rows, int32_t cols, Finder::Typeofarray t)
	{
		if (id < maxid && stack[id] != 0)
		{
			return stack[id]->InitImg(map, rows, cols, t);
		}
	}

	void Init(size_t id, Prop* props, int32_t count)
	{
		if (id < maxid && stack[id] != 0)
		{
			return stack[id]->Init(props, count);
		}
	}

	void Init(size_t id, std::vector<fa4::Prop> prop)
	{
		if (id < maxid && stack[id] != 0)
		{
			return stack[id]->Init(prop);
		}
	}

	std::string fa4::FootCollor(size_t id, float* list, uint8_t* out_I)
	{
		if (id < maxid && stack[id] != 0)
		{
			return stack[id]->FootCollor(list, out_I);
		}
		return "Error : Finder NotInit";
	}
}

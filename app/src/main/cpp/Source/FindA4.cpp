//
//	Version 1.1.4_A
//
//	Date 14.07
//

#include "../Include/FindA4.hpp"

namespace fa4
{
	std::string GetVersion()
	{
		return std::string(version);
	}

	std::string Finder::StartAll()
	{
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

		int32_t orienta, SheetFound, SF, SPF, PF;
		double KritBB;
		float list[8], cstat[7];

		// Brute Find A4

		BruteScan(12000, 350, cstat, list, KritBB, SheetFound, PF, SPF);

		for (int32_t i = 0; i < RandomCount; i++)
			arandom[i] = (rand()) / double(RAND_MAX);
		float B_Krits[4];
		float lstat[8];

		// Make Best Edge

		for (int i = 0; i < 8; i++)
			lstat[i] = list[(i + 2) % 8];
		MakeBest(lstat, 0.2, B_Krits);
		for (int i = 0; i < 8; i++)
			list[i] = lstat[i];

		for (int i = 0; i < 8; i++)
			lstat[i] = list[(i + 2) % 8];
		MakeBest(lstat, 0.11, B_Krits);
		for (int i = 0; i < 8; i++)
			list[i] = lstat[i];

		for (int i = 0; i < 8; i++)
			lstat[i] = list[(i + 2) % 8];
		MakeBest(lstat, 0.075, B_Krits);
		for (int i = 0; i < 8; i++)
			list[i] = lstat[i];

		for (int i = 0; i < 8; i++)
			lstat[i] = list[(i + 2) % 8];
		MakeBest(lstat, 0.05, B_Krits);
		for (int i = 0; i < 8; i++)
			list[i] = lstat[i];

		double xAb = list[0];
		double xBb = list[2];
		double xCb = list[4];
		double xDb = list[6];

		double yAb = list[1];
		double yBb = list[3];
		double yCb = list[5];
		double yDb = list[7];

		// Get Firs Krit of Qualiti

		B_Krits[0] = GetKrit(Vector2(xAb, yAb), Vector2(xBb, yBb), 0.05);
		B_Krits[1] = GetKrit(Vector2(xBb, yBb), Vector2(xCb, yCb), 0.05);
		B_Krits[2] = GetKrit(Vector2(xCb, yCb), Vector2(xDb, yDb), 0.05);
		B_Krits[3] = GetKrit(Vector2(xDb, yDb), Vector2(xAb, yAb), 0.05);

		float K1 = MAX_4(B_Krits[0], B_Krits[1], B_Krits[2], B_Krits[3]);
		float K2 = MIN_4(MAX_3(B_Krits[0], B_Krits[1], B_Krits[2]), MAX_3(B_Krits[0], B_Krits[1], B_Krits[3]), MAX_3(B_Krits[0], B_Krits[3], B_Krits[2]), MAX_3(B_Krits[3], B_Krits[1], B_Krits[2]));
		float K3 = MAX_4(MIN_3(B_Krits[0], B_Krits[1], B_Krits[2]), MIN_3(B_Krits[0], B_Krits[1], B_Krits[3]), MIN_3(B_Krits[0], B_Krits[3], B_Krits[2]), MIN_3(B_Krits[3], B_Krits[1], B_Krits[2]));
		float K4 = MIN_4(B_Krits[0], B_Krits[1], B_Krits[2], B_Krits[3]);
		float K5 = (B_Krits[0] + B_Krits[1] + B_Krits[2] + B_Krits[3]) / 4.0;

		int32_t IsBadList = ((K4 < 15) ? 1 : 0);

		// Get Kit of Blurr

		float B_Krits2[4]{ 100,100,100,100 };
		if (IsBadList == 0)
		{
			B_Krits2[0] = GetKrit2(Vector2(xAb, yAb), Vector2(xBb, yBb), Vector2(xCb, yCb), Vector2(xDb, yDb));
			B_Krits2[1] = GetKrit2(Vector2(xDb, yDb), Vector2(xAb, yAb), Vector2(xBb, yBb), Vector2(xCb, yCb));
			B_Krits2[2] = GetKrit2(Vector2(xCb, yCb), Vector2(xDb, yDb), Vector2(xAb, yAb), Vector2(xBb, yBb));
			B_Krits2[3] = GetKrit2(Vector2(xBb, yBb), Vector2(xCb, yCb), Vector2(xDb, yDb), Vector2(xAb, yAb));
		}

		// Says

		float x = (xAb + xBb + xCb + xDb) / 4.0, y = (yAb + yBb + yCb + yDb) / 4.0;
		if ((abs(x - immaxX / 2.0) > (immaxX * 0.1)) || (abs(y - immaxY / 2.0) > (immaxY * 0.12)))  NotCenteredSheet = 1;

		if (KadrNumber == 1) if (SPF == 0) SayNotCenteredFoot = 1;

		float K21 = MAX_4(B_Krits2[0], B_Krits2[1], B_Krits2[2], B_Krits2[3]);
		float K22 = MIN_4(MAX_3(B_Krits2[0], B_Krits2[1], B_Krits2[2]), MAX_3(B_Krits2[0], B_Krits2[1], B_Krits2[3]), MAX_3(B_Krits2[0], B_Krits2[3], B_Krits2[2]), MAX_3(B_Krits2[3], B_Krits2[1], B_Krits2[2]));
		float K23 = MAX_4(MIN_3(B_Krits2[0], B_Krits2[1], B_Krits2[2]), MIN_3(B_Krits2[0], B_Krits2[1], B_Krits2[3]), MIN_3(B_Krits2[0], B_Krits2[3], B_Krits2[2]), MIN_3(B_Krits2[3], B_Krits2[1], B_Krits2[2]));
		float K24 = MIN_4(B_Krits2[0], B_Krits2[1], B_Krits2[2], B_Krits2[3]);
		float K25 = (B_Krits2[0] + B_Krits2[1] + B_Krits2[2] + B_Krits2[3]) / 4.0;

		int32_t IsBlurred = ((K3 < 18 || K4 < 18 || K5 < 28 || K22 > 4.0) ? 1 : 0), IsProbBlurred = ((K3 < 40 || K4 < 32 || K5 < 42 || K22 > 3) ? 1 : 0);
		SheetFound = SPF;

		if (IsBlurred == 1) SheetFound = 0;
		if (NotCenteredSheet == 1) SheetFound = 0;
		if (IsBlurred == 1) SPF = 0;
		if (NotCenteredSheet == 1) SPF = 0;
		if (IsBlurred == 1) PF = 0;
		if (NotCenteredSheet == 1) PF = 0;

#pragma region Do poisk
		for (int32_t i = 0; i < RandomCount; i++)
			arandom[i] = (rand()) / double(RAND_MAX);

		double out[30][10];

		// Get NewCampos

		float** cam = GetNewCamPos(immaxY, immaxX, xAb, yAb, xBb, yBb, xCb, yCb, xDb, yDb);

		for (int32_t i = 0; i < 30; i++)
			for (int32_t j = 0; j < 10; j++)
				out[i][j] = 0;

		out[0][3] = cam[0][3];
		for (int32_t i = 0; i < 8; i++)for (int32_t j = 0; j < 3; j++) out[i][j] = cam[i][j];
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

		// Корректирую альфа, в зависимости от принятого номера кадра

		int32_t k = 0;
		int32_t Kadr = KadrNumber;

		// UpdOreint

		if (IsBadList == 0)
		{
			k = -UpdateOreint(Vector3(xAb, yAb, 0), Vector3(xBb, yBb, 0), Vector3(xCb, yCb, 0), Vector3(xDb, yDb, 0), cam);
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

		//if ((KadrNumber == 2) || (KadrNumber == 6))
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

		out[12][0] = K1;
		out[12][1] = K2;
		out[12][2] = K3;
		out[12][3] = K4;
		out[12][4] = K5;
		out[13][0] = IsBlurred;
		out[13][1] = IsProbBlurred;
		out[13][2] = IsBadList;
		out[14][0] = SheetFound;
		out[14][1] = SPF;
		out[14][2] = PF;
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

		if (inout != 0)
		{
			inout[0][3] = cam[0][3];
			for (int32_t i = 0; i < 8; i++)for (int32_t j = 0; j < 3; j++) inout[i][j] = cam[i][j];
			inout[8][0] = xAb; inout[8][1] = yAb;
			inout[9][0] = xBb; inout[9][1] = yBb;
			inout[10][0] = xCb; inout[10][1] = yCb;
			inout[11][0] = xDb; inout[11][1] = yDb;
			inout[12][0] = K1;
			inout[12][1] = K2;
			inout[12][2] = K3;
			inout[12][3] = K4;
			inout[12][4] = K5;
			inout[13][0] = IsBlurred;
			inout[13][1] = IsProbBlurred;
			inout[13][2] = IsBadList;
			inout[14][0] = SheetFound;
			inout[14][1] = SPF;
			inout[14][2] = PF;
			inout[15][0] = Kadr;
			inout[16][0] = SayShiftLeft_IfL;
			inout[16][1] = SayShiftRight_IfL;
			inout[16][2] = SayShiftLeft_IfR;
			inout[16][3] = SayShiftRight_IfR;
			inout[16][4] = SayShiftDown;
			inout[16][5] = SayShiftUp;
			inout[16][6] = SayMoveCloser;
			inout[16][7] = SayMoveFurther;
			inout[16][8] = SayPointCameraDown;
			inout[17][0] = K21;
			inout[17][1] = K22;
			inout[17][2] = K23;
			inout[17][3] = K24;
			inout[17][4] = K25;
		}

		std::string Ret = "";
#ifdef __TEST__
		Ret = Ret
			+ "Blur " + std::to_string(IsBlurred) + ' '
			+ "PBlur " + std::to_string(IsProbBlurred) + ' '
			+ "Found " + std::to_string(SheetFound) + ' '
			+ "Bad " + std::to_string(IsBadList) + ' '
			+ "\nK1 " + std::to_string(B_Krits[0]) + ' '
			+ "K2 " + std::to_string(B_Krits[1]) + ' '
			+ "K3 " + std::to_string(B_Krits[2]) + ' '
			+ "K4 " + std::to_string(B_Krits[3]) + ' '
			+ "\nS1 " + std::to_string(B_Krits2[0]) + ' '
			+ "S2 " + std::to_string(B_Krits2[1]) + ' '
			+ "S3 " + std::to_string(B_Krits2[2]) + ' '
			+ "S4 " + std::to_string(B_Krits2[3]) + ' '
			+ "\nAlfa " + std::to_string(AAlfa)
			+ "Beta " + std::to_string(ABeta)
			+ "Gamma " + std::to_string(AGamma)
			+ "\nSayShiftLeft_IfL " + std::to_string(SayShiftLeft_IfL)
			+ ", SayShiftRight_IfL " + std::to_string(SayShiftRight_IfL)
			+ ", SayShiftLeft_IfR " + std::to_string(SayShiftLeft_IfR)
			+ ", SayShiftRight_IfR " + std::to_string(SayShiftRight_IfR)
			+ ", SayShiftLeftBehindHeel_IfL " + std::to_string(SayShiftLeftBehindHeel_IfL)
			+ ", SayShiftRightBehindHeel_IfL " + std::to_string(SayShiftRightBehindHeel_IfL)
			+ ", SayShiftLeftBehindHeel_IfR " + std::to_string(SayShiftLeftBehindHeel_IfR)
			+ ", SayShiftRightBehindHeel_IfR " + std::to_string(SayShiftRightBehindHeel_IfR)
			+ ", SayShiftDown " + std::to_string(SayShiftDown)
			+ ", SayShiftUp " + std::to_string(SayShiftUp)
			+ "\nOld SayTurnLeft " + std::to_string(0)
			+ ", SayTurnRight " + std::to_string(0)
			+ ", SayTurnDown " + std::to_string(0)
			+ ", SayTurnUp " + std::to_string(0)
			+ ", SayRotateClockwise " + std::to_string(0)
			+ ", SayRotateCounterClockwise " + std::to_string(0)
			+ ", SayMoveCloser " + std::to_string(SayMoveCloser)
			+ ", SayMoveFurther " + std::to_string(SayMoveFurther)
			+ ", SayPointCameraDown " + std::to_string(SayPointCameraDown)
			+ ", SayCrumpledSheet " + std::to_string(SayCrumpledSheet)
			+ ", SayNotCenteredFoot " + std::to_string(SayNotCenteredFoot)
			+ ", SayLowSock " + std::to_string(SayLowSock)
			+ ", SayLowPants " + std::to_string(SayLowPants)
			+ ", SayCurledFingers " + std::to_string(SayCurledFingers)
			+ "\nx1 " + std::to_string(xAb)
			+ ", y1 " + std::to_string(yAb)
			+ ", x2 " + std::to_string(xBb)
			+ ", y2 " + std::to_string(yBb)
			+ ", x3 " + std::to_string(xCb)
			+ ", y3 " + std::to_string(yCb)
			+ ", x4 " + std::to_string(xDb)
			+ ", y4 " + std::to_string(yDb)
			+ ", NotCenteredSheet " + std::to_string(NotCenteredSheet)
			// New Tags
			+ ", IsBlurred " + std::to_string(IsBlurred)
			+ ", IsProbBlurred " + std::to_string(IsProbBlurred)
			+ ", IsBadList " + std::to_string(IsBadList)
			// Old Tags

			// MY Tags
			+", KadrNumber " + std::to_string(KadrNumber)
			+ ", xK " + std::to_string(out[0][0])
			+ ", yK " + std::to_string(out[0][1])
			+ ", zK " + std::to_string(out[0][2])
			+ ", equivalentFocalLength " + std::to_string(out[0][3])
			+ ", KXx " + std::to_string(out[1][0])
			+ ", KXy " + std::to_string(out[1][1])
			+ ", KXz " + std::to_string(out[1][2])
			+ ", KYx " + std::to_string(out[2][0])
			+ ", KYy " + std::to_string(out[2][1])
			+ ", KYz " + std::to_string(out[2][2])
			+ ", KZx " + std::to_string(out[3][0])
			+ ", KZy " + std::to_string(out[3][1])
			+ ", KZz " + std::to_string(out[3][2])
			+ ", KritBorder1 " + std::to_string(B_Krits[0])
			+ ", KritBorder2 " + std::to_string(B_Krits[1])
			+ ", KritBorder3 " + std::to_string(B_Krits[2])
			+ ", KritBorder4 " + std::to_string(B_Krits[3])
			+ '\n';
#else
		Ret = Ret + "{"
			+ "\"x1\":" + std::to_string(xAb)
			+ ",\"y1\":" + std::to_string(yAb)
			+ ",\"x2\":" + std::to_string(xBb)
			+ ",\"y2\":" + std::to_string(yBb)
			+ ",\"x3\":" + std::to_string(xCb)
			+ ",\"y3\":" + std::to_string(yCb)
			+ ",\"x4\":" + std::to_string(xDb)
			+ ",\"y4\":" + std::to_string(yDb)
			+ ",\"SheetFound\":" + std::to_string(SheetFound)
			+ ",\"SheetProbablyFound\":" + std::to_string(SPF)
			+ ",\"SheetFoundProbability\":" + std::to_string(PF)
			+ ",\"Alfa\":" + std::to_string(AAlfa)
			+ ",\"Beta\":" + std::to_string(ABeta)
			+ ",\"Gamma\":" + std::to_string(AGamma)
			+ ",\"SayShiftLeft_IfL\":" + std::to_string(SayShiftLeft_IfL)
			+ ",\"SayShiftRight_IfL\":" + std::to_string(SayShiftRight_IfL)
			+ ",\"SayShiftLeft_IfR\":" + std::to_string(SayShiftLeft_IfR)
			+ ",\"SayShiftRight_IfR\":" + std::to_string(SayShiftRight_IfR)
			+ ",\"SayShiftLeftBehindHeel_IfL\":" + std::to_string(SayShiftLeftBehindHeel_IfL)
			+ ",\"SayShiftRightBehindHeel_IfL\":" + std::to_string(SayShiftRightBehindHeel_IfL)
			+ ",\"SayShiftLeftBehindHeel_IfR\":" + std::to_string(SayShiftLeftBehindHeel_IfR)
			+ ",\"SayShiftRightBehindHeel_IfR\":" + std::to_string(SayShiftRightBehindHeel_IfR)
			+ ",\"SayShiftDown\":" + std::to_string(SayShiftDown)
			+ ",\"SayShiftUp\":" + std::to_string(SayShiftUp)
			+ ",\"SayRotateDown\":" + std::to_string(SayRotateDown)
			+ ",\"SayRotateUp\":" + std::to_string(SayRotateUp)
			+ ",\"SayMoveCloser\":" + std::to_string(SayMoveCloser)
			+ ",\"SayMoveFurther\":" + std::to_string(SayMoveFurther)
			+ ",\"SayPointCameraDown\":" + std::to_string(SayPointCameraDown)
			+ ",\"NotCenteredSheet\":" + std::to_string(NotCenteredSheet)
			+ ",\"SayCrumpledSheet\":" + std::to_string(SayCrumpledSheet)
			+ ",\"SayNotCenteredFoot\":" + std::to_string(SayNotCenteredFoot)
			+ ",\"SayLowSock\":" + std::to_string(SayLowSock)
			+ ",\"SayLowPants\":" + std::to_string(SayLowPants)
			+ ",\"SayCurledFingers\":" + std::to_string(SayCurledFingers)
			// New Tags
			+ ",\"IsBlurred\":" + std::to_string(IsBlurred)
			+ ",\"IsProbBlurred\":" + std::to_string(IsProbBlurred)
			+ ",\"IsBadList\":" + std::to_string(IsBadList)
			// Old Tags
			+ ",\"SayTurnLeft\":" + std::to_string(0)
			+ ",\"SayTurnRight\":" + std::to_string(0)
			+ ",\"SayTurnDown\":" + std::to_string(0)
			+ ",\"SayTurnUp\":" + std::to_string(0)
			+ ",\"SayRotateClockwise\":" + std::to_string(0)
			+ ",\"SayRotateCounterClockwise\":" + std::to_string(0)
			// MY Tags
			+ ",\"KadrNumber\":" + std::to_string(KadrNumber)
			+ ",\"xK\":" + std::to_string(out[0][0])
			+ ",\"yK\":" + std::to_string(out[0][1])
			+ ",\"zK\":" + std::to_string(out[0][2])
			+ ",\"equivalentFocalLength\":" + std::to_string(out[0][3])
			+ ",\"KXx\":" + std::to_string(out[1][0])
			+ ",\"KXy\":" + std::to_string(out[1][1])
			+ ",\"KXz\":" + std::to_string(out[1][2])
			+ ",\"KYx\":" + std::to_string(out[2][0])
			+ ",\"KYy\":" + std::to_string(out[2][1])
			+ ",\"KYz\":" + std::to_string(out[2][2])
			+ ",\"KZx\":" + std::to_string(out[3][0])
			+ ",\"KZy\":" + std::to_string(out[3][1])
			+ ",\"KZz\":" + std::to_string(out[3][2])
			+ ",\"KritBorder1\":" + std::to_string(B_Krits[0])
			+ ",\"KritBorder2\":" + std::to_string(B_Krits[1])
			+ ",\"KritBorder3\":" + std::to_string(B_Krits[2])
			+ ",\"KritBorder4\":" + std::to_string(B_Krits[3])
			+ "}";
#endif // __TEST__	

		{
			for (int32_t i = 0; i < 30; i++)
			{
				if (inout != 0)
					for (int32_t j = 0; j < 10; j++) inout[i][j] = out[i][j];
			}
		}


		for (int32_t i = 0; i < 8; i++)delete[] cam[i];
		delete[] cam;


		return Ret;
	};

	std::string Finder::GetCorner(float* olist, int32_t* oisbadlist)
	{
		int32_t orienta, SheetFound, SF, SPF, PF;
		double KritBB;
		float list[8], cstat[7];

		// Brute Find A4

		BruteScan(12000, 350, cstat, list, KritBB, SheetFound, PF, SPF);

		for (int32_t i = 0; i < RandomCount; i++)
			arandom[i] = (rand()) / double(RAND_MAX);
		float B_Krits[4];
		float lstat[8];

		// Make Best Edge

		for (int i = 0; i < 8; i++)
			lstat[i] = list[(i + 2) % 8];
		MakeBest(lstat, 0.2, B_Krits);
		for (int i = 0; i < 8; i++)
			list[i] = lstat[i];

		for (int i = 0; i < 8; i++)
			lstat[i] = list[(i + 2) % 8];
		MakeBest(lstat, 0.11, B_Krits);
		for (int i = 0; i < 8; i++)
			list[i] = lstat[i];

		for (int i = 0; i < 8; i++)
			lstat[i] = list[(i + 2) % 8];
		MakeBest(lstat, 0.075, B_Krits);
		for (int i = 0; i < 8; i++)
			list[i] = lstat[i];

		for (int i = 0; i < 8; i++)
			lstat[i] = list[(i + 2) % 8];
		MakeBest(lstat, 0.05, B_Krits);
		for (int i = 0; i < 8; i++)
			list[i] = lstat[i];

		double xAb = list[0];
		double xBb = list[2];
		double xCb = list[4];
		double xDb = list[6];

		double yAb = list[1];
		double yBb = list[3];
		double yCb = list[5];
		double yDb = list[7];

		// Get Firs Krit of Qualiti

		B_Krits[0] = GetKrit(Vector2(xAb, yAb), Vector2(xBb, yBb), 0.05);
		B_Krits[1] = GetKrit(Vector2(xBb, yBb), Vector2(xCb, yCb), 0.05);
		B_Krits[2] = GetKrit(Vector2(xCb, yCb), Vector2(xDb, yDb), 0.05);
		B_Krits[3] = GetKrit(Vector2(xDb, yDb), Vector2(xAb, yAb), 0.05);

		float K1 = MAX_4(B_Krits[0], B_Krits[1], B_Krits[2], B_Krits[3]);
		float K2 = MIN_4(MAX_3(B_Krits[0], B_Krits[1], B_Krits[2]), MAX_3(B_Krits[0], B_Krits[1], B_Krits[3]), MAX_3(B_Krits[0], B_Krits[3], B_Krits[2]), MAX_3(B_Krits[3], B_Krits[1], B_Krits[2]));
		float K3 = MAX_4(MIN_3(B_Krits[0], B_Krits[1], B_Krits[2]), MIN_3(B_Krits[0], B_Krits[1], B_Krits[3]), MIN_3(B_Krits[0], B_Krits[3], B_Krits[2]), MIN_3(B_Krits[3], B_Krits[1], B_Krits[2]));
		float K4 = MIN_4(B_Krits[0], B_Krits[1], B_Krits[2], B_Krits[3]);
		float K5 = (B_Krits[0] + B_Krits[1] + B_Krits[2] + B_Krits[3]) / 4.0;

		int32_t IsBadList = ((K4 < 15) ? 1 : 0);

		std::string Ret = "";
		Ret = Ret + "{"
			+ "\"IsBadList\":" + std::to_string(IsBadList)
			+ "\"x1\":" + std::to_string(xAb)
			+ ",\"y1\":" + std::to_string(yAb)
			+ ",\"x2\":" + std::to_string(xBb)
			+ ",\"y2\":" + std::to_string(yBb)
			+ ",\"x3\":" + std::to_string(xCb)
			+ ",\"y3\":" + std::to_string(yCb)
			+ ",\"x4\":" + std::to_string(xDb)
			+ ",\"y4\":" + std::to_string(yDb)
			+ '}';

		if (olist != 0)
		{
			for (int i = 0; i < 8; i++)
				olist[i] = list[i];
		}

		if (oisbadlist != 0)
			*oisbadlist = IsBadList;

		if (inout != 0)
		{
			inout[8][0] = xAb; inout[8][1] = yAb;
			inout[9][0] = xBb; inout[9][1] = yBb;
			inout[10][0] = xCb; inout[10][1] = yCb;
			inout[11][0] = xDb; inout[11][1] = yDb;
			inout[12][0] = K1;
			inout[12][1] = K2;
			inout[12][2] = K3;
			inout[12][3] = K4;
			inout[13][2] = IsBadList;
			inout[14][0] = SheetFound;
			inout[14][1] = SPF;
			inout[14][2] = PF;
		}

		return Ret;
	}

	std::string Finder::GetCams(float* list, float** ocamspace, bool reinit)
	{

		double xAb = 0;
		double xBb = 0;
		double xCb = 0;
		double xDb = 0;

		double yAb = 0;
		double yBb = 0;
		double yCb = 0;
		double yDb = 0;

		int32_t IsBadList = 0;

		if (reinit || list == 0)
		{
			int32_t orienta, SheetFound, SF, SPF, PF;
			double KritBB;
			float list[8];
			IsBadList = 1;
			for (int Q = 0; Q < 10 && IsBadList == 1; Q++)
			{
				GetCorner(list, &IsBadList);
			}
			xAb = list[0];
			xBb = list[2];
			xCb = list[4];
			xDb = list[6];

			yAb = list[1];
			yBb = list[3];
			yCb = list[5];
			yDb = list[7];
		}
		else
		{
			xAb = list[0];
			xBb = list[2];
			xCb = list[4];
			xDb = list[6];

			yAb = list[1];
			yBb = list[3];
			yCb = list[5];
			yDb = list[7];
		}

		if (IsBadList)
			return "{\"Error\" : \"A4_Not_Find\"}";

		float** cam = GetNewCamPos(immaxY, immaxX, xAb, yAb, xBb, yBb, xCb, yCb, xDb, yDb);

		// Определяем пространственные углы
		double AAlfa = atan(cam[0][1] / cam[0][0] + 1e-20) / PI * 180;
		if ((cam[0][0] + 1e-20) < 0) AAlfa = AAlfa + 180;
		if ((cam[0][0] + 1e-20) < 0 && (cam[0][1] < 0)) AAlfa = AAlfa - 360;
		double r = sqrt(cam[0][0] * cam[0][0] + cam[0][1] * cam[0][1]);
		double ABeta = atan(cam[0][2] / (r + 1e-20)) / PI * 180;
		Vector3 Z(cam[3][0], cam[3][1], cam[3][2]), Y(0, 0, 1); Y = Z / Y;
		Y.SetLenght(1);
		Vector3 X(cam[1][0], cam[1][1], cam[1][2]);
		X.SetLenght(1);
		r = X.GetLenght();
		r = Y.GetLenght();
		double AGamma = X * Y;
		if (AGamma < 1)
			AGamma = acos(AGamma) / PI * 180.0;
		else
			AGamma = 0;

		float CBeta = acos(abs(cam[3][2])) / PI * 180;


		std::string Ret = "";
		Ret = Ret + "{"
			+ "\"Alfa\":" + std::to_string(AAlfa)
			+ ",\"Beta\":" + std::to_string(ABeta)
			+ ",\"Gamma\":" + std::to_string(AGamma)
			+ ",\"xK\":" + std::to_string(cam[0][0])
			+ ",\"yK\":" + std::to_string(cam[0][1])
			+ ",\"zK\":" + std::to_string(cam[0][2])
			+ ",\"equivalentFocalLength\":" + std::to_string(cam[0][3])
			+ ",\"KXx\":" + std::to_string(cam[1][0])
			+ ",\"KXy\":" + std::to_string(cam[1][1])
			+ ",\"KXz\":" + std::to_string(cam[1][2])
			+ ",\"KYx\":" + std::to_string(cam[2][0])
			+ ",\"KYy\":" + std::to_string(cam[2][1])
			+ ",\"KYz\":" + std::to_string(cam[2][2])
			+ ",\"KZx\":" + std::to_string(cam[3][0])
			+ ",\"KZy\":" + std::to_string(cam[3][1])
			+ ",\"KZz\":" + std::to_string(cam[3][2])
			+ '}';

		if (ocamspace != 0)
		{
			for (int j = 0; j < 4; j++)
				ocamspace[0][j] = cam[0][j];
			for (int i = 1; i < 8; i++)
				for (int j = 0; j < 3; j++)
					ocamspace[i][j] = cam[i][j];
		}

		if (inout != 0)
		{
			inout[0][3] = cam[0][3];
			for (int32_t i = 0; i < 8; i++)for (int32_t j = 0; j < 3; j++) inout[i][j] = cam[i][j];
			inout[8][0] = xAb; inout[8][1] = yAb;
			inout[9][0] = xBb; inout[9][1] = yBb;
			inout[10][0] = xCb; inout[10][1] = yCb;
			inout[11][0] = xDb; inout[11][1] = yDb;
			inout[13][2] = IsBadList;
		}

		for (int32_t i = 0; i < 8; i++)delete[] cam[i];
		delete[] cam;

		return Ret;
	}

	std::string Finder::GetKrit(float* list, float* oKrits, bool reinit)
	{

		double xAb = 0;
		double xBb = 0;
		double xCb = 0;
		double xDb = 0;

		double yAb = 0;
		double yBb = 0;
		double yCb = 0;
		double yDb = 0;

		int32_t IsBadList = 0;

		if (reinit || list == 0)
		{
			int32_t orienta, SheetFound, SF, SPF, PF;
			double KritBB;
			float list[8];
			IsBadList = 1;
			for (int Q = 0; Q < 10 && IsBadList == 1; Q++)
			{
				GetCorner(list, &IsBadList);
			}
			xAb = list[0];
			xBb = list[2];
			xCb = list[4];
			xDb = list[6];

			yAb = list[1];
			yBb = list[3];
			yCb = list[5];
			yDb = list[7];
		}
		else
		{
			xAb = list[0];
			xBb = list[2];
			xCb = list[4];
			xDb = list[6];

			yAb = list[1];
			yBb = list[3];
			yCb = list[5];
			yDb = list[7];
		}

		if (IsBadList)
			return "{\"Error\" : \"A4_Not_Find\"}";

		float B_Krits2[4];

		B_Krits2[0] = GetKrit2(Vector2(xAb, yAb), Vector2(xBb, yBb), Vector2(xCb, yCb), Vector2(xDb, yDb));
		B_Krits2[1] = GetKrit2(Vector2(xDb, yDb), Vector2(xAb, yAb), Vector2(xBb, yBb), Vector2(xCb, yCb));
		B_Krits2[2] = GetKrit2(Vector2(xCb, yCb), Vector2(xDb, yDb), Vector2(xAb, yAb), Vector2(xBb, yBb));
		B_Krits2[3] = GetKrit2(Vector2(xBb, yBb), Vector2(xCb, yCb), Vector2(xDb, yDb), Vector2(xAb, yAb));

		float K21 = MAX_4(B_Krits2[0], B_Krits2[1], B_Krits2[2], B_Krits2[3]);
		float K22 = MIN_4(MAX_3(B_Krits2[0], B_Krits2[1], B_Krits2[2]), MAX_3(B_Krits2[0], B_Krits2[1], B_Krits2[3]), MAX_3(B_Krits2[0], B_Krits2[3], B_Krits2[2]), MAX_3(B_Krits2[3], B_Krits2[1], B_Krits2[2]));
		float K23 = MAX_4(MIN_3(B_Krits2[0], B_Krits2[1], B_Krits2[2]), MIN_3(B_Krits2[0], B_Krits2[1], B_Krits2[3]), MIN_3(B_Krits2[0], B_Krits2[3], B_Krits2[2]), MIN_3(B_Krits2[3], B_Krits2[1], B_Krits2[2]));
		float K24 = MIN_4(B_Krits2[0], B_Krits2[1], B_Krits2[2], B_Krits2[3]);
		float K25 = (B_Krits2[0] + B_Krits2[1] + B_Krits2[2] + B_Krits2[3]) / 4.0;

		int32_t IsBlurred = K22 > 4.0 ? 1 : 0, IsProbBlurred = K22 > 3 ? 1 : 0;

		if (oKrits != 0)
		{
			oKrits[0] = K21;
			oKrits[1] = K22;
			oKrits[2] = K23;
			oKrits[3] = K24;
			oKrits[4] = K25;
		}

		std::string Ret = "";
		Ret = Ret + "{"
			+ "\"IsBlurred\":" + std::to_string(IsBlurred)
			+ ",\"IsProbBlurred\":" + std::to_string(IsProbBlurred)
			+ "K1 " + std::to_string(K21) + ' '
			+ "K2 " + std::to_string(K22) + ' '
			+ "K3 " + std::to_string(K23) + ' '
			+ "K4 " + std::to_string(K24) + ' '
			+ "K5 " + std::to_string(K25)
			+ '}';

		if (inout != 0)
		{
			inout[8][0] = xAb; inout[8][1] = yAb;
			inout[9][0] = xBb; inout[9][1] = yBb;
			inout[10][0] = xCb; inout[10][1] = yCb;
			inout[11][0] = xDb; inout[11][1] = yDb;
			inout[13][0] = IsBlurred;
			inout[13][1] = IsProbBlurred;
			inout[13][2] = IsBadList;
			inout[17][0] = K21;
			inout[17][1] = K22;
			inout[17][2] = K23;
			inout[17][3] = K24;
			inout[17][4] = K25;
		}
		return Ret;
	}

	std::string Finder::UpdateOrientedCams(float* list, float** cam, bool reinit)
	{
		int32_t IsBadList = 1;

		double xAb = 0;
		double xBb = 0;
		double xCb = 0;
		double xDb = 0;

		double yAb = 0;
		double yBb = 0;
		double yCb = 0;
		double yDb = 0;


		if (reinit || cam == 0)
		{
			reinit = true;
			cam = 0;
			cam = new float* [8];
			for (int i = 0; i < 8; i++)
				cam[i] = new float[4];

			IsBadList = 0;

			for (int Q = 0; Q < 10 && IsBadList == 1; Q++)
			{
				GetCorner(list, &IsBadList);
			}

			if (IsBadList)
				return "{\"Error\" : \"A4_Not_Find\"}";

			GetCams(list, cam);

			xAb = list[0];
			xBb = list[2];
			xCb = list[4];
			xDb = list[6];

			yAb = list[1];
			yBb = list[3];
			yCb = list[5];
			yDb = list[7];
		}

		xAb = list[0];
		xBb = list[2];
		xCb = list[4];
		xDb = list[6];

		yAb = list[1];
		yBb = list[3];
		yCb = list[5];
		yDb = list[7];

		// Определяем пространственные углы
		double AAlfa = atan(cam[0][1] / cam[0][0] + 1e-20) / PI * 180;
		if ((cam[0][0] + 1e-20) < 0) AAlfa = AAlfa + 180;
		if ((cam[0][0] + 1e-20) < 0 && (cam[0][1] < 0)) AAlfa = AAlfa - 360;
		double r = sqrt(cam[0][0] * cam[0][0] + cam[0][1] * cam[0][1]);
		double ABeta = atan(cam[0][2] / (r + 1e-20)) / PI * 180;
		Vector3 Z(cam[3][0], cam[3][1], cam[3][2]), Y(0, 0, 1); Y = Z / Y;
		Y.SetLenght(1);
		Vector3 X(cam[1][0], cam[1][1], cam[1][2]);
		X.SetLenght(1);
		r = X.GetLenght();
		r = Y.GetLenght();
		double AGamma = X * Y;
		if (AGamma < 1)
			AGamma = acos(AGamma) / PI * 180.0;
		else
			AGamma = 0;

		float CBeta = acos(abs(cam[3][2])) / PI * 180;

		// Корректирую альфа, в зависимости от принятого номера кадра

		int32_t k = 0;
		int32_t Kadr;
		k = -UpdateOreint(Vector3(xAb, yAb, 0), Vector3(xBb, yBb, 0), Vector3(xCb, yCb, 0), Vector3(xDb, yDb, 0), cam);
		if (AAlfa > 150 && KadrNumber != 1 && KadrNumber != 7) { k = 1; }
		else if (AAlfa < -150 && KadrNumber != 1 && KadrNumber != 7) { k = 1; }
		else if (AAlfa < 30 && AAlfa > -30 && (KadrNumber == 1 || KadrNumber == 7)) { k = 1; }
		if (k == 1)
		{
			r = xAb; xAb = xCb; xCb = r;
			r = yAb; yAb = yCb; yCb = r;
			r = xBb; xBb = xDb; xDb = r;
			r = yBb; yBb = yDb; yDb = r;
			cam[0][0] *= -1; cam[0][1] *= -1; cam[1][0] *= -1; cam[1][1] *= -1; cam[2][0] *= -1; cam[2][1] *= -1; cam[3][0] *= -1; cam[3][1] *= -1;
			if (AAlfa > 0)	AAlfa -= 180.0;
			else			AAlfa += 180.0;
		}


		std::string Ret = "";
		Ret = Ret + "{"
			+ "\"Alfa\":" + std::to_string(AAlfa)
			+ ",\"Beta\":" + std::to_string(ABeta)
			+ ",\"Gamma\":" + std::to_string(AGamma)
			+ ",\"xK\":" + std::to_string(cam[0][0])
			+ ",\"yK\":" + std::to_string(cam[0][1])
			+ ",\"zK\":" + std::to_string(cam[0][2])
			+ ",\"equivalentFocalLength\":" + std::to_string(cam[0][3])
			+ ",\"KXx\":" + std::to_string(cam[1][0])
			+ ",\"KXy\":" + std::to_string(cam[1][1])
			+ ",\"KXz\":" + std::to_string(cam[1][2])
			+ ",\"KYx\":" + std::to_string(cam[2][0])
			+ ",\"KYy\":" + std::to_string(cam[2][1])
			+ ",\"KYz\":" + std::to_string(cam[2][2])
			+ ",\"KZx\":" + std::to_string(cam[3][0])
			+ ",\"KZy\":" + std::to_string(cam[3][1])
			+ ",\"KZz\":" + std::to_string(cam[3][2])
			+ '}';


		if (inout != 0)
		{
			inout[0][3] = cam[0][3];
			for (int32_t i = 0; i < 8; i++)for (int32_t j = 0; j < 3; j++) inout[i][j] = cam[i][j];
			inout[8][0] = xAb; inout[8][1] = yAb;
			inout[9][0] = xBb; inout[9][1] = yBb;
			inout[10][0] = xCb; inout[10][1] = yCb;
			inout[11][0] = xDb; inout[11][1] = yDb;
			inout[13][2] = IsBadList;
		}
		if (reinit)
		{
			for (int32_t i = 0; i < 8; i++)
				delete[] cam[i];
			delete[] cam;
		}

		return Ret;
	}

	std::string Finder::GetTips(float* list/*float[8]*/, float** cam/*float[8][4]*/, uint8_t* otips, bool reinit)
	{
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

		double xAb = list[0];
		double xBb = list[2];
		double xCb = list[4];
		double xDb = list[6];

		double yAb = list[1];
		double yBb = list[3];
		double yCb = list[5];
		double yDb = list[7];


		double AAlfa = atan(cam[0][1] / cam[0][0] + 1e-20) / PI * 180;
		if ((cam[0][0] + 1e-20) < 0) AAlfa = AAlfa + 180;
		if ((cam[0][0] + 1e-20) < 0 && (cam[0][1] < 0)) AAlfa = AAlfa - 360;
		double r = sqrt(cam[0][0] * cam[0][0] + cam[0][1] * cam[0][1]);
		double ABeta = atan(cam[0][2] / (r + 1e-20)) / PI * 180;
		Vector3 Z(cam[3][0], cam[3][1], cam[3][2]), Y(0, 0, 1); Y = Z / Y;
		Y.SetLenght(1);
		Vector3 X(cam[1][0], cam[1][1], cam[1][2]);
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

		//if ((KadrNumber == 2) || (KadrNumber == 6))
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

		float x = (xAb + xBb + xCb + xDb) / 4.0, y = (yAb + yBb + yCb + yDb) / 4.0;
		if ((abs(x - immaxX / 2.0) > (immaxX * 0.1)) || (abs(y - immaxY / 2.0) > (immaxY * 0.12)))  NotCenteredSheet = 1;

		if (inout != 0)
		{
			inout[16][0] = SayShiftLeft_IfL;
			inout[16][1] = SayShiftRight_IfL;
			inout[16][2] = SayShiftLeft_IfR;
			inout[16][3] = SayShiftRight_IfR;
			inout[16][4] = SayShiftDown;
			inout[16][5] = SayShiftUp;
			inout[16][6] = SayMoveCloser;
			inout[16][7] = SayMoveFurther;
			inout[16][8] = SayPointCameraDown;
		}

		if (otips != 0)
		{
			otips[0] = SayShiftLeft_IfL;
			otips[1] = SayShiftRight_IfL;
			otips[2] = SayShiftLeft_IfR;
			otips[3] = SayShiftRight_IfR;
			otips[4] = SayShiftLeftBehindHeel_IfL;
			otips[5] = SayShiftRightBehindHeel_IfL;
			otips[6] = SayShiftLeftBehindHeel_IfR;
			otips[7] = SayShiftRightBehindHeel_IfR;
			otips[8] = SayShiftDown;
			otips[9] = SayShiftUp;
			otips[10] = SayMoveCloser;
			otips[11] = SayMoveFurther;
			otips[12] = SayPointCameraDown;
		}

		std::string Ret = "";

		Ret = Ret + "{"
			+ "\"SayShiftLeft_IfL\":" + std::to_string(SayShiftLeft_IfL)
			+ ",\"SayShiftRight_IfL\":" + std::to_string(SayShiftRight_IfL)
			+ ",\"SayShiftLeft_IfR\":" + std::to_string(SayShiftLeft_IfR)
			+ ",\"SayShiftRight_IfR\":" + std::to_string(SayShiftRight_IfR)
			+ ",\"SayShiftLeftBehindHeel_IfL\":" + std::to_string(SayShiftLeftBehindHeel_IfL)
			+ ",\"SayShiftRightBehindHeel_IfL\":" + std::to_string(SayShiftRightBehindHeel_IfL)
			+ ",\"SayShiftLeftBehindHeel_IfR\":" + std::to_string(SayShiftLeftBehindHeel_IfR)
			+ ",\"SayShiftRightBehindHeel_IfR\":" + std::to_string(SayShiftRightBehindHeel_IfR)
			+ ",\"SayShiftDown\":" + std::to_string(SayShiftDown)
			+ ",\"SayShiftUp\":" + std::to_string(SayShiftUp)
			+ ",\"SayRotateDown\":" + std::to_string(SayRotateDown)
			+ ",\"SayRotateUp\":" + std::to_string(SayRotateUp)
			+ ",\"SayMoveCloser\":" + std::to_string(SayMoveCloser)
			+ ",\"SayMoveFurther\":" + std::to_string(SayMoveFurther)
			+ ",\"SayPointCameraDown\":" + std::to_string(SayPointCameraDown)
			+ ",\"NotCenteredSheet\":" + std::to_string(NotCenteredSheet)
			+ ",\"SayCrumpledSheet\":" + std::to_string(SayCrumpledSheet)
			+ ",\"SayNotCenteredFoot\":" + std::to_string(SayNotCenteredFoot)
			+ ",\"SayLowSock\":" + std::to_string(SayLowSock)
			+ ",\"SayLowPants\":" + std::to_string(SayLowPants)
			+ ",\"SayCurledFingers\":" + std::to_string(SayCurledFingers)
			// Old Tags
			+ ",\"SayTurnLeft\":" + std::to_string(0)
			+ ",\"SayTurnRight\":" + std::to_string(0)
			+ ",\"SayTurnDown\":" + std::to_string(0)
			+ ",\"SayTurnUp\":" + std::to_string(0)
			+ ",\"SayRotateClockwise\":" + std::to_string(0)
			+ ",\"SayRotateCounterClockwise\":" + std::to_string(0)
			+ '}';

		return Ret;
	}

	std::string Finder::FootCollor(float* list, uint8_t* out_I)
	{
		uint8_t I;

		const uint16_t diskret = 30;

		int16_t map[diskret][diskret];
		float avg = 0, h = 0;
		for (int x = 0; x < diskret; x++)
		{
			float _x = (x + 1) / (diskret + 1.0);
			for (int y = 0; y < diskret; y++)
			{
				float _y = (y + 1) / (diskret + 1.0);
				int32_t X = list[0] * (1 - _x) * (1 - _y) + list[2] * (_x) * (1 - _y) + list[4] * (_x) * (_y)+list[6] * (1 - _x) * (_y);
				int32_t Y = list[1] * (1 - _x) * (1 - _y) + list[3] * (_x) * (1 - _y) + list[5] * (_x) * (_y)+list[7] * (1 - _x) * (_y);

				if (((X < 1.5) || (X > immaxX - 2.5) || (Y < 1.5) || (Y > immaxY - 2.5)))
					map[x][y] = -1;
				else
				{
					map[x][y] = Get(Y, X);
				}

				//if (P[0] > 3 && P[1] > 3 && P[0] < immaxY - 5 && P[1] < immaxX - 5)
				//	t = img[int32_t(P[0])][int32_t(P[1])];
				if (map[x][y] >= 0)
				{
					avg += map[x][y];
					h++;
				}
			}
		}

		avg /= h;

		float avg_top = 0, avg_bot = 0;
		float h_top = 0, h_bot = 0;

		for (int x = 0; x < diskret; x++) for (int y = 0; y < diskret; y++)
		{
			if (map[x][y] >= 0)
			{
				if (map[x][y] > avg)
				{
					avg_top += map[x][y];
					h_top++;
				}
				if (map[x][y] < avg)
				{
					avg_bot += map[x][y];
					h_bot++;
				}
				
			}
		}

		avg_top /= h_top;
		avg_bot /= h_bot;

		std::string Ret;
		if(out_I != 0)
			*out_I = avg_bot;

		Ret = Ret 
			+ "{"
			+ "\"IsGray\":" + std::to_string(avg_bot > 50)
			+ ",\"avg\":" + std::to_string(avg)
			+ ",\"top\":" + std::to_string(avg_top)
			+ ",\"bot\":" + std::to_string(avg_bot)
			+ "}";



		return Ret;
	}

}

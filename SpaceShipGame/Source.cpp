#include<DxLib.h>
#include<bits/stdc++.h>

const int FieldX = 4000, FieldY = 4000;
const double TurnMove = 0.1, Acceleration = 0.12, Gravity = 0.5;
char Buf[256];
double VectorX, VectorY, Angle, BackX, BackY;
int SpaceShipPicture, SpacePicture, MiniMap;

void DrawShip();
void Init();


class Star {
public:
	double X, Y;
	int Radius, ColorRed, ColorGreen, ColorBlue;
	double Dist() {
		return sqrt((X - 320) * (X - 320) + (Y - 320) * (Y - 320));
	}
	void Draw() {
		DrawCircleAA(X, Y, Radius, 60, GetColor(ColorRed, ColorGreen, ColorBlue), TRUE);
	}
	void DrawMiniMap() {
		DrawCircleAA((X-320) / 10+320, (Y-320) / 10+320, Radius / 10, 60, GetColor(ColorRed, ColorGreen, ColorBlue), TRUE);
	}
	void Move() {
		X += VectorX;
		Y += VectorY;
		if (X >= FieldX)
			X -= FieldX * 2;
		if (X <= -FieldX)
			X += FieldX * 2;
		if (Y >= FieldY)
			Y -= FieldY * 2;
		if (Y <= -FieldY)
			Y += FieldY * 2;
	}
	void ShipMove() {
		double Distance = Dist();
		VectorX -= Gravity * (X - 320)*pow(1.005, -max(Distance,300.0)) / Distance;
		VectorY -= Gravity * (Y - 320)*pow(1.005, -max(Distance,300.0)) / Distance;
	}
	int Landing() {
		DrawFormatString(X, Y, GetColor(255, 255, 255), "%f", atan2(Y-300, X-300));
		if (Dist() <= Radius + 10) {
			if ((fmod(abs(atan2(Y - 300, X - 300) - Angle), DX_PI * 2) <= 0.5 || abs(fmod(DX_PI * 2 - abs(atan2(Y - 300, X - 300) - Angle), DX_PI * 2)) <= 0.5)&&sqrt(VectorX*VectorX + VectorY * VectorY) <= 2) {
				DrawFormatString(50, 50, GetColor(255, 150, 0), "Game Clear");
				DrawShip();
				ScreenFlip();
				WaitKey();
				Init();
				return 1;
			}
			else {
				DrawFormatString(50, 50, GetColor(255, 255, 255), "Game Over");
				ScreenFlip();
				WaitKey();
				Init();
				return -1;
			}
		}
		else return 0;
	}
};

void TurnRight() {
	Angle += TurnMove;
}
void TurnLeft() {
	Angle -= TurnMove;
}
void Accelerate() {
	VectorX += Acceleration * cos(Angle);
	VectorY += Acceleration * sin(Angle);
	for (int i = 0; i != 20; ++i) {
		DrawLineAA(320 + cos(Angle - DX_PI / 16)*(17 + i), 320 + sin(Angle - DX_PI / 16)*(17 + i), 320 + cos(Angle + DX_PI / 16)*(17 + i), 320 + sin(Angle + DX_PI / 16)*(17 + i), GetColor(255 - 10 * i, 60 - i * 2, 0));
	}
}
void DrawShip() {
	DrawRotaGraph(320, 320, 0.5, Angle, SpaceShipPicture, TRUE);
}
void DrawShipMiniMap() {
	DrawRotaGraph(320, 320, 0.3, Angle, SpaceShipPicture, TRUE);
}
void DrawBack() {
	DrawGraph(BackX, BackY, SpacePicture, TRUE);
	DrawGraph(BackX + 720, BackY, SpacePicture, TRUE);
	DrawGraph(BackX, BackY + 720, SpacePicture, TRUE);
	DrawGraph(BackX + 720, BackY + 720, SpacePicture, TRUE);
}

void BackMove() {
	BackX += VectorX/2;
	BackY += VectorY/2;
	if (BackX >= 0)
		BackX -= 720;
	if (BackY >= 0)
		BackY -= 720;
	if (BackX <= -720)
		BackX += 720;
	if (BackY <= -720)
		BackY += 720;
}

Star StarData[20];


void DrawMap() {
	DrawBoxAA(0, 0, 720, 720, GetColor(0, 0, 0), TRUE);
	for (int i = 0; i != 20; ++i) {
		StarData[i].DrawMiniMap();
	}
	DrawShipMiniMap();
}



void Init() {
	VectorX = 0;
	VectorY = 0;
	SpacePicture = LoadGraph("‰F’ˆ.png");
	SpaceShipPicture = LoadGraph("’…—¤‘D.png");
	Angle = DX_PI / 2;
	StarData[0] = { -150,-900,100,180,255,40 };
	StarData[1] = { -1000,-2100,150,0,190,120 };
	StarData[2] = { -1500,-2100,200,255,20,10 };
	StarData[3] = { -2100,-3000,150,10,255,100 };
	StarData[4] = { -2000,-150,50,100,255,10 };
	StarData[5] = { 300,-2200,50,255,140,0 };
	StarData[6] = { 2700,-2300,150,100,10,250 };
	StarData[7] = { 1800,-1250,250,10,140,255 };
	StarData[8] = { 900,-300,250,0,140,100 };
	StarData[9] = { 3300,-3300,50,200,140,0 };
	StarData[10] = { -750,450,50,200,10,200 };
	StarData[11] = { -210,3000,100,50,140,100 };
	StarData[12] = { -2100,1500,100,180,80,40 };
	StarData[13] = { -2200,2400,150,50,150,100 };
	StarData[14] = { -3300,1200,250,180,30,150 };
	StarData[15] = { 450,2500,100,200,100,100 };
	StarData[16] = { 900,1500,75,100,200,100 };
	StarData[17] = { 2000,900,150,100,100,200 };
	StarData[18] = { 3000,3600,150,100,100,250 };
	StarData[19] = { 2400,2700,200,200,200,200 };
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(TRUE);
	SetOutApplicationLogValidFlag(FALSE);
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetGraphMode(640, 640, 32);
	SetWindowSize(640, 640);
	SetWindowText("’…—¤ƒQ[ƒ€");
	SetFontSize(20);
	//	SetUse3DFlag(false);
	if (DxLib_Init() == -1)
		return -1;
	Init();
	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		SetDrawScreen(DX_SCREEN_BACK);
		DrawBack();
		GetHitKeyStateAll(Buf);
		if (Buf[KEY_INPUT_RIGHT]) {
			TurnRight();
		}
		if (Buf[KEY_INPUT_LEFT]) {
			TurnLeft();
		}
		if (Buf[KEY_INPUT_UP]) {
			Accelerate();
		}
		BackMove();
		for (int i = 0; i != 20; ++i) {
			StarData[i].Move();
			StarData[i].Draw();
			StarData[i].ShipMove();
			StarData[i].Landing();
		}
		if (Buf[KEY_INPUT_M]) {
			DrawMap();
		}
		DrawShip();
		DrawFormatString(0, 0, GetColor(255, 255, 255), "%f %f %f", VectorX, VectorY, fmod(Angle, DX_PI * 2));
		ScreenFlip();
	}
	WaitKey();
	DxLib_End();
	return 0;
}
#include<DxLib.h>
#include<bits/stdc++.h>

const int FieldX = 50000, FieldY = 50000;
const double TurnMove = 0.1, Acceleration = 0.1, Gravity = 0.02;
char Buf[256];
double VectorX, VectorY, Angle, BackX, BackY;
int SpaceShipPicture, SpacePicture;

class Star {
public:
	double X, Y;
	int Radius, ColorRed, ColorGreen, ColorBlue;
	double Dist() {
		return sqrt(X * X + Y * Y);
	}
	void Draw() {
		DrawCircleAA(X, Y, Radius, 60, GetColor(ColorRed, ColorGreen, ColorBlue), TRUE);
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
		if (Distance < 5000) {
			VectorX -= Gravity * (X - 300) / Distance;
			VectorY -= Gravity * (Y - 300) / Distance;
		}
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
		DrawLineAA(300 + cos(Angle - DX_PI / 8)*(35 + i), 300 + sin(Angle - DX_PI / 8)*(35 + i), 300 + cos(Angle + DX_PI / 8)*(35 + i), 300 + sin(Angle + DX_PI / 8)*(35 + i), GetColor(255 - 10 * i, 60 - i * 2, 0));
	}
}
void DrawShip() {
	DrawRotaGraph(300, 300, 1.0, Angle, SpaceShipPicture, TRUE);
}
void DrawBack() {
	DrawGraph(BackX, BackY, SpacePicture, TRUE);
	DrawGraph(BackX + 720, BackY, SpacePicture, TRUE);
	DrawGraph(BackX, BackY + 720, SpacePicture, TRUE);
	DrawGraph(BackX + 720, BackY + 720, SpacePicture, TRUE);
}

void BackMove() {
	BackX += VectorX;
	BackY += VectorY;
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


void Init() {
	SpacePicture = LoadGraph("‰F’ˆ.png");
	SpaceShipPicture = LoadGraph("’…—¤‘D.png");
	Angle = DX_PI / 2;
	StarData[0] = { -2000,-12000,200,180,255,40 };
	StarData[1] = { -14000,-35000,300,0,190,120 };
	StarData[2] = { -20000,-28000,400,255,20,10 };
	StarData[3] = { -30000,-42000,250,10,255,100 };
	StarData[4] = { -35000,-2000,100,100,255,10 };
	StarData[5] = { 4000,-30000,100,255,140,0 };
	StarData[6] = { 37000,-25000,250,100,10,250 };
	StarData[7] = { 25000,-18000,500,10,140,255 };
	StarData[8] = { 12000,-5000,450,0,140,100 };
	StarData[9] = { 42000,-42000,100,200,140,0 };
	StarData[10] = { -10000,5000,100,200,10,200 };
	StarData[11] = { -2500,40000,200,50,140,100 };
	StarData[12] = { -28000,23000,250,180,80,40 };
	StarData[13] = { -33000,32000,350,50,150,100 };
	StarData[14] = { -44000,17000,450,180,30,150 };
	StarData[15] = { 2000,2000,200,200,100,100 };
	StarData[16] = { 13000,25000,50,100,200,100 };
	StarData[17] = { 26000,13000,250,100,100,200 };
	StarData[18] = { 42000,48000,300,100,100,250 };
	StarData[19] = { 32000,36000,400,255,255,255 };
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
		DrawShip();
		
		for (int i = 0; i != 20; ++i) {
			StarData[i].Move();
			StarData[i].Draw();
			StarData[i].ShipMove();
		}

		DrawFormatString(0, 0, GetColor(255, 255, 255), "%f %f", VectorX, VectorY);
		ScreenFlip();
	}
	WaitKey();
	DxLib_End();
	return 0;
}
#include<DxLib.h>
#include<bits/stdc++.h>

const int FieldX = 4000, FieldY = 4000;
const double TurnMove = 0.1, Acceleration = 0.12, Gravity = 0.5;
char Buf[256];
double VectorX, VectorY, Angle, BackX, BackY, UseBrake, Dificulty;
int SpaceShipPicture, SpacePicture, SpaceShipBrokenPicture, TitlePicture, ScreenState, CursorState;
bool HoldEnter,HoldArrow;
void DrawShip();
void DrawBrokenShip();
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
		X += VectorX * Dificulty;
		Y += VectorY * Dificulty;
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
		DrawFormatString(X, Y, GetColor(255, 255, 255), "%f", atan2(Y-320, X-320));
		if (Dist() <= Radius + 14) {
			if ((fmod(abs(atan2(Y - 320, X - 320) - Angle), DX_PI * 2) <= 0.5 || abs(fmod(DX_PI * 2 - abs(atan2(Y - 320, X - 320) - Angle), DX_PI * 2)) <= 0.5) && sqrt(VectorX*VectorX + VectorY * VectorY) * (3 + Dificulty) / 4 <= 2) {
				DrawBoxAA(50, 50, 590, 310, GetColor(0, 0, 0), TRUE);
				DrawBoxAA(50, 50, 590, 310, GetColor(255, 255, 255), FALSE);
				SetFontSize(100);
				DrawString(70, 70, "Game Clear", GetColor(255, 125, 0));
				int Score = (2 - min(fmod(abs(atan2(Y - 320, X - 320) - Angle), DX_PI * 2), abs(fmod(DX_PI * 2 - abs(atan2(Y - 320, X - 320) - Angle), DX_PI * 2))))*(10000 - Radius * 20)*(5 - sqrt(VectorX*VectorX + VectorY * VectorY)*(3 + Dificulty) / 4)*Dificulty;
				if (!UseBrake) {
					Score *= 2;
					SetFontSize(30);
					DrawString(180, 180, "No Hover Bonus ~2", GetColor(255, 175, 0));
				}
				SetFontSize(70);
				DrawFormatString(70, 220, GetColor(255,255,255),"Score:%d",Score);
				SetFontSize(35);
				DrawString(70, 400, "Press Enter to Return Title", GetColor(255, 255, 255));
				DrawShip();
				ScreenFlip();
				while (!CheckHitKey(KEY_INPUT_RETURN)) {
					GetHitKeyStateAll(Buf);
				}
				HoldEnter = TRUE;
				Init();
				return 0;
			}
			else {
				DrawFormatString(50, 50, GetColor(255, 255, 255), "Game Over");
				DrawBrokenShip();
				ScreenFlip();
				WaitKey();
				Init();
				return 0;
			}
		}
		else return 0;
	}
};

Star StarData[20];

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

void DrawBrokenShip() {
	DrawRotaGraph(320, 320, 0.5, Angle, SpaceShipBrokenPicture, TRUE);
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
	BackX += VectorX / 2 * Dificulty;
	BackY += VectorY / 2 * Dificulty;
	if (BackX >= 0)
		BackX -= 720;
	if (BackY >= 0)
		BackY -= 720;
	if (BackX <= -720)
		BackX += 720;
	if (BackY <= -720)
		BackY += 720;
}

void DrawMap() {
	DrawBoxAA(0, 0, 720, 720, GetColor(0, 0, 0), TRUE);
	for (int i = 0; i != 20; ++i) {
		StarData[i].DrawMiniMap();
	}
	DrawShipMiniMap();
}

void Brake() {
	UseBrake = TRUE;
	VectorX += Acceleration* 0.4 * cos(Angle);
	VectorY += Acceleration* 0.4 * sin(Angle);
	VectorX *= 0.97;
	VectorY *= 0.97;	
	for (int i = 0; i != 20; ++i) {
		DrawLineAA(320 + cos(Angle - DX_PI / 8)*(17 + i), 320 + sin(Angle - DX_PI / 8)*(17 + i), 320 + cos(Angle + DX_PI / 8)*(17 + i), 320 + sin(Angle + DX_PI / 8)*(17 + i), GetColor(0, 0, 255 - i * 2));
	}
	for (int i = 0; i != 20; ++i) {
		DrawCircleAA(320, 320, i + 10, 60, GetColor(0, 0, 255 - i * 10), FALSE);
	}
}


void Init() {
	SetFontSize(20);
	VectorX = 0;
	VectorY = 0;
	ScreenState = 0;
	CursorState = 0;
	UseBrake = FALSE;
	Dificulty = 0.8;
	Angle = DX_PI / 2;
	StarData[0] = { -150,-900,100,180,255,40 };
	StarData[1] = { -1000,-2100,150,0,190,120 };
	StarData[2] = { -1500,-2100,200,255,20,10 };
	StarData[3] = { -2100,-3000,150,10,255,100 };
	StarData[4] = { -2000,-150,25,100,255,10 };
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
	StarData[18] = { 3000,3600,300,100,100,250 };
	StarData[19] = { 2400,2700,200,200,200,200 };
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(TRUE);
	SetOutApplicationLogValidFlag(FALSE);
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetGraphMode(640, 640, 32);
	SetWindowSize(640, 640);
	SetWindowText("’…—¤ƒQ[ƒ€");
	if (DxLib_Init() == -1)
		return -1;
	SpacePicture = LoadGraph("‰F’ˆ.png");
	SpaceShipPicture = LoadGraph("’…—¤‘D.png");
	SpaceShipBrokenPicture = LoadGraph("”š”­.png");
	TitlePicture = LoadGraph("ƒ^ƒCƒgƒ‹.png");
	Init();
	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		SetDrawScreen(DX_SCREEN_BACK);
		DrawBack();
		GetHitKeyStateAll(Buf);
		switch(ScreenState) {
			case 0:
				DrawGraph(0, 0, TitlePicture, TRUE);
				DrawBoxAA(170, 420, 460, 490, GetColor(255, 0, 0), FALSE, 1);
				DrawBoxAA(170, 510, 460, 580, GetColor(0, 0, 255), FALSE, 1);
				if (CursorState == 0) {
					DrawBoxAA(170, 420, 460, 490, GetColor(255, 0, 0), FALSE, 8);
					if (Buf[KEY_INPUT_UP] || Buf[KEY_INPUT_DOWN]) {
						if (!HoldArrow) {
							CursorState = 1;
							HoldArrow = TRUE;
						}
					}
					else {
						HoldArrow = FALSE;
					}
					if (Buf[KEY_INPUT_RETURN]) {
						if (!HoldEnter) {
							ScreenState = 1;
							HoldEnter = TRUE;
						}
					}
					else {
						HoldEnter = FALSE;
					}
				}
				else {
					DrawBoxAA(170, 510, 460, 580, GetColor(0, 0, 255), FALSE, 8); 
					if (Buf[KEY_INPUT_UP] || Buf[KEY_INPUT_DOWN]) {
						if (!HoldArrow) {
							CursorState = 0;
							HoldArrow = TRUE;
						}
					}
					else {
						HoldArrow = FALSE;
					}
					if (Buf[KEY_INPUT_RETURN]) {
						if (!HoldEnter) {
							ScreenState = 2;
							HoldEnter = TRUE;
						}
					}
					else {
						HoldEnter = FALSE;
					}
				}
				break;
			case 1:

				break;
			case 2:
				break;
			case 3:
				if (Buf[KEY_INPUT_RIGHT]) {
					TurnRight();
				}
				if (Buf[KEY_INPUT_LEFT]) {
					TurnLeft();
				}
				if (Buf[KEY_INPUT_DOWN]) {
					Brake();
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
				
				DrawShip();
				if (Buf[KEY_INPUT_M]) {
					DrawMap();
				}
				DrawFormatString(0, 0, GetColor(255, 255, 255), "%f %f %f", VectorX, VectorY, fmod(Angle, DX_PI * 2));
			break;
		}
		ScreenFlip();
	}
	WaitKey();
	DxLib_End();
	return 0;
}
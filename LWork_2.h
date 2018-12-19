#pragma once

#include <iostream>
#include <windows.h>
#include "Lwork_3.h"

long RandRGB()
{
   return RGB(rand() % 256, rand() % 256, rand() % 256);
}
void OpenProgramm()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	CreateProcess(_T("C:\\Windows\\notepad.exe"), L"",
		NULL, NULL, FALSE, 0, NULL, NULL,
		&si, &pi);
}
void GetSizeWin(HWND hWnd,Size &size)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	size.height = rect.bottom - rect.top;
	size.wight = rect.right - rect.left ;
}
void CreateFonLine(HWND hWnd,HDC hdc,ConfigINI* Config)
{
	GetSizeWin(hWnd, Config->size);
	int sizeH = Config->size.height-40;
	int sizeW = Config->size.wight-17;
	HPEN hPen = CreatePen(0, 1, Config->ColorRGB_Line);
	SelectObject(hdc, hPen);
	for (int Hs, Ws, i = 0; i <= Config->n; i++)
	{
		Hs = i*sizeW / Config->n;
		Ws = i* sizeH / Config->n;
		MoveToEx(hdc, Hs, 0, NULL);
		LineTo(hdc, Hs, sizeH);
		MoveToEx(hdc, 0, Ws, NULL);
		LineTo(hdc, sizeW, Ws);
	}
	DeleteObject(hPen);

}
void ClickPaintCircle(HWND hWnd,bool* _is,ConfigINI* config,int win_x,int win_y)
{
	HBRUSH brush = CreateSolidBrush(RGB(100,100,100));
	HDC hdc = GetDC(hWnd);
	int sizeH = config->size.height - 40;
	int sizeW = config->size.wight - 17;
	SelectObject(hdc, brush);
	int x = win_x*config->n / sizeW,
		y = win_y*config->n / sizeH;
	_is[x*config->n + y+1] = true;
	Ellipse(hdc,
		x * sizeW / config->n + 1,
		y * sizeH / config->n + 1,
		x * sizeW / config->n + sizeW / config->n,
		y * sizeH / config->n + sizeH / config->n);
	ReleaseDC(hWnd, hdc);
	DeleteObject(brush);
	DeleteObject(hdc);
	
}
void PaintCircle(HDC hdc, bool* _is, ConfigINI* config)
{
	HPEN hPen = CreatePen(0, 1, RGB(0, 0, 0));
	HBRUSH brush = CreateSolidBrush(RGB(100, 100, 100));
	SelectObject(hdc, hPen);
	SelectObject(hdc, brush);
	int sizeH = config->size.height - 40;
	int sizeW = config->size.wight - 17;
	for (int i = 0; i < config->n; i++)
		for (int j = 0; j < config->n; j++)
			if (_is[i*config->n + j+1])
			{
				
				
				Ellipse(hdc,
					i*sizeW / config->n + 1,
					j*sizeH / config->n + 1,
					i*sizeW / config->n + sizeW / config->n,
					j*sizeH / config->n + sizeH / config->n);
				
			}
	DeleteObject(brush);
	DeleteObject(hPen);
}
void CreateFonColor(HWND hWnd, ConfigINI* Config)
{
	long rgb = RandRGB();
	HBRUSH hBrush = CreateSolidBrush(rgb);
	Config->ColorRGB_Backgraund = rgb;
	DeleteObject((HBRUSH)GetClassLong(hWnd, GCLP_HBRBACKGROUND));
	SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG)hBrush);
	InvalidateRect(hWnd, NULL, 1);

}

#pragma once
#include "iostream"
#include "fstream"
#include "string"
#include "LWork_2.h"
#include <windows.h>
#include <vector>
int n_ = 2;
struct Image
{
	char* image;
	int width;
	int heigth;
};	

typedef char* (*pFunction)(const char*, int*,int*);

void Open_hLib(HMODULE &hLib)
{
	hLib = LoadLibrary(L"exp4.dll");
	if (!hLib)
	{
		MessageBox(NULL, L"Библиотека не найдена", L"ERROR", 0);
	}
}
pFunction Get_Funcion_Dll(HMODULE &hLib)
{
	
	pFunction p_Function = (pFunction)GetProcAddress(hLib, "load_image");
	if (!p_Function)
	{
		MessageBox(NULL, L"функция не найдена", L"ERROR", 0);
		FreeLibrary(hLib);
		hLib = nullptr;
		return NULL;

	}
	return p_Function;
}
void Close_hLib(HMODULE &hLib)
{
	if (hLib) {
	FreeLibrary(hLib);
	}
	
	hLib = NULL;
}
std::vector<Image*>* Get_Images(ConfigINI* config)
{
	HMODULE hLib;
	Open_hLib(hLib);
	if (!hLib) return NULL;
	pFunction fun = Get_Funcion_Dll(hLib);
	if (!fun) return NULL;
	std::vector<Image*>* images = new std::vector<Image*>;
	for (int i = 0; i < 2; i++)
	{
		Image *image=new Image;
		image->image = fun(config->resourse[i].c_str(), &image->width, &image->heigth);
		images->push_back(image);
	}
	Close_hLib(hLib);
	return images;
}
void Delete_Images(std::vector<Image*> *images)
{
	if (images)
	{	
		for(int i = 0; i < images->size(); i++)
		{
			delete[](*images)[i]->image;
			delete[](*images)[i];
		}
		delete images;
	}
}
void ClickPaintCircle_(HWND hWnd, char* _is, ConfigINI* config,std::vector<Image*> *images, int win_x, int win_y)
{
	if (images)
	{
		int sizeH = config->size.height - 40;
		int sizeW = config->size.wight - 17;
		int x = win_x*config->n / sizeW,
			y = win_y*config->n / sizeH;
		if (!_is[x*config->n+y+1]) {
		_is[x*config->n + y+1] = rand() % n_+1;
		HBITMAP hBitmap = CreateBitmap((*images)[_is[x*config->n + y+1]-1]->width, (*images)[_is[x*config->n + y+1]-1]->heigth, 1, 32, (*images)[_is[x*config->n + y+1]-1]->image);
		HDC hdc = GetDC(hWnd);
		HDC memBit = CreateCompatibleDC(hdc);
		SelectObject(memBit, hBitmap);
		BitBlt(hdc, x * sizeW / config->n + 1, y * sizeH / config->n + 1, (x+1) * sizeW / config->n , (y+1) * sizeH / config->n, memBit, 0, 0, SRCAND);
		ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, NULL, FALSE);
		DeleteObject(hdc);
		DeleteObject(memBit);
		DeleteObject(hBitmap);
		}
		
	}
	else
		ClickPaintCircle(hWnd, (bool*)_is, config, win_x, win_y);
	PostMessage(HWND_BROADCAST, RegisterWindowMessage(SPECIAL_CTRL_CHANGE_T), 0, 0);
			
}
void PaintCircle_(HDC hdc, char* _is, ConfigINI* config, std::vector<Image*> *images)
{
	if (!images)
		PaintCircle(hdc, (bool*)_is, config);
	else
	{
		int sizeH = config->size.height - 40;
		int sizeW = config->size.wight - 17;
		for (int i = 0; i < config->n; i++)
			for (int j = 0; j < config->n; j++)
				if (_is[i*config->n + j+1])
				{
					HBITMAP hBitmap = CreateBitmap((*images)[_is[i*config->n + j+1] - 1]->width, (*images)[_is[i*config->n + j+1] - 1]->heigth, 1, 32, (*images)[_is[i*config->n + j+1] - 1]->image);
					HDC memBit = CreateCompatibleDC(hdc);
					SelectObject(memBit, hBitmap);
					BitBlt(hdc, i * sizeW / config->n + 1, j * sizeH / config->n + 1, (i + 1) * sizeW / config->n, (j + 1) * sizeH / config->n, memBit, 0, 0, SRCAND);
					DeleteObject(memBit);
					DeleteObject(hBitmap);
				}
	}
}
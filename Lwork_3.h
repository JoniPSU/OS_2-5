#pragma once
#include "iostream"
#include "fstream"
#include "string"
#include <windows.h>
struct Size
{
	int wight=320;
	int height=240;
};
struct ConfigINI
{
	int n=20;
	Size size;
	long ColorRGB_Backgraund= 0xff0000;
	long ColorRGB_Line = 0x0000ff;
	std::string resourse[2];
	void Load(std::string configLineName, std::string config_Size)
	{
		if (configLineName == "FieldSize(n)")
		{
			n = std::atoi(config_Size.c_str());
			return;
		}
		if (configLineName == "WindowSize")
		{
			std::string s1 = "", s2 = "";
			for (int i = 0, j = 0; i < config_Size.length(); i++)
			{
				if (config_Size[i] == ' ') j++;
				else
				{
					switch (j)
					{
					case(0) :
						s1 += config_Size[i];
						break;
					case(1) :
						s2 += config_Size[i];
						break;
					}
				}
			}
			size.height = std::atoi(s2.c_str());
			size.wight = std::atoi(s1.c_str());
			return;
		}
		if (configLineName == "ColorBackgraund")
		{
			std::string s1 = "", s2 = "", s3 = "";
			for (int i = 0, j = 0; i < config_Size.length(); i++)
			{
				if (config_Size[i] == ' ') j++;
				else
				{
					switch (j)
					{
					case(0) :
						s1 += config_Size[i];
						break;
					case(1) :
						s2 += config_Size[i];
						break;
					case(2) :
						s3 += config_Size[i];
						break;

					}
				}
			}
			ColorRGB_Backgraund = (LONG)RGB(std::atoi(s1.c_str()), std::atoi(s2.c_str()), std::atoi(s3.c_str()));
			return;
		}
		if (configLineName == "ColorLine")
		{
			std::string s1 = "", s2 = "", s3 = "";
			for (int i = 0, j = 0; i < config_Size.length(); i++)
			{
				if (config_Size[i] == ' ') j++;
				else
				{
					switch (j)
					{
					case(0) :
						s1 += config_Size[i];
						break;
					case(1) :
						s2 += config_Size[i];
						break;
					case(2) :
						s3 += config_Size[i];
						break;

					}
				}
			}
			ColorRGB_Line = (LONG)RGB(std::atoi(s1.c_str()), std::atoi(s2.c_str()), std::atoi(s3.c_str()));
			return;
		}
		if (configLineName == "Resourse")
		{
			int find = config_Size.find(';');
			resourse[0] = config_Size.substr(0,find);
			resourse[1] = config_Size.substr(find+1, config_Size.length()-1);
		}
	}

};

bool GetArrayString(std::string &line, std::string* &params,int &n)
{
	//
	if (n == -1)
	{
		n = 0;
		for (int i = 0; i < line.length(); i++)
			if ('<' == line[i])	n++;
	}
	params = new std::string[n];
	int origin=0, end, i = 0;
	while (origin >=0)
	{
		origin = line.find('<');
		end = line.find('>');
		
		if (origin >= 0)
		{
			params[i] = line.substr(origin+1, end - origin-1);
			line.erase(origin, end - origin+1);
			i++;
		}
		
	}




	return false;
}
bool GetNameParam(std::string &line, std::string &name, std::string &param)
{
	int find = line.find(':');
	name = line.substr(0, find);
	param = line.substr(find+1, line.length()-1);
	return false;
}
void LoadInfo(ConfigINI* config,std::string &line)
{
	int n = -1;
	std::string *lines, param_1 = "", param_2 = "";
	GetArrayString(line, lines, n);
	for (int i = 0; i < n; i++)
	{
		GetNameParam(lines[i], param_1, param_2);
		config->Load(param_1, param_2);
	}
	delete[] lines;
}
void NewLineConfig(ConfigINI* ini, std::string &line)
{
	line = "<FieldSize(n):" + std::to_string(ini->n) + ">\r\n";
	line += "<WindowSize:" + std::to_string(ini->size.wight) + " " + std::to_string(ini->size.height) + ">\r\n";
	line += "<ColorBackgraund:" + std::to_string((ini->ColorRGB_Backgraund & 255)) + " " + std::to_string(((ini->ColorRGB_Backgraund >> 8) & 255)) + " " + std::to_string(((ini->ColorRGB_Backgraund) >> 16)) + ">\r\n";
	line += "<ColorLine:" + std::to_string((ini->ColorRGB_Line & 255)) + " " + std::to_string(((ini->ColorRGB_Line >> 8) & 255)) + " " + std::to_string(((ini->ColorRGB_Line) >> 16)) + ">\r\n";
	line += "<Resourse:" + ini->resourse[0] + ";" + ini->resourse[1] + ">\r\n";
}
//методы считывания из файла и записи
void NewFileConfig(ConfigINI &con)
{
	// создает файл конфигураций
	std::ofstream file(FileConfiguration);
	file << "<" << "FieldSize(n)" << ":" << con.n << ">\n";
	file << "<" << "WindowSize" << ":" << con.size.wight<<" "<<con.size.height << ">\n";
	file << "<" << "ColorBackgraund" << ":" << (con.ColorRGB_Backgraund & 255) << " " << ((con.ColorRGB_Backgraund >> 8) & 255) << " " << (con.ColorRGB_Backgraund >> 16) << ">\n";
	file << "<" << "ColorLine" << ":" << (con.ColorRGB_Line & 255) << " " << ((con.ColorRGB_Line >> 8) & 255) << " " << (con.ColorRGB_Line >> 16) << ">\n";
	file << "<" << "Resourse" << ":" << con.resourse[0] + ";" + con.resourse[1] +  ">\n";
	file.close();
}
void Write_WinAPI(ConfigINI* config)
{
	HANDLE hFile = CreateFile(L"config.ini", GENERIC_WRITE, 0, nullptr, OPEN_EXISTING|CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
	std::string line;
	NewLineConfig(config, line);
	WriteFile(hFile, line.c_str(), line.length(), 0, NULL);
	CloseHandle(hFile);
}
void Write_MapViewOfFile(ConfigINI* config)
{
	
	HANDLE hFile = CreateFile(L"config.ini", GENERIC_ALL,  FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	std::string line;
	NewLineConfig(config, line);
	HANDLE hMapFileOut = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, line.length(), NULL);
	char* ptrOut = (char*)MapViewOfFile(hMapFileOut, FILE_MAP_WRITE, 0, 0, line.length());
	for (int i = 0; i < line.length(); i++)
	{
		*(ptrOut + i) = line[i];
	}
	FlushViewOfFile(ptrOut, line.size());
	UnmapViewOfFile(ptrOut);
	CloseHandle(hMapFileOut);
	CloseHandle(hFile);

}
void Write_FILE_IO(ConfigINI* config)
{
	FILE *file = fopen(FileConfiguration, "w+");
	std::string line = "";
	NewLineConfig(config, line);
	fprintf(file,line.c_str());
	fclose(file);
}
void Read_FILE_IO(ConfigINI* config)
{
	
		FILE *file = fopen(FileConfiguration, "r+");
		if (file==NULL) 
		{
			NewFileConfig(*config);
			
		}
		else
		{
				char  buff=' ';
				std::string line="";
				fscanf(file, "%c", &buff);
				while (!feof(file))
				{
					line += buff;
					fscanf(file, "%c", &buff);
				}
				LoadInfo(config,line);
				fclose(file);
		}
			
}
void Read_WinAPI(ConfigINI* config)
{
	
	HANDLE hFile = CreateFile(L"config.ini", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile)
	{

		DWORD sizeFile = GetFileSize(hFile, NULL);
		DWORD s = 0;
		char *con = new char[sizeFile];
		ReadFile(hFile, con, sizeFile, &s, NULL);
		std::string line(con, sizeFile);
		CloseHandle(hFile);
		delete[] con;
		LoadInfo(config, line);
	
	}
	else
	{
		NewFileConfig(*config);
	}
	
}
void Read_Fstream(ConfigINI* config)
{
	std::ifstream file(FileConfiguration);
	int i = 0;
	if (!file)
		NewFileConfig(*config);
	else {
		int n=-1;
		std::string line="",s=" ";
		while (s!="") {std::getline(file, s); line += s; }
		LoadInfo(config, line);
		file.close();
	}
	

}
void Read_MapViewOfFile(ConfigINI* config)
{

	HANDLE hFile = CreateFile(L"config.ini", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if ((long)hFile!= 0xffffffff)
	{

		int fsize = GetFileSize(hFile, nullptr);
		HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, fsize, NULL);
		char* dataPtr = (char*)MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, fsize);
		std::string line(dataPtr);
		LoadInfo(config, line);
		UnmapViewOfFile(dataPtr);
		CloseHandle(hMapFile);
		

	}
	else
	{
		NewFileConfig(*config);
	}
		CloseHandle(hFile);
}
void Read_Write_Choise(char* mes, ConfigINI &config, bool read_write)
{
	if (!read_write)
		switch (*mes)
		{
		case '1':
			MessageBox(NULL, L"Read_MapViewOfFile(", L"WinAPI App", 0);
			Read_MapViewOfFile(&config);
			break;
		case '2':
			MessageBox(NULL, L"Read_WinAPI()", L"WinAPI App", 0);
			Read_WinAPI(&config);
			break;
		case '3':
			MessageBox(NULL, L"Read_FILE_IO", L"WinAPI App", 0);
			Read_FILE_IO(&config);
			break;
		default:
			MessageBox(NULL, L"Read_Fstream()", L"WinAPI App", 0);
			Read_Fstream(&config);
			break;
		}
	else
		switch (*mes)
		{
		case '1':
			MessageBox(NULL, L"Write_MapViewOfFile(", L"WinAPI App", 0);
			Write_MapViewOfFile(&config);
			break;
		case '2':
			MessageBox(NULL, L"Write_WinAPI()", L"WinAPI App", 0);
			Write_WinAPI(&config);
			break;
		case '3':
			MessageBox(NULL, L"Write_FILE_IO", L"WinAPI App", 0);
			Write_FILE_IO(&config);
			break;
		default:
			MessageBox(NULL, L"Write_Fstream()", L"WinAPI App", 0);
			NewFileConfig(config);
		
		}
}
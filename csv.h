#pragma once
#include<string>
#include<vector>

#ifdef GPCSV_EXPORTS
#define GPCSV_DLL __declspec(dllexport)
#else
#define GPCSV_DLL __declspec(dllimport)
#endif

//dll导出类，用于生成lib文件,无实际作用
class GPCSV_DLL aawgahkn
{
public:
	int n;
	int n2;
	aawgahkn();
};


GPCSV_DLL void order_title();

GPCSV_DLL void order_write(const char* name_ptr, float q, float q_var, float c, float h, float t, float t_var, float s = 0.95, int f = 0, bool utf8 = false);

GPCSV_DLL void order_read(std::vector<std::string>& name, std::vector<float>& q, std::vector<float>& q_var, std::vector<float>& c, std::vector<float>& h, std::vector<float>& t, std::vector<float>& t_var, std::vector<float>& s, std::vector<int>& f, bool utf8 = false);


GPCSV_DLL bool order_right();

GPCSV_DLL void building_write(std::vector<float>& d);


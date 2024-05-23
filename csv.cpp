#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<Windows.h>
#include "csv.h"

const std::string order_filename = "����.csv";
const std::string building_filename = "����.csv";

char endl[1] = { char(0x0a) };

class order_data
{
public:
	std::string name;
	float q;
	float q_var;
	float c;
	float h;
	float t;
	float t_var;
	float s;
	int f;

	order_data() :name(), q(0), q_var(0), c(0), h(0), t(0), t_var(0), s(0), f(0) {}
};





std::string ansi_utf8(const std::string& str_ansi)
{
	//����һ���յ�std::wstring
	std::wstring wstr = L"";

	 //lpWideCharStr��ΪNULL,cchWideChar��Ϊ0,�ò������ڻ�ȡ��������С
	int len = MultiByteToWideChar(CP_ACP, 0, str_ansi.c_str(), str_ansi.size(), NULL, 0);

	//����wchar_t������Ϊ������,���ڽ���ת������������,���鳤��Ϊlen+1��ԭ�����ַ�����Ҫ��'\0'��β,����+1�����洢'\0'
	wchar_t* wchar = new wchar_t[len + 1];

	//�����������軺������С����ȷ��,ִ��ת��
	MultiByteToWideChar(CP_ACP, 0, str_ansi.c_str(), str_ansi.size(), wchar, len);

	//ʹ��'\0'��β
	wchar[len] = '\0';

	//������ƴ�ӵ�std::wstring
	wstr.append(wchar);

	//�м�Ҫ��ջ�����
	delete[]wchar;

	//�����Ƶ�std::string
	std::string str_utf8;

	len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);

	char* buffer = new char[len + 1];

	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);

	buffer[len] = '\0';

	str_utf8.append(buffer);

	delete[]buffer;

	return str_utf8;
}

std::string utf8_ansi(const std::string& str_utf8)
{
	std::wstring wstr = L"";

	int len = MultiByteToWideChar(CP_UTF8, 0, str_utf8.c_str(), str_utf8.size(), NULL, 0);

	wchar_t* wchar = new wchar_t[len + 1];

	MultiByteToWideChar(CP_UTF8, 0, str_utf8.c_str(), str_utf8.size(), wchar, len);

	wchar[len] = '\0';

	wstr.append(wchar);

	delete[]wchar;

	std::string str_ansi;

	len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);

	char* buffer = new char[len + 1];

	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);

	buffer[len] = '\0';

	str_ansi.append(buffer);

	delete[]buffer;

	return str_ansi;
}

void order_title()
{
	std::fstream a(order_filename, std::ios::out);
	std::string n_= "��������,������,��������,�ɹ��ɱ�,���гɱ�,��ǰ��,��ǰ�ڷ���,����ˮƽ,���淽ʽ";
	n_ = ansi_utf8(n_);
	char bom[4] = { char(0xef),char(0xbb),char(0xbf),char(0) };
	a << bom << n_ << endl;
}

void order_write(const char* name_ptr, float q, float q_var, float c, float h, float t, float t_var, float s, int f, bool utf8)
{
	std::string name = name_ptr;
	std::string temp;
	if (utf8)
	{
		temp = utf8_ansi(name);
	}
	std::fstream a(order_filename, std::ios::app);
	std::string name_;
	for (auto word : temp)
	{
		if (word == '"')
		{
			name_ += "\"\"";
		}
		else
		{
			name_.push_back(word);
		}
	}
	if (f != 0 && f != 1 && f != 2)
	{
		f = 0;
	}



	a << "\"" << ansi_utf8(name_) << "\"," << q << "," << q_var << "," << c << "," << h << "," << t << "," << t_var << "," << s << "," << f << std::endl;
}


void order_read(std::vector<order_data>& d)
{
	d = std::vector<order_data>();
	std::fstream a(order_filename, std::ios::in);
	std::string s;
	std::getline(a, s);
	int j = 0;
	while (std::getline(a, s))
	{
		std::string s_ = utf8_ansi(s);
		if (s_.size() == 0)
		{
			continue;
		}
		d.push_back(order_data());
		int n = 0;
		std::string num;
		std::string name;
		double temp;
		for (auto i : s_)
		{
			if (n >= 0)
			{
				switch (i)
				{
				case '\"':
					n++;
					if (n != 1 && n % 2 == 1)
					{
						name.push_back('\"');
					}
					break;
				case ',':
					if (n % 2 == 0)
					{
						n = -1;
					}
					else
					{
						name.push_back(i);
					}
					break;
				default:
					name.push_back(i);
					break;
				}
			}
			else
			{
				if (i != ',')
				{
					num.push_back(i);
				}
				else
				{
					if(num.size()!=0)
					{
						temp = std::stod(num);
					}
					else
					{
						temp = 0;
					}
					switch (n)
					{
					case -1:
						d[j].q = temp;
						break;
					case -2:
						d[j].q_var = temp;
						break;
					case -3:
						d[j].c = temp;
						break;
					case -4:
						d[j].h = temp;
						break;
					case -5:
						d[j].t = temp;
						break;
					case -6:
						d[j].t_var = temp;
						break;
					case -7:
						d[j].s = temp;
						break;
					case -8:
						d[j].f = temp;
						break;
					default:
						break;
					}
					n--;
					num.clear();
				}
			}
		}

		//std::cout << name.size() << std::endl;

		if (name.size() == 0)
		{
			name.push_back(' ');
		}

		if (num.size() != 0)
		{
			temp = std::stod(num);
		}
		else
		{
			temp = 0;
		}

		d[j].f = temp;
		d[j].name = ansi_utf8(name);
		j++;
	}

}

void order_read(std::vector<std::string>& name, std::vector<float>& q, std::vector<float>& q_var, std::vector<float>& c, std::vector<float>& h, std::vector<float>& t, std::vector<float>& t_var, std::vector<float>& s, std::vector<int>& f, bool utf8)
{
	name = std::vector<std::string>();
	q = std::vector<float>();
	q_var = std::vector<float>();
	c = std::vector<float>();
	h = std::vector<float>();
	t = std::vector<float>();
	t_var = std::vector<float>();
	s = std::vector<float>();
	f = std::vector<int>();


	std::vector<order_data> d;
	order_read(d);
	for (order_data& i : d)
	{
		if (!utf8)
		{
			name.push_back(utf8_ansi(i.name));
		}
		else
		{
			name.push_back(i.name);
		}
		
		q.push_back(i.q);
		q_var.push_back(i.q_var);
		c.push_back(i.c);
		h.push_back(i.h);
		t.push_back(i.t);
		t_var.push_back(i.t_var);
		s.push_back(i.s);
		f.push_back(i.f);
	}
}

bool order_right()
{
	std::fstream a(order_filename, std::ios::in);
	std::string s;
	std::string t = "��������,������,��������,�ɹ��ɱ�,���гɱ�,��ǰ��,��ǰ�ڷ���,����ˮƽ,���淽ʽ";

	char bom[3] = { 0 };
	a.read(bom, 3);
	if (bom[0] == '\xEF' && bom[1] == '\xBB' && bom[2] == '\xBF') {
	}
	else {
		a.seekg(0);
	}

	std::getline(a, s);
	s = utf8_ansi(s);
	if (s.size() != t.size())
	{
		return false;
	}
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] != t[i])
		{
			return false;
		}
	}
	return true;
}


void building_write(std::vector<float>& d)
{
	std::string name[8] = { "����ͣ����","�ּ���","���²ִ���","��زִ���","�䶳������","�칫������","���������","Ա��ͣ����" };
	std::fstream a(building_filename, std::ios::out);
	char bom[4] = { char(0xef),char(0xbb),char(0xbf),char(0) };
	a << bom << endl;
	for (int i = 0; i < 8; i++)
	{
		a << ansi_utf8(name[i]) << ",x,y" << std::endl;
		int k = 0;
		for (int j = 0; j < 16; j++)
		{
			int x_i = i * 34 + j * 2;
			int y_i = i * 34 + j * 2 + 1;
			if (j == 0)
			{
				k++;
				a << k << ',' << d[x_i] << ',' << d[y_i] << std::endl;
			}
			else if (abs(d[x_i] - d[x_i - 2]) < 0.01 && abs(d[y_i] - d[y_i - 2]) < 0.01)
			{
				continue;
			}
			else
			{
				k++;
				a << k << ',' << d[x_i] << ',' << d[y_i] << std::endl;
			}
		}
		a << ansi_utf8("��,") << int(round(d[i * 34 + 32])) << ',' << int(round(d[i * 34 + 33])) << std::endl;
	}
}


aawgahkn::aawgahkn() :n(), n2() {};

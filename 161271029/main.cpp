#pragma comment(linker, "/HEAP:1024000000,1024000000")
#pragma comment(linker, "/STACK:1024000000,1024000000")
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include <string>
#include <numeric>
#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <ctime>

using namespace std;
using vec=vector<string>;
using loco=unordered_map<string,vec>;
using freq=unordered_map<string,int>;

struct wrong //�Զ����쳣��
{
	string what; //�쳣��Ϣ
	explicit wrong(const string s=""):what(s){} //�쳣���캯��
};

bool parse(string,vec&);	//�����û������ƴ����
void read();	//��ȡ�����ļ�
void split();	//�з�ƴ�����������û�ѡ��
void save_user_wd();	//�����û���
template <class Type,class Func> void input(Type&,const string&,Func&);	//�ж��û������ѡ���Ƿ��������


freq* wd_fq;	//��-��Ƶ
loco* py_wd;	//ƴ��-���б�
loco* py_volc;	//ƴ��-���б�
loco* wd_py;	//��-ƴ���б�
loco* py_uservolc;	//ƴ��-�û����б�


int main()
{
	string choice;
	cout << string(10, '-') << "ƴ�����뷨��ʼ�˵�" << string(10, '-') << "\n����Ҫ��ʲô��\na.��������\nb.�˳�����\n�������������";
	input(choice, "������a��b������������:", [](string& ch) { return ch == "a" || ch == "b"; });
	if (choice == "b")
	{
		system("pause");
		return 0;
	}
	cout << string(10, '-') << "�����ı�" << string(10, '-') << endl;
	try { read(); }
	catch (wrong e)
	{
		cout << e.what << endl;
		system("pause");
		return 0;
	}
	while (true)
	{
		cout << string(10, '-') << "���˵�" << string(10, '-') << endl;
		cout << "����Ҫ��ʲô��\na.ʹ�����뷨\nb.�˳�����\n�������������";
		input(choice, "������a��b������������:", [](string& ch) { return ch == "a" || ch == "b"; });
		if (choice == "b")
			break;
		cout << string(10, '-') << "ʹ�����뷨" << string(10, '-') << endl;
		split();
	}
	save_user_wd();
	delete wd_fq;
	delete py_wd;
	delete wd_py;
	delete py_uservolc;
	system("pause");
	return 0;
}

bool parse(string str,vec& arr)
{
	auto lp = str.begin(), rp = str.begin(), ed = str.begin();
	if (ed == str.end())
		return true;
	while (*lp == '\'')
		ed = rp = ++lp;
	if (lp == ed)
		while (ed != str.end() && *ed != '\'')
			++ed;
	size_t&& dis = ed - lp;
	rp = lp + (dis >= 6 ? 6 : dis);
	while (rp - lp >= 1)
	{
		if (py_wd->find(string(lp, rp)) != py_wd->end() && parse(string(rp, str.end()), arr))
		{
			arr.emplace_back(lp, rp);
			break;
		}
		--rp;
	}
	if (lp == rp)
		return false;
	return true;
}

void read()
{
	auto read_in_file = [](const string& filename,function<void(char*&)> func)-> bool
	{
		int t0, t1;
		size_t length;
		char* buff;
		string addr = "d:\\" + filename + ".txt";
		ifstream fin(addr);
		if (!fin.is_open())
		{
			fin.close();
			if (filename != "�û��ʿ�")
				throw wrong(addr + "��ʧ�ܣ�\n�����ļ��Ƿ����D�̸�Ŀ¼��");
			else
			{
				py_uservolc = new loco;
				return true;
			}
		}
		t0 = clock();
		fin.seekg(0, ios::end); // go to the end  
		length = fin.tellg(); // report location (this is the length)  
		fin.seekg(0, ios::beg); // go back to the beginning  
		buff = new char[length + 1];
		memset(buff, '\0', length + 1);
		fin.read(buff, length);
		fin.close(); //�ر��ļ�
		t1 = clock();
		cout << "����" << addr << "��ʱ��" << t1 - t0 << "ms" << string(5, ' ') << "���ȣ�" << length << "�ֽ�" << endl;
		func(buff);
		delete[] buff;
		return true;
	};
	int t0 = clock();
	read_in_file("һ�ִ�", [](char*& buff)
	{
		wd_fq = new freq;
		freq& freq = *wd_fq;
		stringstream sstrm(buff);
		string a;
		int b;
		while (sstrm >> a >> b)
			freq.emplace(a, b);
	});
	int t1 = clock();
	cout << "��-��Ƶ������ʱ��" << t1 - t0 << "ms" << endl;
	t0 = clock();
	read_in_file("pinyin", [](char*& buff)
	{
		py_wd = new loco;
		loco& link = *py_wd;
		string pinyin;
		string word;
		string str(buff);
		for (auto it = str.begin(); it < str.end(); ++it)
		{
			if (*it != ',')
			{
				pinyin.push_back(*it);
				continue;
			}
			++it;
			vec& vec = link[pinyin];
			while (it < str.end() && *it != '\n')
			{
				word.push_back(*it);
				if (word.size() == 2)
				{
					vec.emplace_back(word);
					word.clear();
				}
				++it;
			}
			pinyin.clear();
			if (it == str.end())
				break;
		}
	});
	t1 = clock();
	cout << "ƴ��-���б������ʱ��" << t1 - t0 << "ms" << endl;
	t0 = clock();
	read_in_file("������", [](char*& buff)
	{
		vec pys;
		int count = 0;
		stringstream sstrm(buff);
		string a;
		while (sstrm >> a)
		{
			++count;
			if (count % 2 == 1)
			{
				pys.emplace_back(a);
				string str;
				getline(sstrm, str);
				stringstream ln(str);
				while (ln >> str)
					pys.emplace_back(str);
			}
			else
			{
				for (auto& x : pys)
				{
					vec& vec = py_wd->find(x)->second;
					if (find(vec.begin(), vec.end(), a) == vec.end())
						vec.emplace_back(a);
				}
				pys.clear();
			}
		}
	});
	for (auto& x : *py_wd)
	{
		vec& y = x.second;
		sort(y.begin(), y.end(), [](string& a,string& b) { return (*wd_fq)[a] > (*wd_fq)[b]; });
	}
	t1 = clock();
	cout << "�����ֻ�����ʱ��" << t1 - t0 << "ms" << endl;
	wd_py = new loco;
	t0 = clock();
	for (auto& x : *py_wd)
		for (auto& y : x.second)
			(*wd_py)[y].emplace_back(x.first);
	t1 = clock();
	cout << "��-ƴ���б������ʱ��" << t1 - t0 << "ms" << endl;
	t0 = clock();
	read_in_file("word", [](char*& buff)
	{
		py_volc = new loco;
		stringstream sstrm(buff);
		string volc;
		//word�еĲ�������ġ��ġ���pinyin�в鲻��  ȫ�ǰ��/ �� � �� �w �N ߯ �F �g �� ��
		while (sstrm >> volc)
		{
			if (volc.size() == 2)
				continue;
			string wd;
			vec pinyin;
			pinyin.reserve(100);
			for (auto& ch : volc)
			{
				wd.push_back(ch);
				if (wd.size() != 2)
					continue;
				vec oldpyvec(pinyin);
				size_t&& divi = oldpyvec.size();
				size_t&& newsize = (*wd_py)[wd].size() * divi;
				for (int i = 0; i < newsize - divi; ++i)
					pinyin.emplace_back(pinyin[0]);
				if (divi == 0)
					for (int j = 0; j < (*wd_py)[wd].size(); ++j)
						pinyin.emplace_back((*wd_py)[wd][j]);
				else
				{
					int k = 0;
					for (int i = 0; i < divi; ++i)
						for (int j = 0; j < newsize / divi; ++j)
							pinyin[k++] = oldpyvec[i] + '\'' + (*wd_py)[wd][j];
				}
				wd.clear();
			}
			for (auto& x : pinyin)
				(*py_volc)[x].emplace_back(volc);
		}
	});
	t1 = clock();
	cout << "ƴ��-�ʻ��б������ʱ��" << t1 - t0 << "ms" << endl;
	t0 = clock();
	read_in_file("�û��ʿ�", [](char*& buff)
	{
		py_uservolc = new loco;
		stringstream sstrm(buff);
		string a, b;
		while (sstrm >> a >> b)
			(*py_uservolc)[b].emplace_back(a);
	});
	t1 = clock();
	cout << "�û��ʿ������ʱ��" << t1 - t0 << "ms" << endl;
}

void split()
{
	cout << "������\n������ƴ�������ٽ���ѡ��\n1.\nƴ������Ϸ�\n�������ƴ����Ҫ�ֶ�����\'�Ի���\n"
		<< "2.\nѡ��ʱ������\'-\'��ǰ��ҳ������\'+\'���ҳ\n�����Ӧ�ʵ������ѡ��\n"
		<< "3.����Ҫ����\n�����������ƴ���ˣ���������0����;�˳�\n������˳�ѡ�ʣ����Լ���0����;�˳�\n"
		<< "4.����Ҫ����\nֻ�������˳�������ܱ������Լ��Ĵʻ�\n";
	while (true)
	{
		string choice, splitres, res_str;
		vec arr, words, res;
		size_t&& count = 0;
		cout << string(10, '-') << "ƴ������" << string(10, '-') << endl;
		while (true)
		{
			string str;
			cout << "ƴ������";
			getline(cin, str);
			if (str == "0")
				goto quit;
			if (!str.empty() && parse(str, arr))
				break;
			cout << "�޷���������ƴ���������������룡" << endl;
		}
		reverse(arr.begin(), arr.end());
		for (auto& x : arr)
			splitres += x + '\'';
		splitres.pop_back();
		cout << "�зֽ����\n" << splitres << endl;
		cout << string(10, '-') << "�ʻ�ѡ��" << string(10, '-') << endl;
		while (count < arr.size()) //ѡ����ѭ��
		{
			vec vec(arr.begin() + count, arr.end()), now_words;
			while (!vec.empty())
			{
				string pinyinstr;
				for (auto& x : vec)
					pinyinstr += x + '\'';
				pinyinstr.pop_back();
				auto it = py_uservolc->find(pinyinstr);
				if (it != py_uservolc->end())
					for (auto& x : it->second)
						now_words.emplace_back(x);
				loco* loco = vec.size() == 1 ? py_wd : py_volc;
				it = loco->find(pinyinstr);
				if (it != loco->end())
					for (auto& x : it->second)
						now_words.emplace_back(x);
				vec.pop_back();
			}
			int page = 0;
			int step;
			for (int k = 0; k < arr.size(); ++k)
				cout << (k == count ? "[" + arr[k] + "]" : arr[k] + "\'");
			cout << (arr.size() - 1 != count ? "\b \n" : "\n");
			for (int i = 0; i < 5 && page + i < now_words.size(); ++i)
				cout << i + 1 << "." << now_words[page + i] << " ";
			while (true)
			{
				const unsigned char ch = _getch();
				if (ch == '0')
					goto quit;
				if ('1' <= ch && ch <= '5' && page + (ch - '0') - 1 < now_words.size())
				{
					step = ch - '0' - 1;
					break;
				}
				bool&& get = false;
				if (ch == '-' && page > 0)
				{
					page -= 5;
					get = true;
				}
				else if (ch == '=' && page + 5 < now_words.size())
				{
					page += 5;
					get = true;
				}
				if (get)
				{
					cout << "\r" << string(59, ' ') << "\r";
					for (int i = 0; i < 5 && page + i < now_words.size(); ++i)
						cout << i + 1 << "." << now_words[page + i] << " ";
				}
			}
			res.emplace_back(now_words[page + step]);
			count += now_words[page + step].size() / 2;
			cout << endl << string(10, '-') << endl;
		}
		for (auto& x : res)
			res_str += x;
		cout << res_str;
		vec& volc_vec = py_volc->find(splitres)->second;
		bool&& not_in_volc = find(volc_vec.begin(), volc_vec.end(), res_str) == volc_vec.end();
		bool&& not_in_wd = py_wd->find(splitres) == py_wd->end();
		//�����û��ʿ�
		if (not_in_volc && not_in_wd)
		{
			auto it = py_uservolc->find(splitres);
			if (it == py_uservolc->end() || find(it->second.begin(), it->second.end(), res_str) == it->second.end())
				(*py_uservolc)[splitres].emplace_back(res_str);
		}
	quit:
		cout << "\n�Ƿ��������ƴ����(y/n)��";
		input(choice, "������y��n������������:", [](string& ch) { return ch == "y" || ch == "n"; });
		if (choice == "n")
			break;
	}
}

void save_user_wd()
{
	ofstream fout("d://�û��ʿ�.txt");
	for (auto& x : *py_uservolc)
		for (auto& y : x.second)
			fout << setiosflags(ios::left) << setw(20) << y << string(10, ' ') << x.first << endl;
	fout.close();
}

template <class Type,class Func> void input(Type& para,const string& str,Func& f)
{
	while (true)
	{
		getline(cin, para);
		if (f(para))
			break;
		cout << str;
	}
}

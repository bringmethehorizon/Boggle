#include "Dictionary.h"


Dictionary::Dictionary(string fileName)
{	
	const std::locale empty_locale = std::locale::empty();
    typedef std::codecvt_utf8<wchar_t> converter_type;
    const converter_type* converter = new converter_type;
    const std::locale utf8_locale = std::locale(empty_locale, converter);
	wfstream file,file2;
	file.open(fileName,ios::in);

	file2.open("bg1.dict",ios::out);
	//FILE* file2=fopen("bg1.dict","wb");
	
    //_setmode(_fileno(file2), _O_U16TEXT);
	wstring temp;
    file.imbue(utf8_locale);
	file2.imbue(utf8_locale);
	//int i=0,j;
	int j;
	while(file >> temp)
	{
		//if(i==10) break;
		if(iswupper(temp[0])|| iswdigit(temp[0]) || temp.length()<2) continue;
		if((j=temp.find(L"/"))!=wstring::npos)
		{
			temp=temp.substr(0,j);
		}
		data.insert(temp);
		//i++;
	}
	for(auto i: data)
	{
		file2 << i << endl;
	}
	file.close();
	file2.close();
}


Dictionary::~Dictionary(void)
{
}

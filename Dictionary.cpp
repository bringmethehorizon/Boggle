#include "Dictionary.h"


Dictionary::Dictionary(string fileName)
{	
	//UTF8 encoding
	const std::locale empty_locale = std::locale::empty();
	typedef std::codecvt_utf8<wchar_t> converter_type;
	const converter_type* converter = new converter_type;
	const std::locale utf8_locale = std::locale(empty_locale, converter);


	wfstream file;
	file.open(fileName,ios::in);
	file.imbue(utf8_locale);

	//read from the wordlist
	wstring temp;
	int j;
	while(file >> temp)
	{
		data.insert(temp);
	}
}
void Dictionary::printWords()
{
	for(auto i: data)
	{
		wcout << i << endl;
	}
}


Dictionary::~Dictionary(void)
{
}

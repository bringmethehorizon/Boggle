#include "Dictionary.h"


Dictionary::Dictionary(void)
{
}
void Dictionary::loadFile(string fileName)
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
		if(temp.length()<3 || temp.length()>MAX_DEPTH) continue;
		data.insert(temp);
		triples.insert(temp.substr(0,3));
		pairs.insert(temp.substr(0,2));
	}
}
void Dictionary::printWords()
{
	for(auto i: data)
	{
		wcout << i << endl;
	}
}
bool Dictionary::wordExists(wstring word)
{
	return (data.find(word)!=data.end());
}
bool Dictionary::tripleExists(wstring word)
{
	return (triples.find(word)!=triples.end());
}
bool Dictionary::pairExists(wstring word)
{
	return (pairs.find(word)!=pairs.end());
}
Dictionary::~Dictionary(void)
{
}

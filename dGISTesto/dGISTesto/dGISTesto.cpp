// dGISTesto.cpp : Defines the entry point for the console application.
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <conio.h>
#include <fstream>

using namespace std;

void outputHelp();
int parseCommands( int argc, char** argv, char* inputFile, char* searchWord );
void wordsCount(const char* inputFile, const char* searchWord);
void checksumm(const char* inputFile);

int main(int argc, char* argv[])
{
	char inputFile[255] = "";
	char searchWord[255] = "";
	int  methodId = 0; //0 -wordscount, 1 -checksumm 

	methodId = parseCommands(argc, argv, inputFile, searchWord);

	cout << "\nInput parametres: \nFile:" << inputFile << "\nMethod:" << ((methodId == 0) ? "words":"checksumm") 
		 << "\nSearch word:" << searchWord << "\nBeginning..." << endl; 

	if (methodId == 0)
		wordsCount(inputFile, searchWord);

	_getch();

	return 0;
}

void outputHelp()
{
	cout << "Commands:\n -f - input file\n -m - method id: words - count of words in input file, checksumm - checksumm of input file\n -v - searching word for words command\n -h - HELP" << endl;
}

int parseCommands( int argc, char** argv, char* inputFile, char* searchWord )
{
	int methodId = -1;

	for (int i = 1; i < argc; i++)
	{
		cout << "ARG " << i << ": " << argv[i] << endl;

		char* currArg = argv[i];

		if (currArg[0] == '-')
		{
			if (currArg[1] == 'h')
			{
				outputHelp();
			}
			else if (currArg[1] == 'f')
			{
				if (i <= argc - 2)
				{
					char* nextArg = argv[i + 1];
					strcpy(inputFile, nextArg);

					i++;
				}
				else
				{
					cout << "ERROR: Expected filename after -f command" << endl;
				}
			}
			else if (currArg[1] == 'm')
			{
				if (i <= argc - 2)
				{
					char* nextArg = argv[i + 1];
					if (strcmp(nextArg, "words") == 0)
						methodId = 0;
					else if (strcmp(nextArg, "checksumm") == 0)
						methodId = 1;
					else
						cout << "ERROR: Bad method id specified:" << nextArg << endl;

					i++;
				}
				else
				{
					cout << "ERROR: Expected method name after -m command" << endl;
				}
			}
			else if (currArg[1] == 'v')
			{
				if (i <= argc - 2)
				{
					char* nextArg = argv[i + 1];
					strcpy(searchWord, nextArg);

					i++;
				}
				else
				{
					cout << "ERROR: Expected search value for words command" << endl;
				}
			}
		}
		else
		{
			cout << "WARNING: Parameter is not command:" << currArg << endl;
		}
	}	
	
	return methodId;
}

int getWordsCountInBuffer(const char* buffer, int bufferLength, const char* word)
{
	int wcount = 0;

	int wordLength = strlen(word);

	for (int i = 0, j = 0; i < bufferLength; i++, j++)
	{
		if (buffer[i] != word[j])
		{
			j = -1;
		}

		if (j == wordLength - 1)
		{
			wcount++;
			j = -1;
		}
	}

	return wcount;
}

void wordsCount( const char* inputFile, const char* searchWord )
{
	const int bufferSize = 1024;
	char buffer[1024];
	int bufferLength = 0;

	std::ifstream ifs;
	ifs.open(inputFile, std::ios::binary);
	if (!ifs.is_open())
	{
		cout << "ERROR: Can' open file " << inputFile << "\nAborting" << endl;
		return;
	}

	int wordsCount = 0;

	do 
	{
		ifs.read(buffer, bufferSize);
		bufferLength = ifs.gcount();

		wordsCount += getWordsCountInBuffer(buffer, bufferLength, searchWord);

		if (bufferLength != bufferSize)
			break;
	}
	while (true);

	cout << "In file found " << wordsCount << " words '" << searchWord << "'" << endl;
}

void checksumm( const char* inputFile )
{
}


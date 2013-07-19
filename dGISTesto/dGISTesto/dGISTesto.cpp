// dGISTesto.cpp : Defines the entry point for the console application.
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <conio.h>
#include <fstream>

using namespace std;

enum MethodId { MID_WORDS_COUNT, MID_CHECKSUMM, MID_INVALID };

/** parsing commands, returns parsed method id, getting input filename and searching word. */
MethodId parseCommands( int argc, char** argv, char* inputFile, char* searchWord );

/** Puts help into console. */
void     outputHelp();

/** Returns count of searchWord in file inputFile. */
void     wordsCount(const char* inputFile, const char* searchWord);

/** Return checksumm of inputFile. */
void     checksumm(const char* inputFile);

int main(int argc, char* argv[])
{
	char inputFile[255] = "";
	char searchWord[255] = "";
	MethodId  methodId = MID_INVALID;

	methodId = parseCommands(argc, argv, inputFile, searchWord);

	cout << "Input parametres: \nFile:" << inputFile 
		 << "\nMethod:" << ((methodId == 0) ? "words":"checksumm") 
		 << "\nSearch word:" << searchWord 
		 << "\nBeginning..." << endl; 

	if (methodId == MID_WORDS_COUNT)
		wordsCount(inputFile, searchWord);
	if (methodId == MID_CHECKSUMM)
		checksumm(inputFile);

	cout << "Tap any key to exit";

	_getch();

	return 0;
}

void outputHelp()
{
	cout << "Commands:\n -f - input file\n -m - method id: words - count of words in input file, checksumm - checksumm of input file\n -v - searching word for words command\n -h - HELP" << endl;
}

MethodId parseCommands( int argc, char** argv, char* inputFile, char* searchWord )
{
	MethodId methodId = MID_INVALID;

	//skipping first parameter - .exe path
	for (int i = 1; i < argc; i++)
	{
		char* currArg = argv[i];

		//search command prefix
		if (currArg[0] == '-')
		{
			//print help
			if (currArg[1] == 'h')
			{
				outputHelp();
			}
			//getting input file
			else if (currArg[1] == 'f')
			{
				//check args count, next arg must be filename
				if (i <= argc - 2)
				{
					char* nextArg = argv[i + 1];
					strcpy(inputFile, nextArg);

					//skipping filename argument
					i++;
				}
				else
				{
					cout << "ERROR: Expected filename after -f command" << endl;
				}
			}
			//getting method id
			else if (currArg[1] == 'm')
			{
				//check args count, next argument must be method id
				if (i <= argc - 2)
				{
					char* nextArg = argv[i + 1];

					if (strcmp(nextArg, "words") == 0)
					{
						methodId = MID_WORDS_COUNT;
					}
					else if (strcmp(nextArg, "checksumm") == 0)
					{
						methodId = MID_CHECKSUMM;
					}
					else
					{
						cout << "ERROR: Bad method id specified:" << nextArg << endl;
					}

					//skipping method id argument
					i++;
				}
				else
				{
					cout << "ERROR: Expected method name after -m command" << endl;
				}
			}
			//getting word for words method
			else if (currArg[1] == 'v')
			{
				//check args count, next argument must be word
				if (i <= argc - 2)
				{
					char* nextArg = argv[i + 1];
					strcpy(searchWord, nextArg);					

					//skipping word argument
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

/** Return words count in buffer. */
int getWordsCountInBuffer(const char* buffer, unsigned long bufferLength, const char* word)
{
	int wcount = 0;

	int wordLength = strlen(word);

	for (unsigned long i = 0, j = 0; i < bufferLength; i++, j++)
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
	//getting data by 4mb parts
	const unsigned long bufferSize = 1024*1024*4;
	char* buffer = new char[bufferSize];
	unsigned long bufferLength = 0;

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
		bufferLength = (unsigned long)ifs.gcount();

		wordsCount += getWordsCountInBuffer(buffer, bufferLength, searchWord);

		if (bufferLength != bufferSize)
			break;
	}
	while (true);

	delete[] buffer;

	cout << "In file found " << wordsCount << " words '" << searchWord << "'" << endl;
}

/** returns checksumm in buffer. */
unsigned long long getBufferCheckSumm(const char* buffer, unsigned long bufferSize)
{
	unsigned long long checkSumm = 0;

	for (unsigned long i = 0; i < bufferSize/4; i++)
	{
		checkSumm += (unsigned long long)( *((unsigned int*)(&buffer[i*4])) );
	}

	return checkSumm;
}

void checksumm( const char* inputFile )
{
	//getting data by 4mb parts
	const unsigned long bufferSize = 1024*1024*4;
	char* buffer = new char[bufferSize];
	unsigned long bufferLength = 0;

	std::ifstream ifs;
	ifs.open(inputFile, std::ios::binary);
	if (!ifs.is_open())
	{
		cout << "ERROR: Can' open file " << inputFile << "\nAborting" << endl;
		return;
	}

	unsigned long long checkSumm = 0;

	do 
	{
		ifs.read(buffer, bufferSize);
		bufferLength = (unsigned long)ifs.gcount();

		checkSumm += getBufferCheckSumm(buffer, bufferLength);

		if (bufferLength != bufferSize)
			break;
	}
	while (true);

	delete[] buffer;

	cout << "File checksumm = " << checkSumm << endl;
}


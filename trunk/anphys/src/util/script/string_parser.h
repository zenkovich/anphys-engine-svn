#ifndef STRING_PARSER_H
#define STRING_PARSER_H

#include <vector>

struct cLexBlock
{
	enum BlockType { BT_BLOCK = 0, BT_COMMAND, BT_VALUE };

	char*		mString;
	BlockType	mBlockType;
};

struct cLexOperatorPrototype
{
	typedef std::vector<cLexBlock::BlockType>	OperatorFormatArray;

	OperatorFormatArray	mFormat;
};

struct cLexOperator
{
	typedef std::vector<cLexBlock*>				LexicalBlocksList;

	cLexOperatorPrototype*	mOperatorPrototype;

	LexicalBlocksList		mLexicalBlocks;
};

struct cLexErrorPrototype
{
	enum LexErrorType { ERR_UNKNOWN = 0, W0_UNKNOWN, W1_UNKNOWN };

	LexErrorType	mType;
	char			mErrorLogFormat;
};  

struct cLexOperandsManager
{
	typedef std::vector<cLexOperator*> OperatorsList;

	OperatorsList	mOperators;

	cLexOperandsManager();

	cLexOperator* findOperator(const char* str);
};

struct cLexParser
{
	char*					mBuffer;
	int						mBufferLength;
	int						mPosition;

	cLexOperandsManager*	mLexOperandsManager;
};



#endif //STRING_PARSER_H
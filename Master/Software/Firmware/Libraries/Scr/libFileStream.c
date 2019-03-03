#include "libFileStream.h"

int fputc(int character, FILE *stream) {
	if(stream->outputFunctionPointer) {
		stream->outputFunctionPointer((char)character,stream);
		return character;
	}else
		return -1;
};

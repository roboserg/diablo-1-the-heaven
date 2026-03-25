// BinPatch 1.0 (c) 2009 Desert Dragon
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_FILE_SIZE 1024*1024*1024

int main(int argc, char* argv[])
{
	int result = 0;
	if( argc != 3){
		printf("BinPatch 1.0 (c) 2009 Desert Dragon\nError 1 - Usage: binpatch [modified file] [file with changes]\n");
		return 1;
	}
	char* sourceName = argv[1];
	char* patchName = argv[2];
	char targetName[1024];

	FILE *sourceFile = fopen(sourceName,"rb");
	if(!sourceFile){
		printf("\nError 2 - Can not read modified file: %s\n", sourceName);
		return 2;
	}
	fseek(sourceFile, 0, SEEK_END);
	int size = ftell(sourceFile);
	fseek(sourceFile, 0, SEEK_SET);
	if (size > MAX_FILE_SIZE){
		printf("\nError 3 - Too big modified file: %s\nMaximum 1 Gb\n", sourceName);
		fclose(sourceFile);
		return 3;
	}
	FILE *patchFile = fopen(patchName,"r");
	if(!patchFile){
		printf("\nError 4 - Can not read patch file: %s\n", patchName);
		fclose(sourceFile);
		return 4;
	}
	strcpy(targetName, patchName);
	strcat(targetName, sourceName);
	FILE *targetFile = fopen(targetName,"rb");
	if(targetFile){
		fseek(targetFile, 0, SEEK_END);
		if( ftell(targetFile) == 0 ){
			fclose(targetFile);
		}else{
			printf("\nError 5 - Target file already exist: %s\n", targetName);
			fclose(targetFile);
			fclose(sourceFile);
			fclose(patchFile);
			return 5;
		}
	}
	targetFile = fopen(targetName,"wb");
	if(!targetFile){
		printf("\nError 6 - Can not create target file: %s\n", targetName);
		fclose(sourceFile);
		fclose(patchFile);
		return 6;
	}
	unsigned char* targetContext = new unsigned char[size];
	fread(targetContext, 1, size, sourceFile);

	enum { readOffset, readOldValue, readNewValue, readError };
	int state = readOffset;
	int offset = -1;
	int oldValue = -2;
	int newValue = -3;
	int changeCount = 0;
	bool comment = false;
	bool isOffset;
	char data[257];
	data[256] = 0;
	while( fscanf_s(patchFile, "%s", data, 256) != EOF ) {
		switch( state ) {
			case readOffset:
				isOffset = true;
				for(int i=0; data[i]; i++){
					if( !isxdigit(data[i]) && data[i] != ':' ){
						isOffset = false;
						break;
					}
				}
				offset = strtol(data,0,16);
				if( !isOffset || offset < 0 || offset > size-1 || (offset == 0 && strcmp(data,"00000000:")) ){
					state = readOffset;
					if(!comment){
						printf("\ncomment:");
						comment = true;
					}
					printf(" %s", data);//
				}else{
					state = readOldValue;
					if(offset != newValue || strlen(data) > 2 ){
						printf("\nChanged %08X: ", offset);//
						comment = false;
					}
				}
				break;
			case readOldValue:
				oldValue = strtol(data,0,16);
				if( oldValue < 0 || oldValue > 255 || (oldValue == 0 && strcmp(data,"00")) ){
					state = readError;
				}else if( targetContext[offset] != (unsigned char)oldValue ){
					printf("\nError 7 - Old value %X: %X not match %X\n", offset, (int)targetContext[offset], oldValue);
					fclose(targetFile);
					fclose(sourceFile);
					fclose(patchFile);
					result = 7; goto endLabel;
				}else{
					state = readNewValue;
					printf("%02X ", oldValue);//
					comment = false;
				}
				break;
			case readNewValue:
				newValue = strtol(data,0,16);
				if( newValue < 0 || newValue > 255 || (newValue == 0 && strcmp(data,"00")) ){
					state = readError;
				}else{
					targetContext[offset] = (unsigned char)newValue;
					changeCount++;
					state = readOffset;
					printf("to %02X", newValue);//
					comment = false;
				}
				break;
			case readError:
				printf("\nError 8 - patch file %s read error, check file format\n", patchName);
				fclose(targetFile);
				fclose(sourceFile);
				fclose(patchFile);
				result = 8; goto endLabel;
		}
	}
	if( state != readOffset || changeCount < 1 || changeCount > size ){
		printf("\nError 8 - patch file %s read error, check file format\n", patchName);
		fclose(targetFile);
		fclose(sourceFile);
		fclose(patchFile);
		result = 8; goto endLabel;
	}

	if( fwrite(targetContext, 1, size, targetFile) != size ){
		printf("\nError 9 - target file %s write error\n", targetName);
		fclose(targetFile);
		fclose(sourceFile);
		fclose(patchFile);
		result = 9; goto endLabel;
	}

	printf("\nOK. Patched file name: %s\nCount of changes: %d\n", targetName, changeCount);
	fclose(targetFile);
	fclose(sourceFile);
	fclose(patchFile);
	
endLabel:	
	targetFile = fopen(targetName,"rb");
	if(targetFile){
		fseek(targetFile, 0, SEEK_END);
		if( ftell(targetFile) == 0 ){
			fclose(targetFile);
			remove(targetName);
		}else{
			fclose(targetFile);
		}
	}
	delete[] targetContext;
	return result;
}


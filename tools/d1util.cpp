

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>
#include <list>

FILE* dataseg;
FILE* dataseg_out;

struct OneSpeech {
	std::string		name;
	int				a1;
	int				a2;
	int				a3;
	std::string		text;
};
std::list<OneSpeech> Speeches;


void FirstPhase( char* first_line) {
	//SpeechsPtr      Speech <aAhhTheStoryOfO, 1, 5, 17Fh>
	char shit[256] ={0};
	char name[256] ={0};
	int a1 =0, a2 =0, a3 =0;
	sscanf( first_line, "%[^<]<%[^,], %i, %i, %X", shit, name, &a1, &a2, &a3);
	OneSpeech speech;
	speech.name =name;
	speech.a1 =a1;
	speech.a2 =a2;
	speech.a3 =a3;
	Speeches.push_back( speech);

	for( ;;) {
		char buf[1024] ={0};
		fgets( buf, 1024, dataseg);
		if( feof( dataseg)) break;
		if( strncmp( buf, "NumberOfSpeech", 14) ==0) {
			fputs( buf, dataseg_out);
			break;
		}
		char shit[256] ={0};
		char name[256] ={0};
		sscanf( buf, "%[^<]<%[^,], %i, %i, %X", shit, name, &a1, &a2, &a3);
		char strout[1024] ={0};
		sprintf( strout, "%s %u %u %u", name, a1, a2, a3);
		OneSpeech speech;
		speech.name =name;
		speech.a1 =a1;
		speech.a2 =a2;
		speech.a3 =a3;
		Speeches.push_back( speech);
	}
}

void NormalizeString( char* str) {
	std::string s =str;
	for( ;;) {
		int pos =s.find( "',0Ah");
		if( pos ==std::string::npos) break;
		s =s.substr( 0, pos) +"\\n";
	}
	for( ;;) {
		int pos =s.find( "',27h,'");
		if( pos ==std::string::npos) break;
		s =s.substr( 0, pos) +"#" +s.substr( pos +7);
	}
	strcpy( str, s.c_str());
}

void NormalizeString2( char* str) {
	for( unsigned int n=0;n<strlen(str);n++) if( str[n] =='#') str[n] ='\'';
}

void SecondPhase( char* first_line, std::list<OneSpeech>::iterator& it) {
	//aAhhTheStoryOfO	db ' Ahh, the story of our King, is it?  The tragic fall of Leoric wa'
	NormalizeString( first_line);
	char shit[1024] ={0};
	char text[1024] ={0};
	char rest[1024] ={0};
	sscanf( first_line, "%[^']'%[^']'%s", shit, text, rest);
	NormalizeString2( text);
	it->text =text;
	if( strncmp( rest, ",0", 2) ==0) return;
	for( ;;) {
		char buf[1024] ={0};
		fgets( buf, 1024, dataseg);
		if( feof( dataseg)) break;
		NormalizeString( buf);
		char shit[1024] ={0};
		char text[1024] ={0};
		char rest[1024] ={0};
		sscanf( buf, "%[^']'%[^']'%s", shit, text, rest);
		if( text[0] ==0) return;
		NormalizeString2( text);
		it->text +=text;
		if( strncmp( rest, ",0A", 3) ==0) continue;
		if( strncmp( rest, ",0", 2) ==0) return;
	}
}

void main( void) {
	 dataseg=fopen( "dataseg.asm", "r");
	if( !dataseg) return;
	dataseg_out =fopen( "dataseg.asm.out", "w");
	for( ;;) {
break_break:
		char buf[1024] ={0};
		fgets( buf, 1024, dataseg);
		if( feof( dataseg)) break;
		if( strncmp( buf, "SpeechsPtr", 10) ==0) {
			FirstPhase( buf);
		}
		if( Speeches.size()) {
			for( std::list<OneSpeech>::iterator it=Speeches.begin();it!=Speeches.end();it++) {
				if( strncmp( buf, it->name.c_str(), it->name.size()) ==0) {
					SecondPhase( buf, it);
					goto break_break;
				}
			}
		}
		fputs( buf, dataseg_out);
	}
	fclose( dataseg);
	fclose( dataseg_out);

	for( std::list<OneSpeech>::iterator it=Speeches.begin();it!=Speeches.end();it++) {
		if( it->text =="") {
			for( std::list<OneSpeech>::iterator it2=Speeches.begin();it2!=it;it2++) {
				if( it->name ==it2->name) it->text =it2->text;
			}
		}
	}

	FILE* structs =fopen( "structs_speech.cpp.out", "w");
	fprintf( structs, "#include \"stdafx.h\"\n");
	fprintf( structs, "#include \"../inc/structs.h\"\n");
	fprintf( structs, "\nstruct Speech SpeechsPtr[] ={\n");
	for( std::list<OneSpeech>::iterator it=Speeches.begin();it!=Speeches.end();it++) {
		fprintf( structs, "\t//%s\n", it->name.c_str());
		if( it->text.size() ==0) {
			fprintf( structs, "\t{ \"|\", %i, %i, %i },\n", it->a1, it->a2, it->a3);
		} else if( it->text.size() <70) {
			fprintf( structs, "\t{ \"%s\", %i, %i, %i },\n", it->text.c_str(), it->a1, it->a2, it->a3);
		} else {
			fprintf( structs, "\t{ \"%s\",\n\t%i, %i, %i },\n", it->text.c_str(), it->a1, it->a2, it->a3);
		}
		fprintf( structs, "\n");
	}
	fprintf( structs, "};\n\n");

	fclose( structs);
}


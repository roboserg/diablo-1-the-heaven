BEGIN	{
	StructName = ""
	SingleType = ""
	ArrayType = ""
	print "#ifndef _STRUCTS_H_";
	print "#define _STRUCTS_H_";
	print "";
	print "#include \"defs.h\"";
	print "";
}

# Is not a comment?
($1 != ";")	{

	if( $2 == "struc" && StructName == "" && $5 != "standard")
	{	#	start of struct declaration
		StructName = $1
		print "struct " StructName
		print "{"
	}
	else if( StructName != "" ) # inside structure declaration
	{
		if( $2 == "ends" )
		{	#	end of struct declaration
			print "};"
			print ""
			StructName = ""
		}
		else
		{
			if( $2 == "db" )
			{
				SingleType = "unsigned char"
				ArrayType = "char"
			}
			else if( $2 == "dw" )
			{
				SingleType = "unsigned short"
				ArrayType = "unsigned short"
			}
			else if( $2 == "dd" )
			{
				SingleType = "int"
				ArrayType = "unsigned"
			}
			else
			{
				SingleType = $2
				ArrayType = $2
			}

			if( $3 != "?" && $4 == "dup(?)" )
			{
				print "	" ArrayType "	" $1 "[" $3 "];" "	//[" $3 "," $4 "," $5 "," $6 "," $7 "," $8 "," $9 "]"
			}
			if( $3 != "?" && $4 == "dup(<?>)" )
			{
				print "	" ArrayType "	" $1 "[" $3 "];" "	//[" $3 "," $4 "," $5 "," $6 "," $7 "," $8 "," $9 "]"
			}
			else if( $4 == ";" && $5 == "offset" )
			{
				print "	char*	" $1 ";" "	//[" $3 "," $4 "," $5 "," $6 "," $7 "," $8 "," $9 "]"
			}
			else
			{
				print "	" SingleType "	" $1 ";" "	//[" $3 "," $4 "," $5 "," $6 "," $7 "," $8 "," $9 "]"
			}
		}
	}
	else if( $2 == "=" )
	{
		print "const int " $1 " = " $3 ";"
	}
}

END	{

	print "";
	print "#endif // _STRUCTS_H_";
	print "";
}
#include "pch.h"
#include "ASEfile.h"

#define	NUMBER_OF_RESERVEDWORD				234
LPCSTR Token[256]	 = 
{
	"*3DSMAX_ASCIIEXPORT"		/*  0*/
,  "*COMMENT"					/*  1*/
,  "*SCENE"						/*  2*/
,  "*MATERIAL_LIST"				/*  3*/
,  "*MATERIAL"					/*  4*/
,  "*LIGHTOBJECT"				/*  5*/
,  "*GEOMOBJECT"				/*  6*/
,  "*NODE_NAME"					/*  7*/
,  "*NODE_TM"					/*  8*/
,  "*MESH"						/*  9*/
,  "*MESH_VERTEX"				/* 10*/
,  "*MESH_FACE_LIST"			/* 11*/
,  "*MESH_FACE"					/* 12*/
,  "*MESH_NUMTVERTEX"			/* 13*/
,  "*MESH_NUMCVERTEX"			/* 14*/
,  "*MESH_NORMALS"				/* 15*/
,  "*MESH_FACENORMAL"			/* 16*/
,  "*MESH_VERTEXNORMAL"			/* 17*/
,  "*HELPEROBJECT"				/* 18*/
,  "*NODE_PARENT"				/* 19*/
,  "*HELPER_CLASS"				/* 20*/
,  "*INHERIT_POS"				/* 21*/
,  "*INHERIT_ROT"				/* 22*/
,  "*INHERIT_SCL"				/* 23*/
,  "*TM_ROW0"					/* 24*/
,  "*TM_ROW1"					/* 25*/
,  "*TM_ROW2"					/* 26*/
,  "*TM_ROW3"					/* 27*/
,  "*TM_POS"					/* 28*/
,  "*TM_ROTAXIS"				/* 29*/
,  "*TM_ROTANGLE"				/* 30*/
,  "*TM_SCALE"					/* 31*/
,  "*TM_SCALEAXIS"				/* 32*/
,  "*TM_SCALEAXISANG"			/* 33*/
,  "*BOUNDINGBOX_MIN"			/* 34*/
,  "*BOUNDINGBOX_MAX"			/* 35*/
,  "*TIMEVALUE"					/* 36*/
,  "*MESH_NUMVERTEX"			/* 37*/
,  "*MESH_NUMFACES"				/* 38*/
,  "*GROUP"						/* 39*/
,  "*MESH_TVERT"				/* 40*/
,  "*MESH_TFACELIST"			/* 41*/
,  "*MESH_TVERTLIST"			/* 42*/
,  "*PROP_MOTIONBLUR"			/* 43*/
,  "*PROP_CASTSHADOW"			/* 44*/
,  "*PROP_RECVSHADOW"			/* 45*/
,  "*MATERIAL_REF"				/* 46*/
,  "*NODE_PARENT"				/* 47*/
,  "*SCENE_FILENAME"			/* 48*/
,  "*SCENE_FIRSTFRAME"			/* 49*/
,  "*SCENE_LASTFRAME"			/* 50*/
,  "*SCENE_FRAMESPEED"			/* 51*/
,  "*SCENE_TICKSPERFRAME"		/* 52*/
,  "*SCENE_ENVMAP"				/* 53*/
,  "*MAP_NAME"					/* 54*/
,  "*MAP_CLASS"					/* 55*/
,  "*MAP_SUBNO"					/* 56*/
,  "*MAP_AMOUNT"				/* 57*/
,  "*SCENE_AMBIENT_STATIC"		/* 58*/
,  "*MATERIAL_COUNT"			/* 59*/
,  "*MATERIAL_NAME"				/* 60*/
,  "*MATERIAL_CLASS"			/* 61*/
,  "*MATERIAL_AMBIENT"			/* 62*/
,  "*MATERIAL_DIFFUSE"			/* 63*/
,  "*MATERIAL_SPECULAR"			/* 64*/
,  "*MATERIAL_SHINE"			/* 65*/
,  "*MATERIAL_SHINESTRENGTH"	/* 66*/
,  "*MATERIAL_TRANSPARENCY"		/* 67*/
,  "*MATERIAL_WIRESIZE"			/* 68*/
,  "*MATERIAL_SHADING"			/* 69*/
,  "*MATERIAL_XP_FALLOFF"		/* 70*/
,  "*MATERIAL_SELFILLUM"		/* 71*/
,  "*MATERIAL_TWOSIDED"			/* 72*/
,  "*MATERIAL_FALLOFF"			/* 73*/
,  "*MATERIAL_XP_TYPE"			/* 74*/
,  "*MAP_DIFFUSE"				/* 75*/
,  "*MESH_VERTEX_LIST"			/* 76*/
,  "*MESH_NUMTVFACES"			/* 77*/
,  "*MESH_CVERTLIST"			/* 78*/
,  "*LIGHT_TYPE"				/* 79*/
,  "*LIGHT_SHADOWS"				/* 80*/
,  "*LIGHT_USELIGHT"			/* 81*/
,  "*LIGHT_SPOTSHAPE"			/* 82*/
,  "*LIGHT_USEGLOBAL"			/* 83*/
,  "*LIGHT_ABSMAPBIAS"			/* 84*/
,  "*LIGHT_OVERSHOOT"			/* 85*/
,  "*LIGHT_SETTINGS"			/* 86*/
,  "*LIGHT_COLOR"				/* 87*/
,  "*LIGHT_INTENS"				/* 88*/
,  "*LIGHT_ASPECT"				/* 89*/
,  "*LIGHT_TDIST"				/* 90*/
,  "*LIGHT_MAPBIAS"				/* 91*/
,  "*LIGHT_MAPRANGE"			/* 92*/
,  "*LIGHT_MAPSIZE"				/* 93*/
,  "*LIGHT_RAYBIAS"				/* 94*/
,	"*MESH_SMOOTHING"			/* 95*/
,  "*MESH_MTLID"				/* 96*/
,  "*WIREFRAME_COLOR"			/* 97*/
,	"*TM_ANIMATION"				/* 98*/
,	"*CONTROL_POS_TRACK"		/* 99*/
,	"*CONTROL_ROT_TRACK"		/*100*/
,	"*CONTROL_POS_SAMPLE"		/*101*/
,	"*CONTROL_ROT_SAMPLE"		/*102*/
,	"*CAMERAOBJECT"				/*103*/
,	"*CAMERA_TYPE"				/*104*/
,	"*CAMERA_SETTINGS"			/*105*/
,	"*CAMERA_NEAR"				/*106*/
,	"*CAMERA_FAR"				/*107*/
,	"*CAMERA_FOV"				/*108*/
,	"*CAMERA_TDIST"				/*109*/
,	"*NUMSUBMTLS"				/*110*/
,	"*SUBMATERIAL"				/*111*/
,	"*MAP_SPECULAR"				/*112*/
,	"*MAP_SHINE"				/*113*/
,	"*MAP_GENERIC"				/*114*/
,	"*BITMAP"					/*115*/
,	"*MAP_TYPE"					/*116*/
,	"*UVW_U_OFFSET"				/*117*/
,	"*UVW_V_OFFSET"				/*118*/
,	"*UVW_U_TILING"				/*119*/
,	"*UVW_V_TILING"				/*120*/
,	"*UVW_ANGLE"				/*121*/
,	"*UVW_BLUR"					/*122*/
,	"*UVW_BLUR_OFFSET"			/*123*/
,	"*UVW_NOUSE_AMT"			/*124*/
,	"*UVW_NOISE_SIZE"			/*125*/
,	"*UVW_NOISE_LEVEL"			/*126*/
,	"*UVW_NOISE_PHASE"			/*127*/
,	"*BITMAP_FILTER"			/*128*/
,  "*MESH_MAPPINGCHANNEL"		/*129*/
,  "*MESH_TFACE"				/*130*/
,  "*CONTROL_POS_BEZIER"		/*131*/
,  "*CONTROL_BEZIER_POS_KEY"	/*132*/
,  "*CONTROL_ROT_TCB"			/*133*/
,  "*CONTROL_TCB_ROT_KEY"		/*134*/
,  "*MAP_OPACITY"				/*135*/
,  "*MATERIAL_FACEMAP"			/*136*/
,	"*MESH_NUMSKINWEIGHT"		/*137*/
,	"*MESH_NUMBONE"				/*138*/
,	"*BONE_LIST"				/*139*/
,	"*BONE_NAME"				/*140*/
,	"*MESH_WVERTEXS"			/*141*/
,	"*MESH_WEIGHT"				/*142*/
,	"*BONE_BLENGING_WEIGHT"		/*143*/
,	"*MAP_REFLECT"				/*144*/
,	"*MAP_REFRACT"				/*145*/
,	"*MAP_BUMP"					/*146*/
,	"*SCENE_BACKGROUND_STATIC"	/*147*/
,	"*MAP_SELFILLUM"			/*148*/
,	"*LIGHT_EXCLUDELIST"		/*149*/
,	"*LIGHT_NUMEXCLUDED"		/*150*/
,	"*LIGHT_EXCLUDED_INCLUDE"	/*151*/
,	"*LIGHT_EXCLUDED_AFFECT_ILLUM"	/*152*/
,	"*LIGHT_EXCLUDED_AFFECT_SHADOW"	/*153*/
,	"*LIGHT_EXCLUDED"			/*154*/
,	"*LIGHT_HOTSPOT"			/*155*/
,	"*LIGHT_FALLOFF"			/*156*/
,  "*MESH_FACEMAPLIST"			/*157*/
,  "*MESH_FACEMAP"				/*158*/
,  "*MESH_FACEMAPVERT"			/*159*/
,	"*BITMAP_INVERT"			/*160*/
,	"*SHAPEOBJECT"				/*161*/
,	"*SHAPE_LINECOUNT"			/*162*/
,	"*SHAPE_LINE"				/*163*/
,	"*SHAPE_VERTEXCOUNT"		/*164*/
,	"*SHAPE_VERTEX_KNOT"		/*165*/
,	"*SHAPE_VERTEX_INTERP"		/*166*/
,	"*SHAPE_CLOSED"				/*167*/
,	"*IK_JOINT"					/*168*/
,	"*IK_TYPE"					/*169*/
,	"*IK_DOF"					/*170*/
,	"*IK_XACTIVE"				/*171*/
,	"*IK_YACTIVE"				/*172*/
,	"*IK_ZACTIVE"				/*173*/
,	"*IK_XLIMITED"				/*174*/
,	"*IK_YLIMITED"				/*175*/
,	"*IK_ZLIMITED"				/*176*/
,	"*IK_XEASE"					/*177*/
,	"*IK_YEASE"					/*178*/
,	"*IK_ZEASE"					/*179*/
,	"*IK_LIMITEXACT"			/*180*/
,	"*IK_JOINTINFO"				/*181*/
,	"*LIGHT_ATTNSTART"			/*182*/
,	"*LIGHT_ATTNEND"			/*183*/
,	"*MAP_AMBIENT"				/*184*/
,	"*MESH_VERTCOL"				/*185*/
,	"*MESH_NUMCVFACES"			/*186*/
,	"*MESH_CFACELIST"			/*187*/
,	"*MESH_CFACE"				/*188*/

,	"*MAP_SHINESTRENGTH"		/*189*/
,	"*MAP_FILTERCOLOR"			/*190*/

,	"*NODE_VISIBILITY_TRACK"	/*191*/
,	"*CONTROL_FLOAT_SAMPLE"		/*192*/
,	"*CONTROL_FLOAT_KEY"		/*193*/

,	"*BONE_PROPERTY"			/*194*/
,	"*BONE"						/*195*/
,	"*SKIN_INITTM"				/*196*/
,	"*CONTROL_SCALE_TRACK"		/*197*/
,	"*CONTROL_SCALE_SAMPLE"		/*198*/
,	"*SCENE_AMBIENT_ANIM"		/*199*/
,	"*CONTROL_POINT3_KEY"		/*200*/
,	"*CONTROL_TCB_POINT3_KEY"	/*201*/
,	"*CONTROL_TCB_FLOAT_KEY"	/*202*/
,	"*CONTROL_TCB_POS_KEY"		/*203*/
,	"*CONTROL_TCB_SCALE_KEY"	/*204*/
,	"*CONTROL_BEZIER_FLOAT_KEY"	/*205*/
,	"*CONTROL_BEZIER_POINT3_KEY"/*206*/
,	"*CONTROL_BEZIER_SCALE_KEY"	/*207*/
,	"*CONTROL_POS_LINEAR"		/*208*/
,	"*CONTROL_POS_TCB"			/*209*/
,	"*CONTROL_ROT_LINEAR"		/*210*/
,	"*CONTROL_ROT_BEZIER"		/*211*/
,	"*CONTROL_SCALE_LINEAR"		/*212*/
,	"*CONTROL_SCALE_TCB"		/*213*/
,	"*CONTROL_SCALE_BEZIER"		/*214*/

,	"*CONTROL_POS_KEY"			/*215*/
,	"*CONTROL_ROT_KEY" 			/*216*/
,	"*CONTROL_SCALE_KEY"		/*217*/

,	"*CONTROL_POINT3_TCB"		/*218*/
,	"*CONTROL_POINT3_BEZIER"	/*219*/
,	"*CONTROL_COLOR_BEZIER"		/*220*/
,	"*CONTROL_POINT3_SAMPLE"	/*221*/

,	"*CONTROL_FLOAT_TCB"		/*222*/
,	"*CONTROL_FLOAT_BEZIER"		/*223*/
,	"*CONTROL_FLOAT_LINEAR"		/*224*/

,	"*CAMERA_ANIMATION"			/*225*/
,	"*LIGHT_ANIMATION"			/*226*/

,	"*MATERIAL_WIREUNITS"		/*227*/

,	"*SCENE_MESHFRAMESTEP"		/*228*/
,	"*SCENE_KEYFRAMESTEP"		/*229*/

,	"*PROP_HIDDEN"				/*230*/
,	"*PROP_NORENDERABLE"		/*231*/
,	"*PROP_BONE"				/*232*/
,	"*ANIMATION_NAME"			/*233*/
,};


ASE::CASELexer::CASELexer(LPSTR p_FileName, DWORD p_SizeOfBuffer) : CLexer(p_FileName, p_SizeOfBuffer)
{
}

ASE::CASELexer::~CASELexer()
{
}

//Lexical Analysis
LONG ASE::CASELexer::GetToken(LPSTR p_TokenString)
{
	// 1) 앞쪽 쓸데없는 문자찾기
	//    앞쪽에 공백이나 Tab이나 개행이나 캐리지리턴 같은 문자열이 있으면 무시한다.
	CHAR	Char	 = GetNextChar();
	while(Char == ' ' || Char == '\t' || Char == '\r' || Char == '\n')
	{
		Char	 = GetNextChar();
		if(!Char)
		{
			return	TOKEND_END;
		}
	}

	// 2) '{'인가를 확인한다.
	if(Char == '{')
	{
		p_TokenString[0]	 = '{';
		p_TokenString[1]	 = NULL;

		return	TOKEND_BLOCK_START;
	}

	// 3) '}'인가를 확인한다.
	else if(Char == '}')
	{
		p_TokenString[0]	 = '}';
		p_TokenString[1]	 = NULL;

		return	TOKEND_BLOCK_END;
	}
	// 4) '*'인가를 확인한다.
	//    만약 '*'로 시작하는 것은 Reserved word이므로 검색한다!
	else if(Char == '*')
	{
		LONG	p_StringPoint	 = 0;
		while(Char != ' ' && Char != '\t' && Char != '\r' && Char != '\n' && Char != '"' && Char != '{' && Char != '}')
		{
			p_TokenString[p_StringPoint]	 = Char;

			Char	 = GetNextChar();
			if(!Char)
			{
				return	TOKEND_END;
			}
			p_StringPoint++;
		}
		p_TokenString[p_StringPoint]		 = NULL;

		for(int i=0;i<NUMBER_OF_RESERVEDWORD;i++)
		{
			if(!strcmp(p_TokenString, Token[i]))
			{
				// 맞는 Token을 찾았다! 그 번호를 return시켜준다.
				return	i;
			}
		}

		// 맞는 것은 전혀 찾지 못했다. 정의되지 않은 TOKEN이라고 돌려준다.
		return	TOKEND_NOTDEFINED;
	}
	// 5) 숫자인지를 확인한다.
	//    만약에 '0'에서 '9'사이이거나 '-'부하고 붙어 있으면 숫자라고 판단을 한다.
	else if(Char >= '0' && Char <= '9' || Char == '-')
	{
		LONG	p_StringPoint	 = 0;
		while((Char >= '0' && Char <= '9') || Char == '-' || Char == '.')
		{
			p_TokenString[p_StringPoint]	 = Char;

			Char	 = GetNextChar();
			if(!Char)
			{
				return	TOKEND_END;
			}
			p_StringPoint++;
		}
		p_TokenString[p_StringPoint]	 = NULL;


		return	TOKEND_NUMBER;
	}
	// 6) String인지를 검사한다.
	// '"' 따음표로 시작하면 이것은 String인다!
	else if(Char == '"')
	{
		Char	 = GetNextChar();
		if(!Char)
		{
			return	TOKEND_END;
		}

		LONG	p_StringPoint	 = 0;
		while(Char != '"')
		{
			p_TokenString[p_StringPoint]	 = Char;

			Char	 = GetNextChar();
			if(!Char)
			{
				return	TOKEND_END;
			}
			p_StringPoint++;
		}
		p_TokenString[p_StringPoint]	 = NULL;

		return	TOKEND_STRING;
	}
	// 7) 지금까지 못찾았음 이건 허당이다~ 다음을 기약하자~
	else
	{
		// 첫글짜는 char이 보관하고 있으니 이것을 일단 p_TokenStirng에 넣고 다음 글자를 읽어 들인다.
		p_TokenString[0]	 = Char;
		Char	 = GetNextChar();
		if(!Char)
		{
			return	TOKEND_END;
		}

		// 한 단어를 p_TokeString에다가 복사를 한다.
		LONG	p_StringPoint	 = 1;
		while(Char != ' ' && Char != '\t' && Char != '\r' && Char != '\n')
		{
			p_TokenString[p_StringPoint]	 = Char;

			Char	 = GetNextChar();
			if(!Char)
			{
				return	TOKEND_END;
			}
			p_StringPoint++;
		}
		p_TokenString[p_StringPoint]	 = NULL;

		// 일반적인 Identifier라고 return한다.(Idendifier는 변수이름이라든지 뭐 그런거..)
		return	TOKEND_IDENTIFIER;
	}

	//p_TokenString[0]	 = NULL;

	//TRACE("Have no match token : %s\n", p_TokenString);
	//return TOKEND_NOTDEFINED;
}
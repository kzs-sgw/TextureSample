#include "ObjModifier.h"


ObjModifier::ObjModifier(void)
{
}


ObjModifier::~ObjModifier(void)
{
}


bool ObjModifier::ObjLoadRawData( const char* filename )
{
	std::ifstream file;
	//int cmi = 0;
	char tmp_char[OBJ_NAME_LENGTH];
	char buf[10000];
	char *pbuf;


	strcpy(objFileName, filename);

	file.open( filename, std::ios::in );
	if ( !file.is_open() )
	{
		std::cout << "ERROR!!   : COULD NOT OPEN FILE" << std::endl;
		std::cout << "FILE NAME : "<< filename << std::endl;
		return false;
	}


	// loop until end of file
	while ( !file.eof() )
	{
		vec3 read_vertex;
		vec2 read_texcoord;
		vec3 read_normal;
		ObjFace read_face;

		file.getline( buf, sizeof(buf) );

		switch ( buf[0] )
		{
		case 'v':
			switch ( buf[1] )
			{
			// vertex
			case ' ':
				if ( sscanf( buf+2, "%f %f %f",
					&read_vertex.x, &read_vertex.y, &read_vertex.z ) != 3 )
				{
					std::cout<<"ERROR!! : 頂点座標の数が不正です\n";
					return false;
				}
				raw_vertex.push_back( read_vertex );
				break;

			// texcoord
			case 't':
				if ( sscanf( buf+2, "%f %f", &read_texcoord.x, &read_texcoord.y ) != 2 )
				{
					std::cout<<"ERROR!! : テクスチャ座標の数が不正です\n";
					return false;
				}
				raw_texcoord.push_back( read_texcoord );
				break;

			// normal
			case 'n':
				if ( sscanf( buf+2, "%f %f %f",
					&read_normal.x, &read_normal.y, &read_normal.z ) != 3 )
				{
					std::cout<<"ERROR!! : 頂点法線の数が不正です\n";
					return false;
				}
				raw_normal.push_back( read_normal );
				break;
			}
			break;

		// face---------------------------------------------------------------
		case 'f':
			read_face.element = 0;
			pbuf = buf;
			while ( *pbuf )
			{
				if( *pbuf == ' ' ) read_face.element++;
				pbuf++;
			}

			if ( read_face.element < 3 )
			{
				std::cout<<"ERROR!! : 面を構成するための要素数が不正です\n";
				return false;
			}
			else if ( read_face.element > 3 )
			{
				std::cout<<"ERROR!! : 三角形ポリゴン以外は対応していません。データを変更して下さい\n";
				return false;
			}


			read_face.vertex_id   = new int [ read_face.element ];
			read_face.texcoord_id = new int [ read_face.element ];
			read_face.normal_id   = new int [ read_face.element	];

			pbuf = buf;

			for (int i = 0; i < read_face.element; i++)
			{
				pbuf = strchr(pbuf, ' ');
				pbuf++;

				if ( sscanf(pbuf,"%d/%d/%d", &read_face.vertex_id[i],
					&read_face.texcoord_id[i], &read_face.normal_id[i] ) !=3 )
				{
					if ( sscanf(pbuf, "%d//%d",
						&read_face.vertex_id[i], &read_face.normal_id[i] ) !=2 )
					{
						if ( sscanf(pbuf, "%d/&d",
							&read_face.vertex_id[i], &read_face.texcoord_id[i] ) !=2 )
						{
							sscanf(pbuf, "%d", &read_face.vertex_id[i]);
							read_face.use_texture = false;
							read_face.use_normal  = false;
						}
						else
						{
							read_face.use_texture = true;
							read_face.use_normal  = false;
						} 
					}
					else
					{
						read_face.use_texture = false;
						read_face.use_normal  = true;
					}
				}
				else
				{
					read_face.use_texture = true; 
					read_face.use_normal  = true;
				}

				// 配列の番号と合わせる
				read_face.vertex_id[i]--;
				if ( read_face.use_texture ) read_face.texcoord_id[i]--;
				if ( read_face.use_normal  ) read_face.normal_id[i]--;
			}

			face.push_back( read_face );
			break;

		case 'u':
			std::cout<<"CAUTION! : マテリアル未対応です\n";
			break;
		case 'm':
			std::cout<<"CAUTION! : マテリアル未対応です\n";
			break;

		default:
			break;
		}
		// ---------------------------------------------------------------face
	}

	file.close();
	std::cout<<"\n";
	std::cout<<"ポリゴン数: "<< face.size() <<std::endl;
}


bool ObjModifier::ObjLoadAndMake( const char* filename )
{
	ObjLoadRawData( filename );


	//**隣接ID処理*****************************************************************
	// make vertex_info

	// INPUT neighbor_face_id---------------------------------------------
	for (int faceId = 0; faceId < face.size(); faceId++)
	{
		int elem = face[faceId].element;
		for (int i = 0; i < elem; i++)
		{
			int key = face[faceId].vertex_id[i];
			vertex_info[ key ].neighbor_face_id.push_back( faceId );

			// INPUT neighbor_vertex_id-----------------------------------
			int v_id = face[faceId].vertex_id[ (i+1)%elem ];
			
			bool isDuplicated = false;
			for (int nv_id = 0; nv_id < vertex_info[key].neighbor_vertex_id.size(); nv_id++)
			{
				if( nv_id == v_id ) { isDuplicated = true; break; }
			}

			if ( !isDuplicated )
			{
				vertex_info[ key ].neighbor_vertex_id.push_back( v_id );
			}
		}
	}

	// GPUへ転送可能な配列データに格納-------------------------------------------
	for (int i = 0; i < face.size(); i++)
	{
		// vertex
		for (int j = 0; j < face[i].element; j++)
		{
			vertex.push_back( raw_vertex[ face[i].vertex_id[j] ] );
		}

		// texcoord
		if ( face[i].use_texture )
		{
			for (int j = 0; j < face[i].element; j++)
			{
				texcoord.push_back( raw_texcoord[ face[i].texcoord_id[j] ] );
			}
		}
		
		// normal
		vec3 n(0.0f);
		if ( face[i].use_normal )
		{
			for (int j = 0; j < face[i].element; j++)
			{
				n += raw_normal[ face[i].normal_id[j] ];
			}
		}

		// flat shading
		if ( use_faceNorm )
		{
			n = n * (1.0f / (float)face[i].element);
			for (int j = 0; j < face[i].element; j++)
			{
				normal.push_back( n );
			}
		}
		else
		{
			for (int j = 0; j < face[i].element; j++)
			{
				normal.push_back( raw_normal[ face[i].normal_id[j] ] );
			}
		}
		
	}

}

bool ObjModifier::ObjLoadAndWrite( const char* filename )
{

	ObjLoadRawData( filename );

	//**隣接ID処理*****************************************************************
	// make vertex_info

	// INPUT neighbor_face_id---------------------------------------------
	for (int faceId = 0; faceId < face.size(); faceId++)
	{
		int elem = face[faceId].element;
		for (int i = 0; i < elem; i++)
		{
			int key = face[faceId].vertex_id[i];
			vertex_info[ key ].neighbor_face_id.push_back( faceId );

			// INPUT neighbor_vertex_id-----------------------------------
			int v_id = face[faceId].vertex_id[ (i+1)%elem ];
			
			bool isDuplicated = false;
			for (int nv_id = 0; nv_id < vertex_info[key].neighbor_vertex_id.size(); nv_id++)
			{
				if( nv_id == v_id ) { isDuplicated = true; break; }
			}

			if ( !isDuplicated )
			{
				vertex_info[ key ].neighbor_vertex_id.push_back( v_id );
			}
		}
	}


	//*****************************************************************************

	/// ファイルへ書き出し**********************************************************
	// write to file--------------------------------------------------------------
	// 
	// vertex_infoをobjファイルに追加してそのまま全情報を読み込めるようにする。
	// 拡張子は[.objx]とする。
	// 
	//----------------------------------------------------------------------------

	// --------------------------------------------------------------------------

	return true;
}


bool ObjModifier::ObjLoadModifiedData( const char* filename )
{
	std::ifstream file;
	//int cmi = 0;
	char tmp_char[OBJ_NAME_LENGTH];
	char buf[10000];
	char *pbuf;


	strcpy(objFileName, filename);

	file.open( filename, std::ios::in );
	if ( !file.is_open() )
	{
		std::cout << "ERROR!!   : COULD NOT OPEN FILE" << std::endl;
		std::cout << "FILE NAME : "<< filename << std::endl;
		return false;
	}


	// loop until end of file
	while ( !file.eof() )
	{
		vec3 read_vertex;
		vec2 read_texcoord;
		vec3 read_normal;
		ObjFace read_face;

		file.getline( buf, sizeof(buf) );

		switch ( buf[0] )
		{
		case 'v':
			switch ( buf[1] )
			{
			// vertex
			case ' ':
				if ( sscanf( buf+2, "%f %f %f",
					&read_vertex.x, &read_vertex.y, &read_vertex.z ) != 3 )
				{
					std::cout<<"ERROR!! : 頂点座標の数が不正です\n";
					return false;
				}
				raw_vertex.push_back( read_vertex );
				break;

			// texcoord
			case 't':
				if ( sscanf( buf+2, "%f %f", &read_texcoord.x, &read_texcoord.y ) != 2 )
				{
					std::cout<<"ERROR!! : テクスチャ座標の数が不正です\n";
					return false;
				}
				raw_texcoord.push_back( read_texcoord );
				break;

			// normal
			case 'n':
				if ( sscanf( buf+2, "%f %f %f",
					&read_normal.x, &read_normal.y, &read_normal.z ) != 3 )
				{
					std::cout<<"ERROR!! : 頂点法線の数が不正です\n";
					return false;
				}
				raw_normal.push_back( read_normal );
				break;
			}
			break;

		// face---------------------------------------------------------------
		case 'f':
			read_face.element = 0;
			pbuf = buf;
			while ( *pbuf )
			{
				if( *pbuf == ' ' ) read_face.element++;
				pbuf++;
			}

			if ( read_face.element < 3 )
			{
				std::cout<<"ERROR!! : 面を構成するための要素数が不正です\n";
				return false;
			}
			else if ( read_face.element > 3 )
			{
				std::cout<<"ERROR!! : 三角形ポリゴン以外は対応していません。データを変更して下さい\n";
				return false;
			}


			read_face.vertex_id   = new int [ read_face.element ];
			read_face.texcoord_id = new int [ read_face.element ];
			read_face.normal_id   = new int [ read_face.element	];

			pbuf = buf;

			for (int i = 0; i < read_face.element; i++)
			{
				pbuf = strchr(pbuf, ' ');
				pbuf++;

				if ( sscanf(pbuf,"%d/%d/%d", &read_face.vertex_id[i],
					&read_face.texcoord_id[i], &read_face.normal_id[i] ) !=3 )
				{
					if ( sscanf(pbuf, "%d//%d",
						&read_face.vertex_id[i], &read_face.normal_id[i] ) !=2 )
					{
						if ( sscanf(pbuf, "%d/&d",
							&read_face.vertex_id[i], &read_face.texcoord_id[i] ) !=2 )
						{
							sscanf(pbuf, "%d", &read_face.vertex_id[i]);
							read_face.use_texture = false;
							read_face.use_normal  = false;
						}
						else
						{
							read_face.use_texture = true;
							read_face.use_normal  = false;
						} 
					}
					else
					{
						read_face.use_texture = false;
						read_face.use_normal  = true;
					}
				}
				else
				{
					read_face.use_texture = true; 
					read_face.use_normal  = true;
				}

				// 配列の番号と合わせる
				read_face.vertex_id[i]--;
				if ( read_face.use_texture ) read_face.texcoord_id[i]--;
				if ( read_face.use_normal  ) read_face.normal_id[i]--;
			}

			face.push_back( read_face );
			break;

		case 'u':
			std::cout<<"CAUTION! : マテリアル未対応です\n";
			break;
		case 'm':
			std::cout<<"CAUTION! : マテリアル未対応です\n";
			break;


		// objx format // nv, nf→隣接頂点、隣接フェイス
		case 'n':
			switch ( buf[1] )
			{
			case 'v':
				// 隣接頂点の個数を調べる
				int nv_element = 0;
				pbuf = buf;
				while ( *pbuf )
				{
					if( *pbuf == ' ' ) nv_element++;
					pbuf++;
				}

				if ( nv_element < 2 )
				{
					std::cout<<"CAUTION!: 隣接頂点が2つ未満です\n";
					return false;
				}

				pbuf = buf;

				// 個数分raw_vertexinfoにぶち込む
				for (int i = 0; i < nv_element; i++)
				{
					int key_raw_vi; // key of "raw_vertexinfo"
					int read_nv;

					pbuf = strchr(pbuf, ' ');
					pbuf++;

					if( sscanf( pbuf, "%d/%d", &key_raw_vi, &read_nv ) != 2 )
					{
						std::cout<<"ERROR!!: objx形式が破綻しています。\n";
					}
					raw_vertexinfo[ key_raw_vi ].neighbor_vertex_id.push_back( read_nv );
				}
				break;

			case 'f':
				// 隣接faceの個数を調べる
				int nf_element = 0;
				pbuf = buf;
				while ( *pbuf )
				{
					if( *pbuf == ' ' ) nf_element++;
					pbuf++;
				}

				if ( nf_element < 1 )
				{
					std::cout<<"CAUTION!: 隣接面がありません\n";
					return false;
				}

				pbuf = buf;

				// 個数分raw_vertexinfoにぶち込む
				for (int i = 0; i < nf_element; i++)
				{
					int key_raw_vi; // key of "raw_vertexinfo"
					int read_nf;

					pbuf = strchr(pbuf, ' ');
					pbuf++;

					if( sscanf( pbuf, "%d/%d", &key_raw_vi, &read_nf ) != 2 )
					{
						std::cout<<"ERROR!!: objx形式が破綻しています。\n";
					}
					raw_vertexinfo[ key_raw_vi ].neighbor_face_id.push_back( read_nf );
				}
				break;

			}

		default:
			break;
		}
		// ---------------------------------------------------------------face
	}

	file.close();
	std::cout<<"\n";
	std::cout<<"ポリゴン数: "<< face.size() <<std::endl;
}


#include "OGJsonParser.h"

#include <unordered_map>
#include <sstream>
#include "OGJsonTiledMap.h"
 
#include "OGDirector.h"
#include <fstream>

//添加JSON解析库
#include "json.hpp"
using namespace nlohmann;

#if defined(_MSC_VER)
#pragma warning(disable:4996)
#endif


OG_BEGIN



// implementation JsonMapInfo

JsonMapInfo * JsonMapInfo::create(const std::string& jsonFile)
{
    JsonMapInfo *ret = new (std::nothrow) JsonMapInfo();
    if (ret->initWithJsonFile(jsonFile))
    {
        ret->autorelease();
        return ret;
    }
    OG_SAFE_DELETE(ret);
    return nullptr;
}

bool JsonMapInfo::initWithJsonFile(const std::string& jsonFile)
{
	internalInit(jsonFile, "");
	return parseJSONFile(jsonFile);
}



void JsonMapInfo::internalInit(const std::string& jsonFileName, const std::string& resourcePath)
{
    if (!jsonFileName.empty())
    {
        //_JsonFileName = FileUtils::getInstance()->fullPathForFilename(jsonFileName);
		_JsonFileName = jsonFileName;
    }
    
    if (!resourcePath.empty())
    {
        _resources = resourcePath;
    }

}

bool JsonMapInfo::parseJSONFile(const std::string& jsonFilename)
{
	/*  SAXParser parser;

	  if (false == parser.init("UTF-8") )
	  {
		  return false;
	  }

	  parser.setDelegator(this);

	  return parser.parse(FileUtils::getInstance()->fullPathForFilename(jsonFilename));*/
	
// 	FILE *fp = fopen(jsonFilename.c_str(), "rb");
// 	if (!fp)
// 		return false;
// 	fseek(fp, 0, SEEK_END);
// 	int len = ftell(fp) + 1;
// 	char *data = (char*)malloc(len);
// 	memset(data, 0, len);
	// 	fseek(fp, 0, SEEK_CUR);
// 	fread(data, sizeof(char), len, fp);
// 	fclose(fp);

	std::fstream in(jsonFilename.c_str(), std::ios::in | std::ios::binary);
	if (!in)
	   return false;
	in.seekg(0, std::ios::end);
	int len = (int)in.tellg();
	in.seekg(0, std::ios::beg);
	char* data = new char[len + 1];
	in.read( data, len);
	in.close();

	std::string jsonData(data,len);

	delete[] data;

	

	return parse(jsonData);
}


JsonMapInfo::JsonMapInfo() :
	_mapSize(Size::ZERO)
	, _tileSize(Size::ZERO)
	, _orientation(JSONOrientationOrtho)

{
}

JsonMapInfo::~JsonMapInfo()
{
	// CCLOGINFO("deallocing JsonMapInfo: %p", this);
}


bool JsonMapInfo::parse(const std::string &jsonData)
{

	json data=json::parse(jsonData);



	//地图图块
	float width = data["width"];
	float height = data["height"];

	_mapSize = Vec2(width, height);



	//贴图大小
	float tilewidth = data["tilewidth"];
	float tileheight = data["tileheight"];
	_tileSize = Vec2(tilewidth, tileheight);

	//_position = Vec2::ZERO;
	//计算全部地图大小
	//_contentSize = Vec2(width * tilewidth, height * tileheight);
	//创建裁剪部分
	//Vector<JsonTilesetInfo*> tilesets;

	for (auto& json_tileset : data["tilesets"])
	{
		JsonTilesetInfo * tileset = new(std::nothrow)JsonTilesetInfo();
		std::string name = json_tileset["name"];
		tileset->_name = name;

		std::string image = json_tileset["image"];
		tileset->_originSourceImage = image;
		 
		//需要源位置处理
		if (_JsonFileName.find_last_of('/') != std::string::npos)
		{
			std::string dir = _JsonFileName.substr(0, _JsonFileName.find_last_of('/') + 1);
			tileset->_sourceImage = dir + image;
		}
		else {
			tileset->_sourceImage = _resources + (_resources.size() ? "/" : "") + image;
		}

	//	tileset->_sourceImage = image;
 

		 Size s;
		s.width = json_tileset["tilewidth"];
		s.height = json_tileset["tileheight"];
		tileset->_tileSize = s;
		s.width = json_tileset["imagewidth"];
		s.height = json_tileset["imageheight"];
		tileset->_imageSize = s;
		int firstGid = json_tileset["firstgid"];
		tileset->_firstGid = firstGid;

		int tilecount = json_tileset["tilecount"];
// 		int lastGid = firstGid + tilecount - 1;
// 		tileset->_lastGid = lastGid;
		int columns = json_tileset["columns"];
		tileset->_columns = columns;



		//PUSH进去
		_tilesets.pushBack(tileset);
	}

	for (auto& json_layer : data["layers"])
	{
		JsonLayerInfo * layer = new(std::nothrow)JsonLayerInfo();
		std::string name = json_layer["name"];
		layer->_name = name;

		Size s;
		s.width = json_layer["width"];
		s.height = json_layer["height"];
		layer->_layerSize = s;
		bool visible = json_layer["visible"];
		layer->_visible = visible;
		float x = json_layer["x"];
		float y = json_layer["y"];
		layer->_offset= Vec2(x, y);

		int tileSize = json_layer["data"].size();

		uint32_t *_tiles = (uint32_t*)malloc(tileSize *sizeof(uint32_t));
		memset(_tiles, 0, tileSize *sizeof(uint32_t));
		

		for (int i = 0; i < tileSize; ++i)
		{
			_tiles[i] = json_layer["data"][i];
		}
		layer->_tiles = _tiles;
		
		unsigned opacity = json_layer["opacity"];
		layer->_opacity = opacity  * 255.0f;

		//PUSH进去
		_layers.pushBack(layer);
	}

	data.clear();


	return true;
}




OG_END

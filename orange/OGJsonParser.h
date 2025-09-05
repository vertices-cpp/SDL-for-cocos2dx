

#ifndef __OG_JSON_PARSER__
#define __OG_JSON_PARSER__

/// @cond DO_NOT_SHOW

#include "OGGeometry.h"
 
#include "OGVector.h"
//#include "OGValue.h"
 


#include <string>

OG_BEGIN

typedef enum JSONTileFlags_ {
	kJSONTileHorizontalFlag = 0x80000000,
	kJSONTileVerticalFlag = 0x40000000,
	kJSONTileDiagonalFlag = 0x20000000,
	kJSONFlipedAll = (kJSONTileHorizontalFlag | kJSONTileVerticalFlag | kJSONTileDiagonalFlag),
	kJSONFlippedMask = ~(kJSONFlipedAll)
} JSONTileFlags;
 
class JsonLayerInfo : public Ref
{
public:
    
    JsonLayerInfo()
		: _name("")
		, _tiles(nullptr)
		, _ownTiles(true)

	{
	}

	virtual ~JsonLayerInfo()
	{

		if (_tiles)
		{
			_layerSize = 0;
			free(_tiles);
		}
		//CCLOGINFO("deallocing TMXLayerInfo: %p", this);
	 	if (_ownTiles && _tiles)
	 	{
	 		free(_tiles);
	 		_tiles = nullptr;
	 	}
	}
   

//     void setProperties(ValueMap properties);
//     ValueMap& getProperties();

 //   ValueMap            _properties;
    std::string         _name;
    Size                _layerSize;
    uint32_t            *_tiles;
	uint32_t			_tilesSize;
    bool                _visible;
    unsigned char       _opacity;
	bool                _ownTiles;
    Vec2               _offset;
};
 

class  JsonTilesetInfo : public Ref
{
public:
	//名字
	std::string     _name;
	//图片源
	std::string     _sourceImage;
	//第一个gid
	int             _firstGid;
	
	int   _lastGid;
	//行数
	int _columns;

	//每个tile大小
	Size            _tileSize;

	Size            _imageSize;
	std::string     _originSourceImage;

public:

	JsonTilesetInfo() :_firstGid(0)
		, _tileSize(Size::ZERO)
		, _imageSize(Size::ZERO) {}

	virtual ~JsonTilesetInfo() {

	}
	Rect  getRectForGID(uint32_t gid)
	{
		Rect rect;
		rect.size = _tileSize;
		//gid &= kTMXFlippedMask;
		gid = gid - _firstGid;
		// max_x means the column count in tile map
		// in the origin:
		// max_x = (int)((_imageSize.width - _margin*2 + _spacing) / (_tileSize.width + _spacing));
		// but in editor "Tiled", _margin variable only effect the left side
		// for compatible with "Tiled", change the max_x calculation
		int max_x = (int)(_imageSize.width  / _tileSize.width  );

		rect.origin.x = (gid % max_x) * _tileSize.width ;
		rect.origin.y = (gid / max_x) * _tileSize.height ;
		return rect;
	}
};


 
class JsonMapInfo : public Ref
{    
public:    
 
    static JsonMapInfo * create(const std::string& jsonFile);
	bool initWithJsonFile(const std::string& jsonFile);
	void internalInit(const std::string& jsonFileName, const std::string& resourcePath);
	bool parseJSONFile(const std::string& jsonFilename);


	bool parse(const std::string & jsonData);

    JsonMapInfo();
   
    virtual ~JsonMapInfo();
  
   
	int getOrientation() const { return _orientation; }
	void setOrientation(int orientation) { _orientation = orientation; }

    /// map width & height
    const Size& getMapSize() const { return _mapSize; }
    void setMapSize(const Size& mapSize) { _mapSize = mapSize; }

    /// tiles width & height
    const Size& getTileSize() const { return _tileSize; }
    void setTileSize(const Size& tileSize) { _tileSize = tileSize; }
    
    /// Layers
    const Vector<JsonLayerInfo*>& getLayers() const { return _layers; }
    Vector<JsonLayerInfo*>& getLayers() { return _layers; }
    void setLayers(const Vector<JsonLayerInfo*>& layers) {
        _layers = layers;
    }

    /// tilesets
    const Vector<JsonTilesetInfo*>& getTilesets() const { return _tilesets; }
    Vector<JsonTilesetInfo*>& getTilesets() { return _tilesets; }
    void setTilesets(const Vector<JsonTilesetInfo*>& tilesets) {
        _tilesets = tilesets;
    }
 
    const std::string& getJsonFileName() const { return _JsonFileName; }
    void setJsonFileName(const std::string& fileName){ _JsonFileName = fileName; }

protected:
    
	int _orientation;
  
    /// map width & height
    Size _mapSize;
    /// tiles width & height
    Size _tileSize;
    /// Layers
    Vector<JsonLayerInfo*> _layers;
    /// tilesets
    Vector<JsonTilesetInfo*> _tilesets;
    /// ObjectGroups
  
    //! json format tile index
    int _jsonTileIndex;
    
    //! json filename
    std::string _JsonFileName;
    // json resource path
    std::string _resources;

};
 

OG_END
 
#endif

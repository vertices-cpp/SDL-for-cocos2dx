  

#ifndef __OG_JSON_TILE_MAP_H__
#define __OG_JSON_TILE_MAP_H__

#include "OGPlatformMacros.h"
#include "OGNode.h"
#include "OGValue.h"
#include "OGJsonParser.h"
#include "OGJsonLayer.h"

OG_BEGIN

class JsonLayer; 
class JsonTilesetInfo; 

enum
{
	/** Orthogonal orientation. */
	JSONOrientationOrtho,

	/** Hexagonal orientation. */
	JSONOrientationHex,

	/** Isometric orientation. */
	JSONOrientationIso,

	/** Isometric staggered orientation. */
	JSONOrientationStaggered,
};


class   JsonTiledMap : public Node
{
public:
    
    static JsonTiledMap* create(const std::string& json);
 
    static JsonTiledMap* createWithJson(const std::string& jsonString, const std::string& resourcePath);
 
    JsonLayer* getLayer(const std::string& layerName) const;
    
    const Size& getMapSize() const { return _mapSize; }
    
     void setMapSize(const Size& mapSize) { _mapSize = mapSize; }
 
    const Size& getTileSize() const { return _tileSize; }
    
    void setTileSize(const Size& tileSize) { _tileSize = tileSize; }
    
    int  getLayerNum();
    const std::string& getResourceFile() const { return _jsonFile; }

public:
    
	JsonTiledMap();
    virtual ~JsonTiledMap();
    
    /** initializes a Json Tiled Map with a Json file */
    bool initWithJsonFile(const std::string& jsonFile);

	Vec2 getMapScale()const { return _contentScaleFactor; }
	void setMapScale(const Vec2 & scale)
	{
		_contentScaleFactor = scale;
		
		for (auto it = _children.begin(), itCend = _children.end(); it != itCend; ++it)
		{
		 
			((JsonLayer*)(*it))->setLayerScale(scale);
	 
		}
	}

protected:
    JsonLayer * parseLayer(JsonLayerInfo *layerInfo, JsonMapInfo *mapInfo);
    JsonTilesetInfo * tilesetForLayer(JsonLayerInfo *layerInfo, JsonMapInfo *mapInfo);

    void buildWithMapInfo(JsonMapInfo* mapInfo);

	Vec2 _contentScaleFactor;

    Size _mapSize;
   
    Size _tileSize;

    std::string _jsonFile;
    int _jsonLayerNum;

    static const int JsonLayerTag = 32768;

private:
    OG_DISALLOW_COPY_AND_ASSIGN(JsonTiledMap);

};

// end of tilemap_parallax_nodes group
/// @}

OG_END

#endif //__OG_JSON_TILE_MAP_H__



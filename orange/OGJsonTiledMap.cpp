
#include "OGJsonTiledMap.h"
#include "OGJsonLayer.h"
#include "OGSprite.h"


OG_BEGIN




// implementation JsonTiledMap


JsonTiledMap * JsonTiledMap::create(const std::string& tmxFile)
{
    JsonTiledMap *ret = new (std::nothrow) JsonTiledMap();
    if (ret->initWithJsonFile(tmxFile))
    {
        ret->autorelease();
        return ret;
    }
    OG_SAFE_DELETE(ret);
    return nullptr;
}


bool JsonTiledMap::initWithJsonFile(const std::string& jsonFile)
{
  //  CCASSERT(tmxFile.size()>0, "JsonTiledMap: tmx file should not be empty");

    _jsonFile = jsonFile;

    setContentSize(Size::ZERO);
	
   // auto buffer = FileUtils::getInstance()->fullPathForFilename(jsonFile);
	JsonMapInfo *mapInfo = JsonMapInfo::create(jsonFile);

    if (!mapInfo)
    {
        return false;
    }

	
   // CCASSERT( !mapInfo->getTilesets().empty(), "JsonTiledMap: Map not found. Please check the filename.");
    buildWithMapInfo(mapInfo);

    return true;
}

JsonTiledMap::JsonTiledMap()
    :_mapSize(Size::ZERO)
    ,_tileSize(Size::ZERO)        
    ,_jsonFile("")
    , _jsonLayerNum(0)
	, _contentScaleFactor(Vec2(1.0f,1.0f))
{
}

JsonTiledMap::~JsonTiledMap()
{
	
}

// private
JsonLayer * JsonTiledMap::parseLayer(JsonLayerInfo *layerInfo, JsonMapInfo *mapInfo)
{
    JsonTilesetInfo *tileset = tilesetForLayer(layerInfo, mapInfo);
    if (tileset == nullptr)
        return nullptr;
    
    JsonLayer *layer = JsonLayer::create(tileset, layerInfo, mapInfo);

    if (nullptr != layer)
    {
        // tell the layerinfo to release the ownership of the tiles map.
        layerInfo->_ownTiles = false;
        layer->setupTiles();
    }

    return layer;
}

JsonTilesetInfo * JsonTiledMap::tilesetForLayer(JsonLayerInfo *layerInfo, JsonMapInfo *mapInfo)
{
    auto height = static_cast<uint32_t>(layerInfo->_layerSize.height);
    auto width  = static_cast<uint32_t>(layerInfo->_layerSize.width);
    auto& tilesets = mapInfo->getTilesets();

    for (auto iter = tilesets.crbegin(), end = tilesets.crend(); iter != end; ++iter)
    {
        JsonTilesetInfo* tileset = *iter;

        if (tileset)
        {
            for (uint32_t y = 0; y < height; y++)
            {
                for (uint32_t x = 0; x < width; x++)
                {
                    auto pos = x + width * y;
                    auto gid = layerInfo->_tiles[ pos ];

                    // FIXME:: gid == 0 --> empty tile
                    if (gid != 0)
                    {
                        // Optimization: quick return
                        // if the layer is invalid (more than 1 tileset per layer)
                        // an CCAssert will be thrown later
                        if (tileset->_firstGid < 0 ||
                            (gid & kJSONFlippedMask) >= static_cast<uint32_t>(tileset->_firstGid))
                            return tileset;
                    }
                }
            }        
        }
    }

    // If all the tiles are 0, return empty tileset
  //  CCLOG("cocos2d: Warning: Json Layer '%s' has no tiles", layerInfo->_name.c_str());

    return nullptr;
}

void JsonTiledMap::buildWithMapInfo(JsonMapInfo * mapInfo)
{
	
	_mapSize = mapInfo->getMapSize();
	_tileSize = mapInfo->getTileSize();
// 	_mapOrientation = mapInfo->getOrientation();
// 
// 	_objectGroups = mapInfo->getObjectGroups();
// 
// 	_properties = mapInfo->getProperties();
// 
// 	_tileProperties = mapInfo->getTileProperties();

	int idx = 0;
	//parseLayer
	auto& layers = mapInfo->getLayers();
	for (const auto &layerInfo : layers) {
		/*if (layerInfo->_visible)*/ {
			JsonLayer *child = parseLayer(layerInfo, mapInfo);
			if (child == nullptr) {
				idx++;
				continue;
			}
			
			//addChild(child, idx, idx);
			addChild(child, idx);
			// update content size with the max size
			const Size& childSize = child->getContentSize();
			Size currentSize = this->getContentSize();
			currentSize.width = std::max(currentSize.width, childSize.width);
			currentSize.height = std::max(currentSize.height, childSize.height);
			this->setContentSize(currentSize);

			idx++;
		}
	}
	_jsonLayerNum = idx;
}

// public
JsonLayer * JsonTiledMap::getLayer(const std::string& layerName) const
{
   // CCASSERT(layerName.size() > 0, "Invalid layer name!");
    
    for (auto& child : _children)
    {
        JsonLayer* layer = dynamic_cast<JsonLayer*>(child);
        if(layer)
        {
            if(layerName.compare( layer->getLayerName()) == 0)
            {
                return layer;
            }
        }
    }

    // layer not found
    return nullptr;
}

int JsonTiledMap::getLayerNum()
{
    return _jsonLayerNum;
}

OG_END

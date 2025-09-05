
#include "OGJsonLayer.h"
#include "OGDirector.h"
#include "OGJsonTiledMap.h"
#include "OGPlatformMacros.h"


OG_BEGIN


// JsonLayer - init & alloc & dealloc

JsonLayer * JsonLayer::create(JsonTilesetInfo *tilesetInfo, JsonLayerInfo *layerInfo, JsonMapInfo *mapInfo)
{
    JsonLayer *ret = new (std::nothrow) JsonLayer();
    if (ret->initWithTilesetInfo(tilesetInfo, layerInfo, mapInfo))
    {
        ret->autorelease();
        return ret;
    }
    OG_SAFE_DELETE(ret);
    return nullptr;
}
bool JsonLayer::initWithTilesetInfo(JsonTilesetInfo *tilesetInfo, JsonLayerInfo *layerInfo, JsonMapInfo *mapInfo)
{
	// FIXME:: is 35% a good estimate ?
	Size size = layerInfo->_layerSize;
	float totalNumberOfTiles = size.width * size.height;
	//float capacity = totalNumberOfTiles * 0.35f + 1; // 35 percent is occupied ?

	auto texture = std::make_shared<Texture2D>();
	if (tilesetInfo)
	{
		texture->loadFrom(tilesetInfo->_sourceImage.c_str());
	}

	if (nullptr == texture)
		return false;
	_texture = nullptr;
	_texture = texture;
	//添加渲染模式
	_texture->setBlendMode((SDL_BlendMode)bfunc);
	// layerInfo
	_layerName = layerInfo->_name;
	_layerSize = size;
	_tiles = layerInfo->_tiles;
	_opacity = layerInfo->_opacity;

	_contentScaleFactor = Director::getInstance()->getContentScaleFactor();

	// tilesetInfo
	_tileSet = tilesetInfo;
	OG_SAFE_RETAIN(_tileSet);

	// mapInfo
	_mapTileSize = mapInfo->getTileSize();

	_layerOrientation = mapInfo->getOrientation();
	// offset (after layer orientation is set);
	Vec2 offset =  layerInfo->_offset ;

	this->setPosition(OG_POSITION_SACALE(offset));

//	_atlasIndexArray = ccCArrayNew(totalNumberOfTiles);

	float width = 0;
	float height = 0;
// 	if (_layerOrientation == JsonOrientationHex) {
// 		if (_staggerAxis == JsonStaggerAxis_X) {
// 			height = _mapTileSize.height * (_layerSize.height + 0.5);
// 			width = (_mapTileSize.width + _hexSideLength) * ((int)(_layerSize.width / 2)) + _mapTileSize.width * ((int)_layerSize.width % 2);
// 		}
// 		else {
// 			width = _mapTileSize.width * (_layerSize.width + 0.5);
// 			height = (_mapTileSize.height + _hexSideLength) * ((int)(_layerSize.height / 2)) + _mapTileSize.height * ((int)_layerSize.height % 2);
// 		}
// 	}
// 	else {
		width = _layerSize.width * _mapTileSize.width;
		height = _layerSize.height * _mapTileSize.height;
//	}
		//图层大小
	this->setContentSize(OG_SIZE_SACALE(Size(width, height)));


	return true;
}

JsonLayer::JsonLayer()
:_layerName("")
,_opacity(0)

,_contentScaleFactor(Vec2(1,1))
,_layerSize(Size::ZERO)
,_mapTileSize(Size::ZERO)
//,_tiles(nullptr)
,_tileSet(nullptr)
, bfunc(BlendFunc::DISABLE)
{}

JsonLayer::~JsonLayer()
{
    OG_SAFE_RELEASE(_tileSet);
 //   OG_SAFE_RELEASE(_reusedTile);

//     if (_atlasIndexArray)
//     {
//         ccCArrayFree(_atlasIndexArray);
//         _atlasIndexArray = nullptr;
//     }
	OG_SAFE_FREE(_tiles);
	_mapTiles.clear();
}

void JsonLayer::releaseMap()
{
    if (_tiles)
    {
         free(_tiles);
         _tiles = nullptr;
		_mapTiles.clear();
    }
 
}

// JsonLayer - setup Tiles
void JsonLayer::setupTiles()
{    
    // Optimization: quick hack that sets the image size on the tileset
	_tileSet->_imageSize = _texture->getContentSize();

    // By default all the tiles are aliased
    // pros:
    //  - easier to render
    // cons:
    //  - difficult to scale / rotate / etc.
//     _textureAtlas->getTexture()->setAliasTexParameters();
// 
//     //CFByteOrder o = CFByteOrderGetCurrent();
// 
//     // Parse cocos2d properties
//     this->parseInternalProperties();

    for (int y=0; y < _layerSize.height; y++)
    {
        for (int x=0; x < _layerSize.width; x++)
        {
            int newX = x;
			//交错地图部分先不用
            // fix correct render ordering in Hexagonal maps when stagger axis == x
//             if (_staggerAxis == JsonStaggerAxis_X && _layerOrientation == JsonOrientationHex)
//             {
//                 if (_staggerIndex == JsonStaggerIndex_Odd)
//                 {
//                     if (x >= _layerSize.width/2)
//                         newX = (x - std::ceil(_layerSize.width/2)) * 2 + 1;
//                     else
//                         newX = x * 2;
//                 } else {
//                     // JsonStaggerIndex_Even
//                     if (x >= static_cast<int>(_layerSize.width/2))
//                         newX = (x - static_cast<int>(_layerSize.width/2)) * 2;
//                     else
//                         newX = x * 2 + 1;
//                 }
//             }

            int pos = static_cast<int>(newX + _layerSize.width * y);
            int gid = _tiles[ pos ];

            // gid are stored in little endian.
            // if host is big endian, then swap
            //if( o == CFByteOrderBigEndian )
            //    gid = CFSwapInt32( gid );
            /* We support little endian.*/

            // FIXME:: gid == 0 --> empty tile
            if (gid != 0) 
            {
                this->appendTileForGID(gid, Vec2(newX, y));
            }
        }
    }
}
//添加背景裁剪与目标位置
bool JsonLayer::appendTileForGID(uint32_t gid, const Vec2& pos)
{
	if (gid != 0 && (static_cast<int>(gid) - _tileSet->_firstGid) >= 0)
	{
		Rect clipRect = _tileSet->getRectForGID(gid);
		//源裁剪大小不需要缩放
	//	clipRect = OG_RECT_SCALE(clipRect);
		//SDL2版本不需要减1
		auto position = Vec2(pos.x * _mapTileSize.width,(/*_layerSize.height -*/ pos.y/* - 1*/) * _mapTileSize.height);
	//	cout <<gid<<" "<< sourceRect << endl;
		Rect srcDest = Rect(position, _mapTileSize);
		Rect destRect =OG_RECT_SCALE(srcDest);

		MapTile mpt(gid, pos, clipRect, srcDest, destRect, true);
		
		_mapTiles.push_back(mpt);

		return true;
	}

	return false;
}


std::weak_ptr<Texture2D> JsonLayer::getTexture() const
{
	return _texture; 
}

 Sprite * JsonLayer::getTileAt(const Vec2& pos)
 {
 //     CCASSERT(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >=0 && pos.y >=0, "JsonLayer: invalid position");
 //     CCASSERT(_tiles && _atlasIndexArray, "JsonLayer: the tiles map has been released");
 
     Sprite *tile = nullptr;
      int gid = this->getTileGIDAt(pos);
//  
//      // if GID == 0, then no tile is present
      if (gid) 
      {
		  int z = (int)(pos.x + pos.y * _layerSize.width);
		  tile = static_cast<Sprite*>(this->getChildByTag(z));
//  
//          // tile not created yet. create it
         if (! tile) 
          {
              Rect clip = _tileSet->getRectForGID(gid);
			 // clip = OG_RECT_SCALE(clip);
			
			 // Rect source = { 0,0,clip.size.width,clip.size.width };
              tile = Sprite::createWithTexture(this->getTexture(), clip);
			  //tile->setTextureRect(source);
//            //  tile->setBatchNode(this);
              tile->setPositionNotOffset(getPositionAt(pos) * _contentScaleFactor);
			//  tile->setOffset(Vec2::ZERO);
			  tile->setContentSize(OG_SIZE_SACALE(clip.size * _contentScaleFactor));
			  //设置原始目标渲染位置
			  tile->setOriginalDestPosition({ getPositionForOrthoAt(pos),clip.size });
			  addChild(tile, z, z);
			  
			  for (auto &i:_mapTiles)
			  {
				  if (i == pos)
				  {
					  i.visable = false;
					  break;
				  }
				  
			  }
//              tile->setPositionZ((float)getVertexZForPos(pos));
//              tile->setAnchorPoint(Vec2::ZERO);
//              tile->setOpacity(_opacity);
//  
//              ssize_t indexForZ = atlasIndexForExistantZ(z);
//              this->addSpriteWithoutQuad(tile, static_cast<int>(indexForZ), z);
          }
      }
     
     return tile;
 }
// 
 uint32_t JsonLayer::getTileGIDAt(const Vec2& pos, JSONTileFlags* flags/* = nullptr*/)
 {
//      CCASSERT(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >=0 && pos.y >=0, "JsonLayer: invalid position");
//      CCASSERT(_tiles && _atlasIndexArray, "JsonLayer: the tiles map has been released");
 
     long idx = static_cast<int>(((int) pos.x + (int) pos.y * _layerSize.width));
     // Bits on the far end of the 32-bit global tile ID are used for tile flags
     uint32_t tile = _tiles[idx];
 
     // issue1264, flipped tiles can be changed dynamically
     if (flags) 
     {
         *flags = (JSONTileFlags)(tile & kJSONFlipedAll);
     }
     
     return (tile & kJSONFlippedMask);
 }


Vec2 JsonLayer::getPositionAt(const Vec2& pos)
{
	Vec2 ret;
	switch (_layerOrientation)
	{
	case JSONOrientationOrtho:
		ret = getPositionForOrthoAt(pos);
		break;
	case JSONOrientationIso:
		//ret = getPositionForIsoAt(pos);
		break;
	case JSONOrientationHex:
		//ret = getPositionForHexAt(pos);
		break;
	case JSONOrientationStaggered:
		//ret = getPositionForStaggeredAt(pos);
		break;
	}
	ret = OG_POSITION_SACALE(ret);
	return ret;
}
// 
Vec2 JsonLayer::getPositionForOrthoAt(const Vec2& pos)
{//SDL2版本不需要减1
	return Vec2(pos.x * _mapTileSize.width,
		(/*_layerSize.height -*/ pos.y/* -1*/ ) * _mapTileSize.height);
}


OG_END

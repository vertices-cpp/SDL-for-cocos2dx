 
#ifndef __OG_JSON_LAYER_H__
#define __OG_JSON_LAYER_H__

#include "OGDirector.h"
#include "OGNode.h"
#include "OGJsonParser.h"
#include "OGSprite.h"
#include "Texture2D.h"
 
OG_BEGIN

class JsonMapInfo;
class JsonLayerInfo;
class JsonTilesetInfo;
 
class  JsonLayer : public Node
{
	

public:
	//记录裁剪部分
	struct MapTile {
		int gid;
		float mapSx, mapSy;
		Rect source, sourceDest,dest;
 
		bool visable = true;
		bool operator == (const Vec2 &pos)const
		{
			return mapSx == pos.x && mapSy == pos.y;
		}
		void setSourceRect(const Rect &src) {
			source = src;
		}
		void setSourceDestRect(const Rect &dst) {
			sourceDest = dst;
		}
		void setDestRect(const Rect &dst) {
			dest = dst;
		}
		MapTile(uint32_t sgid, const Vec2 &pos, const Rect &src, const Rect &srcdst, const Rect &dst, bool v) :
			gid(sgid),
			mapSx(pos.x), mapSy(pos.y),visable(v) {
			setSourceDestRect(srcdst);
			setSourceRect(src);

			setDestRect(dst);
		}
	};
    
    static JsonLayer * create(JsonTilesetInfo *tilesetInfo, JsonLayerInfo *layerInfo, JsonMapInfo *mapInfo);
	bool initWithTilesetInfo(JsonTilesetInfo *tilesetInfo, JsonLayerInfo *layerInfo, JsonMapInfo *mapInfo);
	bool appendTileForGID(uint32_t gid, const Vec2& pos);

    JsonLayer();
  
    virtual ~JsonLayer();
 
    

    void releaseMap();
 

	std::weak_ptr<Texture2D> getTexture() const;

	Sprite* getTileAt(const Vec2& tileCoordinate);
 
 	uint32_t getTileGIDAt(const Vec2& tileCoordinate, JSONTileFlags* flags = nullptr);
// 
//    
    void setTileGID(uint32_t gid, const Vec2& tileCoordinate);

//    
// 
//     void setTileGID(uint32_t gid, const Vec2& tileCoordinate, JsonTileFlags flags);

    
//     void removeTileAt(const Vec2& tileCoordinate);
//  
 	Vec2 getPositionAt(const Vec2& tileCoordinate);
 
    
    void setupTiles();
  
    const std::string& getLayerName() { return _layerName; }
    
   
    void setLayerName(const std::string& layerName) { _layerName = layerName; }

    
    const Size& getLayerSize() const { return _layerSize; }
     
    void setLayerSize(const Size& size) { _layerSize = size; }
    
   
    const Size& getMapTileSize() const { return _mapTileSize; }
    
    
    void setMapTileSize(const Size& size) { _mapTileSize = size; }
    
   
	uint32_t* getTiles() const { return _tiles; };
 
	void setTiles(uint32_t* tiles) { _tiles = tiles; };
    
    JsonTilesetInfo* getTileSet() const { return _tileSet; }
    
  
    void setTileSet(JsonTilesetInfo* info) {
        OG_SAFE_RETAIN(info);
	//	OG_SAFE_RELEASE(_tileSet);
     //   _tileSet = info;
    }
    
/*

左上
x1 = 1-1*3*0.5=-0.5*50=-25
y1=1-1*3*0.5=-0.5*50=-25
右上
x2 = 1+1*3*0.5=2.5*50=125
y2=1-1*3*0.5=-0.5*50=-25
左下
x3=1-1*0.5=-0.5*50=-25
y3 = 1+1*3*0.5=2.5*50=125
右下
x4= 1+1*3*0.5=2.5*50=125
y4= 1+1*3*0.5=2.5*50=125
*/
	Vec2 getLayerScale()const { return _contentScaleFactor; }
	void setLayerScale(const Vec2 & scale)
	{
		auto centerPoint = SDLView::getInstance()->getWindowSize() / 2;
		for (auto &i : _mapTiles)
		{
		 

			Size s = i.sourceDest.size * scale;
			Vec2 dst;

		/*
		x=x-(sx*w-w)/2
y=y-(sy*h-h)/2
要算图集相对位置。。
*/
			dst.x = centerPoint.width - centerPoint.width * scale.x + i.sourceDest.origin.x * scale.x;// (s.width - i.sourceDest.size.width) / 2;
			dst.y = centerPoint.height - centerPoint.height* scale.y + i.sourceDest.origin.y * scale.y;// (s.height   - i.sourceDest.size.height) / 2;
  		//	dst = dst * scale;
			Rect r = OG_RECT_SCALE( Rect(dst, s))  ;
// 			auto winsize = SDLView::getInstance()->getWindowSize();
			
//   			r.origin.x  = SDLView::getInstance()->getWindowSize().width - r.origin.x + r.size.width;
//  			r.origin.y = SDLView::getInstance()->getWindowSize().height - r.origin.y + r.size.height;
			i.dest = r;
			 
		}
		for (auto it = _children.begin(), itCend = _children.end(); it != itCend; ++it)
		{
			auto curRect = (*it)->getOriginalDestPosition();
		//	auto tile = (*it)->getPosition();
			auto offsetValue = ((Sprite*)(*it))->getOffset();
			Vec2 dst;
			dst.x = centerPoint.width - centerPoint.width * scale.x + curRect.origin.x * scale.x;// 
			dst.y = centerPoint.height - centerPoint.height* scale.y + curRect.origin.y * scale.y;// 
		
			((Sprite*)(*it))->setPositionNotOffset(OG_POSITION_SACALE(dst /** scale*/ ) + offsetValue);
			(*it)->setContentSize(OG_SIZE_SACALE(curRect.size * scale));
		}
		_contentScaleFactor = scale;
	}

protected:
	//Vec2 getPositionForIsoAt(const Vec2& pos);
	Vec2 getPositionForOrthoAt(const Vec2& pos);
// 	Vec2 getPositionForHexAt(const Vec2& pos);
// 	Vec2 getPositionForStaggeredAt(const Vec2& pos);
// 	Vec2 calculateLayerOffset(const Vec2& offset);

    /* optimization methods */
	
  /*  Sprite* insertTileForGID(uint32_t gid, const Vec2& pos);
	Sprite* updateTileForGID(uint32_t gid, const Vec2& pos);

	intptr_t getZForPos(const Vec2& pos) const;


	void setupTileSprite(Sprite* sprite, const Vec2& pos, uint32_t gid);*/

	
	void draw() {
 
	//	cout << "test" << endl;
		if (!_visible)
			return;

		for (auto &i:_mapTiles)
		{
			if (i.visable)
			{
				Rect source = i.source;
				Rect dest_render_position = { _render_position +i.dest.origin  ,i.dest.size };
	           
				_texture->render(source, dest_render_position, _rotate,
					_pointer, _displayedColor, _flipX, _flipY);
 
			}
		}
	}


    //! name of the layer
    std::string _layerName;
    //! Json Layer supports opacity
    unsigned char _opacity;
    
	//MapTile *mTptr;
    
    // used for retina display
    Vec2 _contentScaleFactor;
    
    /** size of the layer in tiles */
    Size _layerSize;
    /** size of the map's tile (could be different from the tile's size) */
    Size _mapTileSize;
    /** pointer to the map of tiles */
    uint32_t* _tiles;
    /** Tileset information for the layer */
    JsonTilesetInfo* _tileSet;
	int _layerOrientation;
	std::vector<struct MapTile> _mapTiles;
 
	std::shared_ptr<Texture2D> _texture = nullptr;
	BlendFunc bfunc;
};

// end of tilemap_parallax_nodes group
/** @} */

OG_END

#endif //__OG_JSON_LAYER_H__



#ifndef _OGPARTICLE_EXAMPLE_H_
#define _OGPARTICLE_EXAMPLE_H_

#include "OGParticleSystem.h"

// #define OG_DISALLOW_COPY_AND_ASSIGN(TypeName) \
//     TypeName(const TypeName &) = delete; \
//     TypeName &operator =(const TypeName &) = delete;



/**
 * @addtogroup _2d
 * @{
 */

/** @class ParticleFire
 * @brief A fire particle system.
 */

OG_BEGIN

class ParticleExample : public ParticleSystem{
public:
	enum PatticleStyle
	{
		NONE,
		FIRE,
		FIRE_WORK,
		SUN,
		GALAXY,
		FLOWER,
		METEOR,
		SPIRAL,
		EXPLOSION,
		SMOKE,
		SNOW,
		RAIN,
	};

	static ParticleExample* create();
	ParticleExample() {}
	virtual ~ParticleExample() {}

	

	void setStyle(PatticleStyle style);
	void start_run();
protected:
	PatticleStyle _style = NONE;
	
private:
	ParticleExample(const ParticleExample &) = delete;
	ParticleExample &operator =(const ParticleExample &) = delete;
};

OG_END

#endif
#include "OGParticleExamples.h" 
#include "OGDirector.h"
#include "Texture2D.h"

OG_BEGIN

// 
// static Texture2D* getDefaultTexture()
// {
//     Texture2D* texture = nullptr;
//     Image* image = nullptr;
//     do 
//     {
//         const std::string key = "/__firePngData";
//         texture = Director::getInstance()->getTextureCache()->getTextureForKey(key);
//         OG_BREAK_IF(texture != nullptr);
// 
//         image = new (std::nothrow) Image();
//         OG_BREAK_IF(nullptr == image);
//         bool ret = image->initWithImageData(__firePngData, sizeof(__firePngData));
//         OG_BREAK_IF(!ret);
// 
//         texture = Director::getInstance()->getTextureCache()->addImage(image, key);
//     } while (0);
// 
//     OG_SAFE_RELEASE(image);
// 
//     return texture;
// }


ParticleExample * ParticleExample::create()
{
	ParticleExample *particleExample = new ParticleExample;
	if (particleExample != nullptr)
	{
		particleExample->autorelease();
	}
	else
	{
		delete particleExample;
		particleExample = nullptr;
	}
	return particleExample;
	

}
void ParticleExample::setStyle(PatticleStyle style)
{
	if (style == _style)
		return;
	_style = style;

	if (_texture == nullptr)
	{
// 		auto texture = _director->getTextureCache()->addMemoryFile("__firePngData",
// 			(unsigned char *)__firePngData, sizeof(__firePngData));

		auto texture = std::make_shared<Texture2D>();
		texture->loadMemData((unsigned char *)__firePngData, sizeof(__firePngData));

		if (texture == nullptr)
			return;

		setTexture(texture);
		Size winSize = _director->getVisibleSize();
		_position = ((winSize - _contentSize) / 2);
	}

	start_run();
}
void ParticleExample::start_run()
{
	switch (_style)
	{
	case ParticleExample::NONE:
		break;
	case ParticleExample::FIRE:
	{
		initWithTotalParticles(250);

		_duration = DURATION_INFINITY;

		// Gravity Mode
		this->_emitterMode = Mode::GRAVITY;

		// Gravity Mode: gravity
		this->modeA.gravity = Vec2::ZERO;

		// Gravity Mode: radial acceleration
		this->modeA.radialAccel = 0;
		this->modeA.radialAccelVar = 0;

		// Gravity Mode: speed of particles
		this->modeA.speed = -60;
		this->modeA.speedVar = 20;

		// starting angle
		_angle = 90;
		_angleVar = 10;

		

		this->_posVar = { 40.0f, 20.0f };

		// life of particles
		_life = 3;
		_lifeVar = 0.25f;


		// size, in pixels
		_startSize = 54.0f;
		_startSizeVar = 10.0f;
		_endSize = START_SIZE_EQUAL_TO_END_SIZE;

		// emits per frame
		_emissionRate = _totalParticles / _life;

		// color of particles
		_startColor.r = 0.76f;
		_startColor.g = 0.25f;
		_startColor.b = 0.12f;
		_startColor.a = 1.0f;
		_startColorVar.r = 0.0f;
		_startColorVar.g = 0.0f;
		_startColorVar.b = 0.0f;
		_startColorVar.a = 0.0f;
		_endColor.r = 0.0f;
		_endColor.g = 0.0f;
		_endColor.b = 0.0f;
		_endColor.a = 1.0f;
		_endColorVar.r = 0.0f;
		_endColorVar.g = 0.0f;
		_endColorVar.b = 0.0f;
		_endColorVar.a = 0.0f;

		// additive
		this->setBlendAdditive(true);

	}
		break;
	
	case ParticleExample::FIRE_WORK:
	{
		initWithTotalParticles(1500);

		// duration
		_duration = DURATION_INFINITY;

		// Gravity Mode
		this->_emitterMode = Mode::GRAVITY;

		// Gravity Mode: gravity
		this->modeA.gravity = { 0.0f, 90.0f };

		// Gravity Mode:  radial
		this->modeA.radialAccel = 0.0f;
		this->modeA.radialAccelVar = 0.0f;

		//  Gravity Mode: speed of particles
		this->modeA.speed = -180.0f;
		this->modeA.speedVar = 50.0f;

		
		// angle
		this->_angle = 90.0f;
		this->_angleVar = 20.0f;

		// life of particles
		this->_life = 3.5f;
		this->_lifeVar = 1.0f;

		// emits per frame
		this->_emissionRate = _totalParticles / _life;

		// color of particles
		_startColor.r = 0.5f;
		_startColor.g = 0.5f;
		_startColor.b = 0.5f;
		_startColor.a = 1.0f;
		_startColorVar.r = 0.5f;
		_startColorVar.g = 0.5f;
		_startColorVar.b = 0.5f;
		_startColorVar.a = 0.1f;
		_endColor.r = 0.1f;
		_endColor.g = 0.1f;
		_endColor.b = 0.1f;
		_endColor.a = 0.2f;
		_endColorVar.r = 0.1f;
		_endColorVar.g = 0.1f;
		_endColorVar.b = 0.1f;
		_endColorVar.a = 0.2f;

		// size, in pixels
		_startSize = 8.0f;
		_startSizeVar = 2.0f;
		_endSize = START_SIZE_EQUAL_TO_END_SIZE;


		// additive
		this->setBlendAdditive(false);
	}
		break;
	case ParticleExample::SUN:
	{
		initWithTotalParticles(350);

		// additive
		this->setBlendAdditive(true);

		// duration
		_duration = DURATION_INFINITY;

		// Gravity Mode
		setEmitterMode(Mode::GRAVITY);

		// Gravity Mode: gravity
		setGravity(Vec2(0, 0));

		// Gravity mode: radial acceleration
		setRadialAccel(0);
		setRadialAccelVar(0);

		// Gravity mode: speed of particles
		setSpeed(-20);
		setSpeedVar(5);


		// angle
		_angle = 90;
		_angleVar = 360;

		setPosVar(Vec2::ZERO);

		// life of particles
		_life = 1;
		_lifeVar = 0.5f;

		// size, in pixels
		_startSize = 30.0f;
		_startSizeVar = 10.0f;
		_endSize = START_SIZE_EQUAL_TO_END_SIZE;

		// emits per seconds
		_emissionRate = _totalParticles / _life;

		// color of particles
		_startColor.r = 0.76f;
		_startColor.g = 0.25f;
		_startColor.b = 0.12f;
		_startColor.a = 1.0f;
		_startColorVar.r = 0.0f;
		_startColorVar.g = 0.0f;
		_startColorVar.b = 0.0f;
		_startColorVar.a = 0.0f;
		_endColor.r = 0.0f;
		_endColor.g = 0.0f;
		_endColor.b = 0.0f;
		_endColor.a = 1.0f;
		_endColorVar.r = 0.0f;
		_endColorVar.g = 0.0f;
		_endColorVar.b = 0.0f;
		_endColorVar.a = 0.0f;
	}

		break;
	case ParticleExample::GALAXY:
	{
		initWithTotalParticles(200);

		// duration
		_duration = DURATION_INFINITY;

		// Gravity Mode
		setEmitterMode(Mode::GRAVITY);

		// Gravity Mode: gravity
		setGravity(Vec2(0, 0));

		// Gravity Mode: speed of particles
		setSpeed(-60);
		setSpeedVar(10);

		// Gravity Mode: radial
		setRadialAccel(-80);
		setRadialAccelVar(0);

		// Gravity Mode: tangential
		setTangentialAccel(80);
		setTangentialAccelVar(0);

		// angle
		_angle = 90;
		_angleVar = 360;

		setPosVar(Vec2::ZERO);

		// life of particles
		_life = 4;
		_lifeVar = 1;

		// size, in pixels
		_startSize = 37.0f;
		_startSizeVar = 10.0f;
		_endSize = START_SIZE_EQUAL_TO_END_SIZE;

		// emits per second
		_emissionRate = _totalParticles / _life;

		// color of particles
		_startColor.r = 0.12f;
		_startColor.g = 0.25f;
		_startColor.b = 0.76f;
		_startColor.a = 1.0f;
		_startColorVar.r = 0.0f;
		_startColorVar.g = 0.0f;
		_startColorVar.b = 0.0f;
		_startColorVar.a = 0.0f;
		_endColor.r = 0.0f;
		_endColor.g = 0.0f;
		_endColor.b = 0.0f;
		_endColor.a = 1.0f;
		_endColorVar.r = 0.0f;
		_endColorVar.g = 0.0f;
		_endColorVar.b = 0.0f;
		_endColorVar.a = 0.0f;


		// additive
		this->setBlendAdditive(true);

	}
		break;
	case ParticleExample::FLOWER:
	{
		initWithTotalParticles(250);

		// duration
		_duration = DURATION_INFINITY;

		// Gravity Mode
		setEmitterMode(Mode::GRAVITY);

		// Gravity Mode: gravity
		setGravity(Vec2(0, 0));

		// Gravity Mode: speed of particles
		setSpeed(-80);
		setSpeedVar(10);

		// Gravity Mode: radial
		setRadialAccel(-60);
		setRadialAccelVar(0);

		// Gravity Mode: tangential
		setTangentialAccel(15);
		setTangentialAccelVar(0);

		// angle
		_angle = 90;
		_angleVar = 360;

		setPosVar(Vec2::ZERO);

		// life of particles
		_life = 4;
		_lifeVar = 1;

		// size, in pixels
		_startSize = 30.0f;
		_startSizeVar = 10.0f;
		_endSize = START_SIZE_EQUAL_TO_END_SIZE;

		// emits per second
		_emissionRate = _totalParticles / _life;

		// color of particles
		_startColor.r = 0.50f;
		_startColor.g = 0.50f;
		_startColor.b = 0.50f;
		_startColor.a = 1.0f;
		_startColorVar.r = 0.5f;
		_startColorVar.g = 0.5f;
		_startColorVar.b = 0.5f;
		_startColorVar.a = 0.5f;
		_endColor.r = 0.0f;
		_endColor.g = 0.0f;
		_endColor.b = 0.0f;
		_endColor.a = 1.0f;
		_endColorVar.r = 0.0f;
		_endColorVar.g = 0.0f;
		_endColorVar.b = 0.0f;
		_endColorVar.a = 0.0f;

		// additive
		this->setBlendAdditive(true);

	}
		break;
	case ParticleExample::METEOR:
	{
		initWithTotalParticles(150);

		// duration
		_duration = DURATION_INFINITY;

		// Gravity Mode
		setEmitterMode(Mode::GRAVITY);

		// Gravity Mode: gravity
		setGravity(Vec2(-200, -200));

		// Gravity Mode: speed of particles
		setSpeed(-15);
		setSpeedVar(5);

		// Gravity Mode: radial
		setRadialAccel(0);
		setRadialAccelVar(0);

		// Gravity Mode: tangential
		setTangentialAccel(0);
		setTangentialAccelVar(0);

		// angle
		_angle = 90;
		_angleVar = 360;

		setPosVar(Vec2::ZERO);

		// life of particles
		_life = 2;
		_lifeVar = 1;

		// size, in pixels
		_startSize = 60.0f;
		_startSizeVar = 10.0f;
		_endSize = START_SIZE_EQUAL_TO_END_SIZE;

		// emits per second
		_emissionRate = _totalParticles / _life;

		// color of particles
		_startColor.r = 0.2f;
		_startColor.g = 0.4f;
		_startColor.b = 0.7f;
		_startColor.a = 1.0f;
		_startColorVar.r = 0.0f;
		_startColorVar.g = 0.0f;
		_startColorVar.b = 0.2f;
		_startColorVar.a = 0.1f;
		_endColor.r = 0.0f;
		_endColor.g = 0.0f;
		_endColor.b = 0.0f;
		_endColor.a = 1.0f;
		_endColorVar.r = 0.0f;
		_endColorVar.g = 0.0f;
		_endColorVar.b = 0.0f;
		_endColorVar.a = 0.0f;


		// additive
		this->setBlendAdditive(true);
		//�����ӵ�

	}

		break;
	case ParticleExample::SPIRAL:
	{
		initWithTotalParticles(500);

		// duration
		_duration = DURATION_INFINITY;

		// Gravity Mode
		setEmitterMode(Mode::GRAVITY);

		// Gravity Mode: gravity
		setGravity(Vec2(0, 0));

		// Gravity Mode: speed of particles
		setSpeed(-150);
		setSpeedVar(0);

		// Gravity Mode: radial
		setRadialAccel(-380);
		setRadialAccelVar(0);

		// Gravity Mode: tangential
		setTangentialAccel(-45);
		setTangentialAccelVar(0);

		// angle
		_angle = 90;
		_angleVar = 0;

		setPosVar(Vec2::ZERO);

		// life of particles
		_life = 12;
		_lifeVar = 0;

		// size, in pixels
		_startSize = 20.0f;
		_startSizeVar = 0.0f;
		_endSize = START_SIZE_EQUAL_TO_END_SIZE;

		// emits per second
		_emissionRate = _totalParticles / _life;

		// color of particles
		_startColor.r = 0.5f;
		_startColor.g = 0.5f;
		_startColor.b = 0.5f;
		_startColor.a = 1.0f;
		_startColorVar.r = 0.5f;
		_startColorVar.g = 0.5f;
		_startColorVar.b = 0.5f;
		_startColorVar.a = 0.0f;
		_endColor.r = 0.5f;
		_endColor.g = 0.5f;
		_endColor.b = 0.5f;
		_endColor.a = 1.0f;
		_endColorVar.r = 0.5f;
		_endColorVar.g = 0.5f;
		_endColorVar.b = 0.5f;
		_endColorVar.a = 0.0f;


		// additive
		this->setBlendAdditive(false);
	}
		break;
	case ParticleExample::EXPLOSION:
	{
		initWithTotalParticles(700);

		// duration
		_duration = 0.1f;

		setEmitterMode(Mode::GRAVITY);

		// Gravity Mode: gravity
		setGravity(Vec2(0, 0));

		// Gravity Mode: speed of particles
		setSpeed(-70);
		setSpeedVar(40);

		// Gravity Mode: radial
		setRadialAccel(0);
		setRadialAccelVar(0);

		// Gravity Mode: tangential
		setTangentialAccel(0);
		setTangentialAccelVar(0);

		// angle
		_angle = 90;
		_angleVar = 360;

		setPosVar(Vec2::ZERO);

		// life of particles
		_life = 5.0f;
		_lifeVar = 2;

		// size, in pixels
		_startSize = 15.0f;
		_startSizeVar = 10.0f;
		_endSize = START_SIZE_EQUAL_TO_END_SIZE;

		// emits per second
		_emissionRate = _totalParticles / _duration;

		// color of particles
		_startColor.r = 0.7f;
		_startColor.g = 0.1f;
		_startColor.b = 0.2f;
		_startColor.a = 1.0f;
		_startColorVar.r = 0.5f;
		_startColorVar.g = 0.5f;
		_startColorVar.b = 0.5f;
		_startColorVar.a = 0.0f;
		_endColor.r = 0.5f;
		_endColor.g = 0.5f;
		_endColor.b = 0.5f;
		_endColor.a = 0.0f;
		_endColorVar.r = 0.5f;
		_endColorVar.g = 0.5f;
		_endColorVar.b = 0.5f;
		_endColorVar.a = 0.0f;

		// additive
		this->setBlendAdditive(false);
	}

		break;
	case ParticleExample::SMOKE:
	{
		initWithTotalParticles(200);
		// duration
		_duration = DURATION_INFINITY;

		// Emitter mode: Gravity Mode
		setEmitterMode(Mode::GRAVITY);

		// Gravity Mode: gravity
		setGravity(Vec2(0, 0));

		// Gravity Mode: radial acceleration
		setRadialAccel(0);
		setRadialAccelVar(0);

		// Gravity Mode: speed of particles
		setSpeed(-25);
		setSpeedVar(10);

		// angle
		_angle = 90;
		_angleVar = 5;

	
		setPosVar(Vec2(20, 0));

		// life of particles
		_life = 4;
		_lifeVar = 1;

		// size, in pixels
		_startSize = 60.0f;
		_startSizeVar = 10.0f;
		_endSize = START_SIZE_EQUAL_TO_END_SIZE;

		// emits per frame
		_emissionRate = _totalParticles / _life;

		// color of particles
		_startColor.r = 0.8f;
		_startColor.g = 0.8f;
		_startColor.b = 0.8f;
		_startColor.a = 1.0f;
		_startColorVar.r = 0.02f;
		_startColorVar.g = 0.02f;
		_startColorVar.b = 0.02f;
		_startColorVar.a = 0.0f;
		_endColor.r = 0.0f;
		_endColor.g = 0.0f;
		_endColor.b = 0.0f;
		_endColor.a = 1.0f;
		_endColorVar.r = 0.0f;
		_endColorVar.g = 0.0f;
		_endColorVar.b = 0.0f;
		_endColorVar.a = 0.0f;

		// additive
		this->setBlendAdditive(false);
	}

		break;
	case ParticleExample::SNOW:
	{
		initWithTotalParticles(700);

		// duration
		_duration = DURATION_INFINITY;

		// set gravity mode.
		setEmitterMode(Mode::GRAVITY);

		// Gravity Mode: gravity
		setGravity(Vec2(0, 1));

		// Gravity Mode: speed of particles
		setSpeed(-5);
		setSpeedVar(1);

		// Gravity Mode: radial
		setRadialAccel(0);
		setRadialAccelVar(1);

		// Gravity mode: tangential
		setTangentialAccel(0);
		setTangentialAccelVar(1);

	
		setPosVar(Vec2(1.0f *_position.x , 0.0f));

		// angle
		_angle = -90;
		_angleVar = 5;

		// life of particles
		_life = 45;
		_lifeVar = 15;

		// size, in pixels
		_startSize = 10.0f;
		_startSizeVar = 5.0f;
		_endSize = START_SIZE_EQUAL_TO_END_SIZE;

		// emits per second
		_emissionRate = 10;

		// color of particles
		_startColor.r = 1.0f;
		_startColor.g = 1.0f;
		_startColor.b = 1.0f;
		_startColor.a = 1.0f;
		_startColorVar.r = 0.0f;
		_startColorVar.g = 0.0f;
		_startColorVar.b = 0.0f;
		_startColorVar.a = 0.0f;
		_endColor.r = 1.0f;
		_endColor.g = 1.0f;
		_endColor.b = 1.0f;
		_endColor.a = 0.0f;
		_endColorVar.r = 0.0f;
		_endColorVar.g = 0.0f;
		_endColorVar.b = 0.0f;
		_endColorVar.a = 0.0f;


		// additive
		this->setBlendAdditive(false);
	}
		break;
	case ParticleExample::RAIN:
	{
		initWithTotalParticles(1000);

		// duration
		_duration = DURATION_INFINITY;

		setEmitterMode(Mode::GRAVITY);

		// Gravity Mode: gravity
		setGravity(Vec2(10, 10));

		// Gravity Mode: radial
		setRadialAccel(0);
		setRadialAccelVar(1);

		// Gravity Mode: tangential
		setTangentialAccel(0);
		setTangentialAccelVar(1);

		// Gravity Mode: speed of particles
		setSpeed(-130);
		setSpeedVar(30);

		// angle
		_angle = -90;
		_angleVar = 5;


		setPosVar(Vec2(1.0f *_position.x, 0.0f));

		// life of particles
		_life = 4.5f;
		_lifeVar = 0;

		// size, in pixels
		_startSize = 4.0f;
		_startSizeVar = 2.0f;
		_endSize = START_SIZE_EQUAL_TO_END_SIZE;

		// emits per second
		_emissionRate = 20;

		// color of particles
		_startColor.r = 0.7f;
		_startColor.g = 0.8f;
		_startColor.b = 1.0f;
		_startColor.a = 1.0f;
		_startColorVar.r = 0.0f;
		_startColorVar.g = 0.0f;
		_startColorVar.b = 0.0f;
		_startColorVar.a = 0.0f;
		_endColor.r = 0.7f;
		_endColor.g = 0.8f;
		_endColor.b = 1.0f;
		_endColor.a = 0.5f;
		_endColorVar.r = 0.0f;
		_endColorVar.g = 0.0f;
		_endColorVar.b = 0.0f;
		_endColorVar.a = 0.0f;

		// additive
		this->setBlendAdditive(false);
	}
		break;
	default:
		break;
	}
}

OG_END
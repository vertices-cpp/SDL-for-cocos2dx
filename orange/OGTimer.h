#ifndef _TIMER_H_
#define _TIMER_H_

#include "OGPlatformMacros.h"
#include "Header.h"


const float TARGET_FPS = 60;
const float SCREEN_TICK_PER_FRAMES = 1000 / TARGET_FPS;

#define TARGET_DELTATIME 0.2f


class Timer
{
	float  mDeltaTime ;
	Uint32 fpsTimer, mStartTicks, mOldTimeStamp, countedFrames;
public:
	Instance(Timer);

	Timer() {}
	~Timer() {}

	void ticks() {
		fpsTimer = SDL_GetTicks();
	}
	float getFps() {
		float avgFPS = countedFrames / ((SDL_GetTicks() - fpsTimer) / 1000.f);
		//++countedFrames;
		return avgFPS;
	}
	Uint32 GetGlobalCount() {
		return countedFrames;
	}
	void capTimeStart() {

		mStartTicks = SDL_GetTicks();
		

		mDeltaTime = (mStartTicks - mOldTimeStamp) / 1000.f;
		mOldTimeStamp = mStartTicks;




		//添加延迟功能
		//float TARGET_FPS_R = (TARGET_FPS / 1000.0f);


		if (mDeltaTime > TARGET_DELTATIME)
		{
			mDeltaTime = 1 / 60.0f;
		}
		cout << mDeltaTime << endl;

	}
	//void capTimeStart() {

	//	mStartTicks = SDL_GetTicks();

	//	//添加延迟功能

	//	mDeltaTime = (mStartTicks - mOldTimeStamp) /1000.0f;
	//	mOldTimeStamp = mStartTicks;
	//}
	float get_delay() {
		//cout <<">"<< mDeltaTime << " " << 1 / 60.0f << endl;
		return mDeltaTime;
	}

#define FPS60
	void TimeDelay() {

		++countedFrames;
#ifdef FPS60 
		float frameTicks = SDL_GetTicks() - mStartTicks;
		if (frameTicks < SCREEN_TICK_PER_FRAMES)
		{
			//cout << ":::::" << SCREEN_TICK_PER_FRAMES - frameTicks << endl;
			SDL_Delay(SCREEN_TICK_PER_FRAMES - frameTicks);
		}
#endif
	}
};

#endif
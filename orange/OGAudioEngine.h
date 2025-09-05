#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#pragma 


#include "Header.h"

#include "OGPlatformMacros.h"

#include "SDLView.h"

#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <thread>

OG_BEGIN


// �Զ��崦�������������Ʋ��� Mix_Chunk ����Ƶ���ݵ���һ���֣��Լ�������Щ��������ص��޸ġ�  
typedef struct Custom_Mix_PlaybackSpeedEffectHandler
{
	const Mix_Chunk* chunk;
	const float* speed;  // ptr ָ������Ĳ����ٶ�  
	float position; // �����ĵ�ǰλ�ã��Ժ���Ϊ��λ  
	int altered; // ����˲��Ŵ�δ����������Ϊ false�� 

	// ֻ����
	int loop; //���Ƿ���ѭ������ 
	int duration; // �����ĳ���ʱ�䣬�Ժ���Ϊ��λ  
	int chunk_size; // �����Ĵ�С�������������������㣩��ʾ����ʹ���ʵ����������ͣ����������� Uint8*��ʱ��������Ϊ�����С�� 
	int self_halt; // ��ǲ������ʱ�Ƿ�Ӧͨ���˻ص�ֹͣ����  
} Custom_Mix_PlaybackSpeedEffectHandler;



extern Uint16 audio_format; //��ǰ��Ƶ��ʽ����
extern  int audio_frequency, // ��ǰ��Ƶ��ʽ��Ƶ��
audio_channel_count, // ��ǰ��Ƶ��ʽ��ͨ����
audio_allocated_mix_channels_count; // ����Ļ���ͨ������
extern   void Custom_Mix_PlaybackSpeedEffectFuncCallback(int mix_channel, void* stream, int length, void* user_data);




extern Uint16 format_sample_size(Uint16 format);

extern  int Custom_Mix_ComputeChunkLengthMillisec(int chunkSize);

extern  Custom_Mix_PlaybackSpeedEffectHandler* Custom_Mix_CreatePlaybackSpeedEffectHandler(const Mix_Chunk* chunk,
	const float* speed, int loop, int self_halt);



// Mix_EffectDone_t �ص���Ч��ʹ�ý���ʱɾ���������ͨ�� userData ���ݵĴ������ 
extern  void Custom_Mix_PlaybackSpeedEffectDoneCallback(int channel, void *userData);
extern  void Custom_Mix_RegisterPlaybackSpeedEffect(int channel, Mix_Chunk* chunk, float* speed, int loop, int selfHalt);


 

struct MyTrack {
	std::vector<int> channels ;
	Mix_Chunk* wav = nullptr;
};

class AudioEngine {
	std::mutex _play2dMutex;
	  float speed;
	  
	  std::unordered_map<std::string, MyTrack*> wav_chunks;
	  bool play_wav(const std::string &wav_path)
	  {
		  _play2dMutex.lock();
		  auto  it = wav_chunks.find(wav_path);

		  if (it != wav_chunks.end())
		  {
			  int channel = Mix_PlayChannelTimed(-1, it->second->wav, 0, -1);

			  it->second->channels.push_back(channel);

			 Custom_Mix_RegisterPlaybackSpeedEffect(channel, it->second->wav, &speed, 1, 0);
			 

		  }
		  else {
			  MyTrack *mpt = new MyTrack;
			  mpt->wav = Mix_LoadWAV(wav_path.c_str());
			  //  SDL_Log("%s", SDL_GetError());
			  wav_chunks[wav_path] = mpt;

			  int channel = Mix_PlayChannelTimed(-1, mpt->wav, 0, -1);
			  Custom_Mix_RegisterPlaybackSpeedEffect(channel, mpt->wav, &speed, 1, 0);
			  
			  mpt->channels.push_back(channel);
		  }

		  _play2dMutex.unlock();

		  return true;
	  }
public:
	static AudioEngine* getInstace() {
		static AudioEngine _instance;
		return &_instance;
	}
	AudioEngine() {
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
 		Mix_QuerySpec(&audio_frequency, &audio_format, &audio_channel_count);  // ��ѯ���
		audio_allocated_mix_channels_count = Mix_AllocateChannels(MIX_CHANNELS);
		speed = 1;
	}
	~AudioEngine() {
		 free();
		Mix_CloseAudio();
		Mix_Quit();
	}
	void free() {
		for (auto it = wav_chunks.begin(); it != wav_chunks.end(); ++it)
		{
			if (it->second != nullptr &&it->second->wav !=nullptr)
			{
				for (auto &item:it->second->channels)
				{
					Mix_HaltChannel(item);
				}
				
				Mix_FreeChunk(it->second->wav);
				delete it->second;
			}
		}
		wav_chunks.clear();
	}
	static bool play2d(const std::string &wav_path)
	{
		auto audio = AudioEngine::getInstace();
		if (wav_path.empty())
			return false;
		audio->_play2d(wav_path);
		

		return true;
	}

	  bool _play2d(const std::string &wav_path)
	{
		  
		  std::thread(&AudioEngine::play_wav, this, wav_path).detach();
		 

		return true;
	}

	  float getSpeed()  { return speed; }

	static  void stop(const char * wav) {
		auto audio = AudioEngine::getInstace();

		for (auto it = audio->wav_chunks.begin();   it != audio->wav_chunks.end(); ++it)
		{
			if (it->first == wav)
			{
				auto ptr = it->second;
				//Mix_Pause(ptr->channel);
				for (auto &item : it->second->channels)
				{
					Mix_Pause(item);
				}
			}
		}

	}
	static void stopAll() {
		auto audio = AudioEngine::getInstace();
		for (auto it = audio->wav_chunks.begin(); it != audio->wav_chunks.end(); ++it)
		{
			auto ptr = it->second;
			for (auto &item : it->second->channels)
			{
				Mix_Pause(item);
			}
		}
	}

	static void resume(const char * wav) {
		auto audio = AudioEngine::getInstace();
		for (auto it = audio->wav_chunks.begin(); it != audio->wav_chunks.end(); ++it)
		{
			if (it->first == wav )
			{
				auto ptr = it->second;
				 
				for (auto &item : it->second->channels)
				{
					Mix_Resume(item);
				}
				//Mix_PlayChannelTimed(ptr->channel, ptr->wav, 0, -1);
			//	Custom_Mix_RegisterPlaybackSpeedEffect(ptr->channel, ptr->wav, &speed, 1, 0);
			}
		}
	}
	
	static 	void resumeAll() {
		auto audio = AudioEngine::getInstace();
		for (auto it = audio->wav_chunks.begin(); it != audio->wav_chunks.end(); ++it)
		{
			auto ptr = it->second;
 
			for (auto &item : it->second->channels)
			{
				Mix_Resume(item);
			}
				//Mix_PlayChannelTimed(ptr->channel, ptr->wav, 0, -1);
			//	Custom_Mix_RegisterPlaybackSpeedEffect(ptr->channel, ptr->wav, &speed, 1, 0);
 
		}
	}
	static 	void closeAll() {
		auto audio = AudioEngine::getInstace();
		audio->free();
	}
// 	void speedInc() {
// 		speed += 0.1;
// 
// 	}
// 	void speedDec() {
// 		if (speed > 0.1)
// 			speed -= 0.1;
// 
// 	}

};
 

OG_END

#endif
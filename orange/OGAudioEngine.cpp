#include "OGAudioEngine.h"

OG_BEGIN


  Uint16 audio_format; //��ǰ��Ƶ��ʽ����
  int audio_frequency, // ��ǰ��Ƶ��ʽ��Ƶ��
audio_channel_count, // ��ǰ��Ƶ��ʽ��ͨ����
audio_allocated_mix_channels_count; // ����Ļ���ͨ������


//Uint8 �汾�Ļص���ʵ�� 
#define Custom_Mix_PlaybackSpeedEffectFuncCallback Custom_Mix_PlaybackSpeedEffectFuncCallbackUint8
#define AUDIO_FORMAT_TYPE Uint8
#include "custom_mix_pitch_func.inc"
#undef Custom_Mix_PlaybackSpeedEffectFuncCallback
#undef AUDIO_FORMAT_TYPE

// Sint8 �汾�ص���ʵ��  
#define Custom_Mix_PlaybackSpeedEffectFuncCallback Custom_Mix_PlaybackSpeedEffectFuncCallbackSint8
#define AUDIO_FORMAT_TYPE Sint8
#include "custom_mix_pitch_func.inc"
#undef Custom_Mix_PlaybackSpeedEffectFuncCallback
#undef AUDIO_FORMAT_TYPE

//Uint16�汾�Ļص���ʵ�� 
#define Custom_Mix_PlaybackSpeedEffectFuncCallback Custom_Mix_PlaybackSpeedEffectFuncCallbackUint16
#define AUDIO_FORMAT_TYPE Uint16
#include "custom_mix_pitch_func.inc"
#undef Custom_Mix_PlaybackSpeedEffectFuncCallback
#undef AUDIO_FORMAT_TYPE

// Sint16 �汾�Ļص���ʵ��  
#define Custom_Mix_PlaybackSpeedEffectFuncCallback Custom_Mix_PlaybackSpeedEffectFuncCallbackSint16
#define AUDIO_FORMAT_TYPE Sint16
#include "custom_mix_pitch_func.inc"
#undef Custom_Mix_PlaybackSpeedEffectFuncCallback
#undef AUDIO_FORMAT_TYPE

// Sint32 �汾�Ļص���ʵ��  
#define Custom_Mix_PlaybackSpeedEffectFuncCallback Custom_Mix_PlaybackSpeedEffectFuncCallbackSint32
#define AUDIO_FORMAT_TYPE Sint32
#include "custom_mix_pitch_func.inc"
#undef Custom_Mix_PlaybackSpeedEffectFuncCallback
#undef AUDIO_FORMAT_TYPE

// Float �汾�Ļص�ʵ��  
#define Custom_Mix_PlaybackSpeedEffectFuncCallback Custom_Mix_PlaybackSpeedEffectFuncCallbackFloat
#define AUDIO_FORMAT_TYPE float
#include "custom_mix_pitch_func.inc"
#undef Custom_Mix_PlaybackSpeedEffectFuncCallback
#undef AUDIO_FORMAT_TYPE


Uint16 format_sample_size(Uint16 format) { 
	return (format & 0xFF) / 8;
}

int Custom_Mix_ComputeChunkLengthMillisec(int chunkSize)
{
	const Uint32 points = chunkSize / format_sample_size(audio_format); // �ֽ� / samplesize == ������
	const Uint32 frames = (points / audio_channel_count); // ������/ͨ�� == ����֡
	return frames;// ((frames * 1000) / audio_frequency);  // (sample frames * 1000) / frequency == ���ų��ȣ��Ժ���Ϊ��λ
}

Custom_Mix_PlaybackSpeedEffectHandler* Custom_Mix_CreatePlaybackSpeedEffectHandler(const Mix_Chunk* chunk,
	const float* speed, int loop, int self_halt)
{
	Custom_Mix_PlaybackSpeedEffectHandler* handler = (Custom_Mix_PlaybackSpeedEffectHandler*)malloc(sizeof(Custom_Mix_PlaybackSpeedEffectHandler));
	handler->chunk = chunk;
	handler->speed = speed;
	handler->position = 0;
	handler->altered = 0;
	handler->loop = loop;
	handler->duration = Custom_Mix_ComputeChunkLengthMillisec(chunk->alen);
	handler->chunk_size = chunk->alen / format_sample_size(audio_format);
	handler->self_halt = self_halt;
	return handler;
}


// Mix_EffectDone_t �ص���Ч��ʹ�ý���ʱɾ���������ͨ�� userData ���ݵĴ������ 
 void Custom_Mix_PlaybackSpeedEffectDoneCallback(int channel, void *userData)
{
	free(userData);
}
  void Custom_Mix_RegisterPlaybackSpeedEffect(int channel, Mix_Chunk* chunk, float* speed, int loop, int selfHalt)
{
	Mix_EffectFunc_t effect_func_callback;
	// ���ݵ�ǰ��Ƶ��ʽΪ��Ƶ��ע����ʵĲ����ٶ�Ч���������Ч�����Ե�ǰ������һ����������Ч�� 
	switch (audio_format)
	{
	case AUDIO_U8:  effect_func_callback = Custom_Mix_PlaybackSpeedEffectFuncCallbackUint8;  break;
	case AUDIO_S8:  effect_func_callback = Custom_Mix_PlaybackSpeedEffectFuncCallbackSint8;  break;
	case AUDIO_U16: effect_func_callback = Custom_Mix_PlaybackSpeedEffectFuncCallbackUint16; break;
	default:
	case AUDIO_S16: effect_func_callback = Custom_Mix_PlaybackSpeedEffectFuncCallbackSint16; break;
	case AUDIO_S32: effect_func_callback = Custom_Mix_PlaybackSpeedEffectFuncCallbackSint32; break;
	case AUDIO_F32: effect_func_callback = Custom_Mix_PlaybackSpeedEffectFuncCallbackFloat;  break;
	}

	Mix_RegisterEffect(channel, effect_func_callback,
		Custom_Mix_PlaybackSpeedEffectDoneCallback, Custom_Mix_CreatePlaybackSpeedEffectHandler(chunk, speed, loop, selfHalt));
}

   

OG_END
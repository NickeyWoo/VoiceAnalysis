#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sndfile.h>

#include <boost/lexical_cast.hpp>

extern void HexDump(unsigned char* lpBuffer, int32_t iBufferSize);

#define FRAMES_NUM			22050 * 5

int main(int argc, char* argv[])
{
	int iFrequency = atoi(argv[2]);
	int iAmplitude = atoi(argv[3]);
	/*
	if(iAmplitude >= (0xFFFF / 2))
	{
		printf("amplitude must less than PCM_16/2.\n");
		return -1;
	}
	*/
	
	SF_INFO sndinfo;
	sndinfo.frames = FRAMES_NUM;		// 10s
	sndinfo.samplerate = 22050;
	sndinfo.channels = 1;
	sndinfo.format = SF_FORMAT_WAV|SF_FORMAT_PCM_16;
	sndinfo.sections = 1;
	sndinfo.seekable = 1;
	SNDFILE* fdSnd = sf_open(argv[1], SFM_WRITE, &sndinfo);

	short* sample_buffer = new short[FRAMES_NUM];
	memset(sample_buffer, 0, sizeof(short)*FRAMES_NUM);

	int SampleCountPerFreq = 22050 / iFrequency;
	short ilen = (short)iAmplitude / SampleCountPerFreq;
	int halfCountPerFreq = SampleCountPerFreq / 2;
	
	for(int i=0, j=0; i<FRAMES_NUM; ++i, ++j)
	{
		if(j < halfCountPerFreq)
			sample_buffer[i] = -iAmplitude + j*ilen;
		else if(j >= halfCountPerFreq)
		{
			sample_buffer[i] = iAmplitude - j*ilen;
			if(j + 1 > SampleCountPerFreq)
				j = 0;
		}
	}

	sf_count_t retCount2 = sf_writef_short(fdSnd, sample_buffer, FRAMES_NUM);
	printf("write count: %lld\n", retCount2);

	sf_close(fdSnd);

	return 0;
}


void HexDump(unsigned char* lpBuffer, int32_t iBufferSize)
{
	char strBuffer[17];
	printf("%08x  ", 0);
	int iLine = 0;
	for(int i=0; i<iBufferSize; ++i)
	{
		memset(strBuffer, 0, 17);
		printf("%02x ", lpBuffer[i]);
		strBuffer[(i % 16)] = lpBuffer[i];
		if(i != 0 && ((i + 1) % 16) == 0)
		{
			++iLine;
			printf("| %s\n%08x  ", strBuffer, iLine);
		}
	}
}



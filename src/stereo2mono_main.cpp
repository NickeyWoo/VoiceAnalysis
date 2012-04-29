#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sndfile.h>

extern void HexDump(unsigned char* lpBuffer, int32_t iBufferSize);

int main(int argc, char* argv[])
{
	SF_INFO sndInfo;
	SNDFILE* fdSnd = sf_open(argv[1], SFM_READ, &sndInfo);
	if(fdSnd == NULL)
	{
		printf("sf_open fail, %s.\n", sf_strerror(fdSnd));
		return -1;
	}

	printf("Sound File: %s\n", argv[1]);
	printf("---------------------------------------------------------\n");
	printf("frames: %llu\n", sndInfo.frames);
	printf("samplerate: %d\n", sndInfo.samplerate);
	printf("channels: %d\n", sndInfo.channels);
	printf("format: 0x%08X\n", sndInfo.format);
	printf("sections: %d\n", sndInfo.sections);
	printf("seekable: %d\n", sndInfo.seekable);

	short* sample_buff = new short[sndInfo.frames * 2];
	memset(sample_buff, 0, sndInfo.frames * 2);

	sf_count_t retCount = sf_readf_short(fdSnd, sample_buff, sndInfo.frames);

	printf("---------------------------------------------------------\n");
	printf("sf_count_t: %lld\n", retCount);
	printf("---------------------------------------------------------\n");

	sf_close(fdSnd);

	SF_INFO sndInfo2;
	sndInfo2.frames = sndInfo.frames;
	sndInfo2.samplerate = 44100;
	sndInfo2.channels = 1;
	sndInfo2.format = SF_FORMAT_WAV|SF_FORMAT_PCM_16;
	sndInfo2.sections = 1;
	sndInfo2.seekable = 1;
	SNDFILE* fdSnd2 = sf_open(argv[2], SFM_WRITE, &sndInfo2);

	unsigned short* sample_buffer2 = new unsigned short[sndInfo.frames];
	for(int i=0; i<sndInfo.frames; ++i)
	{
		unsigned short tmp = (sample_buff[i*2] + sample_buff[i*2 +1] )/ 2;
		sample_buffer2[i] = tmp;
	}
	sf_count_t retCount2 = sf_writef_short(fdSnd2, (const short*)sample_buffer2, sndInfo.frames);
	printf("write count: %lld\n", retCount2);

	sf_close(fdSnd2);

	return 0;
}




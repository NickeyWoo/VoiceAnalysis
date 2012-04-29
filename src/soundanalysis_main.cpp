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
	HexDump((unsigned char*)sample_buff, sndInfo.frames*2);

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



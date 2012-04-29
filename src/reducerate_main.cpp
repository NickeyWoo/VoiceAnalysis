#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sndfile.h>

int main(int argc, char* argv[])
{
	int reduce = atoi(argv[1]);
	SF_INFO sfinfo;
	SNDFILE* fdSnd = sf_open(argv[2], SFM_READ, &sfinfo);
	if(fdSnd == NULL)
	{
		printf("sf_open fail, %s.\n", sf_strerror(fdSnd));
		return -1;
	}

	printf("Sound File: %s\n", argv[2]);
	printf("---------------------------------------------------------\n");
	printf("frames: %llu\n", sfinfo.frames);
	printf("samplerate: %d\n", sfinfo.samplerate);
	printf("channels: %d\n", sfinfo.channels);
	printf("format: 0x%08X\n", sfinfo.format);
	printf("sections: %d\n", sfinfo.sections);
	printf("seekable: %d\n", sfinfo.seekable);
	printf("---------------------------------------------------------\n");

	float* lpSampleBuffer = new float[sfinfo.channels * sfinfo.frames];
	sf_count_t retCount = sf_readf_float(fdSnd, lpSampleBuffer, sfinfo.channels * sfinfo.frames);
	printf("sf_readf_float: %lld frames\n", retCount);
	sf_close(fdSnd);
	
	if(sfinfo.channels != 1)
	{
		printf("stereo sound can't reduce, use stereo2mono encode the sound.\n");
		return 0;
	}
	
/*
0         1         2         3         4         5
--------------------------------------------------------------
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B
0         1         2
*/

/*
	int iWriteSize = sfinfo.channels * (sfinfo.frames + (reduce - sfinfo.frames % reduce)) / reduce;
	float* lpWriteSampleBuffer = new float[iWriteSize];
	for(int i=0, j=0; j<iWriteSize && i<sfinfo.channels * sfinfo.frames;++j)
	{
		float tmp = 0;		
		int ic = 0;
		int ichnl = 0;
		while(ichnl < sfinfo.channels)
		{
			while(ic < reduce)
			{
				tmp += lpSampleBuffer[i+ic*sfinfo.channels + ichnl];
				++ic;
			}
			++ichnl;
			lpWriteSampleBuffer[j] = tmp / reduce;
			++j;
		}
		i+=sfinfo.channels*reduce;
	}
*/

	int iWriteSize =  (sfinfo.frames + reduce - sfinfo.frames % reduce) / reduce;
	float* lpWriteSampleBuffer = new float[iWriteSize];
	memset(lpWriteSampleBuffer, 0, iWriteSize);
	for(int i=0, idx=0; i<sfinfo.frames && idx<iWriteSize; ++idx, i+=reduce)
	{
		float tmp = 0;
		for(int j=0; j<reduce; ++j)
		{
			tmp += lpSampleBuffer[j+i];
		}
		lpWriteSampleBuffer[idx] = tmp / reduce;
	}


	SF_INFO sfinfo2;
	sfinfo2.frames = iWriteSize;
	sfinfo2.samplerate = sfinfo.samplerate / reduce;
	sfinfo2.channels = 1;
	sfinfo2.format = SF_FORMAT_WAV|SF_FORMAT_PCM_16;
	sfinfo2.sections = 1;
	sfinfo2.seekable = 1;

	SNDFILE* fdWrite = sf_open(argv[3], SFM_WRITE, &sfinfo2);
	if(fdWrite == NULL)
	{
		printf("sf_open fail, %s\n", sf_strerror(fdWrite));
		return -1;
	}
	sf_count_t retWriteCount = sf_write_float(fdWrite, lpWriteSampleBuffer, iWriteSize);
	printf("sf_writef_float: %lld frames\n", retWriteCount);
	sf_close(fdWrite);
	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sndfile.h>

#include <boost/lexical_cast.hpp>

extern void HexDump(unsigned char* lpBuffer, int32_t iBufferSize);

int main(int argc, char* argv[])
{
	SF_INFO sndInfo2;
	sndInfo2.frames = 441000;
	sndInfo2.samplerate = 44100;
	sndInfo2.channels = 1;
	sndInfo2.format = SF_FORMAT_WAV|SF_FORMAT_PCM_16;
	sndInfo2.sections = 1;
	sndInfo2.seekable = 1;
	SNDFILE* fdSnd2 = sf_open(argv[1], SFM_WRITE, &sndInfo2);

	short tmp = boost::lexical_cast<short, char*>(argv[2]);
	short* sample_buffer2 = new short[441000];
	bool act = true;
	for(int i=0, j=0; i<441000 && j>=-tmp && j<=tmp; ++i)
	{
		sample_buffer2[i] = j;
		if(j==tmp)
			act = false;
		else if(j == -tmp)
			act = true;
			
		if(act) ++j;
		else --j;
	}
	
//	HexDump((unsigned char*)sample_buffer2, 441000*sizeof(short));
//	exit(0);

	sf_count_t retCount2 = sf_writef_short(fdSnd2, (const short*)sample_buffer2, 441000);
	printf("write count: %lld\n", retCount2);

	sf_close(fdSnd2);

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



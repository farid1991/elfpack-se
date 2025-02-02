#include "..\include\Lib_Clara.h"
#include "header.hpp"

#define INDEX(x) x/8
#define SHIFT(x) x%8
#define GETBIT(v,x) ((v[INDEX(x)] << SHIFT(x)) & 0x80)>>7

int synchsafeToNormal(char tagSize[4])
{
	int synchsafe, sizeloc, size, power, x;

	size=sizeloc=0;
	for(synchsafe=31;synchsafe>=0;synchsafe--)
	{
		if(GETBIT(tagSize, synchsafe))
		{
			power=1;
			for(x=0;x<sizeloc;x++) power*=2;
			size+=power;
		}
		if(synchsafe%8) sizeloc++;
	}
	return size;
};

MP3Header::MP3Header()
{
    hasHeader = false;
    intBitRate = 0;
    lngFileSize = 0;
    intFrequency  = 0;
    wstrMode = 0;
    intLength = 0;
    wstrLengthFormatted[0] = 0;
    wstrFrequency[0] = 0;
    wstrBitRate[0] = 0;
    bithdr = 0;
    boolVBitRate = false;
    intVFrames = 0;
//    wstrFileName = 0;
}

MP3Header::~MP3Header()
{
//    if(wstrFileName) mfree(wstrFileName);
}

bool MP3Header::ReadMP3Information(wchar_t * path, wchar_t * fname)
{
    hasHeader = false;
    if(fname == 0) return false;
    if(path == 0) return false;

//    if(wstrFileName) mfree(wstrFileName);

//    wstrFileName = (wchar_t *)malloc(wstrlen(fname)*sizeof(wchar_t));
    // Set the filename not including the path information
//    wstrcpy(wstrFileName,fname);

    FSTAT _fstat;
    int file;
    if(fstat(path, fname, &_fstat) == 0)
    {
        // Set the file size
        lngFileSize = _fstat.fsize;
        if( (file = _fopen(path, fname, FSX_O_RDONLY, FSX_S_IREAD|FSX_S_IWRITE, 0)) >= 0)
        {
            char bytHeader[4];
            char bytVBitRate[12];
            int intPos = 0;
            int Position;

            char BUF[16+1];
            fread(file,BUF,16);
            if (BUF[0]=='I' && BUF[1]=='D' && BUF[2]=='3') // проверяем, есть ли тэги v2.x
            {
              char tagSize[4];
              tagSize[0]=BUF[6];
              tagSize[1]=BUF[7];
              tagSize[2]=BUF[8];
              tagSize[3]=BUF[9];
              intPos = synchsafeToNormal(tagSize);
            }

            // Keep reading 4 bytes from the header until we know for sure that in
            // fact it's an MP3
            do
            {
                Position = intPos;
                lseek(file,Position,0);
                fread(file,bytHeader,4);
                intPos++;
                LoadMP3Header(bytHeader);
            }
            while(!IsValidHeader() && (Position!=lngFileSize));

            // If the current file stream position is equal to the length,
            // that means that we've read the entire file and it's not a valid MP3 file
            if(Position != lngFileSize)
            {
                intPos += 3;

                if(getVersionIndex() == 3)    // MPEG Version 1
                {
                    if(getModeIndex() == 3)    // Single Channel
                    {
                        intPos += 17;
                    }
                    else
                    {
                        intPos += 32;
                    }
                }
                else                        // MPEG Version 2.0 or 2.5
                {
                    if(getModeIndex() == 3)    // Single Channel
                    {
                        intPos += 9;
                    }
                    else
                    {
                        intPos += 17;
                    }
                }

                // Check to see if the MP3 has a variable bitrate
                Position = intPos;
                lseek(file,Position,0);
                fread(file,bytVBitRate,12);
                boolVBitRate = LoadVBRHeader(bytVBitRate);

                // Once the file's read in, then assign the properties of the file to the public variables
                intBitRate = getBitrate();
                intFrequency = getFrequency();
                wstrMode = getMode();
                intLength = getLengthInSeconds();
                getFormattedLength();
                getFormattedFrequency();
                getFormattedBitRate();
                fclose(file);
                hasHeader = true;
                return true;
            }
            fclose(file);
            return false;
        }
        else
        {
          fclose(file);
          return false;
        }
    }
    return false;
}

void MP3Header::LoadMP3Header(char* c)
{
        // this thing is quite interesting, it works like the following
        // c[0] = 00000011
        // c[1] = 00001100
        // c[2] = 00110000
        // c[3] = 11000000
        // the operator << means that we'll move the bits in that direction
        // 00000011 << 24 = 00000011000000000000000000000000
        // 00001100 << 16 =         000011000000000000000000
        // 00110000 << 24 =                 0011000000000000
        // 11000000       =                         11000000
        //                +_________________________________
        //                  00000011000011000011000011000000
        bithdr = (unsigned long)(((unsigned long)(c[0] & 255) << 24) |
                                 ((unsigned long)(c[1] & 255) << 16) |
                                 ((unsigned long)(c[2] & 255) << 8)  |
                                 ((unsigned long)(c[3] & 255)));
}

bool MP3Header::LoadVBRHeader(char* inputheader)
{
        // If it's a variable bitrate MP3, the first 4 bytes will read 'Xing'
        // since they're the ones who added variable bitrate-edness to MP3s
        if(inputheader[0] == 88 && inputheader[1] == 105 &&
            inputheader[2] == 110 && inputheader[3] == 103)
        {
            int flags = (int)(((inputheader[4] & 255) << 24) | ((inputheader[5] & 255) << 16) | ((inputheader[6] & 255) <<  8) | ((inputheader[7] & 255)));
            if((flags & 0x0001) == 1)
            {
                intVFrames = (int)(((inputheader[8] & 255) << 24) | ((inputheader[9] & 255) << 16) | ((inputheader[10] & 255) <<  8) | ((inputheader[11] & 255)));
                return true;
            }
            else
            {
                intVFrames = -1;
                return true;
            }
        }
        return false;
}

bool MP3Header::IsValidHeader()
{
        return (((getFrameSync()      & 2047)==2047) &&
                ((getVersionIndex()   &    3)!=   1) &&
                ((getLayerIndex()     &    3)!=   0) &&
                ((getBitrateIndex()   &   15)!=   0) &&
                ((getBitrateIndex()   &   15)!=  15) &&
                ((getFrequencyIndex() &    3)!=   3) &&
                ((getEmphasisIndex()  &    3)!=   2)    );
}

double MP3Header::getVersion() {
        double table[] = {2.5, 0.0, 2.0, 1.0};
        return table[getVersionIndex()];
}

int MP3Header::getBitrate()
{
        // If the file has a variable bitrate, then we return an integer average bitrate,
        // otherwise, we use a lookup table to return the bitrate
        if(boolVBitRate)
        {
            double medFrameSize = (double)lngFileSize / (double)getNumberOfFrames();
            return (int)((medFrameSize * (double)getFrequency()) / (1000.0 * ((getLayerIndex()==3) ? 12.0 : 144.0)));
        }
        else
        {
            int table[2][3][16] =  {
                                { // MPEG 2 & 2.5
                                    {0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0}, // Layer III
                                    {0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0}, // Layer II
                                    {0, 32, 48, 56, 64, 80, 96,112,128,144,160,176,192,224,256,0}  // Layer I
                                },
                                { // MPEG 1
                                    {0, 32, 40, 48, 56, 64, 80, 96,112,128,160,192,224,256,320,0}, // Layer III
                                    {0, 32, 48, 56, 64, 80, 96,112,128,160,192,224,256,320,384,0}, // Layer II
                                    {0, 32, 64, 96,128,160,192,224,256,288,320,352,384,416,448,0}  // Layer I
                                }
                               };

            return table[getVersionIndex() & 1][getLayerIndex()-1][getBitrateIndex()];
        }
}

int MP3Header::getFrequency()
{
        int table[4][3] = {
                            {32000, 16000,  8000}, // MPEG 2.5
                            {    0,     0,     0}, // reserved
                            {22050, 24000, 16000}, // MPEG 2
                            {44100, 48000, 32000}  // MPEG 1
                        };

        return table[getVersionIndex()][getFrequencyIndex()];
}

wchar_t* MP3Header::getMode()
{
        switch(getModeIndex())
        {
            default:
                return L"Stereo";
            case 1:
                return L"Joint Stereo";
            case 2:
                return L"Dual Channel";
            case 3:
                return L"Single Channel";
        }
}

int MP3Header::getLengthInSeconds()
{
        // "intKilBitFileSize" made by dividing by 1000 in order to match the "Kilobits/second"
        int intKiloBitFileSize = (int)((8 * lngFileSize) / 1000);
        return (int)(intKiloBitFileSize/getBitrate());
}

wchar_t* MP3Header::getFormattedLength()
{
        // Complete number of seconds
        int s  = getLengthInSeconds();

        // Seconds to display
        int ss = s%60;

        // Complete number of minutes
        int m  = (s-ss)/60;

        // Minutes to display
        int mm = m%60;

        // Complete number of hours
        int h = (m-mm)/60;

        // Make "hh:mm:ss"
        snwprintf(wstrLengthFormatted,MAXELEMS(wstrLengthFormatted),L"%02d:%02d%02d",h,mm,ss);
        return wstrLengthFormatted;
}

int MP3Header::getNumberOfFrames()
{
        // Again, the number of MPEG frames is dependant on whether it's a variable bitrate MP3 or not
        if (!boolVBitRate)
        {
            double medFrameSize = (double)(((getLayerIndex()==3) ? 12 : 144) *((1000.0 * (float)getBitrate())/(float)getFrequency()));
            return (int)(lngFileSize/medFrameSize);
        }
        else
            return intVFrames;
}

wchar_t* MP3Header::getFormattedFrequency()
{
        #ifdef ENG
          snwprintf(wstrFrequency,MAXELEMS(wstrFrequency),L"%d Hz",getFrequency());
        #else
          snwprintf(wstrFrequency,MAXELEMS(wstrFrequency),L"%d Гц",getFrequency());
        #endif
        return wstrFrequency;
}

wchar_t* MP3Header::getFormattedBitRate()
{
  if(boolVBitRate)
  {
    #ifdef ENG
      snwprintf(wstrBitRate,MAXELEMS(wstrBitRate),L"%d kbps (VBR)",getBitrate());
    #else
      snwprintf(wstrBitRate,MAXELEMS(wstrBitRate),L"%d кб/с (VBR)",getBitrate());
    #endif
  }
  else
  {
    #ifdef ENG
      snwprintf(wstrBitRate,MAXELEMS(wstrBitRate),L"%d kbps",getBitrate());
    #else
      snwprintf(wstrBitRate,MAXELEMS(wstrBitRate),L"%d кб/с",getBitrate());
    #endif
  }
        return wstrBitRate;
}



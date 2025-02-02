#ifndef HEADER_HPP_INCLUDED
#define HEADER_HPP_INCLUDED

///////////////////////////////////////////////////////////
// { class MP3Header
class MP3Header {
public:
    MP3Header();  // class constructor
    ~MP3Header(); // class destructor

    // Public variables for storing the information about the MP3
    bool hasHeader;
    int intBitRate;
//    wchar_t *wstrFileName;
    long lngFileSize;
    int intFrequency;
    wchar_t *wstrMode;
    int intLength;
    wchar_t wstrLengthFormatted[16];
    wchar_t wstrFrequency[16];
    wchar_t wstrBitRate[32];

    bool ReadMP3Information(wchar_t * path, wchar_t * fname);

private:
    // Private variables used in the process of reading in the MP3 files
    unsigned long bithdr;
    bool boolVBitRate;
    int intVFrames;


    void LoadMP3Header(char* c);
    bool LoadVBRHeader(char* inputheader);
    bool IsValidHeader();
    int getFrameSync()     { return (int)((bithdr>>21) & 2047); }
    int getVersionIndex()  { return (int)((bithdr>>19) & 3); }
    int getLayerIndex()    { return (int)((bithdr>>17) & 3); }
    int getProtectionBit() { return (int)((bithdr>>16) & 1); }
    int getBitrateIndex()  { return (int)((bithdr>>12) & 15); }
    int getFrequencyIndex(){ return (int)((bithdr>>10) & 3); }
    int getPaddingBit()    { return (int)((bithdr>>9) & 1); }
    int getPrivateBit()    { return (int)((bithdr>>8) & 1); }
    int getModeIndex()     { return (int)((bithdr>>6) & 3); }
    int getModeExtIndex()  { return (int)((bithdr>>4) & 3); }
    int getCoprightBit()   { return (int)((bithdr>>3) & 1); }
    int getOrginalBit()    { return (int)((bithdr>>2) & 1); }
    int getEmphasisIndex() { return (int)(bithdr & 3); }
    double getVersion();
    int getLayer() { return (int)(4 - getLayerIndex()); }
    int getBitrate();
    int getFrequency();
    wchar_t* getMode();
    int getLengthInSeconds();
    wchar_t* getFormattedLength();
    int getNumberOfFrames();
    wchar_t* getFormattedFrequency();
    wchar_t* getFormattedBitRate();
};
// } class MP3Header
///////////////////////////////////////////////////////////

#endif // HEADER_HPP_INCLUDED




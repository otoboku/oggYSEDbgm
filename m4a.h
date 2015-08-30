#pragma once
#include "neaacdec.h"
#include "mp4ff.h"
#include <limits.h>
#include <stdlib.h>

uint32_t mkps = 0;
// M4a
static const long g_aacFreq[] = {
	96000,88200,64000,48000,44100,32000,
	24000,22050,16000,12000,11025, 8000,
	-1,   -1,   -1,   -1
};

//ADTS_HEADER�\����
typedef struct tagADTS_HEADER {
	int		iProfile;	//�����ƈ��k�� 0:main,01:LC,2:SSR,3:(�\��)
	DWORD	sampleRate;	//���g��
	WORD	nChannel;	//�`�����l����
	DWORD	nFrameSize; //AAC�t���[����(�o�C�g�A�w�b�_���݁j
	DWORD	nFullness;	//ADTS�o�b�t�@�c�� 0x7FF VBR
}ADTS_HEADER;


typedef struct {
	int version;
	int channels;
	int sampling_rate;
	int bitrate;
	int length;
	int object_type;
	int headertype;
} faadAACInfo;

//faad �� common/faad/aacinfo.c ������
//get_AAC_format:   ��1������ char* ���� FILE* �ɕύX
//read_ADIF_header: ��1������ FILE_STREAM* ���� FILE* �ɕύX
//read_ADTS_header: ��1������ FILE_STREAM* ���� FILE* �ɕύX
static int get_AAC_format(FILE *file, faadAACInfo *info,
	unsigned long **seek_table, int *seek_table_len,
	int no_seek_table);

static int read_ADIF_header(FILE *file, faadAACInfo *info);
static int read_ADTS_header(FILE *file, faadAACInfo *info,
	unsigned long **seek_table, int *seek_table_len,
	int tagsize, int no_seek_table);
static int StringComp(char const *str1, char const *str2, unsigned long len) { return memcmp(str1, str2, len); }

#define ADIF_MAX_SIZE 30 /* Should be enough */
#define ADTS_MAX_SIZE 10 /* Should be enough */

static int sample_rates[] = { 96000,88200,64000,48000,44100,32000,24000,22050,16000,12000,11025,8000 };

int read_ADIF_header(FILE *file, faadAACInfo *info)
{
	int bitstream;
	unsigned char buffer[ADIF_MAX_SIZE];
	int skip_size = 0;
	int sf_idx;

	/* Get ADIF header data */
	info->headertype = 1;

	if (fread(buffer, 1, ADIF_MAX_SIZE, file) < ADIF_MAX_SIZE)
		return -1;

	/* copyright string */
	if (buffer[0] & 0x80)
		skip_size += 9; /* skip 9 bytes */

	bitstream = buffer[0 + skip_size] & 0x10;
	info->bitrate = ((unsigned int)(buffer[0 + skip_size] & 0x0F) << 19) |
		((unsigned int)buffer[1 + skip_size] << 11) |
		((unsigned int)buffer[2 + skip_size] << 3) |
		((unsigned int)buffer[3 + skip_size] & 0xE0);

	if (bitstream == 0)
	{
		info->object_type = ((buffer[6 + skip_size] & 0x01) << 1) | ((buffer[7 + skip_size] & 0x80) >> 7);
		sf_idx = (buffer[7 + skip_size] & 0x78) >> 3;
	}
	else {
		info->object_type = (buffer[4 + skip_size] & 0x18) >> 3;
		sf_idx = ((buffer[4 + skip_size] & 0x07) << 1) | ((buffer[5 + skip_size] & 0x80) >> 7);
	}
	info->sampling_rate = sample_rates[sf_idx];

	return 0;
}

int read_ADTS_header(FILE *file, faadAACInfo *info,
	unsigned long **seek_table, int *seek_table_len,
	int tagsize, int no_seek_table)
{
	/* Get ADTS header data */
	unsigned char buffer[ADTS_MAX_SIZE];
	int frames, framesinsec = 0, t_framelength = 0, frame_length, sr_idx, ID;
	int second = 0, pos;
	float frames_per_sec = 0;
	unsigned long bytes;
	unsigned long *tmp_seek_table = NULL;

	info->headertype = 2;

	/* Read all frames to ensure correct time and bitrate */
	for (frames = 0; /* */; frames++, framesinsec++)
	{
		pos = ftell(file);

		/* 12 bit SYNCWORD */
		bytes = fread(buffer, 1, ADTS_MAX_SIZE, file);

		if (bytes != ADTS_MAX_SIZE)
		{
			/* Bail out if no syncword found */
			break;
		}

		/* check syncword */
		if (!((buffer[0] == 0xFF) && ((buffer[1] & 0xF6) == 0xF0)))
			break;

		if (!frames)
		{
			/* fixed ADTS header is the same for every frame, so we read it only once */
			/* Syncword found, proceed to read in the fixed ADTS header */
			ID = buffer[1] & 0x08;
			info->object_type = (buffer[2] & 0xC0) >> 6;
			sr_idx = (buffer[2] & 0x3C) >> 2;
			info->channels = ((buffer[2] & 0x01) << 2) | ((buffer[3] & 0xC0) >> 6);

			frames_per_sec = sample_rates[sr_idx] / 1024.f;
		}

		/* ...and the variable ADTS header */
		if (ID == 0) {
			info->version = 4;
		}
		else { /* MPEG-2 */
			info->version = 2;
		}
		frame_length = ((((unsigned int)buffer[3] & 0x3)) << 11)
			| (((unsigned int)buffer[4]) << 3) | (buffer[5] >> 5);

		t_framelength += frame_length;

		//if(!file->http)
		if (1)
		{
			if (framesinsec == 43)
				framesinsec = 0;

			if (framesinsec == 0 && seek_table_len)
			{
				tmp_seek_table = (unsigned long *)realloc(tmp_seek_table, (second + 1) * sizeof(unsigned long));
				tmp_seek_table[second] = pos;
			}
			if (framesinsec == 0)
				second++;
		}

		/* NOTE: While simply skipping ahead by reading may seem to be more work than seeking,
		it is actually much faster, and keeps compatibility with streaming */
		fseek(file, frame_length - ADTS_MAX_SIZE, SEEK_CUR);
		/*for(i=0; i < frame_length - ADTS_MAX_SIZE; i++)
		{
		if(read_byte_filestream(file) < 0)
		break;
		}*/
	}

	if (seek_table_len)
	{
		*seek_table_len = second;
		*seek_table = tmp_seek_table;
	}

	info->sampling_rate = sample_rates[sr_idx];
	info->bitrate = (int)(((t_framelength / frames) * (info->sampling_rate / 1024.0)) + 0.5) * 8;

#if 0
	if (file->http)
	{
		/* Since we only use 5 seconds of aac data to get a rough bitrate, we must use a different
		method of calculating the overall length */
		if (filelength_filestream(file))
		{
			info->length = (int)((filelength_filestream(file) / (((info->bitrate * 8) / 1024) * 16)) * 1000);
		}
		else
		{
			/* Since the server didnt tell us how long the file is,
			we have no way of determining length */
			info->length = 0;
		}
	}
	else
#endif
	{
		//info->length = (int)((float)(frames/frames_per_sec))*1000;
		info->length = MulDiv(frames, 1000, (int)frames_per_sec);
	}

	return 0;
}

int get_AAC_format(FILE *file, faadAACInfo *info,
	unsigned long **seek_table, int *seek_table_len,
	int no_seek_table)
{
	unsigned long tagsize;
	//    FILE *file;
	char buffer[10];
	unsigned long file_len;
	unsigned char adxx_id[5];
	//unsigned long tmp;

	memset(info, 0, sizeof(faadAACInfo));

	//file = open_filestream(filename);

	//file_len = filelength_filestream(file);
	file_len = fseek(file, 0, SEEK_END);
	fseek(file, 0, SEEK_SET);
	/* Skip the tag, if it's there */
	//tmp = read_buffer_filestream(file, buffer, 10);
	if (fread(buffer, 1, 10, file) != 10) {
		return -1;
	}
	if (StringComp(buffer, "ID3", 3) == 0)
	{
		//unsigned int i;

		/* high bit is not used */
		tagsize = (buffer[6] << 21) | (buffer[7] << 14) |
			(buffer[8] << 7) | (buffer[9] << 0);

		/*for(i=0; i < tagsize; i++)
		if(read_byte_filestream(file) < 0)
		return -1;
		*/
		fseek(file, tagsize, SEEK_CUR);
		tagsize += 10;
	}
	else
	{
		tagsize = 0;
		/* Simple hack to reset to the beginning */
		//		file->buffer_offset = 0;
		//		file->file_offset = 0;
		fseek(file, 0, SEEK_SET);
	}

	if (file_len)
		file_len -= tagsize;

	//    tmp = read_buffer_filestream(file, adxx_id, 2);
	fread(adxx_id, 1, 2, file);

	adxx_id[5 - 1] = 0;
	info->length = 0;

	// Determine the header type of the file, check the first two bytes
	if (StringComp((const char*)adxx_id, "AD", 2) == 0)
	{
		// We think its an ADIF header, but check the rest just to make sure
		//tmp = read_buffer_filestream(file, adxx_id + 2, 2);
		fread(adxx_id + 2, 1, 2, file);
		if (StringComp((const char*)adxx_id, "ADIF", 4) == 0)
		{
			read_ADIF_header(file, info);
		}
	}
	else
	{
		/* No ADIF, check for ADTS header */
		if ((adxx_id[0] == 0xFF) && ((adxx_id[1] & 0xF6) == 0xF0))
		{
			/* ADTS  header located */
			/* Since this routine must work for streams, we can't use the seek function to go backwards, thus
			we have to use a quick hack as seen below to go back where we need to. */

			/*if(file->buffer_offset >= 2)
			{
			// simple seeking hack, though not really safe, the probability of it causing a problem is low.
			file->buffer_offset -= 2;
			file->file_offset -= 2;
			}
			*/
			fseek(file, -2, SEEK_CUR);
			read_ADTS_header(file, info, seek_table, seek_table_len, tagsize,
				no_seek_table);
		}
		else
		{
			/* Unknown/headerless AAC file, assume format: */
			info->version = 2;
			info->bitrate = 128000;
			info->sampling_rate = 44100;
			info->channels = 2;
			info->headertype = 0;
			info->object_type = 1;
		}
	}

	//close_filestream(file);
	//fclose(file);//�N���[�Y���Ă͂����Ȃ�
	return 0;
}

static int id3v2_tag(unsigned char *buffer)
{
	if (memcmp(buffer, "ID3", 3) == 0) {
		unsigned long tagsize;
		tagsize = (buffer[6] << 21) | (buffer[7] << 14) |
			(buffer[8] << 7) | (buffer[9] << 0);
		tagsize += 10;
		return tagsize;
	}
	else {
		return 0;
	}
}
class IKbAacDecoder
{
public:
	virtual BOOL  __fastcall Open(const _TCHAR *cszFileName, SOUNDINFO *pInfo) = 0;
	virtual void  __fastcall Close(void) = 0;
	virtual int   __fastcall SetPosition(DWORD dwPos) = 0;
	virtual DWORD __fastcall Render(BYTE *Buffer, DWORD dwSize) = 0;
	virtual ~IKbAacDecoder(void) {};
};

class KbAacDecoder : public IKbAacDecoder
{
private:
	void  *m_hDecoder;
	FILE  *m_pFile;
	unsigned long *m_seek_table;
	int   m_seek_table_length;
	BYTE  m_buffer[FAAD_MIN_STREAMSIZE * 2];
	int   m_buffercount;
	BOOL  m_bDecodeDone;
	DWORD m_dwRemain;
	BYTE *m_pSample;
public:
	BOOL __fastcall Open(const _TCHAR *cszFileName, SOUNDINFO *pInfo);
	void __fastcall Close(void);
	int __fastcall SetPosition(DWORD dwPos);
	DWORD __fastcall Render(BYTE *Buffer, DWORD dwSize);
	KbAacDecoder(void);
	~KbAacDecoder(void);
};

KbAacDecoder::KbAacDecoder(void)
{
	m_hDecoder = NULL;
	m_pFile = NULL;
	m_seek_table = NULL;
	m_seek_table_length = 0;
	m_buffercount = 0;
	m_bDecodeDone = TRUE;
	m_pSample = NULL;
	m_dwRemain = 0;
}

KbAacDecoder::~KbAacDecoder(void)
{
	Close();
}

BOOL __fastcall KbAacDecoder::Open(const _TCHAR *cszFileName, SOUNDINFO *pInfo)
{
	ZeroMemory(pInfo, sizeof(SOUNDINFO));
	int buffercount;
	int tagsize;
	DWORD samplerate = 0;
	BYTE  channels = 0;
	DWORD dwRead;
	unsigned long *seek_table = NULL;
	int seek_table_length = 0;

#if UNICODE	
	m_pFile = _wfopen(cszFileName, _T("rb"));
#else
	m_pFile = fopen(cszFileName, _T("rb"));
#endif
	if (!m_pFile) {
		return FALSE;
	}
	dwRead = fread(m_buffer, 1, FAAD_MIN_STREAMSIZE * 2, m_pFile);
	if (dwRead != FAAD_MIN_STREAMSIZE * 2) {
		Close();
		return FALSE;
	}
	//id3v2 �^�O���΂�
	tagsize = id3v2_tag(m_buffer);
	if (tagsize) {
		fseek(m_pFile, tagsize, SEEK_SET);
		if (ftell(m_pFile) != tagsize) {
			Close();
			return FALSE;
		}
		dwRead = fread(m_buffer, 1, FAAD_MIN_STREAMSIZE * 2, m_pFile);
		if (dwRead != FAAD_MIN_STREAMSIZE * 2) {
			Close();
			return FALSE;
		}
	}
	m_hDecoder = NeAACDecOpen();
	if ((buffercount = NeAACDecInit(m_hDecoder, m_buffer, sizeof(m_buffer), &samplerate, &channels)) < 0) {
		Close();
		return FALSE;
	}
	if (buffercount > 0) {
		int k;
		for (k = 0; k < (FAAD_MIN_STREAMSIZE * 2 - buffercount); k++) {
			m_buffer[k] = m_buffer[k + buffercount];
		}
		if (fread(m_buffer + (FAAD_MIN_STREAMSIZE * 2) - buffercount, 1, buffercount, m_pFile) != buffercount) {
			Close();
			return FALSE;
		}
		buffercount = 0;
	}
	faadAACInfo file_info = { 0 };

	DWORD dwCurPos = ftell(m_pFile);//�t�@�C���|�C���^��ۑ�
	if (get_AAC_format(m_pFile, &file_info, &seek_table, &seek_table_length, 0) < 0) {
		Close();
		return FALSE;
	}
	fseek(m_pFile, dwCurPos, SEEK_SET);//���̈ʒu�ɖ߂�
								
	pInfo->dwSamplesPerSec = samplerate;
	pInfo->dwChannels = channels;
	pInfo->dwBitsPerSample = 16;
	pInfo->dwSeekable = file_info.headertype == 2;
	pInfo->dwUnitRender = 0;//�����ł� OK
	pInfo->dwLength = file_info.length;
	mkps = 0;
	if (pInfo->dwLength == 0 && file_info.bitrate != 0) {
		//�Ȃ̒������킩��Ȃ��ꍇ�̓r�b�g���[�g�ƃt�@�C���T�C�Y������
		//�ߎ��l���v�Z����
		dwCurPos = ftell(m_pFile);
		fseek(m_pFile, 0, SEEK_END);
		DWORD dwFileSize = ftell(m_pFile);
		fseek(m_pFile, dwCurPos, SEEK_SET);
		pInfo->dwLength = (DWORD)(1000.0 * ((double)dwFileSize) / (file_info.bitrate / 8));
	}
	pInfo->dwReserved1 = 0;
	pInfo->dwReserved2 = 0;
	m_buffercount = buffercount;
	m_bDecodeDone = FALSE;
	m_seek_table = seek_table;
	m_seek_table_length = seek_table_length;
	return TRUE;
}
void __fastcall KbAacDecoder::Close(void)
{
	if (m_hDecoder) {
		NeAACDecClose(m_hDecoder);
		m_hDecoder = NULL;
	}
	if (m_seek_table) {
		free(m_seek_table);
		m_seek_table = NULL;
	}
	if (m_pFile) {
		fclose(m_pFile);
	}
	m_seek_table_length = 0;
	m_buffercount = 0;
	m_pSample = NULL;
	m_dwRemain = 0;
}

int __fastcall KbAacDecoder::SetPosition(DWORD dwPos)
{
	if (dwPos == 0 && !m_pSample) {//���ɐ擪�ʒu�ɂ���
		return 0;
	}
	m_bDecodeDone = TRUE;
	if (!m_hDecoder) {
		return 0;
	}
	DWORD dwOffsetSec = 0xFFFFFFFF;
	NeAACDecClose(m_hDecoder);
	m_hDecoder = NeAACDecOpen();

	if (m_seek_table && m_seek_table_length > 0) {//�V�[�N�\
		dwOffsetSec = dwPos / 1000;
		if (dwOffsetSec >= m_seek_table_length) {
			dwOffsetSec = m_seek_table_length - 1;
		}
		fseek(m_pFile, m_seek_table[dwOffsetSec], SEEK_SET);
		if (ftell(m_pFile) != m_seek_table[dwOffsetSec]) {
			return 0;
		}
	}
	else {//�V�[�N�s�B�擪�ʒu�ɖ߂�
		fseek(m_pFile, 0, SEEK_SET);
		dwPos = 0;
	}
	m_buffercount = 0;
	DWORD dwRead = fread(m_buffer, 1, FAAD_MIN_STREAMSIZE * 2, m_pFile);
	if (dwRead != FAAD_MIN_STREAMSIZE * 2) {
		return 0;
	}
	//id3v2 �^�O���΂�
	int tagsize = id3v2_tag(m_buffer);
	if (tagsize) {
		fseek(m_pFile, tagsize, SEEK_SET);
		if (ftell(m_pFile) != tagsize) {
			Close();
			return FALSE;
		}
		dwRead = fread(m_buffer, 1, FAAD_MIN_STREAMSIZE * 2, m_pFile);
		if (dwRead != FAAD_MIN_STREAMSIZE * 2) {
			return 0;
		}
	}
	DWORD samplerate = 0;
	BYTE  channels = 0;
	int buffercount;
	if ((buffercount = NeAACDecInit(m_hDecoder, m_buffer, FAAD_MIN_STREAMSIZE * 2, &samplerate, &channels)) < 0) {
		return 0;
	}
	if (buffercount > 0) {
		int k;
		for (k = 0; k < (FAAD_MIN_STREAMSIZE * 2 - buffercount); k++) {
			m_buffer[k] = m_buffer[k + buffercount];
		}
		if (fread(m_buffer + (FAAD_MIN_STREAMSIZE * 2) - buffercount, 1, buffercount, m_pFile) != buffercount) {
			return 0;
		}
	}
	m_buffercount = 0;
	m_bDecodeDone = FALSE;
	m_pSample = NULL;
	m_dwRemain = 0;
	if (dwOffsetSec != 0xFFFFFFFF) {
		//����Ȃ����͍ő�łP�b������ǂ�
		DWORD dwDeltaMS = dwPos - dwOffsetSec * 1000;
		if (dwDeltaMS > 1000) {
			dwDeltaMS = 1000;//seektable ������Ȃ���s����Ȃ��͂�
		}
		DWORD dwDeltaSample = MulDiv(dwDeltaMS, samplerate, 1000);
		DWORD dwDeltaBytes = dwDeltaSample * 2 * channels;
		BYTE *pBuffer = (BYTE*)malloc(dwDeltaBytes);
		dwDeltaBytes = Render(pBuffer, dwDeltaBytes);
		dwDeltaMS = MulDiv(dwDeltaBytes / 2 / channels, 1000, samplerate);
		dwPos = dwOffsetSec * 1000 + dwDeltaMS;
		free(pBuffer);
	}
	return dwPos;
}
DWORD __fastcall KbAacDecoder::Render(BYTE *Buffer, DWORD dwSize)
{
	if (m_bDecodeDone) {
		return 0;
	}
	DWORD dwRet = 0;
	DWORD dwCopySize = m_dwRemain;
	if (dwCopySize) {
		if (dwCopySize > dwSize) {
			m_dwRemain -= dwSize;
			dwCopySize = dwSize;
		}
		else {
			m_dwRemain -= dwCopySize;
		}
		memcpy(Buffer, m_pSample, dwCopySize);
		m_pSample += dwCopySize;
		dwRet += dwCopySize;
	}
	while (dwRet < dwSize) {
		NeAACDecFrameInfo frameInfo;
		if (m_buffercount > 0) {
			int k;
			for (k = 0; k < (FAAD_MIN_STREAMSIZE * 2 - m_buffercount); k++) {
				m_buffer[k] = m_buffer[k + m_buffercount];
			}
			DWORD dwRead = fread(m_buffer + (FAAD_MIN_STREAMSIZE * 2) - m_buffercount, 1, m_buffercount, m_pFile);
			if (dwRead != m_buffercount) {
				m_bDecodeDone = TRUE;
				return dwRet;
			}
			m_buffercount = 0;
		}
		m_pSample = (BYTE*)NeAACDecDecode(m_hDecoder, &frameInfo, m_buffer, sizeof(m_buffer));
		mkps = (int)((float)(frameInfo.bytesconsumed * 8) /
			(frameInfo.samples /
				frameInfo.num_front_channels)
			* frameInfo.samplerate) / 1000;
		if (frameInfo.error || !m_pSample) {
			m_bDecodeDone = TRUE;
			return dwRet;
		}
		m_buffercount += frameInfo.bytesconsumed;
		m_dwRemain = dwCopySize = frameInfo.samples * 2;
		if (dwRet + dwCopySize > dwSize) {
			dwCopySize = dwSize - dwRet;
		}
		memcpy(Buffer + dwRet, m_pSample, dwCopySize);
		m_pSample += dwCopySize;
		m_dwRemain -= dwCopySize;
		dwRet += dwCopySize;
	}
	return dwRet;
}


class KbMp4AacDecoder : public IKbAacDecoder
{
private:
	NeAACDecHandle   m_hDecoder;
	mp4ff_callback_t m_callback;
	mp4ff_t *m_pMp4ff;
	BYTE    *m_pSample;
	DWORD    m_dwLastSample;
	DWORD    m_dwCurrentSample;
	DWORD    m_dwRemain;
	DWORD    m_dwSampleRate;
	DWORD    m_dwChannels;
	int      m_nNumSampleId;
	int      m_nCurrentSampleId;
	int      m_nTrack;
public:
	BOOL  __fastcall Open(const _TCHAR *cszFileName, SOUNDINFO *pInfo);
	void  __fastcall Close(void);
	int   __fastcall SetPosition(DWORD dwPos);
	DWORD __fastcall Render(BYTE *Buffer, DWORD dwSize);
	KbMp4AacDecoder(void);
	~KbMp4AacDecoder(void);
};

static uint32_t read_callback(void *user_data, void *buffer, uint32_t length)
{
	return fread(buffer, 1, length, (FILE*)user_data);
}
static uint32_t seek_callback(void *user_data, uint64_t position)
{//�߂�l�̈Ӗ����s��(�g���ĂȂ��H)
	return fseek((FILE*)user_data, position, SEEK_SET);
}
//faad �� aacDECdrop/decode.c ������p
int GetAACTrack(mp4ff_t *infile)
{
	/* find AAC track */
	int i, rc;
	int numTracks = mp4ff_total_tracks(infile);

	for (i = 0; i < numTracks; i++)
	{
		unsigned char *buff = NULL;
		unsigned int buff_size = 0;
		mp4AudioSpecificConfig mp4ASC;

		mp4ff_get_decoder_config(infile, i, &buff, &buff_size);

		if (buff)
		{
			rc = NeAACDecAudioSpecificConfig(buff, buff_size, &mp4ASC);
			free(buff);

			if (rc < 0)
				continue;
			return i;
		}
	}

	/* can't decode this */
	return -1;
}
KbMp4AacDecoder::KbMp4AacDecoder(void)
{
	m_hDecoder = NULL;
	m_callback.read = read_callback;
	m_callback.seek = seek_callback;
	m_callback.truncate = NULL;
	m_callback.write = NULL;
	m_callback.user_data = NULL;
	m_pMp4ff = NULL;
	m_pSample = NULL;
	m_dwLastSample = 0;
	m_dwCurrentSample = 0;
	m_dwRemain = 0;
	m_dwSampleRate = 0;
	m_dwChannels = 0;
	m_nNumSampleId = 0;
	m_nCurrentSampleId = 0;
	m_nTrack = 0;
}

KbMp4AacDecoder::~KbMp4AacDecoder(void)
{
	Close();
}


BOOL __fastcall KbMp4AacDecoder::Open(const _TCHAR *cszFileName, SOUNDINFO *pInfo)
{
	ZeroMemory(pInfo, sizeof(SOUNDINFO));
	FILE *fp;
#if UNICODE	
	fp = _wfopen(cszFileName, _T("rb"));
#else
	fp = fopen(cszFileName, _T("rb"));
#endif	
	if (!fp) {
		return FALSE;
	}

	int track;
	unsigned long samplerate;
	unsigned char channels;
	mp4ff_t *infile;
	NeAACDecHandle hDecoder;

	unsigned char *buffer;
	unsigned int buffer_size;
	m_callback.user_data = fp;
	infile = mp4ff_open_read(&m_callback);
	if (!infile) {
		fclose(fp);
		m_callback.user_data = NULL;
		return FALSE;
	}
	if ((track = GetAACTrack(infile)) < 0) {
		mp4ff_close(infile);
		fclose(fp);
		m_callback.user_data = NULL;
		return FALSE;
	}

	buffer = NULL;
	buffer_size = 0;
	mp4ff_get_decoder_config(infile, track, &buffer, &buffer_size);

	hDecoder = NeAACDecOpen();

	if (NeAACDecInit2(hDecoder, buffer, buffer_size, &samplerate, &channels) < 0) {
		// If some error initializing occured, skip the file
		NeAACDecClose(hDecoder);
		mp4ff_close(infile);
		fclose(fp);
		m_callback.user_data = NULL;
		if (buffer) {
			free(buffer);
		}
		return FALSE;
	}
	if (buffer) {
		free(buffer);
	}
	m_nNumSampleId = mp4ff_num_samples(infile, track);
	m_dwLastSample = mp4ff_get_track_duration(infile, track);
	pInfo->dwSamplesPerSec = m_dwSampleRate = samplerate;
	pInfo->dwChannels = m_dwChannels = channels;
	pInfo->dwBitsPerSample = 16;
	pInfo->dwLength = MulDiv(m_dwLastSample, 1000, samplerate);
	pInfo->dwSeekable = 1;
	unsigned long bitrate = mp4ff_get_avg_bitrate(infile, track) / 1000;
	mkps = bitrate;
	m_pMp4ff = infile;
	m_hDecoder = hDecoder;
	m_nTrack = track;
	return TRUE;
}
void __fastcall KbMp4AacDecoder::Close(void)
{
	if (m_hDecoder) {
		NeAACDecClose(m_hDecoder);
		m_hDecoder = NULL;
	}
	if (m_pMp4ff) {
		mp4ff_close(m_pMp4ff);
		m_pMp4ff = NULL;
	}
	if (m_callback.user_data) {
		FILE *fp = (FILE*)m_callback.user_data;
		fclose(fp);
		m_callback.user_data = NULL;
	}
	m_pSample = NULL;
	m_dwLastSample = 0;
	m_dwCurrentSample = 0;
	m_dwRemain = 0;
	m_dwSampleRate = 0;
	m_dwChannels = 0;
	m_nNumSampleId = 0;
	m_nCurrentSampleId = 0;
	m_nTrack = 0;
}
//////////////////////////////////////////////////////////////////////////
int __fastcall KbMp4AacDecoder::SetPosition(DWORD dwPos)
{
	if (dwPos == 0 && m_nCurrentSampleId == 0) {//���ɐ擪�ʒu�ɂ���
												//�Ȃ��Ă����Ȃ����A���ʂȂ̂�
		return 0;
	}
	int64_t i64PosSample = MulDiv(dwPos, m_dwSampleRate, 1000);
	//���ړI�̈ʒu���T���v���P�ʂɂ�������

	int nStartId = 0;
	int nEndId = m_nNumSampleId;
	int64_t i64CurPos;
	while (1) {
		//2���T���ŖړI�̈ʒu��T���i�ꔭ�Ō�������@��������Ȃ��̂Łj
		//mp4ff_find_sample ���g���Έꔭ�ōs�����������ǎg������������Ȃ�
		int nId = nStartId + (nEndId - nStartId) / 2;
		i64CurPos = mp4ff_get_sample_position(m_pMp4ff, m_nTrack, nId);
		if (i64CurPos == i64PosSample) {//�ő��ɐ������Ȃ�
			nStartId = nEndId = nId;
		}
		else if (i64CurPos > i64PosSample) {
			nEndId = nId - 1;
		}
		else {
			nStartId = nId + 1;
		}
		if (nEndId <= nStartId) {
			break;
		}
	}
	m_nCurrentSampleId = nEndId - 1;//1��O�ɃV�[�N����
	if (m_nCurrentSampleId >= m_nNumSampleId) {
		m_nCurrentSampleId = m_nNumSampleId - 1;
	}
	if (m_nCurrentSampleId < 0) {
		m_nCurrentSampleId = 0;
	}
	m_pSample = NULL;
	m_dwRemain = 0;
	//����Ȃ������ő�1�b������ǂ�
	//KbMedia Player �{�̂��K�v�ɉ����Ă��̂Ŏ��ۂ͕s�v
	m_dwCurrentSample = mp4ff_get_sample_position(m_pMp4ff, m_nTrack, m_nCurrentSampleId);
	DWORD dwCurPosMS = MulDiv(m_dwCurrentSample, 1000, m_dwSampleRate);
	DWORD dwDeltaMS = dwPos - dwCurPosMS;
	if (dwDeltaMS > 1000) {
		dwDeltaMS = 1000;
	}
	DWORD dwDeltaSample = MulDiv(dwDeltaMS, m_dwSampleRate, 1000);
	DWORD dwDeltaBytes = dwDeltaSample * 2 * m_dwChannels;
	if (dwDeltaBytes) {
		BYTE *pBuffer = (BYTE*)malloc(dwDeltaBytes);
		dwDeltaBytes = Render(pBuffer, dwDeltaBytes);
		dwDeltaMS = MulDiv(dwDeltaBytes / 2 / m_dwChannels, 1000, m_dwSampleRate);
		dwPos = dwCurPosMS + dwDeltaMS;
		free(pBuffer);
	}
	return dwPos;
}
//////////////////////////////////////////////////////////////////////////
DWORD __fastcall KbMp4AacDecoder::Render(BYTE *Buffer, DWORD dwSize)
{
	if (m_pMp4ff == NULL) {
		return dwSize;
	}
	DWORD dwRet = 0;
	DWORD dwCopySize = m_dwRemain;
	if (dwCopySize) {
		if (dwCopySize > dwSize) {
			m_dwRemain -= dwSize;
			dwCopySize = dwSize;
		}
		else {
			m_dwRemain -= dwCopySize;
		}
		memcpy(Buffer, m_pSample, dwCopySize);
		m_pSample += dwCopySize;
		dwRet += dwCopySize;
	}
	while (dwRet < dwSize && m_nCurrentSampleId < m_nNumSampleId) {
		BYTE *buffer = NULL;
		UINT buffer_size = 0;
		int rc = mp4ff_read_sample(m_pMp4ff, m_nTrack, m_nCurrentSampleId, &buffer, &buffer_size);
		if (rc == 0) {
			break;
		}

		NeAACDecFrameInfo frameInfo;
		void *sample_buffer;
		sample_buffer = NeAACDecDecode(m_hDecoder, &frameInfo, buffer, buffer_size);
		if (buffer) {
			free(buffer);
		}
		if (frameInfo.error > 0) {
			break;
		}
		dwCopySize = frameInfo.samples * 2;//2==bps/8==16/8
		if (dwRet + dwCopySize > dwSize) {
			dwCopySize = dwSize - dwRet;
			m_dwRemain = frameInfo.samples * 2 - dwCopySize;
		}
		else {
			m_dwRemain = 0;
		}
		memcpy(Buffer + dwRet, sample_buffer, dwCopySize);
		m_nCurrentSampleId++;
		m_pSample = ((BYTE*)sample_buffer) + dwCopySize;
		dwRet += dwCopySize;
	}
	//�M���b�v���X�Đ��p
	//m_dwLastSample ���������l�łȂ��̂ŋ@�\���Ȃ�
	m_dwCurrentSample += (dwRet / 2 / m_dwChannels);
	if (m_dwCurrentSample > m_dwLastSample) {
		dwRet -= (m_dwCurrentSample - m_dwLastSample) / 2 / m_dwChannels;
		m_dwCurrentSample = m_dwLastSample;
	}
	//�����܂�

	return dwRet;
}


class m4a
{
public:
	static HKMP WINAPI Open(const _TCHAR *cszFileName, SOUNDINFO *pInfo)
	{//MP4 �R���e�i�Ɋi�[����Ă���� KbMp4AacDecoder
	 //MP4 �R���e�i�Ɋi�[����Ȃ���� KbAacDecoder
	 //���쐬���A���̃|�C���^��Ԃ�
		FILE *fp;
#if UNICODE	
		fp = _wfopen(cszFileName, _T("rb"));
#else
		fp = fopen(cszFileName, _T("rb"));
#endif	
		if (!fp) {
			return NULL;
		}
		BYTE header[8];
		if (fread(header, 1, sizeof(header), fp) != 8) {
			fclose(fp);
			return NULL;
		}
		BOOL bIsMp4;
		if (header[4] == 'f' && header[5] == 't' && header[6] == 'y' && header[7] == 'p') {
			bIsMp4 = TRUE;
		}
		else {
			bIsMp4 = FALSE;
		}
		fclose(fp);
		IKbAacDecoder *pAAC;
		if (bIsMp4) {
			pAAC = new KbMp4AacDecoder;
		}
		else {
			pAAC = new KbAacDecoder;
		}
		if (pAAC->Open(cszFileName, pInfo)) {
			return pAAC;
		}
		delete pAAC;
		return NULL;
	}

	static void WINAPI Close(HKMP hKMP)
	{
		if (hKMP) {
			IKbAacDecoder *pAAC = (IKbAacDecoder*)hKMP;
			delete pAAC;
			hKMP = NULL;
		}
	}

	static DWORD WINAPI Render(HKMP hKMP, BYTE* Buffer, DWORD dwSize)
	{
		if (!hKMP)return 0;
		IKbAacDecoder *pAAC = (IKbAacDecoder*)hKMP;
		return pAAC->Render(Buffer, dwSize);
	}

	static DWORD WINAPI SetPosition(HKMP hKMP, DWORD dwPos)
	{
		if (!hKMP)return 0;
		IKbAacDecoder *pAAC = (IKbAacDecoder*)hKMP;
		return pAAC->SetPosition(dwPos);
	}
};
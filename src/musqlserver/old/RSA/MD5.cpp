////////////////////////////////////////////////////////////////////////////////
// MD5.cpp
#include "stdafx.h"
#include "MD5.h"
#include "MD5_keyval.h"
#include <assert.h>
#include <iostream>
#include <fstream>

// WZMD5 - additional methods

// Creates WZMD5 128 bits length key (16 bytes) using the input and key index (0~65535)
bool WZMD5::WZMD5_EncodeKeyVal	(					
	char * lpszInputStr,			
	char * lpszOutputKeyVal,			
	int iKeyIndex			
)
{
	if( (iKeyIndex < 0) || (iKeyIndex >= MAX_KEY_INDEX))
		return false;

	unsigned int inputlen = (unsigned int)strlen ( (char *)lpszInputStr);

	setmagicnum(iKeyIndex);
	update ((BYTE *)lpszInputStr, inputlen);
	finalize ();
	for (int i = 0 ; i < 32 ; i++)
		lpszOutputKeyVal[i] = digest[i];
	init();

	return true;
}

// WZMD5 string is generated from input string and the key index (0~255) - 128x2 (32 bytes)
bool WZMD5::WZMD5_EncodeString	(	
	char * lpszInputStr,			
	char * lpszOutputStr,			
	int iKeyIndex
	)
{
	if( (iKeyIndex < 0) || (iKeyIndex >= MAX_KEY_INDEX))
		return false;

	unsigned int inputlen = (unsigned int)strlen ( (char *)lpszInputStr);

	setmagicnum(iKeyIndex);
	update ((BYTE *)lpszInputStr, inputlen);
	finalize ();
	strcpy_s (lpszOutputStr,sizeof(lpszOutputStr), hex_digest());
	init();

	return true;
}

// 문자열과 WZMD5 키값과 키인덱스 (0~255) 를 입력받아 키값을 인증함 (true : 맞음 / false : 틀림)
bool WZMD5::WZMD5_CheckValue	(	
	char * lpszInputStr,		
	char * szKeyVal,			
	int iKeyIndex	
	)
{
	char szBUF[32] = {0,};
	WZMD5_EncodeKeyVal (lpszInputStr, szBUF, iKeyIndex);

	for (int i = 0 ; i < 32 ; i++) {
		if (szKeyVal[i] == 0x00)
			continue;

		if (szKeyVal[i] != szBUF[i])
			return false;
	}

	return true;
}

void WZMD5::setmagicnum (int keyindex)
{
	if( (keyindex < 0) || (keyindex >= MAX_KEY_INDEX))
		return;

	// Load magic initialization constants
	state[0] = MD5_KEYVAL[keyindex*4+0];
	state[1] = MD5_KEYVAL[keyindex*4+1];
	state[2] = MD5_KEYVAL[keyindex*4+2];
	state[3] = MD5_KEYVAL[keyindex*4+3];
}

//---------------------------------------------
//	WZMD5 - existing methods

// WZMD5 simple initialization method

WZMD5::WZMD5(){

  init();

}


// WZMD5 block update operation. Continues an WZMD5 message-digest
// operation, processing another message block, and updating the
// context.

void WZMD5::update (uint1 *input, uint4 input_length) {

  uint4 input_index, buffer_index;
  uint4 buffer_space;                // how much space is left in buffer

  if (finalized){  // so we can't update!
    std::cerr << "WZMD5::update:  Can't update a finalized digest!" << std::endl;
    return;
  }

  // Compute number of bytes mod 64
  buffer_index = (unsigned int)((count[0] >> 3) & 0x3F);

  // Update number of bits
  if (  (count[0] += ((uint4) input_length << 3))<((uint4) input_length << 3) )
    count[1]++;

  count[1] += ((uint4)input_length >> 29);

  buffer_space = 64 - buffer_index;  // how much space is left in buffer

  // Transform as many times as possible.
  if (input_length >= buffer_space) { // ie. we have enough to fill the buffer
    // fill the rest of the buffer and transform
    std::memcpy (buffer + buffer_index, input, buffer_space);
    transform (buffer);

    // now, transform each 64-byte piece of the input, bypassing the buffer
    for (input_index = buffer_space; input_index + 63 < input_length; 
	 input_index += 64)
      transform (input+input_index);

    buffer_index = 0;  // so we can buffer remaining
  }
  else
    input_index=0;     // so we can buffer the whole input

  // and here we do the buffering:
  std::memcpy(buffer+buffer_index, input+input_index, input_length-input_index);
}


// WZMD5 update for files.
// Like above, except that it works on files (and uses above as a primitive.)

void WZMD5::update(FILE *file){

  BYTE buffer[1024];
  int len;

  while (len=(int)fread(buffer, 1, 1024, file))
    update(buffer, len);

  fclose (file);

}


// WZMD5 update for istreams.
// Like update for files; see above.

void WZMD5::update(std::istream& stream){

  BYTE buffer[1024];
  int len;

  while (stream.good()){
    stream.read((char*)buffer, 1024); // note that return value of read is unusable.
    len=stream.gcount();
    update(buffer, len);
  }

}


// WZMD5 update for ifstreams.
// Like update for files; see above.

void WZMD5::update(std::ifstream& stream){

  BYTE buffer[1024];
  int len;

  while (stream.good()){
    stream.read((char*)buffer, 1024); // note that return value of read is unusable.
    len=stream.gcount();
    update(buffer, len);
  }

}


// WZMD5 finalization. Ends an WZMD5 message-digest operation, writing the
// the message digest and zeroizing the context.

void WZMD5::finalize (){

  BYTE bits[8];
  unsigned int index, padLen;
  static uint1 PADDING[64]={
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

  if (finalized){
    std::cerr << "WZMD5::finalize:  Already finalized this digest!" << std::endl;
    return;
  }

  // Save number of bits
  encode (bits, count, 8);

  // Pad out to 56 mod 64.
  index = (uint4) ((count[0] >> 3) & 0x3f);
  padLen = (index < 56) ? (56 - index) : (120 - index);
  update (PADDING, padLen);

  // Append length (before padding)
  update (bits, 8);

  // Store state in digest
  encode (digest, state, 16);

  // Zeroize sensitive information
  memset (buffer, 0, sizeof(*buffer));

  finalized=1;

}


WZMD5::WZMD5(FILE *file){

  init();  // must be called be all constructors
  update(file);
  finalize ();
}


WZMD5::WZMD5(std::istream& stream){

  init();  // must called by all constructors
  update (stream);
  finalize();
}


WZMD5::WZMD5(std::ifstream& stream){

  init();  // must called by all constructors
  update (stream);
  finalize();
}


BYTE *WZMD5::raw_digest(){

  if (!finalized){
    std::cerr << "WZMD5::raw_digest:  Can't get digest if you haven't "<<
      "finalized the digest!" <<std::endl;
    return ( (BYTE*) "");
  }

  std::memcpy((uint1*)m_cRaw_digest, digest, 16);
  return m_cRaw_digest;
}

char *WZMD5::hex_digest(){

  int i;

  if (!finalized){
    std::cerr << "WZMD5::hex_digest:  Can't get digest if you haven't "<<
      "finalized the digest!" <<std::endl;
    return "";
  }

  for (i=0; i<16; i++)
    sprintf_s(m_cHex_digest+i*2,sizeof(m_cHex_digest), "%02x", digest[i]);

  m_cHex_digest[32]='\0';

  return m_cHex_digest;
}

std::ostream& operator<<(std::ostream &stream, WZMD5 context){

	stream << context.hex_digest();
	
	return stream;
}

// PRIVATE METHODS:


void WZMD5::init(){
  finalized=0;  // we just started!

  // Nothing counted, so count=0
  count[0] = 0;
  count[1] = 0;

  // Load magic initialization constants.
  state[0] = 0x67452301;
  state[1] = 0xefcdab89;
  state[2] = 0x98badcfe;
  state[3] = 0x10325476;
}

// WZMD5 basic transformation. Transforms state based on block.
void WZMD5::transform (uint1 block[64]){

  uint4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];

  decode (x, block, 64);

  assert(!finalized);  // not just a user error, since the method is private

  /* Round 1 */
  FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
  FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
  FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
  FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
  FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
  FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
  FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
  FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
  FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
  FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
  FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
  FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
  FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
  FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
  FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
  FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

 /* Round 2 */
  GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
  GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
  GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
  GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
  GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
  GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
  GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
  GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
  GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
  GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
  GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
  GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
  GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
  GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
  GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
  GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

  /* Round 3 */
  HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
  HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
  HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
  HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
  HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
  HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
  HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
  HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
  HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
  HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
  HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
  HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
  HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
  HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
  HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
  HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

  /* Round 4 */
  II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
  II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
  II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
  II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
  II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
  II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
  II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
  II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
  II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
  II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
  II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
  II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
  II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
  II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
  II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
  II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;

  // Zeroize sensitive information.
  memset ( (uint1 *) x, 0, sizeof(x));

}


// Encodes input (UINT4) into output (BYTE). Assumes len is
// a multiple of 4.
void WZMD5::encode (uint1 *output, uint4 *input, uint4 len) {

  unsigned int i, j;

  for (i = 0, j = 0; j < len; i++, j += 4) {
    output[j]   = (uint1)  (input[i] & 0xff);
    output[j+1] = (uint1) ((input[i] >> 8) & 0xff);
    output[j+2] = (uint1) ((input[i] >> 16) & 0xff);
    output[j+3] = (uint1) ((input[i] >> 24) & 0xff);
  }
}


// Decodes input (BYTE) into output (UINT4). Assumes len is
// a multiple of 4.
void WZMD5::decode (uint4 *output, uint1 *input, uint4 len){

  unsigned int i, j;

  for (i = 0, j = 0; j < len; i++, j += 4)
    output[i] = ((uint4)input[j]) | (((uint4)input[j+1]) << 8) |
      (((uint4)input[j+2]) << 16) | (((uint4)input[j+3]) << 24);
}


// Note: Replace "for loop" with standard std::memcpy if possible.
void WZMD5::std::memcpy (uint1 *output, uint1 *input, uint4 len){

  unsigned int i;

  for (i = 0; i < len; i++)
    output[i] = input[i];
}


// Note: Replace "for loop" with standard memset if possible.
void WZMD5::memset (uint1 *output, uint1 value, uint4 len){

  unsigned int i;

  for (i = 0; i < len; i++)
    output[i] = value;
}


// ROTATE_LEFT rotates x left n bits.

inline unsigned int WZMD5::rotate_left  (uint4 x, uint4 n){
  return (x << n) | (x >> (32-n))  ;
}


// F, G, H and I are basic WZMD5 functions.

inline unsigned int WZMD5::F            (uint4 x, uint4 y, uint4 z){
  return (x & y) | (~x & z);
}

inline unsigned int WZMD5::G            (uint4 x, uint4 y, uint4 z){
  return (x & z) | (y & ~z);
}

inline unsigned int WZMD5::H            (uint4 x, uint4 y, uint4 z){
  return x ^ y ^ z;
}

inline unsigned int WZMD5::I            (uint4 x, uint4 y, uint4 z){
  return y ^ (x | ~z);
}


// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.

inline void WZMD5::FF(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
		    uint4  s, uint4 ac){
 a += F(b, c, d) + x + ac;
 a = rotate_left (a, s) +b;
}

inline void WZMD5::GG(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
		    uint4 s, uint4 ac){
 a += G(b, c, d) + x + ac;
 a = rotate_left (a, s) +b;
}

inline void WZMD5::HH(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
		    uint4 s, uint4 ac){
 a += H(b, c, d) + x + ac;
 a = rotate_left (a, s) +b;
}

inline void WZMD5::II(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			     uint4 s, uint4 ac){
 a += I(b, c, d) + x + ac;
 a = rotate_left (a, s) +b;
}


////////////////////////////////////////////////////////////////////////////////
//  vnDev.Games - MuServer S12EP2 IGC v12.0.1.0 - Trong.LIVE - DAO VAN TRONG  //
////////////////////////////////////////////////////////////////////////////////


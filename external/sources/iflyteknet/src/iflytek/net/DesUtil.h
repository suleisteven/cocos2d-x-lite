#pragma once

#include <openssl/des.h>
#include <string>
#include <stdint.h>
//#include "inv_base64.h"



static const char* k = "f334acc30370c1e4e7ddae78";
#define LEN_OF_KEY 24   

#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib") 

class DESDecryption {
public:


	void test()
	{
		int docontinue = 1;
		char *data = "hello"; /* 明文 */
		int data_len;
		int data_rest;
		unsigned char ch;
		unsigned char *src = NULL; /* 补齐后的明文 */
		unsigned char *dst = NULL; /* 解密后的明文 */
		int len;
		unsigned char tmp[8];
		unsigned char in[8];
		unsigned char out[8];
		char *k = "f334acc30370c1e4e7ddae78"; /* 原始密钥 */
		int key_len;
#define LEN_OF_KEY 24   
		unsigned char key[LEN_OF_KEY]; /* 补齐后的密钥 */
		unsigned char block_key[9];
		DES_key_schedule ks, ks2, ks3;
		/* 构造补齐后的密钥 */
		key_len = strlen(k);
		memcpy(key, k, key_len);
		memset(key + key_len, 0x00, LEN_OF_KEY - key_len);
		/* 分析补齐明文所需空间及补齐填充数据 */
		data_len = strlen(data);
		data_rest = data_len % 8;
		len = data_len + (8 - data_rest);
		ch = 8 - data_rest;
		src = (unsigned char *)malloc(len);
		dst = (unsigned char *)malloc(len);
		if (NULL == src || NULL == dst)
		{
			docontinue = 0;
		}
		if (docontinue)
		{
			int count;
			int i;
			/* 构造补齐后的加密内容 */
			memset(src, 0, len);
			memcpy(src, data, data_len);
			memset(src + data_len, ch, 8 - data_rest);
			/* 密钥置换 */
			memset(block_key, 0, sizeof(block_key));
			memcpy(block_key, key + 0, 8);
			DES_set_key_unchecked((const_DES_cblock*)block_key, &ks);
			memcpy(block_key, key + 8, 8);
			DES_set_key_unchecked((const_DES_cblock*)block_key, &ks2);
			memcpy(block_key, key + 16, 8);
			DES_set_key_unchecked((const_DES_cblock*)block_key, &ks3);
			printf("before encrypt:\n");
			for (i = 0; i < len; i++)
			{
				printf("0x%.2X ", *(src + i));
			}
			printf("\n");
			/* 循环加密/解密，每8字节一次 */
			count = len / 8;
			for (i = 0; i < count; i++)
			{
				memset(tmp, 0, 8);
				memset(in, 0, 8);
				memset(out, 0, 8);
				memcpy(tmp, src + 8 * i, 8);
				/* 加密 */
				DES_ecb3_encrypt((const_DES_cblock*)tmp, (DES_cblock*)in, &ks, &ks2, &ks3, DES_ENCRYPT);
				/* 解密 */
				DES_ecb3_encrypt((const_DES_cblock*)in, (DES_cblock*)out, &ks, &ks2, &ks3, DES_DECRYPT);
				/* 将解密的内容拷贝到解密后的明文 */
				memcpy(dst + 8 * i, out, 8);
			}
			printf("after decrypt :\n");
			for (i = 0; i < len; i++)
			{
				printf("0x%.2X ", *(dst + i));
			}
			printf("\n");
		}
		if (NULL != src)
		{
			free(src);
			src = NULL;
		}
		if (NULL != dst)
		{
			free(dst);
			dst = NULL;
		}
	}

	DESDecryption()
	{
		init();

		test();
	}

	

	std::string encode(const std::string& data)
	{
		std::string result;
		if (data.empty())
		{
			return result;
		}

		unsigned char tmp[8];
		unsigned char in[8];
		unsigned char out[8];

		int32_t data_len = data.size();
		int32_t data_rest = data_len % 8;
		int32_t len = data_len + (8 - data_rest);
		unsigned char ch = 8 - data_rest;
		char* src = new char[len];
		/* 构造补齐后的加密内容 */
		memset(src, 0, len);
		memcpy(src, data.data(), data.size());
		memset(src + data_len, ch, 8 - data_rest);

		char* dst = new char[len];
		memset(dst, 0, len);
		/* 循环加密/解密，每8字节一次 */
		int32_t count = len / 8;
		for (int32_t i = 0; i < count; i++)
		{
			memset(tmp, 0, 8);
			memset(out, 0, 8);
			memcpy(tmp, src + 8 * i, 8);
			/* 加密 */
			DES_ecb3_encrypt((const_DES_cblock*)tmp, (DES_cblock*)out, &ks, &ks2, &ks3, DES_ENCRYPT);
			//memcpy(dst + 8 * i, out, 8);
		}
		result.append(dst, len);

		delete[] src;
		delete[] dst;

		return result;
	}


	void encode(int8_t* &_return, int32_t& destLen, void* buf, int32_t srcLen)
	{
		destLen = 0;
		if (NULL == buf || 0 >= srcLen)
		{
			return;
		}
		unsigned char tmp[8];
		unsigned char in[8];
		unsigned char out[8];

		int32_t data_len = srcLen;
		int32_t data_rest = data_len % 8;
		int32_t len = data_len + (8 - data_rest);
		unsigned char ch = 8 - data_rest;
		char* src = new char[len];
		/* 构造补齐后的加密内容 */
		memset(src, 0, len);
		memcpy(src, buf, srcLen);
		memset(src + data_len, ch, 8 - data_rest);

		char* dst = new char[len];
		memset(dst, 0, len);
		/* 循环加密/解密，每8字节一次 */
		int32_t count = len / 8;
		for (int32_t i = 0; i < count; i++)
		{
			memset(tmp, 0, 8);
			memset(out, 0, 8);
			memcpy(tmp, src + 8 * i, 8);
			/* 加密 */
			DES_ecb3_encrypt((const_DES_cblock*)tmp, (DES_cblock*)out, &ks, &ks2, &ks3, DES_ENCRYPT);
			memcpy(dst + 8 * i, out, 8);
		}

		destLen = len;
		_return = (int8_t*)dst;

		return;
	}

	void decode(int8_t* &_return, int32_t& destLen, void* buf, int32_t len)
	{
		destLen = 0;
		if (NULL == buf || 0 >= len)
		{
			return;
		}
		unsigned char tmp[8];
		unsigned char in[8];
		unsigned char out[8];

		char* src = new char[len];
		memcpy(src, buf, len);
		char* dst = new char[len];
		memset(dst, 0, len);

		/* 循环加密/解密，每8字节一次 */
		int32_t count = len / 8;
		for (int32_t i = 0; i < count; i++)
		{
			memset(tmp, 0, 8);
			memset(out, 0, 8);
			memcpy(tmp, src + 8 * i, 8);
			/* 解密 */
			DES_ecb3_encrypt((const_DES_cblock*)tmp, (DES_cblock*)out, &ks, &ks2, &ks3, DES_DECRYPT);
			/* 将解密的内容拷贝到解密后的明文 */
			memcpy(dst + 8 * i, out, 8);
		}
		/* 去掉多余数据 */
		char rest = dst[len - 1];
		if (1 > rest || 8 < rest)
		{
			return;
		}

		_return = (int8_t*)dst;
		destLen = len - rest;

		return;
	}

	std::string  decode(const std::string& data)
	{
		std::string result;
		if (data.empty())
		{
			return result;
		}
		unsigned char tmp[8];
		unsigned char in[8];
		unsigned char out[8];

		int32_t len = data.size();
		char* src = new char[len];
		memcpy(src, data.data(), data.size());
		char* dst = new char[len];
		memset(dst, 0, len);

		/* 循环加密/解密，每8字节一次 */
		int32_t count = len / 8;
		for (int32_t i = 0; i < count; i++)
		{
			memset(tmp, 0, 8);
			memset(out, 0, 8);
			memcpy(tmp, src + 8 * i, 8);
			/* 解密 */
			DES_ecb3_encrypt((const_DES_cblock*)tmp, (DES_cblock*)out, &ks, &ks2, &ks3, DES_DECRYPT);
			/* 将解密的内容拷贝到解密后的明文 */
			memcpy(dst + 8 * i, out, 8);
		}
		/* 去掉多余数据 */
		char rest = dst[len - 1];
		if (1 > rest || 8 < rest)
		{
			return result;
		}
		result.append(dst, len - rest);

		delete[] src;
		delete[] dst;

		return result;
	}

private:
	void init()
	{
		int key_len;
		unsigned char key[LEN_OF_KEY]; /* 补齐后的密钥 */
		unsigned char block_key[9];
		/* 构造补齐后的密钥 */
		key_len = strlen(k);
		memcpy(key, k, key_len);
		memset(key + key_len, 0x00, LEN_OF_KEY - key_len);
		/* 密钥置换 */
		memset(block_key, 0, sizeof(block_key));
		memcpy(block_key, key + 0, 8);
		DES_set_key_unchecked((const_DES_cblock*)block_key, &ks);
		memcpy(block_key, key + 8, 8);
		DES_set_key_unchecked((const_DES_cblock*)block_key, &ks2);
		memcpy(block_key, key + 16, 8);
		DES_set_key_unchecked((const_DES_cblock*)block_key, &ks3);
		
	}

private:
	DES_key_schedule ks, ks2, ks3;
};



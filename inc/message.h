#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <assert.h>

class message
{
public:
	enum { header_length = 8 };
	enum { max_body_length = 512 };
	
	message(size_t nBodySize = max_body_length)
		:body_length_(0),
		data_(0),
		msg_id_(0)
	{
		if (nBodySize <= 0)
			return;

		buffer_length_ = header_length + nBodySize + 1;

		data_ = new char[buffer_length_];
		memset(data_, 0, buffer_length_);
	}

	message(const message &msg)
	{
		body_length_ = msg.body_length();
		buffer_length_ = header_length + body_length_ + 1;
		msg_id_ = msg.msg_id_;

		data_ = new char[buffer_length_];
		memset(data_, 0, buffer_length_);
		std::memcpy(data_, msg.data(), msg.length());
		fill_end(body_length_);
	}

	~message()
	{
		free();
	}

	message& operator=(const message &rhs)
	{
		if (this != &rhs)
		{
			//当前缓存小于要存储数据长度
			if (buffer_length_-1 < rhs.length())
			{
				delete[] data_;

				buffer_length_ = rhs.length() + 1;
				data_ = new char[buffer_length_];
				memset(data_, 0, buffer_length_);
			}

			body_length_ = rhs.body_length();
			msg_id_ = rhs.msg_id_;
			std::memcpy(data_, rhs.data(), rhs.length());
			fill_end(body_length_);
		}

		return *this;
	}

	const char* data() const
	{
		return data_;
	}

	char* data()
	{
		return data_;
	}

	size_t buffer_length()
	{
		return buffer_length_;
	}

	size_t length() const
	{
		return header_length + body_length_;
	}

	const char* body() const
	{
		return data_ + header_length;
	}

	char* body()
	{
		return data_ + header_length;
	}

	size_t body_length() const
	{
		return body_length_;
	}

	void set_body_length(size_t len)
	{
		body_length_ = len;
	}

	bool decode_header()
	{
		char header[header_length + 1] = "";
		std::strncat(header, data_, header_length);
		body_length_ = std::atoi(header);
		if (body_length_ <= 0)
		{
			body_length_ = 0;
			return false;
		}
		return true;
	}

	void encode_header()
	{
		if (0 == data_)
			return;

		char header[header_length + 1] = "";
		sprintf(header, "%8d", static_cast<int>(body_length_));
		std::memcpy(data_, header, header_length);
	}

	void fill_msg(const char *szMsg, size_t nLen = 0)
	{
		assert(szMsg);

		if (nLen > buffer_length_-1)
		{
			delete[] data_;

			buffer_length_ = header_length + nLen + 1;
			data_ = new char[buffer_length_];
			memset(data_, 0, buffer_length_);
		}

		body_length_ = nLen;

		encode_header();
		std::memcpy(data_ + header_length, szMsg, body_length_);
		fill_end(body_length_);
	}

	// 在指定的位置(通常指有效内容的下一个位置)插入字符串结束符
	void fill_end(int pos)
	{
		assert(data_);

		// 如果要插入的位置不是有效的位置，则返回
		if ( (pos < 0) || (pos > buffer_length_) )
			return;

		data_[header_length+pos] = 0;
	}

	// 重新分配缓存大小
	void realloc(size_t nBodySize)
	{
		if (nBodySize <= 0)
			return;

		delete[] data_;// 删除旧的缓存

		buffer_length_ = header_length + nBodySize + 1;

		data_ = new char[buffer_length_];
		memset(data_, 0, buffer_length_);
		body_length_ = 0;
		msg_id_ = 0;
	}

	void free()
	{
		assert(data_);
		delete[] data_;
		data_ = NULL;
	}

	void set_msg_id(int msg_id)
	{
		msg_id_ = msg_id;
	}

	int get_msg_id()
	{
		return msg_id_;
	}

private:
	char   *data_;
	size_t buffer_length_;
	size_t body_length_;// 不包括字符串的结束符'\0'
	int    msg_id_;
};



#endif // MESSAGE_H

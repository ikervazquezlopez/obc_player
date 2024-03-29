#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include "CodecUtils.h"


class Coder
{

private:
	
	std::fstream file;

	void write_4_bytes(char* data);
	void write_4_bytes(uint32_t data);


public:
	Coder();
	~Coder();

	void open_file(const char* filename);
	void close_file();
	void write_header(uint32_t frame_count, uint32_t frame_rate, uint32_t video_width, uint32_t video_height);
	void write_background(unsigned char* data, uint32_t size);
	void write_frame_metadata(std::vector<Object_data> data);
	void write_frame_texture_atlas(unsigned char* data);

	Object_data fill_object_data(uint32_t id,	uint32_t x, uint32_t y, uint32_t w, uint32_t h,
												uint32_t tx, uint32_t ty, uint32_t tw, uint32_t th);

};


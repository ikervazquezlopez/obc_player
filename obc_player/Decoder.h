#pragma once
#include <fstream>
#include <iostream>
#include <vector>

#include "CodecUtils.h"

class Decoder {

private:
	bool FLAG_HEADER_READED;
	bool FLAG_BACKGROUND_READED;

	uint32_t frame_count;
	uint32_t frame_rate;
	uint32_t video_width;
	uint32_t video_height;

	std::fstream file;

	
	void read_4_bytes(char buffer[5]);
	

public:
	

	Decoder();
	~Decoder();

	void open_file(const char* filename);
	void close_file();
	void read_header();
	unsigned char* read_background();
	std::vector<Object_data*> read_frame_meatadata();
	unsigned char* read_frame_texture_atlas();
	Object_data* read_object();
	
};
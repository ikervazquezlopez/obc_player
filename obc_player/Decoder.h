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
	bool finished;
	uint32_t readed_frames;

	
	void read_4_bytes(char buffer[5]);
	
	std::vector<Object_data> read_frame_meatadata();
	unsigned char* read_frame_texture_atlas();
	Object_data read_object();

public:
	

	Decoder();
	~Decoder();

	void open_file(const char* filename);
	void close_file();
	void read_header();
	unsigned char* read_background();
	std::pair < std::vector<Object_data>, unsigned char*> read_frame();
	bool is_finised();

	uint32_t get_width();
	uint32_t get_height();
	uint32_t get_frame_rate();
	
};
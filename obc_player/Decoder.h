#pragma once
#include <fstream>
#include <iostream>
#include <vector>

class Decoder {

private:
	struct Quad_data {
		uint32_t x;
		uint32_t y;
		uint32_t w;
		uint32_t h;
	};

	struct Texture_data {
		uint32_t x;
		uint32_t y;
		uint32_t w;
		uint32_t h;
	};

	struct Object_data {
		uint32_t id;
		Quad_data quad;
		Texture_data texture;
	};
	uint32_t frame_count;
	uint32_t frame_rate;
	uint32_t video_width;
	uint32_t video_height;

	std::fstream file;

	Object_data* read_object();
	void read_4_bytes(char buffer[5]);
	

public:
	Decoder();
	~Decoder();

	void open_file(const char* filename);
	void close_file();
	void read_header();
	unsigned char* read_background();
	std::vector<Object_data*> read_frame_meatadata();
	
};
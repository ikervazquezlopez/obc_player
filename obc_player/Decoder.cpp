#include "Decoder.h"
#include "CodecUtils.h"

void Decoder::read_4_bytes(char buffer[5])
{
	this->file.read(buffer, 4); 
	buffer[4] = '\0';
}

Decoder::Decoder()
{
	this->FLAG_HEADER_READED = false;
	this->FLAG_BACKGROUND_READED = false;

	this->frame_count = 0;
	this->frame_rate = 0;

	this->video_width = 0;
	this->video_height = 0;

}

Decoder::~Decoder()
{
	this->close_file();
}

void Decoder::open_file(const char* filename)
{
	file.open(filename, std::ios_base::in | std::ios_base::binary);
	if (!this->file.is_open()) {
		std::cerr << "(Decoder) The file could not be open!" << std::endl;
		exit(-1);
	}
	this->finished = false;
	this->readed_frames = 0;
}

void Decoder::close_file()
{
	this->file.close();
}

void Decoder::read_header()
{
	char* buffer = new char[5];
	read_4_bytes(buffer);
	
	if (!strcmp(buffer, "16KC") == 0) {
		std::cerr << "Incorrect file format!" << std::endl;
		exit(-1);
	}

	read_4_bytes(buffer);
	this->frame_count = *reinterpret_cast<uint32_t*>(buffer);


	read_4_bytes(buffer);
	this->frame_rate = *reinterpret_cast<uint32_t*>(buffer);

	read_4_bytes(buffer);
	this->video_width = *reinterpret_cast<uint32_t*>(buffer);

	read_4_bytes(buffer);
	this->video_height = *reinterpret_cast<uint32_t*>(buffer);

	this->FLAG_HEADER_READED = true;

	delete[] buffer;
}

unsigned char* Decoder::read_background()
{
	if (!this->FLAG_HEADER_READED)
	{
		std::cout << "WARNING! Trying to read the background without reading the header. [read_background()]" << std::endl;
		read_header();
		std::cout << "Header readed! [read_background()]" << std::endl;
	}

	char* buffer = new char[5];
	read_4_bytes(buffer);
	uint32_t n = *reinterpret_cast<uint32_t*>(buffer);
	
	char* background_data = new char[n];
	this->file.read(background_data, n);

	this->FLAG_BACKGROUND_READED = true;

	delete[] buffer;
	return (unsigned char *) background_data;
}

std::vector<Object_data> Decoder::read_frame_meatadata()
{
	if (!this->FLAG_BACKGROUND_READED) 
	{
		std::cout << "ERROR on decoding order, trying to read frame metadata without reading the background!. [read_frame_metadata()]" << std::endl;
		exit(-1);
	}

	std::vector<Object_data> objs;
	char* buffer = new char[5];

	read_4_bytes(buffer);
	uint32_t n = *reinterpret_cast<uint32_t*>(buffer);

	for (uint32_t i = 0; i < n; i++) {
		objs.push_back(read_object());
	}
	
	delete[] buffer;
	return objs;
}

unsigned char* Decoder::read_frame_texture_atlas()
{
	char* atlas_data = new char[TEXTURE_ATLAS_RESOLUTION * 3];
	this->file.read(atlas_data, TEXTURE_ATLAS_RESOLUTION * 3);
	return reinterpret_cast<unsigned char*> (atlas_data);
}

std::pair<std::vector<Object_data>, unsigned char*> Decoder::read_frame()
{
	std::pair <std::vector<Object_data>, unsigned char*> p;
	p.first = this->read_frame_meatadata();
	p.second = this->read_frame_texture_atlas();
	this->readed_frames++;
	std::cout << this->readed_frames << std::endl;
	if (this->readed_frames >= this->frame_count)
		this->finished = true;
	return p;
}

bool Decoder::is_finised()
{
	return this->finished;
}

uint32_t Decoder::get_width()
{
	return this->video_width;
}

uint32_t Decoder::get_height()
{
	return this->video_height;
}

uint32_t Decoder::get_frame_rate()
{
	return this->frame_rate;
}

Object_data Decoder::read_object()
{
	
	char* buffer = new char[5];
	Object_data obj;

	read_4_bytes(buffer);
	obj.id = *reinterpret_cast<uint32_t*>(buffer);

	read_4_bytes(buffer);
	obj.quad.x = *reinterpret_cast<uint32_t*>(buffer);
	read_4_bytes(buffer);
	obj.quad.y = *reinterpret_cast<uint32_t*>(buffer);
	read_4_bytes(buffer);
	obj.quad.w = *reinterpret_cast<uint32_t*>(buffer);
	read_4_bytes(buffer);
	obj.quad.h = *reinterpret_cast<uint32_t*>(buffer);

	read_4_bytes(buffer);
	obj.texture.x = *reinterpret_cast<uint32_t*>(buffer);
	read_4_bytes(buffer);
	obj.texture.y = *reinterpret_cast<uint32_t*>(buffer);
	read_4_bytes(buffer);
	obj.texture.w = *reinterpret_cast<uint32_t*>(buffer);
	read_4_bytes(buffer);
	obj.texture.h = *reinterpret_cast<uint32_t*>(buffer);

	delete[] buffer;
	return obj;

}

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
		std::cerr << "The file could not be open!" << std::endl;
		exit(-1);
	}
}

void Decoder::close_file()
{
	this->file.close();
}

void Decoder::read_header()
{
	char* buffer = new char[5];
	read_4_bytes(buffer);
	
	if (strcmp(buffer, "16KC") == 0) {
		std::cerr << "Incorrect file format!" << std::endl;
		exit(-1);
	}

	read_4_bytes(buffer);
	this->frame_count = (uint32_t)buffer;

	read_4_bytes(buffer);
	this->frame_rate = (uint32_t)buffer;

	read_4_bytes(buffer);
	this->video_width = (uint32_t)buffer;

	read_4_bytes(buffer);
	this->video_height = (uint32_t)buffer;

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
	uint32_t n = (uint32_t)buffer;
	
	char* background_data = new char[n];
	this->file.read(background_data, n);

	this->FLAG_BACKGROUND_READED = true;

	delete[] buffer;
	return (unsigned char *) background_data;
}

std::vector<Object_data*> Decoder::read_frame_meatadata()
{
	if (!this->FLAG_BACKGROUND_READED) 
	{
		std::cout << "ERROR on decoding order, trying to read frame metadata without reading the background!. [read_frame_metadata()]" << std::endl;
		exit(-1);
	}

	std::vector<Object_data*> objs;
	char* buffer = new char[5];

	read_4_bytes(buffer);
	uint32_t n = (uint32_t)buffer;

	for (uint32_t i = 0; i < n; i++) {
		objs.push_back(read_object());
	}
	
	delete[] buffer;
	return objs;
}

Object_data* Decoder::read_object()
{
	char* buffer = new char[5];
	Object_data obj;

	read_4_bytes(buffer);
	obj.id = (uint32_t)buffer;

	read_4_bytes(buffer);
	obj.quad.x = (uint32_t)buffer;
	read_4_bytes(buffer);
	obj.quad.y = (uint32_t)buffer;
	read_4_bytes(buffer);
	obj.quad.w = (uint32_t)buffer;
	read_4_bytes(buffer);
	obj.quad.h = (uint32_t)buffer;

	read_4_bytes(buffer);
	obj.texture.x = (uint32_t)buffer;
	read_4_bytes(buffer);
	obj.texture.y = (uint32_t)buffer;
	read_4_bytes(buffer);
	obj.texture.w = (uint32_t)buffer;
	read_4_bytes(buffer);
	obj.texture.h = (uint32_t)buffer;

	delete[] buffer;
	return &obj;

}

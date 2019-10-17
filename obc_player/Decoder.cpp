#include "Decoder.h"


Decoder::Object_data* Decoder::read_object()
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

void Decoder::read_4_bytes(char buffer[5])
{
	this->file.read(buffer, 4); 
	buffer[5] = '\0';
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
	this->file.read(buffer, 4); buffer[5] = '\0';
	
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
}

unsigned char* Decoder::read_background()
{
	char* buffer = new char[5];
	read_4_bytes(buffer);
	uint32_t n = (uint32_t)buffer;
	
	char* background_data = new char[n];
	this->file.read(background_data, n);

	return (unsigned char *) background_data;
}

std::vector<Decoder::Object_data*> Decoder::read_frame_meatadata()
{
	std::vector<Decoder::Object_data*> objs;
	char* buffer = new char[5];

	read_4_bytes(buffer);
	uint32_t n = (uint32_t)buffer;

	for (uint32_t i = 0; i < n; i++) {
		objs.push_back(read_object());
	}
	
	return objs;
}

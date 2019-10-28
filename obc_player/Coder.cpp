#include "Coder.h"

Object_data Coder::fill_object_data(uint32_t id, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t tx, uint32_t ty, uint32_t tw, uint32_t th)
{
	Quad_data quad;
	quad.x = x;
	quad.y = y;
	quad.w = w;
	quad.h = h;

	Texture_data texture;
	texture.x = tx;
	texture.y = ty;
	texture.w = tw;
	texture.h = th;

	Object_data obj;
	obj.id = id;
	obj.quad = quad;
	obj.texture = texture;

	return obj;
}

void Coder::write_4_bytes(char data[5])
{
	this->file.write(data, 4*sizeof(char));
}

void Coder::write_4_bytes(uint32_t data)
{
	this->file.write((char*)&data, sizeof(data));
}

void Coder::open_file(const char* filename)
{
	file.open(filename, std::ios_base::in | std::ios_base::binary);
	if (!this->file.is_open()) {
		std::cerr << "(Coder) The file could not be open!" << std::endl;
		exit(-1);
	}
}

void Coder::close_file()
{
	this->file.close();
}

void Coder::write_header(uint32_t frame_count, uint32_t frame_rate, uint32_t video_width, uint32_t video_height)
{
	char tag[5] = "16KC"; tag[5] = '\0';
	write_4_bytes(tag);
	write_4_bytes(frame_count);
	write_4_bytes(frame_rate);
	write_4_bytes(video_width);
	write_4_bytes(video_height);
}

void Coder::write_background(unsigned char* data, unsigned int size)
{
	this->file.write((char*) data, size);
}

void Coder::write_frame_metadata(std::vector<Object_data> data)
{
	// Write number of objects
	write_4_bytes(data.size());

	// Write objects' data
	for (Object_data d : data) {
		write_4_bytes(d.id);
		write_4_bytes(d.quad.x);
		write_4_bytes(d.quad.y);
		write_4_bytes(d.quad.w);
		write_4_bytes(d.quad.h);
		write_4_bytes(d.texture.x);
		write_4_bytes(d.texture.y);
		write_4_bytes(d.texture.w);
		write_4_bytes(d.texture.h);
	}
	
}

void Coder::write_frame_texture_atlas(unsigned char* data)
{
	// Write 4k resolution texture atlas with 24bit (3byte) pixel depth
	this->file.write((char*)data, TEXTURE_ATLAS_RESOLUTION * 3 * sizeof(char));
}

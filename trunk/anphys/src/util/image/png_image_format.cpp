#include "png_image_format.h"

#include "image.h"
#include "png.h"

#include "util/log/log_system.h"

bool loadPngImage( const char* filename, cImage* image )
{
	//header for testing if it is a png
	png_byte header[8];

	//open file as binary
	FILE *fp = fopen(filename, "rb");
	if (!fp) 
	{
		*gLog << formatStr("ERROR: Can't load PNG file '%s'", filename);
		return false;
	}

	//read the header
	fread(header, 1, 8, fp);

	//test if png
	int is_png = !png_sig_cmp(header, 0, 8);
	if (!is_png) 
	{
		fclose(fp);
		*gLog << formatStr("ERROR: Can't load PNG file '%s': not PNG", filename);
		return false;
	}

	//create png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) 
	{
		fclose(fp);
		*gLog << formatStr("ERROR: Can't load PNG file '%s': TEXTURE_LOAD_ERROR", filename);
		return false;
	}

	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) 
	{
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(fp);

		*gLog << formatStr("ERROR: Can't load PNG file '%s': TEXTURE_LOAD_ERROR", filename);
		return false;
	}

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) 
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		*gLog << formatStr("ERROR: Can't load PNG file '%s': TEXTURE_LOAD_ERROR", filename);
		return false;
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr))) 
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);

		*gLog << formatStr("ERROR: Can't load PNG file '%s': TEXTURE_LOAD_ERROR", filename);
		return false;
	}

	//init png reading
	png_init_io(png_ptr, fp);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 twidth, theight;

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,	NULL, NULL, NULL);
	
	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// Allocate the image_data as a big block, to be given to opengl
	image->create(cImage::FMT_R8G8B8A8, vec2((float)twidth, (float)theight));
	png_byte *image_data = image->mData;
	if (!image_data) 
	{
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		*gLog << formatStr("ERROR: Can't load PNG file '%s': TEXTURE_LOAD_ERROR", filename);
		return false;
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = new png_bytep[theight];
	if (!row_pointers) 
	{
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] image_data;
		fclose(fp);
		*gLog << formatStr("ERROR: Can't load PNG file '%s': TEXTURE_LOAD_ERROR", filename);
		return false;
	}

	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < (int)theight; ++i)
		row_pointers[theight - 1 - i] = image_data + i * rowbytes;

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] row_pointers;
	fclose(fp);

	return true;
}

bool savePngImage( const char* filename, cImage* image )
{
	return false;
}

#include "texture_png_loader.h"

#include "libpng/png.h"

bool loadGLTextureFromPNG( const char* filename, GLuint* textureHandle, vec2f* size )
{
	png_byte header[8];

	FILE *fp = fopen(filename, "rb");
	if (fp == 0)
	{
		printf("ERROR: Failed to load texture '%s': can't open file\n", filename);
		return false;
	}
	
	// read the header
	/*fread(header, 1, 8, fp);
	
	if (png_sig_cmp(header, 0, 8))
	{
		printf("ERROR: Failed to load texture '%s': is not a PNG.\n", filename);
		fclose(fp);
		return 0;
	}
	
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		printf("ERROR: Failed to load texture '%s': png_create_read_struct returned 0.\n");
		fclose(fp);
		return 0;
	}
	
	// create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		printf("ERROR: Failed to load texture '%s': png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return 0;
	}
	
	// create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		printf("ERROR: Failed to load texture '%s': png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		return 0;
	}
	
	// the code in this if statement gets called if libpng encounters an error
	if (setjmp(png_jmpbuf(png_ptr))) 
	{
		printf("ERROR: Failed to load texture '%s': error from libpng\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 0;
	}
	
	// init png reading
	png_init_io(png_ptr, fp);
	
	// let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);
	
	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);
	
	// variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 temp_width, temp_height;
	
	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
	    NULL, NULL, NULL);
	
	if (size)
	{
		size->x = (float)temp_width;
		size->y = (float)temp_height;
	}
	
	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);
	
	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	
	// glTexImage2d requires rows to be 4-byte aligned
	//rowbytes += 3 - ((rowbytes-1) % 4);
	
	// Allocate the image_data as a big block, to be given to opengl
	png_byte * image_data = new png_byte[temp_width*temp_height*8];
	if (image_data == NULL)
	{
		printf("ERROR: Failed to load texture '%s': could not allocate memory for PNG image data\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 0;
	}
	
	// row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep * row_pointers = new png_bytep[temp_height];
	if (row_pointers == NULL)
	{
		printf("ERROR: Failed to load texture '%s': could not allocate memory for PNG row pointers\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		free(image_data);
		fclose(fp);
		return 0;
	}
	
	// set the individual row_pointers to point at the correct offsets of image_data
	int i;
	for (i = 0; i < (int)temp_height; i++)
	{
		row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
	}
	
	// read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);*/

	//read the header
	fread(header, 1, 8, fp);

	//test if png
	int is_png = !png_sig_cmp(header, 0, 8);
	if (!is_png) 
	{
		fclose(fp);
		printf("ERROR: Can't load PNG file '%s': not PNG\n", filename);
		return false;
	}

	//create png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) 
	{
		fclose(fp);
		printf("ERROR: Can't load PNG file '%s': TEXTURE_LOAD_ERROR\n", filename);
		return false;
	}

	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) 
	{
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(fp);
		printf("ERROR: Can't load PNG file '%s': TEXTURE_LOAD_ERROR\n", filename);
		return false;
	}

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) 
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		printf("ERROR: Can't load PNG file '%s': TEXTURE_LOAD_ERROR\n", filename);
		return false;
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr))) 
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		printf("ERROR: Can't load PNG file '%s': TEXTURE_LOAD_ERROR\n", filename);
		return false;
	}

	//init png reading
	//png_set_read_fn(png_ptr, pngImageFile, customPngReadFn);

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
	png_byte *image_data = new png_byte[twidth*theight*4];
	if (!image_data) 
	{
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		printf("ERROR: Can't load PNG file '%s': TEXTURE_LOAD_ERROR\n", filename);
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
		printf("ERROR: Can't load PNG file '%s': TEXTURE_LOAD_ERROR\n", filename);
		return false;
	}

	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < (int)theight; ++i)
		row_pointers[theight - 1 - i] = image_data + i * rowbytes;

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);
	
	// Generate the OpenGL texture object	GLuint texture;
	*textureHandle = 0;
	glGenTextures(1, textureHandle);
	glBindTexture(GL_TEXTURE_2D, *textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, theight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	size->x = (float)twidth;
	size->y = (float)theight;
	
	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

	delete[] image_data;
	delete[] row_pointers;
	fclose(fp);

	return true;
}

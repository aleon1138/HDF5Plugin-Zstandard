#include <stdlib.h>
#include <zstd.h>
#include "zstd_h5plugin.h"

#define ZSTD_FILTER 32015

DLL_EXPORT size_t zstd_filter(unsigned int flags, size_t cd_nelmts,
	const unsigned int cd_values[], size_t nbytes,
	size_t *buf_size, void **buf)
{
	size_t new_size = 0;
	void *outbuf = NULL;    /* Pointer to new output buffer */

	if (flags & H5Z_FLAG_REVERSE)
	{
		new_size = ZSTD_getDecompressedSize(*buf, nbytes);
		outbuf = malloc(new_size);
		if (!outbuf)
			goto error;

		new_size = ZSTD_decompress(outbuf, new_size, *buf, nbytes);
		if (ZSTD_isError(new_size))
			goto error;
	}
	else
	{
		int comp_level = (cd_nelmts > 0)? (int)cd_values[0] : ZSTD_CLEVEL_DEFAULT;

		if (comp_level < 1 /*ZSTD_minCLevel()*/)
			comp_level = 1 /*ZSTD_minCLevel()*/;
		else if (comp_level > ZSTD_maxCLevel())
			comp_level = ZSTD_maxCLevel();

		new_size = ZSTD_compressBound(nbytes);
		outbuf = malloc(new_size);
		if (!outbuf)
			goto error;

		new_size = ZSTD_compress(outbuf, new_size, *buf, nbytes, comp_level);
		if (ZSTD_isError(new_size))
			goto error;
	}

	free(*buf);
	*buf = outbuf;
	*buf_size = new_size;
	return new_size;

error:
	if (outbuf)
		free(outbuf);
	return 0;
}

const H5Z_class_t zstd_H5Filter =
{
	H5Z_CLASS_T_VERS,
	(H5Z_filter_t)(ZSTD_FILTER),
	1, 1,
	"Zstandard compression: http://www.zstd.net",
	NULL, NULL,
	(H5Z_func_t)(zstd_filter)
};

DLL_EXPORT H5PL_type_t H5PLget_plugin_type(void)
{
	return H5PL_TYPE_FILTER;
}

DLL_EXPORT const void* H5PLget_plugin_info(void)
{
	return &zstd_H5Filter;
}

TARGET_LIB := libH5Zzstd.so
PLUGIN_DIR := /usr/local/hdf5/lib/plugin
HDF5 := $(shell pkg-config --cflags --libs hdf5)
ZSTD := -lzstd

all: ${TARGET_LIB}

${TARGET_LIB}: zstd_h5plugin.c
	cc -shared -fPIC ${HDF5} $^ ${ZSTD} -o $@

clean:
	rm -f ${TARGET_LIB}

install:
	mkdir -p ${PLUGIN_DIR} && cp ${TARGET_LIB} ${PLUGIN_DIR}

TARGET := libH5Zzstd.so
HDF5 := $(shell pkg-config --cflags --libs hdf5)
ZSTD := -lzstd

all: ${TARGET}

${TARGET}: zstd_h5plugin.c
	cc -shared -fPIC ${HDF5} $^ ${ZSTD} -o $@

clean:
	rm -f ${TARGET}

install:
	mkdir -p /usr/local/hdf5/lib/plugin
	cp ${TARGET} /usr/local/hdf5/lib/plugin

#include <stdio.h>

#include "feature_builder.h"
#include "feature_verifier.h"

#include "flatcc/support/hexdump.h"
#include "flatcc/support/elapsed.h"
#include "../../config/config.h"

#undef ns
#define ns(x) FLATBUFFERS_WRAP_NAMESPACE(FlatGeobuf, x)
#undef nsc
#define nsc(x) FLATBUFFERS_WRAP_NAMESPACE(flatbuffers, x)

int gen_flatgeobuf1(flatcc_builder_t *B)
{
    double xy[] = { 0, 1 };
    //double z[] = { 0 };

    FlatGeobuf_Geometry_ref_t geometry;

    flatcc_builder_reset(B);

    flatbuffers_double_vec_ref_t xyRef = flatbuffers_double_vec_create(B, xy, 2);
    //flatbuffers_double_vec_ref_t zRef = flatbuffers_double_vec_create(B, z, 1);

    ns(Geometry_start(B));
    ns(Geometry_xy_add(B, xyRef));
    //ns(Geometry_z_add(B, zRef));
    geometry = ns(Geometry_end(B));

    ns(Feature_start_as_root_with_size(B));
    ns(Feature_geometry_add(B, geometry));
    ns(Feature_end_as_root(B));
    return 0;
}

int gen_flatgeobuf2(flatcc_builder_t *B)
{
    FlatGeobuf_Geometry_ref_t geometry;

    flatcc_builder_reset(B);

    ns(Geometry_start(B));
    ns(Geometry_xy_start(B));
    ns(Geometry_xy_push_create(B, 1));
    ns(Geometry_xy_push_create(B, 2));
    ns(Geometry_xy_end(B));
    //ns(Geometry_z_start(B));
    //ns(Geometry_z_push_create(B, 1));
    //ns(Geometry_z_end(B));
    geometry = ns(Geometry_end(B));

    ns(Feature_start_as_root_with_size(B));
    ns(Feature_geometry_add(B, geometry));
    ns(Feature_end_as_root(B));
    return 0;
}

int test_flatgeobuf1(flatcc_builder_t *B)
{
    void *buffer, *frame;
    size_t size, size2, esize;
    int ret;

    printf("test_flatgeobuf1\n");

    gen_flatgeobuf1(B);

    frame = flatcc_builder_finalize_aligned_buffer(B, &size);
    hexdump("flatgeobuf table with size", frame, size, stderr);

    buffer = flatbuffers_read_size_prefix(frame, &size2);
    esize = size - sizeof(flatbuffers_uoffset_t);
    if (size2 != esize) {
        printf("Size prefix has unexpected size, got %i, expected %i\n", (int)size2, (int)esize);
        return -1;
    }

    if ((ret = ns(Feature_verify_as_root(buffer, size)))) {
        printf("Feature buffer failed to verify, got: %s\n", flatcc_verify_error_string(ret));
        return -1;
    }

    FlatGeobuf_Feature_table_t feature = ns(Feature_as_root(buffer));
    FlatGeobuf_Geometry_table_t geometry = ns(Feature_geometry(feature));

    if (feature == NULL) {
        printf("feature == NULL\n");
    }
    if (geometry == NULL) {
        printf("geometry == NULL\n");
    }


    flatcc_builder_aligned_free(frame);
    return ret;
}

int test_flatgeobuf2(flatcc_builder_t *B)
{
    void *buffer, *frame;
    size_t size, size2, esize;
    int ret;

    printf("test_flatgeobuf2\n");

    gen_flatgeobuf2(B);

    frame = flatcc_builder_finalize_aligned_buffer(B, &size);
    hexdump("flatgeobuf table with size", frame, size, stderr);

    buffer = flatbuffers_read_size_prefix(frame, &size2);
    esize = size - sizeof(flatbuffers_uoffset_t);
    if (size2 != esize) {
        printf("Size prefix has unexpected size, got %i, expected %i\n", (int)size2, (int)esize);
        return -1;
    }

    if ((ret = ns(Feature_verify_as_root(buffer, size)))) {
        printf("Feature buffer failed to verify, got: %s\n", flatcc_verify_error_string(ret));
        return -1;
    }

    FlatGeobuf_Feature_table_t feature = ns(Feature_as_root(buffer));
    FlatGeobuf_Geometry_table_t geometry = ns(Feature_geometry(feature));

    if (feature == NULL) {
        printf("feature == NULL\n");
    }
    if (geometry == NULL) {
        printf("geometry == NULL\n");
    }

    flatcc_builder_aligned_free(frame);
    return ret;
}


int main(int argc, char *argv[])
{
    flatcc_builder_t builder, *B;

    (void)argc;
    (void)argv;

    B = &builder;
    flatcc_builder_init(B);

#ifdef NDEBUG
    printf("running optimized flatgebuf test\n");
#else
    printf("running debug flatgebuf test\n");
#endif
#if 1
    if (test_flatgeobuf1(B)) {
        printf("TEST FAILED\n");
        return -1;
    }
#endif
#if 1
    if (test_flatgeobuf2(B)) {
        printf("TEST FAILED\n");
        return -1;
    }
#endif

    flatcc_builder_clear(B);
    return 0;
}

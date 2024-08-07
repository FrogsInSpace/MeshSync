#include "pch.h"
#include "BufferEncoder.h"

#include "MeshSync/SceneCache/msSceneCacheEncoderSettings.h"

#define ZSTD_STATIC_LINKING_ONLY
#include <zstd.h>
#pragma comment(lib, "libzstd_static.lib")

namespace ms {

class PlainBufferEncoder : public BufferEncoder {
public:
    void EncodeV(RawVector<char>& dst, const RawVector<char>& src) override;
    void DecodeV(RawVector<char>& dst, const RawVector<char>& src) override;
};

void PlainBufferEncoder::EncodeV(RawVector<char>& dst, const RawVector<char>& src) {
    dst = src;
}

void PlainBufferEncoder::DecodeV(RawVector<char>& dst, const RawVector<char>& src) {
    dst = src;
}

//----------------------------------------------------------------------------------------------------------------------

class ZSTDBufferEncoder : public BufferEncoder
{
public:
    explicit ZSTDBufferEncoder(int cl);
    void EncodeV(RawVector<char>& dst, const RawVector<char>& src) override;
    void DecodeV(RawVector<char>& dst, const RawVector<char>& src) override;

private:
    int m_compression_level;
};

//----------------------------------------------------------------------------------------------------------------------

ZSTDBufferEncoder::ZSTDBufferEncoder(const int cl) {
    m_compression_level = mu::clamp(cl, ZSTD_minCLevel(), ZSTD_maxCLevel());
}

void ZSTDBufferEncoder::EncodeV(RawVector<char>& dst, const RawVector<char>& src) {
    const size_t size = ZSTD_compressBound(src.size());
    dst.resize_discard(size);
    const size_t csize = ZSTD_compress(dst.data(), dst.size(), src.data(), src.size(), m_compression_level);
    dst.resize(csize);
}

void ZSTDBufferEncoder::DecodeV(RawVector<char>& dst, const RawVector<char>& src) {
    size_t dsize = static_cast<size_t>(ZSTD_findDecompressedSize(src.data(), src.size()));
    dst.resize_discard(dsize);
    dsize = ZSTD_decompress(dst.data(), dst.size(), src.data(), src.size());
    dst.resize(dsize);
}

//----------------------------------------------------------------------------------------------------------------------

BufferEncoderPtr BufferEncoder::CreateEncoder(const ms::SceneCacheEncoding encoding, 
    const ms::SceneCacheEncoderSettings& settings) 
{
    BufferEncoderPtr ret = nullptr;
    switch (encoding) {
        case SceneCacheEncoding::ZSTD: {
            ret = std::make_shared<ZSTDBufferEncoder>(settings.zstd.compressionLevel);
            break;
        }
        default: {
            ret = std::make_shared<PlainBufferEncoder>();
            break;
        }
    }
    return ret;
}

} // namespace ms

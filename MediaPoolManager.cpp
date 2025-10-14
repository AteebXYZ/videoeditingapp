extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

#include <string>
#include <iostream>

#include "MediaPoolManager.h"

double getVideoFPS(const std::string &filePath)
{
    AVFormatContext *fmtCtx = nullptr;

    // Open video file
    if (avformat_open_input(&fmtCtx, filePath.c_str(), nullptr, nullptr) != 0)
    {
        return 0.0; // could not open file
    }

    if (avformat_find_stream_info(fmtCtx, nullptr) < 0)
    {
        avformat_close_input(&fmtCtx);
        return 0.0; // could not find stream info
    }

    // Find the first video stream
    int videoStreamIndex = -1;
    for (unsigned i = 0; i < fmtCtx->nb_streams; ++i)
    {
        if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1)
    {
        avformat_close_input(&fmtCtx);
        return 0.0; // no video stream
    }

    AVStream *videoStream = fmtCtx->streams[videoStreamIndex];

    // fps = num / den
    double fps = 0.0;
    if (videoStream->avg_frame_rate.den != 0)
        fps = static_cast<double>(videoStream->avg_frame_rate.num) /
              static_cast<double>(videoStream->avg_frame_rate.den);

    avformat_close_input(&fmtCtx);
    return fps;
}

int getVideoFrames(const std::string &filePath)
{
    AVFormatContext *fmtCtx = nullptr;

    // Open input file
    if (avformat_open_input(&fmtCtx, filePath.c_str(), nullptr, nullptr) != 0)
    {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return 0;
    }

    if (avformat_find_stream_info(fmtCtx, nullptr) < 0)
    {
        std::cerr << "Could not find stream info" << std::endl;
        avformat_close_input(&fmtCtx);
        return 0;
    }

    // Find first video stream
    int videoStreamIndex = -1;
    for (unsigned i = 0; i < fmtCtx->nb_streams; ++i)
    {
        if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1)
    {
        std::cerr << "No video stream found" << std::endl;
        avformat_close_input(&fmtCtx);
        return 0;
    }

    AVCodecParameters *codecPar = fmtCtx->streams[videoStreamIndex]->codecpar;
    const AVCodec *codec = avcodec_find_decoder(codecPar->codec_id);
    if (!codec)
    {
        std::cerr << "Codec not found" << std::endl;
        avformat_close_input(&fmtCtx);
        return 0;
    }

    AVCodecContext *codecCtx = avcodec_alloc_context3(codec);
    if (!codecCtx)
    {
        std::cerr << "Could not allocate codec context" << std::endl;
        avformat_close_input(&fmtCtx);
        return 0;
    }

    if (avcodec_parameters_to_context(codecCtx, codecPar) < 0)
    {
        std::cerr << "Could not copy codec params" << std::endl;
        avcodec_free_context(&codecCtx);
        avformat_close_input(&fmtCtx);
        return 0;
    }

    if (avcodec_open2(codecCtx, codec, nullptr) < 0)
    {
        std::cerr << "Could not open codec" << std::endl;
        avcodec_free_context(&codecCtx);
        avformat_close_input(&fmtCtx);
        return 0;
    }

    AVPacket *packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    int frameCount = 0;

    while (av_read_frame(fmtCtx, packet) >= 0)
    {
        if (packet->stream_index == videoStreamIndex)
        {
            if (avcodec_send_packet(codecCtx, packet) == 0)
            {
                while (avcodec_receive_frame(codecCtx, frame) == 0)
                {
                    frameCount++;
                    av_frame_unref(frame);
                }
            }
        }
        av_packet_unref(packet);
    }

    // Flush decoder
    avcodec_send_packet(codecCtx, nullptr);
    while (avcodec_receive_frame(codecCtx, frame) == 0)
    {
        frameCount++;
        av_frame_unref(frame);
    }

    // Cleanup
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&codecCtx);
    avformat_close_input(&fmtCtx);

    return frameCount;
}

QString generateThumbnail(const QString &videoPath, int width, int height)
{
    QString thumbDir = QDir::currentPath() + "/thumbnails";
    QDir().mkpath(thumbDir);
    QString thumbPath = thumbDir + "/" + QFileInfo(videoPath).completeBaseName() + ".png";

    AVFormatContext *fmtCtx = nullptr;
    if (avformat_open_input(&fmtCtx, videoPath.toStdString().c_str(), nullptr, nullptr) < 0)
    {
        qWarning() << "❌ Failed to open video:" << videoPath;
        return QString();
    }
    if (avformat_find_stream_info(fmtCtx, nullptr) < 0)
    {
        qWarning() << "❌ Could not find stream info for" << videoPath;
        avformat_close_input(&fmtCtx);
        return QString();
    }

    int videoStream = av_find_best_stream(fmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (videoStream < 0)
    {
        qWarning() << "❌ No video stream found in" << videoPath;
        avformat_close_input(&fmtCtx);
        return QString();
    }

    AVCodecParameters *codecPar = fmtCtx->streams[videoStream]->codecpar;
    const AVCodec *codec = avcodec_find_decoder(codecPar->codec_id);
    if (!codec)
    {
        qWarning() << "❌ Codec not found for" << videoPath;
        avformat_close_input(&fmtCtx);
        return QString();
    }

    AVBufferRef *hwDeviceCtx = nullptr;
    bool useHW = false;

    // Try to initialize CUDA/NVDEC
    AVHWDeviceType hwType = av_hwdevice_find_type_by_name("cuda");
    if (hwType != AV_HWDEVICE_TYPE_NONE)
    {
        if (av_hwdevice_ctx_create(&hwDeviceCtx, hwType, nullptr, nullptr, 0) >= 0)
        {
            qDebug() << "✅ Using NVIDIA GPU decoding (NVDEC)";
            useHW = true;
        }
        else
        {
            qWarning() << "⚠️ Failed to init CUDA hwaccel, falling back to CPU.";
        }
    }
    else
    {
        qWarning() << "⚠️ No CUDA device found, using CPU decoding.";
    }

    AVCodecContext *codecCtx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecCtx, codecPar);

    if (useHW)
        codecCtx->hw_device_ctx = av_buffer_ref(hwDeviceCtx);

    if (avcodec_open2(codecCtx, codec, nullptr) < 0)
    {
        qWarning() << "❌ Could not open codec for" << videoPath;
        avformat_close_input(&fmtCtx);
        avcodec_free_context(&codecCtx);
        if (hwDeviceCtx)
            av_buffer_unref(&hwDeviceCtx);
        return QString();
    }

    AVPacket *packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    AVFrame *swFrame = av_frame_alloc();

    SwsContext *swsCtx = nullptr;
    bool gotFrame = false;

    while (av_read_frame(fmtCtx, packet) >= 0)
    {
        if (packet->stream_index == videoStream)
        {
            if (avcodec_send_packet(codecCtx, packet) == 0)
            {
                while (avcodec_receive_frame(codecCtx, frame) == 0)
                {
                    AVFrame *useFrame = frame;

                    // If using GPU, download to system memory
                    if (frame->format == AV_PIX_FMT_CUDA || frame->format == AV_PIX_FMT_DXVA2_VLD)
                    {
                        if (av_hwframe_transfer_data(swFrame, frame, 0) < 0)
                        {
                            qWarning() << "⚠️ Failed to transfer GPU frame to CPU.";
                            continue;
                        }
                        useFrame = swFrame;
                    }

                    swsCtx = sws_getCachedContext(
                        swsCtx,
                        useFrame->width, useFrame->height, (AVPixelFormat)useFrame->format,
                        width, height, AV_PIX_FMT_RGB24,
                        SWS_BILINEAR, nullptr, nullptr, nullptr);

                    if (!swsCtx)
                    {
                        qWarning() << "❌ Failed to init swscale context.";
                        break;
                    }

                    uint8_t *rgbData[4];
                    int rgbLinesize[4];
                    av_image_alloc(rgbData, rgbLinesize, width, height, AV_PIX_FMT_RGB24, 1);

                    sws_scale(swsCtx, useFrame->data, useFrame->linesize, 0,
                              useFrame->height, rgbData, rgbLinesize);

                    QImage img(rgbData[0], width, height, rgbLinesize[0], QImage::Format_RGB888);
                    img.save(thumbPath);

                    av_freep(&rgbData[0]);
                    gotFrame = true;
                    break;
                }
            }
        }
        av_packet_unref(packet);
        if (gotFrame)
            break;
    }

    if (!gotFrame)
        qWarning() << "⚠️ No frame decoded for thumbnail from" << videoPath;

    // Cleanup
    sws_freeContext(swsCtx);
    av_frame_free(&frame);
    av_frame_free(&swFrame);
    av_packet_free(&packet);
    avcodec_free_context(&codecCtx);
    avformat_close_input(&fmtCtx);
    if (hwDeviceCtx)
        av_buffer_unref(&hwDeviceCtx);

    return gotFrame ? thumbPath : QString();
}
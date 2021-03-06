include $(SUBDIR)../config.mak

NAME    = bvutil

HEADERS = adler32.h                                                     \
          aes.h                                                         \
          attributes.h                                                  \
          audio_fifo.h                                                  \
          audioconvert.h                                                \
          bvassert.h                                                    \
          bvstring.h                                                    \
          bvutil.h                                                      \
          base64.h                                                      \
          blowfish.h                                                    \
          bprint.h                                                      \
          bswap.h                                                       \
          buffer.h                                                      \
          cast5.h                                                       \
          channel_layout.h                                              \
          common.h                                                      \
          cpu.h                                                         \
          crc.h                                                         \
          display.h                                                     \
          downmix_info.h                                                \
          error.h                                                       \
          eval.h                                                        \
          fifo.h                                                        \
          file.h                                                        \
          frame.h                                                       \
          hash.h                                                        \
          hmac.h                                                        \
          imgutils.h                                                    \
          intfloat.h                                                    \
          intreadwrite.h                                                \
          lfg.h                                                         \
          log.h                                                         \
          macros.h                                                      \
          mathematics.h                                                 \
          md5.h                                                         \
          mem.h                                                         \
          motion_vector.h                                               \
          murmur3.h                                                     \
          dict.h                                                        \
          old_pix_fmts.h                                                \
          opt.h                                                         \
          parseutils.h                                                  \
          pixdesc.h                                                     \
          pixelutils.h                                                  \
          pixfmt.h                                                      \
          random_seed.h                                                 \
          replaygain.h                                                  \
          rational.h                                                    \
          ripemd.h                                                      \
          samplefmt.h                                                   \
          sha.h                                                         \
          sha512.h                                                      \
          stereo3d.h                                                    \
          threadmessage.h                                               \
          time.h                                                        \
          timecode.h                                                    \
          timestamp.h                                                   \
          version.h                                                     \
          xtea.h                                                        \
          packet.h                                                      \
          list.h                                                        \


HEADERS-$(BV_CONFIG_LZO)                   += lzo.h

HEADERS-$(BV_CONFIG_OPENCL)                += opencl.h

BV_ARCH_HEADERS = bswap.h                                                  \
                  intmath.h                                                \
                  intreadwrite.h                                           \
                  timer.h                                                  \

BUILT_HEADERS = bbconfig.h                                              \
                bbversion.h

OBJS = adler32.o                                                        \
       aes.o                                                            \
       atomic.o                                                         \
       audio_fifo.o                                                     \
       bvstring.o                                                       \
       base64.o                                                         \
       blowfish.o                                                       \
       bprint.o                                                         \
       buffer.o                                                         \
       cast5.o                                                          \
       channel_layout.o                                                 \
       cpu.o                                                            \
       crc.o                                                            \
       des.o                                                            \
       display.o                                                        \
       downmix_info.o                                                   \
       error.o                                                          \
       eval.o                                                           \
       fifo.o                                                           \
       file.o                                                           \
       file_open.o                                                      \
       float_dsp.o                                                      \
       frame.o                                                          \
       hash.o                                                           \
       hmac.o                                                           \
       imgutils.o                                                       \
       intmath.o                                                        \
       lfg.o                                                            \
       lls.o                                                            \
       log.o                                                            \
       log2_tab.o                                                       \
       mathematics.o                                                    \
       md5.o                                                            \
       mem.o                                                            \
       murmur3.o                                                        \
       dict.o                                                           \
       opt.o                                                            \
       parseutils.o                                                     \
       pixdesc.o                                                        \
       pixelutils.o                                                     \
       random_seed.o                                                    \
       rational.o                                                       \
       rc4.o                                                            \
       ripemd.o                                                         \
       samplefmt.o                                                      \
       sha.o                                                            \
       sha512.o                                                         \
       stereo3d.o                                                       \
       threadmessage.o                                                  \
       time.o                                                           \
       timecode.o                                                       \
       tree.o                                                           \
       utils.o                                                          \
       xga_font_data.o                                                  \
       xtea.o                                                           \
       packet.o                                                         \
       list.o                                                           \
       network.o                                                        \
       os_support.o                                                     \


OBJS-$(BV_CONFIG_LZO)                      += lzo.o
OBJS-$(BV_CONFIG_OPENCL)                   += opencl.o opencl_internal.o

OBJS += $(COMPAT_OBJS:%=../compat/%)

# Windows resource file
SLIBOBJS-$(BV_HAVE_GNU_WINDRES)            += bvutilres.o

SKIPHEADERS          = old_pix_fmts.h

SKIPHEADERS-$(BV_HAVE_ATOMICS_GCC)        += atomic_gcc.h
SKIPHEADERS-$(BV_HAVE_ATOMICS_SUNCC)      += atomic_suncc.h
SKIPHEADERS-$(BV_HAVE_ATOMICS_WIN32)      += atomic_win32.h
SKIPHEADERS-$(BV_CONFIG_OPENCL)           += opencl.h

TESTPROGS = adler32                                                     \
            aes                                                         \
            atomic                                                      \
            bvstring                                                    \
            base64                                                      \
            blowfish                                                    \
            bprint                                                      \
            cast5                                                       \
            cpu                                                         \
            crc                                                         \
            des                                                         \
            dict                                                        \
            error                                                       \
            eval                                                        \
            file                                                        \
            fifo                                                        \
            float_dsp                                                   \
            hmac                                                        \
            lfg                                                         \
            lls                                                         \
            log                                                         \
            md5                                                         \
            murmur3                                                     \
            opt                                                         \
            pca                                                         \
            parseutils                                                  \
            pixdesc                                                     \
            pixelutils                                                  \
            random_seed                                                 \
            rational                                                    \
            ripemd                                                      \
            sha                                                         \
            sha512                                                      \
            softfloat                                                   \
            tree                                                        \
            utf8                                                        \
            xtea                                                        \

TESTPROGS-$(BV_HAVE_LZO1X_999_COMPRESS) += lzo

TOOLS = crypto_bench ffhash ffeval ffescape

tools/crypto_bench$(EXESUF): ELIBS += $(if $(VERSUS),$(subst +, -l,+$(VERSUS)),)
tools/crypto_bench$(EXESUF): CFLAGS += -DUSE_EXT_LIBS=0$(if $(VERSUS),$(subst +,+USE_,+$(VERSUS)),)

$(SUBDIR)lzo-test$(EXESUF): ELIBS = -llzo2

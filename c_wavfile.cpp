#include "c_wavfile.h"
#include <QDebug>
#include "fasttransforms.h"


c_wavfile::c_wavfile(QObject *parent) : QObject(parent)
{

}

void c_wavfile::load_file(const QString& filename)
{
    SDL_AudioSpec wav_spec;
    Uint32 wav_length; // in bytes
    Uint8 *wav_buffer;



    /* Load the WAV */
    if (SDL_LoadWAV(filename.toLatin1().data(), &wav_spec, &wav_buffer, &wav_length) == NULL) {
        //fprintf(stderr, "Could not open test.wav: %s\n", SDL_GetError());
        qDebug()<<filename<<"not loaded. Error: "<<SDL_GetError();

    } else {
        /* Do stuff with the WAV data, and then... */
        qDebug()<<filename<<" loaded.";
        qDebug()<<"chan-s: "<<wav_spec.channels;
        qDebug()<<"format: "<<wav_spec.format;
        qDebug()<<"format.ISUNSIGNED:"<<SDL_AUDIO_ISUNSIGNED(wav_spec.format);
        qDebug()<<"format.ISLITTLEENDIAN:"<<SDL_AUDIO_ISLITTLEENDIAN(wav_spec.format);
        qDebug()<<"format.ISINT:"<<SDL_AUDIO_ISINT(wav_spec.format);
        qDebug()<<"format.BITSIZE"<<SDL_AUDIO_BITSIZE(wav_spec.format);
        qDebug()<<"freq:   "<<wav_spec.freq;
        qDebug()<<"samples:"<<wav_spec.samples;
        qDebug()<<"silence:"<<wav_spec.silence;
        qDebug()<<"size:   "<<wav_spec.size;
        qDebug()<<"wav_len:"<<wav_length;
        // load data to class member
        int screen_res=wav_length*8/SDL_AUDIO_BITSIZE(wav_spec.format)/2048/8;
        //int screen_res=10;
        qDebug()<<"resolution"<<screen_res;

        for (Uint32 i=0;  i<wav_length; i+=screen_res*2) {
            int16_t s=0;
            s+=(wav_buffer[i+1]<<8)|wav_buffer[i];
            //if (s>0) qDebug()<<wav_buffer[i+1]<<":"<<wav_buffer[i]<<"="<<s;
            m_xmax=i/2.0/wav_spec.freq;
            m_points.append(QPointF(m_xmax,(s*1.0)/32764.0));
        }

        qDebug()<<"maxtime:"<<m_xmax;
        m_source->replace(m_points);
        m_source->attachedAxes()[0]->setMax(m_xmax);
        //qDebug()<<m_points;
        SDL_FreeWAV(wav_buffer);

        using namespace alglib;
            //
            // first we demonstrate forward FFT:
            // [1,1,1,1] is converted to [4, 0, 0, 0]
            //
            real_1d_array x = "[1,1,1,1]";
            complex_1d_array f;
            real_1d_array x2;
            fftr1d(x, f);
            printf("%s\n", f.tostring(3).c_str()); // EXPECTED: [4,0,0,0]

            //
            // now we convert [4, 0, 0, 0] back to [1,1,1,1]
            // with backward FFT
            //
            fftr1dinv(f, x2);
            printf("%s\n", x2.tostring(3).c_str()); // EXPECTED: [1,1,1,1]





    }

}

QLineSeries *c_wavfile::source() const
{
    return m_source;
}

void c_wavfile::setSource(QLineSeries *source)
{
    m_source = source;
}





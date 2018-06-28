#include "c_wavfile.h"
#include <QDebug>


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
        int p=0;

        for (Uint32 i=0;  i<wav_length/8; i+=2) {
            int16_t s=0;
            s+=(wav_buffer[i+1]<<8)|wav_buffer[i];
            //if (s>0) qDebug()<<wav_buffer[i+1]<<":"<<wav_buffer[i]<<"="<<s;
            m_points.append(QPointF(p++*1.0/wav_spec.freq,(s*1.0)/32764.0));
        }
        m_xmax=(p-1)*1.0/wav_spec.freq;
        qDebug()<<"maxtime:"<<m_xmax;
        m_source->replace(m_points);
        m_source->attachedAxes()[0]->setMax(m_xmax);
        //qDebug()<<m_points;
        SDL_FreeWAV(wav_buffer);
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





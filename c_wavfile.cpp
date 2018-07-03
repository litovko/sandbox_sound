#include "c_wavfile.h"
#include <QDebug>
#include "fasttransforms.h"

#include <QtMath>
c_wavfile::c_wavfile(QObject *parent) : QObject(parent)
{

}

void c_wavfile::load_file(const QString& filename)
{
    SDL_AudioSpec wav_spec;
    Uint32 wav_length; // in bytes
    //Uint8 *wav_buffer;



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
        m_sample_rate=wav_spec.freq;
        emit sample_rateChanged();
        // load data to class member
        int screen_res=wav_length*8/SDL_AUDIO_BITSIZE(wav_spec.format)/2048/8;
        //int screen_res=10;
        qDebug()<<"resolution"<<screen_res;
        //wav_length=100000;
        //screen_res=1;
        for (Uint32 i=0;  i<wav_length; i+=screen_res*2) {
            int16_t s=0;
            s+=(wav_buffer[i+1]<<8)|wav_buffer[i];
            //if (s>0) qDebug()<<wav_buffer[i+1]<<":"<<wav_buffer[i]<<"="<<s;
            m_xmax=i/2.0/wav_spec.freq;
            m_points.append(QPointF(m_xmax,(s*1.0)/32767.0));
        }

        qDebug()<<"maxtime:"<<m_xmax;
        m_source->replace(m_points);
        m_source->attachedAxes()[0]->setMax(m_xmax);
        //qDebug()<<m_points;
        //SDL_FreeWAV(wav_buffer);






    }

}

void c_wavfile::fft()
{
    qint32 num=m_frame_width*m_sample_rate; //количество сэмплов во фрейме
    qint32 ind=m_frame_start*m_sample_rate; //стартовая точка в массиве сэмплов
    qDebug()<<"start:"<<ind<<" number:"<<num;
    using namespace alglib;
        real_1d_array x;
        x.setlength(num);
        qDebug()<<"x.length:"<<x.length()<<"ind:"<<ind<<"num:"<<num;
        m_points2.clear();
        int p=0;
        for (auto i=ind*2; i<(num+ind)*2; i+=2) {
            int16_t s=0; s+=(wav_buffer[i+1]<<8)|wav_buffer[i];
            x[p]=(s*1.0)/32764.0;
            m_xmax=(p+ind)*1.0/m_sample_rate;
            m_points2.append(QPointF(m_xmax,x[p]));
            //qDebug()<<p<<" "<<m_points2.last();
            p++;
        }
        m_source->replace(m_points2);
        qDebug()<<m_xmax;
        m_source->attachedAxes()[0]->setMax(m_frame_start+m_frame_width);
        m_source->attachedAxes()[0]->setMin(m_frame_start);
        qDebug()<<"m_xmax:"<<m_xmax;
        complex_1d_array f;
        real_1d_array x2;
        fftr1d(x, f);
        qreal maxy=0;
        m_res_points.clear();
        for (auto i=0; i<f.length(); i++) {


            m_res_points.append(QPointF( i * 48000/f.length() , qSqrt( f[i].x*f[i].x+f[i].y*f[i].y )));
            maxy=qMax(maxy,m_res_points[i].y());
            //qDebug()<<p<<" "<<m_res_points.last();
        }
        qDebug()<<"line ser"<<m_result->count()<<"res points:"<<m_res_points.length();
        m_result->replace(m_res_points);
        qDebug()<<"replace points";
        //m_result->attachedAxes()[0]->setMax(3000);
        qDebug()<<"maxy:"<<maxy;
        m_result->attachedAxes()[1]->setMax(maxy);
        m_result->attachedAxes()[1]->setMin(0);
        qDebug()<<"3";
}

int c_wavfile::sample_rate() const
{
    return m_sample_rate;

}



qreal c_wavfile::frame_start() const
{
    return m_frame_start;

}

void c_wavfile::setFrame_start(const qreal &frame_start)
{
    m_frame_start = frame_start;
}

qreal c_wavfile::frame_width() const
{
    return m_frame_width;
}

void c_wavfile::setFrame_width(const qreal &frame_width)
{
    m_frame_width = frame_width;
}

QLineSeries *c_wavfile::result() const
{
    return m_result;
}

void c_wavfile::setResult(QLineSeries *result)
{
    m_result = result;
}

QLineSeries *c_wavfile::source() const
{
    return m_source;
}

void c_wavfile::setSource(QLineSeries *source)
{
    m_source = source;
}





#include "c_wavfile.h"
#include <QDebug>
#include "fasttransforms.h"
#include "qcustomplot.h"
#include <QtMath>
c_wavfile::c_wavfile(QObject *parent) : QObject(parent)
{

}

void c_wavfile::load_file(const QString& filename)
{
    if (m_chart->CustomPlot()==nullptr) return;
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
        //int screen_res=wav_length*8/SDL_AUDIO_BITSIZE(wav_spec.format)/2048/8;
        int screen_res=1;
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
//        m_source->replace(m_points);
//        m_source->attachedAxes()[0]->setMax(m_xmax);
        //qDebug()<<m_points;
        //SDL_FreeWAV(wav_buffer);
        qDebug()<<"1:";
        if (m_chart==nullptr) qDebug()<<"INIT!!!";
        qDebug()<<m_chart->CustomPlot();
        m_chart->CustomPlot()->clearGraphs();
        qDebug()<<"2:";
        m_chart->CustomPlot()->addGraph()->setName("wav");
        qDebug()<<"3:";
        m_chart->CustomPlot()->graph(0)->setPen( QPen( QColor(255,255,0,255) ) );
        m_chart->CustomPlot()->graph(0)->setVisible(true);
        m_chart->CustomPlot()->addGraph()->setName("fft");
        m_chart->CustomPlot()->graph(1)->setPen( QPen( QColor(0,255,0,255) ) );
        m_chart->CustomPlot()->graph(1)->setVisible(true);
        //m_chart->CustomPlot()->graph(0)->setSelectable(QCP::stWhole);
        //m_chart->CustomPlot()->graph(0)->selectionDecorator()->setBrush(QColor(255,255,255,20));
        m_chart->CustomPlot()->xAxis->setLabel( "Время, сек" );
        //m_chart->CustomPlot()->plotLayout()->set
        //m_chart->CustomPlot()->xAxis->setRange( 0, 500 );
        m_chart->CustomPlot()->yAxis->setRange( -0.5, 0.5 );
        qDebug()<<"4:";
        for (auto el: m_points) {
            m_chart->CustomPlot()->graph(0)->addData(el.x(), el.y());
            //qDebug()<<"x:"<<el.x()<<" y:"<<el.y();
        }
        qDebug()<<"5:";
        m_chart->CustomPlot()->xAxis->setRange( 0, m_xmax);

//        m_chart->CustomPlot()->yAxis->setRange( ymin, ymax*2 );
        qDebug()<<"6:";
        draw_frame();
        m_chart->CustomPlot()->replot();





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
        qDebug()<<m_xmax;
        qDebug()<<"m_xmax:"<<m_xmax;
        complex_1d_array f;
        real_1d_array x2;
        fftr1d(x, f);
        qreal maxy=0;
        m_res_points.clear();
        m_chartf->CustomPlot()->clearGraphs();
        m_chartf->CustomPlot()->addGraph()->setName("fft");
        qDebug()<<"fft length:"<<f.length();
        for (auto i=0; i<f.length(); i++) {
            m_res_points.append(QPointF( i * 48000/f.length() , qSqrt( f[i].x*f[i].x+f[i].y*f[i].y )));
            m_chartf->CustomPlot()->graph(0)->addData(m_res_points[i].x(), m_res_points[i].y());
            maxy=qMax(maxy,m_res_points[i].y());
        }

//        for (auto el: m_res_points) {
//            m_chartf->CustomPlot()->graph(0)->addData(el.x(), el.y());
//            //qDebug()<<"x:"<<el.x()<<" y:"<<el.y();
//        }
        qDebug()<<"maxy:"<<maxy;
        qDebug()<<"3";
        m_chartf->CustomPlot()->graph(0)->setPen( QPen( QColor(255,255,100,255) ) );
        m_chartf->CustomPlot()->graph(0)->setVisible(true);
        m_chartf->CustomPlot()->xAxis->setRange( 300, 2000);
        m_chartf->CustomPlot()->yAxis->setRange( 0, 200);
        draw_frame();
        m_chartf->CustomPlot()->replot();
        m_chart->CustomPlot()->replot();
}

void c_wavfile::draw_frame()
{
    if (rect==nullptr)
        rect= new QCPItemRect(m_chart->CustomPlot());
    rect->setPen(QColor(255,255,255,255));
    rect->topLeft->setCoords(m_frame_start,0.5);
    rect->bottomRight->setCoords(m_frame_start+m_frame_width,-0.5);
    rect->setVisible(true);
}

void c_wavfile::calc_feature1()
{
    //для начала определим какая у нас частота сигнала, возьмем отрезок 2 секунды и найдем частоту у которой максимальный уровень на этом отрезке.

    //Надо пробежаться по всему буферу фреймом и определить в каждом фрейме уровень частоты 800 гц.

    qint32 num=2*m_sample_rate; //количество сэмплов во фрейме
    qint32 ind=0*m_sample_rate; //стартовая точка в массиве сэмплов
    for (auto i=0; i<100; i++)
}

CustomPlotItem *c_wavfile::chartf() const
{
    return m_chartf;
}

void c_wavfile::setChartf(CustomPlotItem *chartf)
{
    m_chartf = chartf;
}

CustomPlotItem *c_wavfile::chart() const
{
    return m_chart;
}

void c_wavfile::setChart(CustomPlotItem *chart)
{
    m_chart = chart;
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








